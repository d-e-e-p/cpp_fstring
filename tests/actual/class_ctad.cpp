/**
 * @file class_ctad.cpp
 * misc demo of cpp-fstring
 *
 * @ingroup examples
 *
 * @author Sandeep M
 * @copyright Copyright 2023 Sandeep M<deep@tensorfield.ag>
   @license MIT License
*/
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <array>

#include "fstr.h"

// from https://stackoverflow.com/questions/65781641/im-trying-to-format-a-template-using-fmt
template <typename T, T Min, T Max>
class LimitedInt {
   T mValue{Min};
public:
   explicit LimitedInt(const T value) {
      setValue(value);
   }
   void setValue(const T value) {
      mValue = value;
   }
   T getValue() const {
      return mValue;
   }
// Generated to_string for PUBLIC CLASS_TEMPLATE LimitedInt<T, Min, Max>
  public:
  auto to_string() const {
    return fstr::format(R"( LimitedInt<T, Min, Max>: <{}> mValue={}
)", typeid(T).name(), mValue);
  }
};

template<auto n>
struct Auto {
  int v = 1;
// Generated to_string for PUBLIC CLASS_TEMPLATE Auto<n>
  public:
  auto to_string() const {
    return fstr::format(R"( Auto<n>: int v={}
)", v);
  }
};

/* TODO
template <auto... Values>
struct ValueList {
    static constexpr auto values = std::array{Values...};

    static void print() {
        for (const auto& value : values) {
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }
};
ValueList<42, 'A'>vl;
*/

// from https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
//
//
template<class T>
struct A {
    T t;

    struct {
        long a, b;
    // Generated to_string for PUBLIC STRUCT_DECL A<T>::(unnamed struct at input/class_ctad.cpp:62:5)
  public:
  auto to_string() const {
    return fstr::format(R"( A<T>::(unnamed struct at input/class_ctad.cpp:62:5): long a={}, b={}
)", a, b);
  }
} u;
// Generated to_string for PUBLIC CLASS_TEMPLATE A<T>
  public:
  auto to_string() const {
    return fstr::format(R"( A<T>: <{}> t={}, long u.a={}, u.b={}
)", typeid(T).name(), t, this->u.a, this->u.b);
  }
};


template<class T>
struct B {
    T t;
    A<T> a;
// Generated to_string for PUBLIC CLASS_TEMPLATE B<T>
  public:
  auto to_string() const {
    return fstr::format(R"( B<T>: <{}> t={}, A<T> a={}
)", typeid(T).name(), t, a);
  }
};

int main() {
  using std::cout;
  cout << fmt::format("file: {}\ntime: {}\n", __FILE_NAME__, __TIMESTAMP__);

  A<int> a{1,{2,3}};
  auto b = B<int>{1, {2,{3,4}}};
  cout << fmt::format("a={} b={}", a, b);
  cout << "\n";

}