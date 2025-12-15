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

/*
 * Required properties to support range-based for loops:
 *
 *  1) begin() - end() must be available
 *      ↳ Either as member functions
 *      ↳ or as free functions, found by ADL.
 *  2) The returned types of begin/end must form an input range
 *     This is an iterator that has:
 *      ↳ a dereference operation: *it
 *      ↳ pre-increment: ++it
 *      ↳ comparison for inequality: it != end_it
 *     NOTE: begin/end must return types comparable with each other, but
 *      ↳ operator!= is required for range-for;
 *      ↳ operator== is optional for range-for.
 *  3) Constant overloads of begin/end needed to use on constant instances
 *      ↳ const_iterator begin() const;
 *      ↳ const_iterator end() const;
 *
 * Good practices:
 *
 *  1) Provide const and non-const iteration
 *  2) Use nested iterator types when appropriate
 *        class MyContainer {
 *          public:
 *              class iterator { ... };
 *              class const_iterator { ... };
 *        };
 *  3) Keep iterators lightweight and pointers-like:
 *       ↳ small;
 *       ↳ cheap to copy;
 *       ↳ deterministic increment/dereference.
 *  4) Avoid invalidation surprises
 *       ↳ For modifying containers, define clear rules on when iterators
 *         remain valid or become invalid (STL defines these very precisely).
 *  5) Prefer implementing both operator!= and operator== for iterators
 *       ↳ Range-for only needs !=, but generic algorithms often need both.
 *
 * About types of iterators:
 * https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
 * About iterators tags:
 * https://en.cppreference.com/w/cpp/iterator/iterator_tags.html
 *
 * What is standard to define as nested types?
 *  1) Mandatory / universal nested types
 *      ↳ using value_type        = T;
 *      ↳ using reference         = T&;
 *      ↳ using const_reference   = const T&;
 *      ↳ using pointer           = T*;
 *      ↳ using const_pointer     = const T*;
 *      ↳ using size_type         = std::size_t;
 *      ↳ using difference_type   = std::ptrdiff_t;
 *  2) Iterator types (must match iterator category)
 *      ↳ using iterator          = <your iterator type>;
 *      ↳ using const_iterator    = <your const iterator type>;
 *  3) Optional but beneficial
 *      ↳ using iterator_category = std::random_access_iterator_tag;
 *  4) Containers also often define:
 *      ↳ using reverse_iterator       = std::reverse_iterator<iterator>;
 *      ↳ using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 *
 * Should you rely on ADL?
 *
 *   General rule:
 *     - If writing a “container-like” object → prefer member begin()/end()
 *     - If writing a non-owning view or adapting an external type → using ADL
 *       is excellent
 *        ↳ this is exactly how C++20 ranges work
 *
 * When is ADL useful?
 *
 *   If your type is not meant to own iteration semantics, but you want to
 * enable iteration without modifying the class, ADL is an elegant solution:
 *
 *      namespace my_namespace {
 *          struct ForeignWrapper { ExternalLibType t; };
 *          auto begin(ForeignWrapper& w) { return w.t.start(); }
 *          auto end(ForeignWrapper& w)   { return w.t.finish(); }
 *      }
 */

#include <cstddef>
#include <iostream>
#include <iterator>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T, std::size_t N>
class SmallVector {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  struct iterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;

    iterator() : p(nullptr) {}
    explicit iterator(T* ptr) : p(ptr) {}
    reference operator*() const { return *p; }
    pointer operator->() const { return p; }
    iterator& operator++() {
      ++p;
      return *this;
    }
    iterator operator++(int) {
      iterator tmp = *this;
      ++p;
      return tmp;
    }
    iterator& operator--() {
      --p;
      return *this;
    }
    iterator operator+(difference_type n) const { return iterator(p + n); }
    difference_type operator-(const iterator& other) const {
      return p - other.p;
    }
    reference operator[](difference_type n) const { return p[n]; }
    bool operator!=(const iterator& o) const { return p != o.p; }
    bool operator==(const iterator& o) const { return p == o.p; }

   private:
    T* p;
  };

  using const_iterator = iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;

  SmallVector() noexcept : size_(0) {}
  ~SmallVector() { destroy_inline(); }

  void push_back(const T& v) {
    if (!using_heap()) {
      if (size_ < N) {
        new (inline_ptr(size_)) T(v);
        ++size_;
        return;
      }
      switch_to_heap();
    }
    heap_.push_back(v);
    ++size_;
  }

  void push_back(T&& v) {
    if (!using_heap()) {
      if (size_ < N) {
        new (inline_ptr(size_)) T(std::move(v));
        ++size_;
        return;
      }
      switch_to_heap();
    }
    heap_.push_back(std::move(v));
    ++size_;
  }

  reference operator[](size_type i) {
    return using_heap() ? heap_[i] : *inline_ptr(i);
  }
  const_reference operator[](size_type i) const {
    return using_heap() ? heap_[i] : *inline_ptr(i);
  }

  size_type size() const noexcept { return size_; }
  bool empty() const noexcept { return size_ == 0; }

  iterator begin() noexcept {
    return iterator(using_heap() ? heap_.data() : inline_ptr(0));
  }
  iterator end() noexcept {
    return iterator(using_heap() ? heap_.data() + size_ : inline_ptr(size_));
  }
  const_iterator begin() const noexcept {
    return const_iterator(using_heap() ? const_cast<T*>(heap_.data())
                                       : inline_ptr(0));
  }
  const_iterator end() const noexcept {
    return const_iterator(using_heap() ? const_cast<T*>(heap_.data() + size_)
                                       : inline_ptr(size_));
  }

 private:
  bool using_heap() const noexcept { return heap_.capacity() > 0; }

  T* inline_ptr(size_type i) noexcept {
    return reinterpret_cast<T*>(inline_storage_) + i;
  }

  const T* inline_ptr(size_type i) const noexcept {
    return reinterpret_cast<const T*>(inline_storage_) + i;
  }

  void switch_to_heap() {
    /*
     * Use a temporary to internally maintain invariants strong. During the loop
     * here below that pushes elements, heap_ is already considered active
     * storage. Although the inline elements are still alive, using_heap() will
     * suddenly return true. Any method (e.g., begin(), operator[], push_back)
     * used during the switch risks seeing an inconsistent internal state.
     */
    std::vector<T> new_heap{};
    new_heap.reserve(size_ * 2 + 1);

    // Move from inline to heap
    for (size_type i = 0; i < size_; ++i) {
      new_heap.push_back(std::move(*inline_ptr(i)));
    }

    // Destroy inline objects
    destroy_inline();

    // Install heap vector
    heap_ = std::move(new_heap);
  }

  void destroy_inline() noexcept {
    if (!using_heap()) {
      std::cerr << "Destroying inline...\n";
      for (size_type i = 0; i < size_; ++i) {
        inline_ptr(i)->~T();
      }
    }
  }

  // data members
  std::size_t size_ = 0;
  std::vector<T> heap_{};
  alignas(T) std::byte inline_storage_[sizeof(T) * N];
};

// Example
int main() {
  SmallVector<int, 4> sv;
  for (int i = 0; i < 10; ++i) {
    sv.push_back(i);
    std::cout << "Size is now " << sv.size() << "\n";
  }
  for (auto x : sv) std::cout << x << " ";
  std::cout << "\n";
}
