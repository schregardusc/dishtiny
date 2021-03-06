#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_SPAWNREQUESTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_SPAWNREQUESTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class SpawnRequestWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  SpawnRequestWrapper() = default;

  SpawnRequestWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::SpawnRequest;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::SpawnRequest
    >();
  }

  value_type* operator->() { return &operator*(); }

  SpawnRequestWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  SpawnRequestWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  SpawnRequestWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  SpawnRequestWrapper operator--(int) {
    SpawnRequestWrapper res{ *this };
    res -= 1;
    return res;
  }

  SpawnRequestWrapper operator+(const size_t rhs) {
    SpawnRequestWrapper res{ *this };
    res += rhs;
    return res;
  }

  SpawnRequestWrapper operator-(const size_t rhs) {
    SpawnRequestWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_SPAWNREQUESTWRAPPER_HPP_INCLUDE
