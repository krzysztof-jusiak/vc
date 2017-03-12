#include "vc/type_erasure/any.hpp"
#include <GUnit.h>
#include <iostream>
#include "vc/concepts/callable.hpp"
#include "vc/concepts/concept.hpp"
#include "vc/mocking/gmock.hpp"

// clang-format off
const auto Readable =
  REQUIRES(auto&& t, std::ostream& os)( os << t ) &&
  Callable<void(float)>( FNAME(read) ) &&
  Callable<short(char, double)>( FNAME(write) );
// clang-format on

struct FileReader {
  void read(float) const { std::cout << __PRETTY_FUNCTION__ << std::endl; }

  short write(char, double) const {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return {};
  }
};

std::ostream& operator<<(std::ostream& os, FileReader&) { return os; }

template <class T>
std::ostream& operator<<(std::ostream& os, GMock2<T>&) {
  return os;
}

GTEST("any") {
  SHOULD("hold anything Readable") {
    any<decltype(Readable)> readable = FileReader{};
    readable.read(42);
    readable.write(2, 42.0);
  }

  SHOULD("Be constructible from GMock") {
    // static_assert(Readable(type<GMock2<decltype(Readable)>>), "");
    GMock2<decltype(Readable)> mock;
    any<decltype(Readable)> readable = mock;
    EXPECT_CALL(mock, (read)(42));
    EXPECT_CALL(mock, (write)(1, 2.0));
    readable.read(42);
    mock.write(1, 2.0);
  }
}
