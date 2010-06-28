bin_PROGRAMS = filereader_compat_f tracer_compat_f

filereader_compat_f_SOURCES = FileReader_compatibility_test_f.f
filereader_compat_f_LDADD = @builddir@/../../ccmc/libkameleon-plus.la \
			@builddir@/../../ccmc/wrappers/fortran/libkameleon-plus-wrapper-fortran.la \
			@builddir@/../../ccmc/wrappers/c/libkameleon-plus-wrapper-c.la
filereader_compat_f_LDFLAGS = -lstdc++

tracer_compat_f_SOURCES = Tracer_test_f.f
tracer_compat_f_LDADD = @builddir@/../../ccmc/libkameleon-plus.la \
			@builddir@/../../ccmc/wrappers/fortran/libkameleon-plus-wrapper-fortran.la \
			@builddir@/../../ccmc/wrappers/c/libkameleon-plus-wrapper-c.la
tracer_compat_f_LDFLAGS = -lstdc++

