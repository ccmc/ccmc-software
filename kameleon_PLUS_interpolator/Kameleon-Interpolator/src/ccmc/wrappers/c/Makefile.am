pkgincludedir = $(includedir)/ccmc/wrappers/c
pkginclude_HEADERS = FileReader_c.h

lib_LTLIBRARIES = libkameleon-wrapper-c.la
libkameleon_wrapper_c_la_SOURCES = FileReader_c.h FileReader_c.cpp Kameleon_c.h Kameleon_c.cpp
libkameleon_wrapper_c_la_LDFLAGS = -release @RELEASE@
libkameleon_wrapper_c_la_CXXFLAGS = -fPIC
libkameleon_wrapper_c_la_CPPFLAGS = -I@srcdir@/../../..
#kameleon_wrapper_python_la_LIBTOOLFLAGS = --tag=disable-static