/*
 *===================================================
 *
 *    Copyright (c) 2024
 *      Alessandro Sciarra
 *
 *    GNU General Public License (GPLv3 or later)
 *
 *===================================================
 */

#include <iostream>
#include <string_view>
#include <vector>

/*
 * 1. C++11 uniform initialisation (aka braced initialisation)
 * 2. Narrowing conversion and C++11 uniform initialisation
 * 3. How is it better to declare members? Built-in types VS custom types
 *    -> https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 * 4. What happens to the default ctor when I declare a non default
 *    constructor or e.g. a copy ctor?
 * 5. Discuss a bit signature and what the special members are
 * 6. What happens to uniform initialization when a constructor taking an
 *    std::initializer_list argument is introduced?
 */

void I_am(std::string_view sv) { std::cout << sv << "\n"; }

constexpr int f() {
  // static int i = 0;
  // std::cout << "i = " << i << "\n";
  // i++;
  return 888;
}

struct PlayWithMe {
  // Default constructor
  PlayWithMe() { I_am("PlayWithMe()"); };
  // Copy constructor
  PlayWithMe(const PlayWithMe&) { I_am("PlayWithMe(const PlayWithMe&)"); }
  // Copy assignment operator
  PlayWithMe& operator=(const PlayWithMe&) {
    I_am("operator=(const PlayWithMe&)");
    return *this;
  }
  // Move constructor
  PlayWithMe(PlayWithMe&&) { I_am("PlayWithMe(PlayWithMe&&)"); }
  // Move assignment operator
  PlayWithMe operator=(PlayWithMe&&) {
    I_am("operator=(PlayWithMe&&)");
    return *this;
  }
  // Destructor
  ~PlayWithMe() { I_am("~PlayWithMe()"); }

  PlayWithMe(int m) : N{m}, ptr{nullptr}, s{"Hello"} {}

  void print() {
    std::cout << "  N = " << N << "\n"
              << "ptr = " << ((ptr) ? std::to_string(*ptr) : "nullptr") << "\n"
              << "  s = \"" << s << "\"\n";
  }

 private:
  int N{42};
  double* ptr{nullptr};
  std::string s{"Hi"};
};

// Taken from Scott Meyers, Effective Modern C++, Item 7
struct Widget {
  Widget() { I_am("Widget()"); };
  Widget(float f) : B{f < 0} { I_am("Widget(float)"); }
  Widget(int i, bool b) : N{i}, B{b} { I_am("Widget(int, bool)"); }
  Widget(int i, double d) : N{i}, B{d > 3.14} { I_am("Widget(int, double)"); }
  Widget(const Widget&) { I_am("Widget(const Widget&)"); }
  Widget& operator=(const Widget&) {
    I_am("Widget& operator=(const Widget&)");
    return *this;
  }
  Widget(Widget&&) { I_am("Widget(Widget&&)"); }
  Widget& operator=(Widget&&) {
    I_am("Widget& operator=(Widget)");
    return *this;
  }
  // Case (1)
  // Widget(std::initializer_list<long double> il) {
  //   I_am("Widget(std::initializer_list<long double>)");
  // }
  // operator float() const { return 0.0; }
  // Case (2)
  // Widget(std::initializer_list<bool> il) {
  //   I_am("Widget(std::initializer_list<bool>)");
  // }
  // Case (3)
  Widget(std::initializer_list<std::string> il) {
    I_am("Widget(std::initializer_list<std::string>)");
  }

  int N{0};
  bool B{false};
};

int main() {
  // (1): What happens here? And what if using uniform initialisation?
  {
      // std::vector<int> v = {1,2,3};
      // int N{v.size()};
      // double* ptr{};
      // std::string s{};
      // std::cout << "  N = " << N << "\n"
      //           << "ptr = " << ((ptr) ? std::to_string(*ptr) : "nullptr") <<
      //           "\n"
      //           << "  s = \"" << s << "\"\n";
  }

  // (2): Do the following lines compile?
  {
      // int J(3.14);
      // int K = 3.14;
      // int L{3.14};
  }

  // (3): How is it better to declare the object: p VS p() VS p{}
  //      How to avoid such a headache? -> C.48 and C.49
  {
      // PlayWithMe p{}, p2{};
      // p.print();
      // p2.print();
  }

  // (4): What happens to the following line when declaring e.g. a ctor?
  //      -> discuss https://www.foonathan.net/2019/02/special-member-functions/
  {
      // PlayWithMe p{};
      // p.print();
  }

  // std::vector<int> v1{2,3};
  // std::vector<int> v2(2,3);

  // (6): Discuss some examples with Widget class
  {
    std::cout << "w1: ";
    Widget w1(10, true);
    std::cout << "w2: ";
    Widget w2{10, true};
    std::cout << "w3: ";
    Widget w3(10, 5.0);
    std::cout << "w4: ";
    Widget w4{"10", "5.0"};
    std::cout << "w5: ";
    Widget w5(w4);
    std::cout << "w6: ";
    Widget w6{w4};
    std::cout << "w7: ";
    Widget w7(std::move(w4));
    std::cout << "w8: ";
    Widget w8{std::move(w4)};
    std::cout << "w9: ";
    Widget w9;
    std::cout << "w10: ";
    Widget w10{};
    // std::cout << "w11: ";
    // Widget w11({});
    // //
    // http://scottmeyers.blogspot.com/2016/11/help-me-sort-out-meaning-of-as.html
    // // Widget w12{{}};
    // // C++ most vexing parse!
    // // Widget w13();
  }
}
