#pragma once
#ifndef DISH2_WEB_GETTERS_PHYLOGENETICROOTGETTER_HPP_INCLUDE
#define DISH2_WEB_GETTERS_PHYLOGENETICROOTGETTER_HPP_INCLUDE

#include "../../cell/Cell.hpp"
#include "../../config/cfg.hpp"
#include "../../genome/Genome.hpp"
#include "../../genome/RootID.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class PhylogeneticRootGetter {

  const dish2::ThreadWorld<Spec>& thread_world;

public:

  using value_type = size_t;

  PhylogeneticRootGetter(const dish2::ThreadWorld<Spec>& thread_world_)
  : thread_world(thread_world_)
  {}

  const value_type& Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return thread_world.GetCell( cell_idx ).GetGenome().root_id;
  }

  size_t GetNumCells() const { return dish2::cfg.N_CELLS(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_GETTERS_PHYLOGENETICROOTGETTER_HPP_INCLUDE