%module CCMC
%include "std_string.i"
%include "std_vector.i"
%include "cpointer.i"
%import "ccmc/cdf.h"
/* %include "../doc/xml/kameleonswigdocs.i" */
%include "typemaps.i"



%rename(interpolate_dc) *::interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& dc0, float& dc1, float& dc2);
%rename(interpolate_dc) *::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2);
%apply float *OUTPUT {float& dc0, float& dc1, float& dc2};



%{
#include <ccmc/FileReader.h>
#include <ccmc/GeneralFileReader.h>
#include <ccmc/CDFFileReader.h>
#include <ccmc/TimeInterpolator.h>
#include <ccmc/Kameleon.h>
#include <ccmc/Interpolator.h>
#include <ccmc/KameleonInterpolator.h>
#include <ccmc/Tracer.h>
#include <ccmc/Fieldline.h>
#include <ccmc/Point3f.h>
#include <ccmc/Attribute.h>
#include <ccmc/Model.h>
#include <ccmc/BATSRUS.h>
#include <ccmc/ENLIL.h>
#include <ccmc/MAS.h>
#include <ccmc/OpenGGCM.h>
#include <ccmc/SWMFIono.h>
#include <ccmc/BATSRUSInterpolator.h>
#include <ccmc/ENLILInterpolator.h>
#include <ccmc/MASInterpolator.h>
#include <ccmc/OpenGGCMInterpolator.h>
#include <ccmc/SWMFIonoInterpolator.h>
#include <ccmc/LFMInterpolator.h>
#include <ccmc/Polyhedron.h>
#include <ccmc/CCMCTime.h>
#include <ccmc/Point.h>
#include <ccmc/Vector.h>
using namespace std;
using namespace ccmc;

%}



%include <ccmc/FileReader.h>
%include <ccmc/GeneralFileReader.h>
%include <ccmc/CDFFileReader.h>
%include <ccmc/TimeInterpolator.h>
%include <ccmc/Kameleon.h>
%include <ccmc/Interpolator.h>
%include <ccmc/KameleonInterpolator.h>
%include <ccmc/Tracer.h>
%include <ccmc/Fieldline.h>
%include <ccmc/Point3f.h>
%include <ccmc/Attribute.h>
%include <ccmc/Model.h>
%include <ccmc/OpenGGCM.h>
%include <ccmc/BATSRUS.h>
%include <ccmc/ENLIL.h>
%include <ccmc/MAS.h>
%include <ccmc/OpenGGCM.h>
%include <ccmc/SWMFIono.h>
%include <ccmc/BATSRUSInterpolator.h>
%include <ccmc/ENLILInterpolator.h>
%include <ccmc/MASInterpolator.h>
%include <ccmc/OpenGGCMInterpolator.h>
%include <ccmc/SWMFIonoInterpolator.h>
%include <ccmc/LFMInterpolator.h>
%include <ccmc/Polyhedron.h>
%include <ccmc/CCMCTime.h>
%include <ccmc/Point.h>
%include <ccmc/Vector.h>

using namespace ccmc;
using namespace std;
typedef ccmc::Fieldline Fieldline;
typedef ccmc::Point3f Point3f;
typedef ccmc::Point<T> Point<T>;
typedef ccmc::Vector<T> Vector<T>;
typedef ccmc::Polyhedron<T> Polyhedron<T>;

%template(point_float) ccmc::Point<float>;
%template(vector_point_float) vector<ccmc::Point<float> >;
%template(polyhedron_float) ccmc::Polyhedron<float>;

%template(eucl_vector_float) ccmc::Vector<float>;
%template(vector_eucl_vector_float) vector<ccmc::Vector<float> >;
%template(vector_point3f) vector<Point3f>;
%template(vector_string) vector<string>;
%template(vector_float) vector<float>;
%template(vector_fieldline) vector<Fieldline>;
%template(vector_int) vector<int>;
%template(vector_time) vector<Time>;
