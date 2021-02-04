#pragma once
#ifndef DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_KIN_NEIGHBORS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_KIN_NEIGHBORS_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../runninglog/SpawnEvent.hpp"
#include "../world/iterators/LogIteratorAdapter.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t get_total_spawn_event_kin_neighbors(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto& population = world.population;

  using iterator_t = dish2::LogIteratorAdapter<
    dish2::SpawnEvent<Spec>,
    decltype( std::begin(population) )
  >;

  const auto [begin, end] = iterator_t::make(
    std::begin( population ), std::end( population )
  );

  return std::accumulate(
    begin, end, 0,
    [=]( const size_t accumulator, const auto& event ){
      const size_t peripherality = event.peripherality_parent[lev];
      emp_assert( event.num_neighbors_parent >= peripherality );
      return accumulator + event.num_neighbors_parent - peripherality;
    }
  );

}

template< typename Spec >
size_t get_total_spawn_event_kin_neighbors(
  const dish2::ThreadWorld<Spec>& world,
  const size_t lev,
  const size_t kin_id_commonality_parent_eliminated
) {

  const auto& population = world.population;

  using iterator_t = dish2::LogIteratorAdapter<
    dish2::SpawnEvent<Spec>,
    decltype( std::begin(population) )
  >;

  const auto [begin, end] = iterator_t::make(
    std::begin( population ), std::end( population )
  );

  return std::accumulate(
    begin, end, 0,
    [=]( const size_t accumulator, const auto& event ){
      const size_t peripherality = event.peripherality_parent[lev];
      emp_assert( event.num_neighbors_parent >= peripherality );
      return (
        event.kin_id_commonality_parent_eliminated == kin_id_commonality_parent_eliminated
      ) ? accumulator + event.num_neighbors_parent - peripherality
        : accumulator;
    }
  );

}

template< typename Spec >
size_t get_total_spawn_event_kin_neighbors(
  const dish2::ThreadWorld<Spec>& world,
  const size_t lev,
  const size_t kin_id_commonality_parent_eliminated,
  const size_t replev
) {

  const auto& population = world.population;

  using iterator_t = dish2::LogIteratorAdapter<
    dish2::SpawnEvent<Spec>,
    decltype( std::begin(population) )
  >;

  const auto [begin, end] = iterator_t::make(
    std::begin( population ), std::end( population )
  );

  return std::accumulate(
    begin, end, 0,
    [=]( const size_t accumulator, const auto& event ){
      const size_t peripherality = event.peripherality_parent[lev];
      emp_assert( event.num_neighbors_parent >= peripherality );
      return (
        event.replev == replev
        && event.kin_id_commonality_parent_eliminated
          == kin_id_commonality_parent_eliminated
      ) ? accumulator + event.num_neighbors_parent - peripherality
        : accumulator;
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_TOTAL_SPAWN_EVENT_KIN_NEIGHBORS_HPP_INCLUDE
