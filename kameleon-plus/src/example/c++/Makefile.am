bin_PROGRAMS = kameleon coord_test open_ggcm adapt3d \
				magnetogram  enlil tracer integrator mas
				
kameleon_SOURCES = kameleon_prog.cpp
kameleon_CPPFLAGS = -I@srcdir@/../..
kameleon_LDADD = @builddir@/../../ccmc/libkameleon-plus.la

coord_test_SOURCES = coordinate_transformation_test.cpp
coord_test_CPPFLAGS = -I@srcdir@/../..
coord_test_LDADD = @builddir@/../../ccmc/libkameleon-plus.la

open_ggcm_SOURCES = open_ggcm_test.cpp
open_ggcm_LDADD = @builddir@/../../ccmc/libkameleon-plus.la
open_ggcm_CPPFLAGS = -I@srcdir@/../..

enlil_SOURCES = enlil_test.cpp
enlil_LDADD = @builddir@/../../ccmc/libkameleon-plus.la
enlil_CPPFLAGS = -I@srcdir@/../..

magnetogram_SOURCES = magnetogram_test.cpp
magnetogram_LDADD = @builddir@/../../ccmc/libkameleon-plus.la
magnetogram_CPPFLAGS = -I@srcdir@/../..

tracer_SOURCES = tracer_prog.cpp
tracer_CPPFLAGS = -I@srcdir@/../..
tracer_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_PROGRAM_OPTIONS_LIB)

integrator_SOURCES = integrator_prog.cpp
integrator_CPPFLAGS = -I@srcdir@/../..
integrator_LDADD = @builddir@/../../ccmc/libkameleon-plus.la

mas_SOURCES = mas_test.cpp
mas_LDADD = @builddir@/../../ccmc/libkameleon-plus.la
mas_CPPFLAGS = -I@srcdir@/../..

adapt3d_SOURCES = adapt3d_test.cpp
adapt3d_LDADD = @builddir@/../../ccmc/libkameleon-plus.la
adapt3d_CPPFLAGS = -I@srcdir@/../..