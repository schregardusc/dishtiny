#pragma once
#ifndef DISH2_INTROSPECTION_GET_MAXIMUM_PROGRAM_LENGTH_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MAXIMUM_PROGRAM_LENGTH_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t get_maximum_program_length( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  emp::vector< size_t > program_lengths;
  std::transform(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population ),
    std::back_inserter( program_lengths ),
    []( const dish2::Cell<Spec>& cell ){
      return cell.genome->program.size();
    }
  );

  return *std::max_element(
    std::begin( program_lengths ),
    std::end( program_lengths )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MAXIMUM_PROGRAM_LENGTH_HPP_INCLUDE