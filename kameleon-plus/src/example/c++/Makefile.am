bin_PROGRAMS = kameleon coord_test open_ggcm adapt3d \
				magnetogram  enlil tracer integrator mas \
				cdfreader swmf_iono minmax lfm time_interp 			
# h5reader
				
kameleon_SOURCES = kameleon_prog.cpp
kameleon_CPPFLAGS = -I@srcdir@/../..
kameleon_LDADD = @builddir@/../../ccmc/libkameleon_plus.la

coord_test_SOURCES = coordinate_transformation_test.cpp
coord_test_CPPFLAGS = -I@srcdir@/../..
coord_test_LDADD = @builddir@/../../ccmc/libkameleon_plus.la

open_ggcm_SOURCES = open_ggcm_test.cpp
open_ggcm_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
open_ggcm_CPPFLAGS = -I@srcdir@/../..

enlil_SOURCES = enlil_test.cpp
enlil_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
enlil_CPPFLAGS = -I@srcdir@/../..

magnetogram_SOURCES = magnetogram_test.cpp
magnetogram_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
magnetogram_CPPFLAGS = -I@srcdir@/../..

lfm_SOURCES = lfm_test.cpp
lfm_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
lfm_CPPFLAGS = -I@srcdir@/../..

tracer_SOURCES = tracer_prog.cpp
tracer_CPPFLAGS = -I@srcdir@/../..
tracer_LDADD = @builddir@/../../ccmc/libkameleon_plus.la

integrator_SOURCES = integrator_prog.cpp
integrator_CPPFLAGS = -I@srcdir@/../..
integrator_LDADD = @builddir@/../../ccmc/libkameleon_plus.la

minmax_SOURCES = minmax_prog.cpp
minmax_CPPFLAGS = -I@srcdir@/../..
minmax_LDADD = @builddir@/../../ccmc/libkameleon_plus.la

mas_SOURCES = mas_test.cpp
mas_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
mas_CPPFLAGS = -I@srcdir@/../..

adapt3d_SOURCES = adapt3d_test.cpp
adapt3d_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
adapt3d_CPPFLAGS = -I@srcdir@/../..

#h5reader_SOURCES = HDF5Reader.cpp
#h5reader_LDADD = @builddir@/../../ccmc/libkameleon-plus.la
#h5reader_CPPFLAGS = -I@srcdir@/../..

cdfreader_SOURCES = CDFReader.cpp
cdfreader_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
cdfreader_CPPFLAGS = -I@srcdir@/../..

swmf_iono_SOURCES = swmf_iono_test.cpp
swmf_iono_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
swmf_iono_CPPFLAGS = -I@srcdir@/../..

time_interp_SOURCES = time_interp.cpp
time_interp_LDADD = @builddir@/../../ccmc/libkameleon_plus.la
time_interp_CPPFLAGS = -I@srcdir@/../..
