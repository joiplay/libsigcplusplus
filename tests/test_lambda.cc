// -*- c++ -*-
/* Copyright 2002, The libsigc++ Development Team
 *  Assigned to public domain.  Use as you wish without restriction.
 */

#include <iostream>
#include <sigc++/functors/functors.h>
#include <sigc++/adaptors/lambda/lambda.h>

using namespace sigc::functor;

#ifndef SIGC_CXX_TYPEOF
// other template libraries (e.g. boost::lambda) have similar hacks built in
// to make lambda shift operators work with streams
namespace sigc { namespace functor { namespace internal {
template <class T_arg>
struct shiftleft_deduce_result_type<std::ostream&, T_arg>
{
  typedef std::ostream& type;
};
} } }
#endif

int foo(int i, int j)
  {std::cout << "foo(int "<<i<<", int "<<j<<")" << std::endl; return 4*i+j;}

void foo_void(int i)
  {std::cout << "foo_void(int "<<i<<")" << std::endl;}

struct bar
{
  int test(int i, int j)
    {std::cout << "bar::test(int "<<i<<", int "<<j<<")" << std::endl; return 4*i+j;}
  void test_void(int i)
    {std::cout << "bar::test_void(int "<<i<<")" << std::endl;}
};

main()
{
  // test lambda operators
  std::cout << (_1 + _2)(3,4) << std::endl;
  std::cout << (_1 + 1) (3,4) << std::endl;
  std::cout << (_2 + 1) (3,4) << std::endl;
  std::cout << (2 + _1) (3,4) << std::endl;
  std::cout << (2 + _2) (3,4) << std::endl;
  std::cout << (_1+_2*_3) (1,2,3) << std::endl;

       // c++ restrictions:
       // - cannot use std::endl without much hackery because it is defined as a template function
       // - cannot use "\n" because arrays cannot be copied
  (std::cout << lambda<int>(1) << std::string("\n"))();
  (std::cout << _1 << std::string("\n"))("hello world");

  // test grp adaptor
  std::cout << (group(&foo, _1, _2)) (1, 2) << std::endl;
  std::cout << (group(&foo, _2, _1)) (1, 2) << std::endl;
//  std::cout << (group(&bar::test, _1, _2)) (bar(), 1, 2) << std::endl; // TODO: doesn't work

  // same functionality as bind
  std::cout << (group(&foo, _1, 2))  (1)    << std::endl;
  std::cout << (group(&foo, 1, 2))   ()     << std::endl;
  (group(ptr_fun(&foo_void), 1)) ();

  //TODO: is there an equivalent for bind_return ?

  // same functionality as hide
  std::cout << (group(&foo, _1, _2)) (1,2,3) << std::endl;
  (group(ptr_fun(&foo_void), _2)) (1, 2);

  // same functionality as compose
  std::cout << (group(&foo, group(&foo, _1, _2), _3)) (1,2,3) << std::endl;
}
