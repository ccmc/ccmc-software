%module CCMC
%include "std_string.i"
%include "std_vector.i"
%include "cpointer.i"
%import "ccmc/cdf.h"


%{
#include <ccmc/Kameleon.h>
#include <ccmc/Interpolator.h>
#include <ccmc/Point3f.h>
#include <ccmc/Fieldline.h>
#include <ccmc/Kameleon-Tracer.h>
#include <ccmc/Attribute.h>
#include <ccmc/FileReader.h>
using namespace std;


%}

%include <ccmc/FileReader.h>
%include <ccmc/Kameleon.h>
%include <ccmc/Interpolator.h>
%include <ccmc/Kameleon-Tracer.h>
%include <ccmc/Fieldline.h>
%include <ccmc/Point3f.h>
%include <ccmc/Attribute.h>

using namespace ccmc;
typedef ccmc::Fieldline Fieldline;
typedef ccmc::Point3f Point3f;


%template(vector_point3f) vector<Point3f>;
%template(vector_string) vector<string>;
%template(vector_float) vector<float>;
%template(vector_fieldline) vector<Fieldline>;
%template(vector_int) vector<int>;
