<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>

---------------------------------------

#Virtual Concepts

###Concept
```cpp
const auto Readable =
  REQUIRES(auto&& t, std::ostream& os)(
    os << t // printable
  ) &&
  Callable<void(int)>(FNAME(read));
```

###Implementation
```cpp
struct FileReader {
  void read(int) const {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

std::ostream& operator<<(std::ostream& os, FileReader&) {
  return os;
}
```

###Test
```cpp
int main() {
  static_assert(satisfies<FileReader>(Readable), "");

  // mocking
  Mock<decltype(Readable)> mock;
  EXPECT_CALL(mock, (read)(42));
  mock.read(42);

  // type erasure
  any<decltype(Readable)> readable = FileReader{};
  readable.read(42);
}
```
