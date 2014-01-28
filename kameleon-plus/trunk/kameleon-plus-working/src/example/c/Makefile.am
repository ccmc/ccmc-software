bin_PROGRAMS = kameleon-compat tracer_c time_interp_c#filereader_compat tracer_c

kameleon_compat_SOURCES = Kameleon_compatibility_test.c
kameleon_compat_LDADD = @builddir@/../../ccmc/libkameleon_plus.la @builddir@/../../ccmc/wrappers/c/libkameleon_plus_wrapper_c.la
kameleon_compat_LDFLAGS = -lstdc++
kameleon_compat_CFLAGS = -I@srcdir@/../..

tracer_c_SOURCES = tracer_prog.c
tracer_c_LDADD = @builddir@/../../ccmc/libkameleon_plus.la @builddir@/../../ccmc/wrappers/c/libkameleon_plus_wrapper_c.la
tracer_c_LDFLAGS = -lstdc++
tracer_c_CFLAGS = -I@srcdir@/../..

time_interp_c_SOURCES = time_interpolator_test.c
time_interp_c_LDADD = @builddir@/../../ccmc/libkameleon_plus.la @builddir@/../../ccmc/wrappers/c/libkameleon_plus_wrapper_c.la
time_interp_c_LDFLAGS = -lstdc++
time_interp_c_CFLAGS = -I@srcdir@/../..
