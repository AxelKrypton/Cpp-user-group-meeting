/*
 *===================================================
 *
 *    Copyright (c) 2025
 *      Alessandro Sciarra
 *
 *    GNU General Public License (GPLv3 or later)
 *
 *===================================================
 */

#include <iostream>
#include <list>
#include <memory>
#include <vector>

/*
 * Some type traits techniques:
 *
 *  0) Basic ingredients
 *      ↳ SFINAE basic idea
 *      ↳ std::integral_constant (std::bool_constant, std::[true|false]_type)
 *      ↳ decltype and std::declval
 *      ↳ std::void_t
 *  1) Pure partial class template specialization
 *      ↳ is_pointer
 *  2) Function overload (one "catch all" function)
 *      ↳ is_implicitly_convertible
 *  3) Partial class template specialization with utility 2nd template parameter
 *      ↳ has_begin
 */

//============================ EXERCISE 1 =================================

template <typename T>
struct is_pointer : public std::false_type {};

// TASK: Implement a partial class template specialization inheriting from
//       std:::true_type for the pointer types.
template <typename T>
struct is_pointer<T*> : public std::true_type {};

template <typename T>
struct is_pointer<T* const> : public std::true_type {};

template <typename T>
struct is_pointer<T* volatile> : public std::true_type {};

template <typename T>
struct is_pointer<T* const volatile> : public std::true_type {};

//Optional:
template <typename T>
struct is_pointer<std::unique_ptr<T>> : public std::true_type {};

/* // ALTERNATIVE 1:
template <typename T>
struct is_pointer_impl : public std::false_type {};

template <typename T>
struct is_pointer_impl<T*> : public std::true_type {};

template <typename T>
struct is_pointer : public is_pointer_impl<std::remove_cv_t<T>> {};

template <typename T>
constexpr bool is_pointer_v = is_pointer<T>::value;
*/

/* // ALTERNATIVE 2:
namespace detail {
   template <typename T>
   std::false_type test_is_pointer(T);

   template <typename T>
   std::true_type  test_is_pointer(T*);
}

template <typename T>
struct is_pointer : public decltype( detail::test_is_pointer( std::declval<T>()
) ) {};
*/

template <typename T>
constexpr bool is_pointer_v = is_pointer<T>::value;

//============================ EXERCISE 2 =================================

// Some classes to demonstrate what we want
struct Foo {};

struct Bar {
  Bar(Foo);
};

struct Qux {
  operator Foo();
};

struct Waldo : public Foo {};

namespace detail {

// TASK: Implement two function templates overloads, the first returning
//       std::true_type and the second std::false_type. The idea in the first is
//       to use the template parameter as argument, while the second is meant to
//       serve as a catch all case. How can you pass anything to it?
template <typename To>
std::true_type test_is_implicitly_convertible(To);

template <typename To>
std::false_type test_is_implicitly_convertible(...);

}  // namespace detail

// TASK: Implement a class template with two template parameters that inherits
//       "from the return value" of detail::test_is_implicitly_convertible
template <typename From, typename To>
struct is_implicitly_convertible
    : public decltype(detail::test_is_implicitly_convertible<To>(
          std::declval<From>())) {};

template <typename From, typename To>
constexpr bool is_implicitly_convertible_v =
    is_implicitly_convertible<From, To>::value;

//============================ EXERCISE 3 =================================

// Definition of a type with 'begin()' member function
struct Fred {
  void begin();
};

// Definition of a type without 'begin()' member function
struct Plugh {};

template <typename T, typename AUX = void>
struct has_begin : std::false_type {};

template <typename T>
struct has_begin<T, std::void_t<decltype(std::declval<T>().begin())>>
    : std::true_type {};

// TASK: Implement a class template with a second unnamed template parameter.
//       Make it inherit from std::false_type. This will be the general case.
//       Add a partial specialization of the class on the second template type
//       and make this inherit from std::true_type. Implement the specialization
//       such that this is preferred if the first template parameter has a
//       begin() method.

template <typename T>
constexpr bool has_begin_v = has_begin<T>::value;

//==============================================================================

int main() {
  // TECHNIQUE 1:
  static_assert(!is_pointer_v<int>);
  static_assert(!is_pointer<int>::value);
  static_assert(!is_pointer_v<const int>);
  static_assert(!is_pointer_v<volatile int>);
  static_assert(!is_pointer_v<const volatile int>);
  static_assert(!is_pointer_v<int&>);
  static_assert(!is_pointer_v<const int&>);
  static_assert(!is_pointer_v<int*&>);
  static_assert(is_pointer_v<int*>);
  static_assert(is_pointer_v<int**>);
  static_assert(is_pointer_v<int* const>);
  static_assert(is_pointer_v<int* volatile>);
  static_assert(is_pointer_v<int* const volatile>);
  // Optional: add smart pointers
  static_assert(is_pointer_v<std::unique_ptr<const int>>);

  // TECHNIQUE 2:
  static_assert(is_implicitly_convertible_v<int, double>);
  static_assert(is_implicitly_convertible_v<double, int>);
  static_assert(is_implicitly_convertible_v<Foo, Bar>);
  static_assert(is_implicitly_convertible_v<Qux, Foo>);
  static_assert(is_implicitly_convertible_v<Waldo*, Waldo const*>);
  static_assert(is_implicitly_convertible_v<Waldo*, Foo*>);
  static_assert(!is_implicitly_convertible_v<int*, double*>);
  static_assert(!is_implicitly_convertible_v<double*, int*>);
  static_assert(!is_implicitly_convertible_v<Bar, Foo>);
  static_assert(!is_implicitly_convertible_v<Foo, Qux>);
  static_assert(!is_implicitly_convertible_v<Waldo const*, Waldo*>);
  static_assert(!is_implicitly_convertible_v<Foo*, Waldo*>);

  // TECHNIQUE 3:
  static_assert(has_begin_v<std::vector<int>>);
  static_assert(has_begin_v<std::list<double>&>);
  static_assert(has_begin_v<std::string const&>);
  static_assert(has_begin_v<Fred>);
  static_assert(!has_begin_v<int>);
  static_assert(!has_begin_v<double&>);
  static_assert(!has_begin_v<Plugh>);
  static_assert(has_begin<Fred>::value);

  return 0;
}
