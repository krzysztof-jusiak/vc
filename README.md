<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>

---------------------------------------

#Virtual Concepts

###Concept
```cpp
const auto Concept =
  $requires(auto&& t, std::ostream& os) (
    T(t),   // copy constructible
    os << t // printable
  );
```

```cpp
template<class T, class... Ts>
const auto Creatable = $(create)<T(Ts...)>();  
```

```cpp
struct Readable {
  auto operator()() {
    return $requires(auto&& t, std::ostream& os) (
      T(t), // copy constructible
      os << t
    ) && $(read)<void(int)>();
  }
};
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

###Usage
```cpp
int main() {
  // constraint checking
  static_assert(requires<FileReader>(Readable), "");

  // type erasure - dynamic dispatch
  any<decltype(Readable)> readable = FileReader{};
  readable.read(42);
  
  // type erasure mocking
  readable = GMock<decltype(Readable)>{};
  EXPECT_CALL(mock, read, 42);
  readable.read(42);
  
  // template mocking
  GMock<decltype(Readable)> mock;
  EXPECT_CALL(mock, read, 42);
  mock.read(42);
}
```

---

##Dependencies
* concepts
  * STL
* type_erasure (any)
  * dyno
    * boost.hana
    * callable_traits
* mocking (gmock)
  * GUnit
    * GoogleTest/GoogleMock

##References
* http://boost-experimental.github.io/di/concepts-driven-design-with-di/#/
