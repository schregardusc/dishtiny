#pragma once
#ifndef DISH2_CELL_SERVICES_APOPTOSISSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICES_APOPTOSISSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../../config/cfg.hpp"

#include "../cardinal_iterators/ApoptosisRequestWrapper.hpp"

namespace dish2 {

struct ApoptosisService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.APOPTOSIS_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    if ( std::any_of(
      cell.template begin<dish2::ApoptosisRequestWrapper<Spec>>(),
      cell.template end<dish2::ApoptosisRequestWrapper<Spec>>(),
      std::identity
    ) ) cell.DeathRoutine();

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICES_APOPTOSISSERVICE_HPP_INCLUDE
