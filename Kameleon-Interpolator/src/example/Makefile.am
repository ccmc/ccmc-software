bin_PROGRAMS = derived kameleon-compat coord_test open_ggcm magnetogram  filereader_compat filereader_compat_f #imf 
derived_SOURCES = derived_prog.cpp
#derived_LDFLAGS = -L@srcdir@/../ccmc -lkameleon-interpolator -lkameleon -lcdf
derived_CXXFLAGS = -I@srcdir@/.. -I@srcdir@/../ccmc
derived_LDADD = @builddir@/../ccmc/libkameleon-interpolator.la



#imf_SOURCES = IMFTest.cpp
#imf_CPPFLAGS = -I@srcdir@/..
#imf_LDADD = @builddir@/../ccmc/libkameleon-plus.la

kameleon_compat_SOURCES = Kameleon_compatibility_test.c
kameleon_compat_LDFLAGS = -L@builddir@/../ccmc -lstdc++ -lkameleon-interpolator
kameleon_compat_CFLAGS = -I@srcdir@/..

filereader_compat_SOURCES = FileReader_compatibility_test.c
filereader_compat_LDFLAGS = -L@builddir@/../ccmc -lstdc++ -lkameleon-interpolator
filereader_compat_CFLAGS = -I@srcdir@/..

filereader_compat_f_SOURCES = FileReader_compatibility_test_f.f
filereader_compat_f_LDADD = @builddir@/../ccmc/libkameleon-interpolator.la @FLIBS@
#filereader_compat_f_LDFLAGS = -L@builddir@/../ccmc -lstdc++ -lkameleon-interpolator
#filereader_compat_f_CFLAGS = -I@srcdir@/..

coord_test_SOURCES = coordinate_transformation_test.cpp
coord_test_LDFLAGS = @builddir@/../ccmc/libkameleon-interpolator.la
coord_test_CXXFLAGS = -I@srcdir@/.. -I@srcdir@/../ccmc

open_ggcm_SOURCES = open_ggcm_test.cpp
open_ggcm_LDFLAGS = @builddir@/../ccmc/libkameleon-interpolator.la
open_ggcm_CXXFLAGS = -I@srcdir@/..

magnetogram_SOURCES = magnetogram_test.cpp
magnetogram_LDFLAGS = @builddir@/../ccmc/libkameleon-interpolator.la
magnetogram_CXXFLAGS = -I@srcdir@/..