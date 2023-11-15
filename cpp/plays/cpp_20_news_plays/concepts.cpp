#include <string>
#include <cstddef>
#include <concepts>

template<typename T>
concept Sorter = requires(T a) {

    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;

};

struct asdf {};

template<Sorter T>
void f(T) {}

int main() {
  using namespace std::string_literals;
  // using std::operators;

  f("abc"s);

  f(asdf{});
}