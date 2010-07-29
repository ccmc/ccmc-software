bin_PROGRAMS = derived coord_test open_ggcm \
				magnetogram  enlil tracer
				
derived_SOURCES = derived_prog.cpp
derived_CPPFLAGS = -I@srcdir@/../..
derived_LDADD = @builddir@/../../ccmc/libkameleon-plus.la

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
tracer_LDADD = @builddir@/../../ccmc/libkameleon-plus.la