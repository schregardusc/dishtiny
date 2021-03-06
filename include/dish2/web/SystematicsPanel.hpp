#pragma once
#ifndef DISH2_WEB_SYSTEMATICSPANEL_HPP_INCLUDE
#define DISH2_WEB_SYSTEMATICSPANEL_HPP_INCLUDE

#include <functional>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/Document.hpp"

#include "../spec/Spec.hpp"

#include "DataPillCollection.hpp"
#include "DataPill.hpp"

namespace dish2 {

class SystematicsPanel {

  emp::web::Document button_dash{ "emp_systematics_dash" };

public:

  SystematicsPanel( const dish2::ThreadWorld< dish2::Spec >& world ) {

    button_dash.SetAttr(
      "class", "row"
    );

    // adapted from https://stackoverflow.com/a/45498003
    std::apply(
      [this, &world](auto ...x){(..., (
        button_dash << decltype(x)::Make( world )
      ) );},
      dish2::DataPillCollection{}
    );

  }

  void Redraw() { button_dash.Redraw(); }

};

} // namespace dish2



#endif // #ifndef DISH2_WEB_SYSTEMATICSPANEL_HPP_INCLUDE
