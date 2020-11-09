#pragma once
#ifndef DISH2_SERVICES_INTERMESSAGECOUNTERCLEARSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMESSAGECOUNTERCLEARSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/IncomingInterMessageCounterWrapper.hpp"
#include "../config/cfg.hpp"

namespace dish2 {

struct InterMessageCounterClearService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq
      = dish2::cfg.INTER_MESSAGE_COUNTER_CLEAR_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    std::fill(
      cell.template begin<dish2::IncomingInterMessageCounterWrapper<spec_t> >(),
      cell.template end< dish2::IncomingInterMessageCounterWrapper<spec_t> >(),
      0
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMESSAGECOUNTERCLEARSERVICE_HPP_INCLUDE