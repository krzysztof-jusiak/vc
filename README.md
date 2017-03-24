<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>

---------------------------------------

#Virtual Concepts

###Concept
```cpp
template<class T>
const auto Equality_comparable =
  $requires(T)(auto a, auto b) (
    bool(a == b),
    bool(a != b)
  );
```

```cpp
template<class T>
const auto Iterator() {
  return CopyConstructible<T> &&
         CopyAssignable<T> &&
         Destructible<T> &&
         $requires(T)(auto&& t) (
           *t,
           typename decltype(t)::type
         );
  }
}
```

```cpp
template<class T, class R, class... Ts>
const auto Creatable = Callable<T, R(Ts...)>($(create));
```

```cpp
struct Readable {
  template<class T>
  auto operator()() {
    return CopyConstructible() &&
       $requires(T)(auto&& t, std::ostream& os) (
          os = (os << t)
       ) &&
       Callable<T, void(int)>($(read));
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
  static_assert(Equality_comparable<int>, "");
  static_assert(Readable{}.operator()<FileReader>(), "");
  
  // template mocking
  testing::GMock<Readable> mockReadable;
  EXPECT_CALL(mockReadable, read, 42);
  mock.read(42);
  
  // type erasure - dynamic dispatch
  type_erasure::any<Readable> readable = FileReader{};
  readable.read(42);
  
  // type erasure mocking
  readable = GMock<Readable>{};
  EXPECT_CALL(mock, read, 42);
  readable.read(42);
}
```

###DI
```cpp
template<class TReader = Readable> // = 'Concept'
class App {
  TReader reader;                       // static dispatch
  type_erasure::any<Printable> printer; // dynamic dispatch
  
public:
  App(TReader reader, type_erasure::any<Printable> printer)
   : reader(reader), printer(printer)
  { }
  
  void run() {
    printer.print(reader.read());
  }
};

int main() {
  const auto injector = di::make_injector(
     di::bind<Readable>.to<FileReader>()
     di::bind<Printable>.to<ConsolePrinter>()
  );
  
  injector.create<App>().run();
}
```

###Testing
```cpp
"should print read text"_test = [] {
  constexpr auto value = 42;

  auto [sut, mocks] = testing::make<App>(); // creates System Under Test
                                            // and Mocks

  InSequence sequence;
  EXPECT_CALL(mocks<Readable>(), read).WillOnce(Return(value));
  EXPECT_CALL(mocks<Printable>(), print, value);

  sut.run();
};
```

---

##Dependencies
* Concepts
  * [STL](http://en.cppreference.com/w)
* Type Erasure (any)
  * [dyno](https://github.com/ldionne/dyno)
    * [Boost.Hana](https://github.com/boostorg/hana)
    * [CallableTraits](https://github.com/badair/callable_traits)
* Mocking (GMock)
  * [GUnit](https://github.com/cpp-testing/GUnit)
    * [Google Test/Mock](https://github.com/google/googletest)
* Dependency Injection
    * [[Boost].DI](https://github.com/boost-experimental/di)

##References
* [Concept Checking in C++11](http://ericniebler.com/2013/11/23/concept-checking-in-c11) | [Type requirements in C++](http://pfultz2.com/blog/2014/08/17/type-requirements)
* [A bit of background for concepts](https://isocpp.org/blog/2016/02/a-bit-of-background-for-concepts-and-cpp17-bjarne-stroustrup)
* [Concepts Lite](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3580.pdf)
* [Vitual Concepts Proposal](https://github.com/andyprowl/virtual-concepts/blob/master/draft/Dynamic%20Generic%20Programming%20with%20Virtual%20Concepts.pdf)
* [Concepts driven design with DI](http://boost-experimental.github.io/di/concepts-driven-design-with-di)
* [Static reflection](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0194r0.pdf)
