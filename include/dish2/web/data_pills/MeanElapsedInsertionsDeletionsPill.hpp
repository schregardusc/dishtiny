#pragma once
#ifndef DISH2_WEB_DATA_PILLS_MEANELAPSEDINSERTIONSDELETIONSPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_MEANELAPSEDINSERTIONSDELETIONSPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/get_mean_elapsed_insertions_deletions.hpp"
#include "../../spec/Spec.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../DataPill.hpp"

namespace dish2 {

class MeanElapsedInsertionsDeletionsPill {

  static auto DoWork( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::get_mean_elapsed_insertions_deletions<dish2::Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::DataPill(
      "Mean Elapsed Insertions and Deletions",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_MEANELAPSEDINSERTIONSDELETIONSPILL_HPP_INCLUDE
