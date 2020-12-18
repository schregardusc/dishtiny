#pragma once
#ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE
#define DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/GlobalAnchorIterator.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../cell/Cell.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> nop_out_phenotypically_neutral_modules(
  const dish2::Genome<Spec>& orig_genome
) {

  const dish2::LogScope guard( "evaluating module-by-module" );

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  const auto& orig_prog = orig_genome.program;

  auto res = orig_genome;

  uitsl::for_each(
    std::next( sgpl::GlobalAnchorIterator<sgpl_spec_t>::make_begin(orig_prog) ),
    sgpl::GlobalAnchorIterator<sgpl_spec_t>::make_end( orig_prog ),
    sgpl::GlobalAnchorIterator<sgpl_spec_t>::make_begin( orig_prog ),
    sgpl::CountingIterator{},
    [&]( const auto& module_back, const auto& module_front, const auto idx ) {

      const dish2::LogScope guard( emp::to_string("evaluating module ", idx) );

      auto trial_genome = res;
      auto& trial_prog = trial_genome.program;

      std::for_each(
        &module_front, &module_back,
        [&](const auto& inst) {
          const size_t idx = std::distance( &orig_prog.front(), &inst );
          trial_prog[ idx ].NopOutIfNotAnchor();
        }
      );

      if ( !dish2::detect_phenotypic_divergence<Spec>(
        orig_genome, trial_genome
      ) ) {
        dish2::log_event({dish2::info, emp::to_string("nopping module ", idx)});
        res = trial_genome;
      }

    }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE