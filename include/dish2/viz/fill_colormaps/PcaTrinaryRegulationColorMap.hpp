#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_PCATRINARYREGULATIONCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_PCATRINARYREGULATIONCOLORMAP_HPP_INCLUDE

#include <algorithm>
#include <functional>
// #include <unordered_map> // TODO fixme
#include <map>

#include "../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../third-party/Empirical/source/base/optional.h"
#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/header-only-pca/include/hopca/normalize.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/pca.hpp"

#include "../../introspection/make_cardi_coord_to_live_cardi_idx_translator.hpp"
#include "../../introspection/count_live_cells.hpp"
#include "../../introspection/summarize_module_regulation.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

class PcaTrinaryRegulationColorMap {

  std::reference_wrapper< const dish2::ThreadWorld<Spec> > thread_world;

  std::map< // todo fixme
    std::tuple<size_t, size_t>, size_t
  > cardi_coord_to_live_cardi_idx_translator;
  emp::optional<hopca::Matrix> pca_result;

public:

  template<typename... Args>
  PcaTrinaryRegulationColorMap(
    const dish2::ThreadWorld<Spec>& thread_world_,
    Args&&...
  ) : thread_world( thread_world_ ) {
    Refresh();
  }

  template<typename ValueType>
  std::string Paint(const ValueType& cardi_coord) const {
    if ( cardi_coord_to_live_cardi_idx_translator.count(
      cardi_coord
    ) == 0 ) return "transparent";
    else if ( !pca_result.has_value() ) return "transparent";

    const size_t live_cardi_idx = cardi_coord_to_live_cardi_idx_translator.at(
      cardi_coord
    );
    hopca::Vector v = hola::matrix_row_copy( *pca_result, live_cardi_idx );
    const auto data = DATA(v);

    emp_assert( std::clamp( v->length, 1, 3 ) == v->length );

    return emp::ColorRGB(
      255 - std::clamp( uitsl::clamp_cast<int>( data[0] * 255.0 ), 0, 255 ),
      v->length >= 1
        ? 255 - std::clamp( uitsl::clamp_cast<int>( data[1] * 255.0 ), 0, 255 )
        : 0
      ,
      v->length >= 2
        ? 255 - std::clamp( uitsl::clamp_cast<int>( data[2] * 255.0 ), 0, 255 )
        : 0
    );

  }

  void Refresh() {

    pca_result.reset();

    hopca::Matrix raw_regulation_summary = dish2::summarize_module_regulation(
      thread_world.get()
    );
    std::transform(
      DATA( raw_regulation_summary ),
      DATA( raw_regulation_summary )
        + raw_regulation_summary->n_row * raw_regulation_summary->n_col
      ,
      DATA( raw_regulation_summary ),
      []( const double val ){ return emp::Sgn( val ); }
    );

    const auto trinary_regulation_summary = hopca::drop_homogenous_columns(
      raw_regulation_summary
    );
    cardi_coord_to_live_cardi_idx_translator
      = dish2::make_cardi_coord_to_live_cardi_idx_translator< Spec >(
        thread_world.get()
      );

    if ( !trinary_regulation_summary.has_value() ) return;

    hopca::PCA module_regulation_pca{ std::min(
      3ul, uitsl::audit_cast<size_t>(trinary_regulation_summary.value()->n_row)
    ) };

    pca_result = hopca::unit_normalize(
      hopca::drop_homogenous_columns(
        module_regulation_pca.doPCA( *trinary_regulation_summary )
      ).value()
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_PCATRINARYREGULATIONCOLORMAP_HPP_INCLUDE