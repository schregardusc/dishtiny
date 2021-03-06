#pragma once
#ifndef DISH2_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/ResourceNodeInputWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

struct ResourceReceivingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_RECEIVING_SERVICE_FREQUENCY();
    // must run whether cell is alive or not to keep aggregated ducts in sync
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "resource receiving service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<spec_t>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    // how much resource have we received across all cardinals?
    const float received_amount = std::accumulate(
      cell.template begin<dish2::ResourceNodeInputWrapper<spec_t>>(),
      cell.template end<dish2::ResourceNodeInputWrapper<spec_t>>(),
      0.0f,
      [](const auto& cumulative_sum, auto& addend){ return addend.JumpGet(); }
    );

    // dead cells receive resource but do not absorb it
    if ( !cell.IsAlive() ) return;

    // how much do we already have?
    const float current_amount
      = *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>();

    // update stockpiles
    std::fill(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      received_amount + current_amount
    );

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<spec_t>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
