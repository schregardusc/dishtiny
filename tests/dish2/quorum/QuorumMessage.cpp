#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/quorum/QuorumMessage.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec;

TEST_CASE("Test QuorumMessage") {

  dish2::QuorumMessage< Spec >{};

}
