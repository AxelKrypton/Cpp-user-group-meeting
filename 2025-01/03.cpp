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

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

/*
 * EXCELLENT TALK:
 *  C++ and Beyond 2012: Scott Meyers Universal References in C++11
 *    -> https://www.youtube.com/watch?v=6lq8PntQMV4
 *
 * -----------------------------------------------------------------------------
 * 1. C++11 introduced rvalue references. Distinguish between rvalue references
 *    and forwarding (universal) references.
 */

// Taken from https://stackoverflow.com/a/56766138/14967071
template <typename T>
constexpr auto type_name() {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

void print(const int& N) { std::cout << "print(const int&)\n"; }

void print(int&& N) { std::cout << "print(int&&)\n"; }

void print_rref_type(int&& N) {
  // The following two calls to show that N here is an lvalue!
  std::cout << std::setw(40) << "print(N) -> ";
  print(N);
  std::cout << std::setw(40) << "print(std::move(N)) -> ";
  print(std::move(N));
}

template <typename T>
void print_uref_type(T&& param) {
  std::cout << std::setw(40) << "print(param) -> ";
  print(param);
  std::cout << std::setw(40) << "print(std::move(param)) -> ";
  print(std::move(param));
  std::cout << std::setw(40) << "print(std::forward<T>(param)) -> ";
  print(std::forward<T>(param));
}


int main() {
  // (1): What do you expect?
  {
    std::cout << "print_rref_type(123):\n";
    print_rref_type(123);
    int value = 222;
    std::cout << "\nprint_rref_type(value):\n";
    print_rref_type(std::move(value));

    int N{42};
    std::cout << "\nprint_uref_type(N):\n";
    print_uref_type(N);
    std::cout << "\nprint_uref_type(666):\n";
    print_uref_type(666);
  }
}
