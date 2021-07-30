#include <iostream>
#include <type_traits>
#include <tuple>

struct A
{
  using parent_t = int;
};

struct B
{
  using parent_t = int;
  using all_parents_t = std::tuple<int, double>;
};

template<typename T, typename K = void>
struct has_many_parents: std::false_type {};

template<typename T>
struct has_many_parents<T, typename std::enable_if_t<std::is_class_v<typename T::all_parents_t>>>: std::true_type {};

// don't work in get_parent_types by unknown (now) reason
template<typename T>
using has_many_parents_v = typename has_many_parents<T>::value;

template<typename T, typename K = void>
struct get_parent_types
{
  using type = std::tuple<typename T::parent_t>;
};

template<typename T>
struct get_parent_types<T, typename std::enable_if_t<has_many_parents<T>::value>>
{
    using type = typename T::all_parents_t;
};


int main()
{
    std::cout << "a has many parents: " << has_many_parents<A>::value << std::endl;
    std::cout << "b has many parents: " << has_many_parents<B>::value << std::endl;
  
    std::cout << "a parents count: " << std::tuple_size_v<get_parent_types<A>::type> << std::endl;
    std::cout << "b parents count: " << std::tuple_size_v<get_parent_types<B>::type> << std::endl;
}
