#include "vc/concepts/concept.hpp"
#include <GUnit.h>
#include <iostream>
#include "vc/concepts/callable.hpp"

// clang-format off
const auto Readable =
  $requires(auto&& t, std::ostream& os)(
    os << t,
    T(t)
  ) &&
  $(read)<void(float)>() &&
  $(write)<short(char, double)>();
// clang-format off

struct FileReader {
  void read(float) const { std::cout << __PRETTY_FUNCTION__ << std::endl; }

  short write(char, double) const {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return {};
  }
};

std::ostream& operator<<(std::ostream& os, FileReader&) { return os; }

GTEST("Should satisfies Readable") {
  struct empty {};
  static_assert(!Readable(type<empty>), "");
  // static_assert(Readable(type<FileReader>), "");
}
