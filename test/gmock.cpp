#include "vc/mocking/gmock.hpp"
#include <GUnit.h>
#include <iostream>
#include "vc/concepts/callable.hpp"
#include "vc/concepts/concept.hpp"

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

GTEST("GMock") {
  SHOULD("mock Readable") {
    GMock2<decltype(Readable)> mock;
    EXPECT_CALL(mock, (read)(42));
    EXPECT_CALL(mock, (write)(1, 2.0)).WillOnce(Return(22));
    mock.read(42);
    EXPECT_EQ(22, mock.write(1, 2.0));
  }
}
