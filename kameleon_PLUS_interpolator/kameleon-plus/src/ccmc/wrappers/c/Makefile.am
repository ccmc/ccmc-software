lib_LTLIBRARIES = libkameleon_plus_wrapper_c.la
libkameleon_plus_wrapper_c_la_SOURCES = TimeInterpolator_c.h TimeInterpolator_c.cpp GeneralFileReader_c.h GeneralFileReader_c.cpp Kameleon_c.h Kameleon_c.cpp Tracer_c.h Tracer_c.cpp
libkameleon_plus_wrapper_c_la_LDFLAGS = -release @RELEASE@ @builddir@/../../../ccmc/libkameleon_plus.la
libkameleon_plus_wrapper_c_la_CXXFLAGS = -fPIC
libkameleon_plus_wrapper_c_la_CPPFLAGS = -I@srcdir@/../../..

pkgincludedir = $(includedir)/ccmc/wrappers/c
pkginclude_HEADERS = GeneralFileReader_c.h Kameleon_c.h Tracer_c.h TimeInterpolator_c.h
