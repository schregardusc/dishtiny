#pragma once
#ifndef DISH2_SERVICES_QUORUMCAPSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_QUORUMCAPSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

struct QuorumCapService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.QUORUM_CAP_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static bool DoService( Cell& cell, const size_t lev ) {

    const size_t num_quorum_bits
      = cell.cell_quorum_state.GetNumKnownQuorumBits( lev );
    if ( num_quorum_bits < cfg.QUORUM_CAP()[ lev ] ) return false;

    const float p = cfg.P_QUORUM_CAP_KILL()[ lev ];
    auto& rand = sgpl::ThreadLocalRandom::Get();
    if ( rand.P( p ) ) cell.DeathRoutine();

    return true;

  }


  template<typename Cell>
  static void DoService( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    for (size_t lev{}; lev < spec_t::NLEV; ++lev) {
      if ( DoService( cell, lev ) ) break;
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_QUORUMCAPSERVICE_HPP_INCLUDE