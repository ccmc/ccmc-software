bin_PROGRAMS = kameleon coord_test open_ggcm adapt3d \
				magnetogram  enlil tracer integrator mas \
				cdfreader				
# h5reader
				
kameleon_SOURCES = kameleon_prog.cpp
kameleon_CPPFLAGS = -I@srcdir@/../..
kameleon_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)

coord_test_SOURCES = coordinate_transformation_test.cpp
coord_test_CPPFLAGS = -I@srcdir@/../..
coord_test_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)

open_ggcm_SOURCES = open_ggcm_test.cpp
open_ggcm_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)
open_ggcm_CPPFLAGS = -I@srcdir@/../..

enlil_SOURCES = enlil_test.cpp
enlil_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)
enlil_CPPFLAGS = -I@srcdir@/../..

magnetogram_SOURCES = magnetogram_test.cpp
magnetogram_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)
magnetogram_CPPFLAGS = -I@srcdir@/../..

tracer_SOURCES = tracer_prog.cpp
tracer_CPPFLAGS = -I@srcdir@/../..
tracer_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_THREAD_LIB)

integrator_SOURCES = integrator_prog.cpp
integrator_CPPFLAGS = -I@srcdir@/../..
integrator_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)

mas_SOURCES = mas_test.cpp
mas_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)
mas_CPPFLAGS = -I@srcdir@/../..

adapt3d_SOURCES = adapt3d_test.cpp
adapt3d_LDADD = @builddir@/../../ccmc/libkameleon-plus.la -lprofiler $(BOOST_THREAD_LIB)
adapt3d_CPPFLAGS = -I@srcdir@/../..

#h5reader_SOURCES = HDF5Reader.cpp
#h5reader_LDADD = @builddir@/../../ccmc/libkameleon-plus.la
#h5reader_CPPFLAGS = -I@srcdir@/../..

cdfreader_SOURCES = CDFReader.cpp
cdfreader_LDADD = @builddir@/../../ccmc/libkameleon-plus.la $(BOOST_THREAD_LIB)
cdfreader_CPPFLAGS = -I@srcdir@/../..