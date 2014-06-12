bin_PROGRAMS = generalfilereader_compat_f kameleon_compat_f timeinterp_compat_f #tracer_compat_f 

generalfilereader_compat_f_SOURCES = GeneralFileReader_compatibility_test_f.f
generalfilereader_compat_f_LDADD = @builddir@/../../ccmc/libkameleon_plus.la \
			@builddir@/../../ccmc/wrappers/fortran/libkameleon_plus_wrapper_fortran.la \
			@builddir@/../../ccmc/wrappers/c/libkameleon_plus_wrapper_c.la
generalfilereader_compat_f_LDFLAGS = -lstdc++

#tracer_compat_f_SOURCES = Tracer_test_f.f
#tracer_compat_f_LDADD = @builddir@/../../ccmc/libkameleon-plus.la \
#			@builddir@/../../ccmc/wrappers/fortran/libkameleon-plus-wrapper-fortran.la \
#			@builddir@/../../ccmc/wrappers/c/libkameleon-plus-wrapper-c.la
#tracer_compat_f_LDFLAGS = -lstdc++

kameleon_compat_f_SOURCES = Kameleon_compatibility_test_f.f
kameleon_compat_f_LDADD = @builddir@/../../ccmc/libkameleon_plus.la \
			@builddir@/../../ccmc/wrappers/fortran/libkameleon_plus_wrapper_fortran.la \
			@builddir@/../../ccmc/wrappers/c/libkameleon_plus_wrapper_c.la
kameleon_compat_f_LDFLAGS = -lstdc++

timeinterp_compat_f_SOURCES = timeinterp_compatibility_test_f.f
timeinterp_compat_f_LDADD = @builddir@/../../ccmc/libkameleon_plus.la \
			@builddir@/../../ccmc/wrappers/fortran/libkameleon_plus_wrapper_fortran.la \
			@builddir@/../../ccmc/wrappers/c/libkameleon_plus_wrapper_c.la
timeinterp_compat_f_LDFLAGS = -lstdc++
