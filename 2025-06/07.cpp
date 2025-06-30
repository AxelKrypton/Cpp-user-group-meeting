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
#include <memory>

/*
 * WANNA DIG MORE?
 *  -> CppCon 2017: Louis Dionne “Runtime Polymorphism: Back to the Basics”
 *         https://www.youtube.com/watch?v=gVGtNFg4ay0
 *
 * DISCLAIMER: It is not really about basics! Still, try to watch it.
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

void I_am(std::string_view sv) { std::cout << sv << "\n"; }

struct Base {
  Base() { I_am("Base()"); };
  Base(const Base&) { I_am("Base(const Base&)"); }
  Base& operator=(const Base&) {
    I_am("operator=(const Base&)");
    return *this;
  }
  Base(Base&&) { I_am("Base(Base&&)"); }
  Base operator=(Base&&) {
    I_am("operator=(Base&&)");
    return *this;
  }
  virtual ~Base() { I_am("~Base()"); }

  void talk() const {
    std::cout << s_base << "\n";
  }

 private:
  std::string s_base{"Hi"};
};

struct Derived final : public Base {
  Derived() { I_am("Derived()"); };
  Derived(const Derived&) { I_am("Derived(const Derived&)"); }
  Derived& operator=(const Derived&) {
    I_am("operator=(const Derived&)");
    return *this;
  }
  Derived(Derived&&) { I_am("Derived(Derived&&)"); }
  Derived operator=(Derived&&) {
    I_am("operator=(Derived&&)");
    return *this;
  }
  ~Derived() override { I_am("~Derived()"); }

  void talk() const {
    std::cout << s_derived << "\n";
  }

 private:
  std::string s_derived{"Bye"};
};

void chat(const Base& b)
{
  std::cout << "Chatting: ";
  b.talk();
}

void danger_chat(Base b)
{
  std::cout << "Danger chatting: ";
  b.talk();
}


int main() {
  // What do you expect?
  // {
  //   // Base b{};
  //   // b.talk();
  //   Derived d{};
  //   d.talk();
  // }
  {
    // std::unique_ptr<Base> b_ptr = std::make_unique<Derived>();
    Base* b_ptr = new Derived;
    b_ptr -> talk();
    delete b_ptr;
  }
  // {
  //   Derived d{};
  //   Base& b = d;
  //   b.talk();
  //   std::cout << type_name<decltype(b)>() << "\n";
  // }
  // {
  //   Derived d{};
  //   chat(d);
  // }
  // {
  //   Derived d{};
  //   danger_chat(d);
  // }
  // {
  //   Derived d{};
  //   danger_chat(std::move(d));
  // }
  // {
  //   std::unique_ptr<Base> b_ptr = std::make_unique<Derived>();
  //   if (Derived* d = dynamic_cast<Derived*>(b_ptr.get()); d != nullptr)
  //   {
  //       std::cout << "downcast from b to d successful\n";
  //       d->talk(); // safe to call
  //   }
  // }
  // {
  //   std::unique_ptr<Base> b_ptr = std::make_unique<Base>();
  //   if (Derived* d = dynamic_cast<Derived*>(b_ptr.get()); d != nullptr)
  //   {
  //       std::cout << "downcast from b to d successful\n";
  //       d->talk(); // safe to call
  //   } else {
  //     std::cout << "downcast from b to d NOT successful\n";
  //   }
  // }
}
