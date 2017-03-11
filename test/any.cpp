#include <iostream>
#include "vc/concepts/concept.hpp"
#include "vc/concepts/callable.hpp"
#include "vc/type_erasure/any.hpp"
#include <GUnit.h>

const auto Readable =
  REQUIRES(auto&& t, std::ostream& os)(
    os << t,
    t
  ) &&
  Callable<void(float)>(FNAME(read)) &&
  Callable<short(char, double)>(FNAME(write));

struct FileReader {
  void read(float) const {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  short write(char, double) const {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return {};
  }
};

std::ostream& operator<<(std::ostream& os, FileReader&) {
  return os;
}

GTEST("any") {
  SHOULD("hold anything Readable") {
    any<decltype(Readable)> readable = FileReader{};
    readable.read(42);
    readable.write(2, 42.0);
  }

  SHOULD("Be constructible from GMock") {
    //any<decltype(Readable)> readable = mock;
  }
}
