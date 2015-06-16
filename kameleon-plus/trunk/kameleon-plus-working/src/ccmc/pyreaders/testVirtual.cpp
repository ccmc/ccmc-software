#include <boost/python.hpp>
using std::string;
using namespace boost::python;

namespace ccmc
{
  class Base
  {
    public:

      virtual string foo() const = 0; //pure virtual method
      virtual string foo(int a) const = 0;
      string fooBase() const { return foo(); }
      // string fooBase(int a) const { return foo(a); }
  };
}

class BaseWrapper: public ccmc::Base, public wrapper<ccmc::Base>
{
public:
    string fooBar() const {
      std::cout << "fooBar calling overide with no args" << std::endl;
      return this->get_override("foo")();
    }
  	string foo() const {
      std::cout << "calling overide with no args" << std::endl;
  		return this->get_override("foo")();
  	}
    string foo(int a) const {
      std::cout << "calling override with int" << std::endl;
      return this->get_override("foo")(a);
    }
};


// // Base member function pointers 
string (ccmc::Base::*foo1)() const        = &ccmc::Base::foo;
string (ccmc::Base::*foo2)(int) const     = &ccmc::Base::foo;
string (BaseWrapper::*foobar1)() const = &BaseWrapper::fooBar;
// string (BaseWrapper::*foo3)() const        = &BaseWrapper::foo;
// string (BaseWrapper::*foo4)(int) const     = &BaseWrapper::foo;

string (ccmc::Base::*foobase1)() const = &ccmc::Base::fooBase;
// string (ccmc::Base::*foobase2)(int) const = &ccmc::Base::fooBase;





struct X
{
    bool f(int a)
    {
        return true;
    }

    bool f(int a, double b)
    {
        return true;
    }

    bool f(int a, double b, string c)
    {
      std::cout << "called f with " << a << ' ' << b<< ' ' << c << std::endl;
        return true;
    }

    int f(int a, int b, int c)
    {
        return a + b + c;
    };
};

bool    (X::*fx1)(int)              = &X::f;
bool    (X::*fx2)(int, double)      = &X::f;
bool    (X::*fx3)(int, double, string)= &X::f;
int     (X::*fx4)(int, int, int)    = &X::f;


BOOST_PYTHON_MODULE(Baz){
  class_<BaseWrapper,boost::noncopyable>("Base")
    .def("foo",pure_virtual(foo1))
    .def("foo",pure_virtual(foo2))
    // .def("foo",foo3)
    // .def("foo",foo4)
    .def("fooBar", foobar1)
    .def("fooBase",foobase1)
    // .def("fooBase",foobase2)
  ;
  class_<X,boost::noncopyable>("X")
    .def("f", fx1)
    .def("f", fx2)
    .def("f", fx3)
    .def("f", fx4)
  ;
}





