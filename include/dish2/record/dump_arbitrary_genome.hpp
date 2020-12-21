#pragma once
#ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"

#include "../introspection/count_live_cells.hpp"
#include "../world/iterators/LiveCellIterator.hpp"

#include "../world/ThreadWorld.hpp"

#include "make_dump_arbitrary_genome_filename.hpp"

namespace dish2 {

template< typename Spec >
bool dump_arbitrary_genome(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  // abort if no live cells
  if ( dish2::count_live_cells<Spec>(world) == 0 ) return false;

  const std::string filename(
    dish2::make_dump_arbitrary_genome_filename( thread_idx )
  );

  std::ofstream os( filename );
  cereal::JSONOutputArchive archive( os );

  // get the first /live/ cell
  archive(
    *(dish2::LiveCellIterator<Spec>::make_begin( world.population )->genome)
  );

  return true;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_ARBITRARY_GENOME_HPP_INCLUDE