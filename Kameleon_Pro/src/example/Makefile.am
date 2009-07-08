bin_PROGRAMS = reader derived kameleon-compat#imf
derived_SOURCES = derived_prog.cpp
#derived_LDFLAGS = -L@srcdir@/../ccmc -lkameleon-interpolator -lkameleon -lcdf
derived_CPPFLAGS = -I@srcdir@/.. -I@srcdir@/../ccmc
derived_LDADD = @srcdir@/../ccmc/libkameleon-interpolator.la
reader_SOURCES = KameleonTest.cpp
reader_CPPFLAGS = -I@srcdir@/.. -I@srcdir@/../ccmc
reader_LDADD = @srcdir@/../ccmc/libkameleon-interpolator.la


#imf_SOURCES = IMFTest.cpp
#imf_CPPFLAGS = -I@srcdir@/..
#imf_LDADD = @srcdir@/../ccmc/libkameleon-plus.la

kameleon_compat_SOURCES = Kameleon_compatibility_test.c
kameleon_compat_LDFLAGS = -L@srcdir@/../ccmc -lstdc++ -lkameleon-interpolator
kameleon_compat_CPPFLAGS = -I@srcdir@/..

