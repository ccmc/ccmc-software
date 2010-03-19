bin_PROGRAMS = derived kameleon-compat coord_test open_ggcm \
				magnetogram  filereader_compat enlil $(FILEREADER_FORTRAN) \
				$(TRACER) $(TRACER_C)
EXTRA_PROGRAMS = filereader_compat_f tracer tracer_c

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
filereader_compat_f_LDADD = @builddir@/../ccmc/libkameleon-interpolator.la
filereader_compat_f_LDFLAGS = -lstdc++

coord_test_SOURCES = coordinate_transformation_test.cpp
coord_test_LDFLAGS = @builddir@/../ccmc/libkameleon-interpolator.la
coord_test_CXXFLAGS = -I@srcdir@/.. -I@srcdir@/../ccmc

open_ggcm_SOURCES = open_ggcm_test.cpp
open_ggcm_LDFLAGS = @builddir@/../ccmc/libkameleon-interpolator.la
open_ggcm_CXXFLAGS = -I@srcdir@/..

enlil_SOURCES = enlil_test.cpp
enlil_LDFLAGS = @builddir@/../ccmc/libkameleon-interpolator.la
enlil_CXXFLAGS = -I@srcdir@/..

magnetogram_SOURCES = magnetogram_test.cpp
magnetogram_LDFLAGS = @builddir@/../ccmc/libkameleon-interpolator.la
magnetogram_CXXFLAGS = -I@srcdir@/..

tracer_SOURCES = tracer_prog.cpp
tracer_CXXFLAGS = -I@srcdir@/..
tracer_LDADD = @builddir@/../ccmc/libkameleon-tracer.la

tracer_c_SOURCES = tracer_prog.c
tracer_c_CFLAGS = -I@srcdir@/..
tracer_c_LDADD = @builddir@/../ccmc/libkameleon-tracer.la -lstdc++