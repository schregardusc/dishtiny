#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/viz/artists/DummyArtist.hpp"
#include "dish2/web/GridViewer.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test GridViewer") {

  auto thread_world = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);

  dish2::GridViewer<dish2::DummyArtist> viewer2{ thread_world, emp_base };

}
