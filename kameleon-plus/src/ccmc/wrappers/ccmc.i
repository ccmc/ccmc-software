%module CCMC
%include "std_string.i"
%include "std_vector.i"
%include "cpointer.i"
%import "ccmc/cdf.h"


%{
#include <ccmc/FileReader.h>
#include <ccmc/GeneralFileReader.h>
#include <ccmc/CDFFileReader.h>
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
#include <ccmc/CCMCTime.h>
using namespace std;
using namespace ccmc;


%}

%include <ccmc/FileReader.h>
%include <ccmc/GeneralFileReader.h>
%include <ccmc/CDFFileReader.h>
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
%include <ccmc/CCMCTime.h>

using namespace ccmc;
using namespace std;
typedef ccmc::Fieldline Fieldline;
typedef ccmc::Point3f Point3f;


%template(vector_point3f) vector<Point3f>;
%template(vector_string) vector<string>;
%template(vector_float) vector<float>;
%template(vector_fieldline) vector<Fieldline>;
%template(vector_int) vector<int>;
