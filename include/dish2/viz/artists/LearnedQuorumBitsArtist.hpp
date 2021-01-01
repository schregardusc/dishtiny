#pragma once
#ifndef DISH2_VIZ_ARTISTS_LEARNEDQUORUMBITSARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_LEARNEDQUORUMBITSARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/QuorumBitsColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/LearnedQuorumBitsGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::learned_quorum_bits_artist {

  template<
    typename LearnedQuorumBitsGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::QuorumBitsColorMap,
      LearnedQuorumBitsGetter
    >,
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderColorMap,
      KinGroupIDGetter
    >
  >;

} // namespace internal::learned_quorum_bits_artist

template<
  typename LearnedQuorumBitsGetter=dish2::LearnedQuorumBitsGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class LearnedQuorumBitsArtist
: public internal::learned_quorum_bits_artist::parent_t<
  LearnedQuorumBitsGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::learned_quorum_bits_artist::parent_t<
    LearnedQuorumBitsGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Learned Quorum Bits"; }

  template< typename Spec >
  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>& thread_world) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_LEARNEDQUORUMBITSARTIST_HPP_INCLUDE
