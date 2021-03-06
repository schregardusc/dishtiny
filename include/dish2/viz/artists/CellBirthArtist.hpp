#pragma once
#ifndef DISH2_VIZ_ARTISTS_CELLBIRTHARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_CELLBIRTHARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/CellBirthColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/CellAgeGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::cell_birth_artist {

  template<
    typename CellAgeGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::CellBirthColorMap,
      CellAgeGetter
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

} // namespace internal::cell_birth_artist

template<
  typename CellAgeGetter=dish2::CellAgeGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class CellBirthArtist
: public internal::cell_birth_artist::parent_t<
  CellAgeGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::cell_birth_artist::parent_t<
    CellAgeGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Cell Birth"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_CELLBIRTHARTIST_HPP_INCLUDE
