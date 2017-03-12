#include "vc/concepts/concept.hpp"
#include <GUnit.h>
#include <iostream>
#include "vc/concepts/callable.hpp"

const auto Readable = REQUIRES(auto&& t, std::ostream& os)(os << t, t) && Callable<void(float)>(FNAME(read)) &&
                      Callable<short(char, double)>(FNAME(write));

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
