bin_PROGRAMS = kameleon-compat filereader_compat tracer_c

kameleon_compat_SOURCES = Kameleon_compatibility_test.c
kameleon_compat_LDADD = @builddir@/../../ccmc/libkameleon.la @builddir@/../../ccmc/wrappers/c/libkameleon-wrapper-c.la
kameleon_compat_LDFLAGS = -lstdc++
kameleon_compat_CFLAGS = -I@srcdir@/../..

filereader_compat_SOURCES = FileReader_compatibility_test.c
filereader_compat_LDADD = @builddir@/../../ccmc/libkameleon.la @builddir@/../../ccmc/wrappers/c/libkameleon-wrapper-c.la
filereader_compat_LDFLAGS = -lstdc++
filereader_compat_CFLAGS = -I@srcdir@/../..

tracer_c_SOURCES = tracer_prog.c
tracer_c_LDADD = @builddir@/../../ccmc/libkameleon.la @builddir@/../../ccmc/wrappers/c/libkameleon-wrapper-c.la
tracer_c_LDFLAGS = -lstdc++
tracer_c_CFLAGS = -I@srcdir@/../..