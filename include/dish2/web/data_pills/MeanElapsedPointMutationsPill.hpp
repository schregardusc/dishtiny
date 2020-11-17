#pragma once
#ifndef DISH2_WEB_DATA_PILLS_MEANELAPSEDPOINTMUTATIONSPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_MEANELAPSEDPOINTMUTATIONSPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../introspection/get_mean_elapsed_point_mutations.hpp"
#include "../../world/ThreadWorld.hpp"
#include "../../spec/Spec.hpp"

#include "../DataPill.hpp"

namespace dish2 {

class MeanElapsedPointMutationsPill {

  static auto DoWork( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::get_mean_elapsed_point_mutations<dish2::Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::DataPill(
      "Mean Elapsed Point Mutations",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_MEANELAPSEDPOINTMUTATIONSPILL_HPP_INCLUDE
