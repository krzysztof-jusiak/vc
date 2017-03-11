#include <iostream>
#include "vc/concepts/concept.hpp"
#include "vc/concepts/callable.hpp"
#include "vc/mocking/gmock.hpp"
#include "vc/type_erasure/any.hpp"

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

int main() {
  static_assert(satisfies<FileReader>(Readable), "");

  Mock<decltype(Readable)> mock;
  EXPECT_CALL(mock, (read)(42));
  EXPECT_CALL(mock, (write)(1, 2.0)).WillOnce(Return(22));
  mock.read(42);
  EXPECT_EQ(22, mock.write(1, 2.0));


  any<decltype(Readable)> readable = FileReader{};
  readable.read(42);
  readable.write(2, 42.0);

  {
  //any<decltype(Readable)> readable = mock;
  }
}


