bin_PROGRAMS = derived kameleon-compat#imf
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

