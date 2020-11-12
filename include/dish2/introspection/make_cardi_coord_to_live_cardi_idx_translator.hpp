#pragma once
#ifndef DISH2_INTROSPECTION_MAKE_CARDI_COORD_TO_LIVE_CARDI_IDX_TRANSLATOR_HPP_INCLUDE
#define DISH2_INTROSPECTION_MAKE_CARDI_COORD_TO_LIVE_CARDI_IDX_TRANSLATOR_HPP_INCLUDE

#include <iterator>
#include <tuple>
#include <map> // todo fixme

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/cell_iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
std::map<std::tuple<size_t, size_t>, size_t>
make_cardi_coord_to_live_cardi_idx_translator(
  const dish2::ThreadWorld<Spec>& world
) {

  const auto& population = world.population;

  std::map<std::tuple<size_t, size_t>, size_t> res; // todo fixme

  size_t live_cardi_idx{};

  for (
    auto it = ++dish2::LiveCellIterator<Spec>(
      std::prev( std::begin( population ) ), std::end( population )
    );
    it != dish2::LiveCellIterator<Spec>(
      std::end(population), std::end(population)
    );
    ++it
  ) {
    const size_t cell_pos = it.CalcDistance( std::begin( population ) );
    const auto& cell = *it;

    for (size_t cardi_pos{}; cardi_pos < cell.GetNumCardinals(); ++cardi_pos) {
      const std::tuple<size_t, size_t> cardi_coord{ cell_pos, cardi_pos };
      res[ cardi_coord ] = live_cardi_idx++;
    }
  }

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_MAKE_CARDI_COORD_TO_LIVE_CARDI_IDX_TRANSLATOR_HPP_INCLUDE
