#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([kameleon-interpolator], [0.1.2], [David.H.Berrios@nasa.gov], [kameleon-interpolator])
AM_INIT_AUTOMAKE([-Wall -Werror no-dependencies tar-ustar dist-bzip2 dist-zip])
AC_CONFIG_SRCDIR([src/ccmc/Kameleon.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])


AC_LANG([C++])
# Checks for programs.
AC_PROG_CXX
AC_PROG_F77
AC_PROG_CC
AM_PROG_LIBTOOL
AM_PROG_CC_C_O

# FIXME: Replace `main' with a function in `-lc':
AC_ARG_ENABLE([universal],
              AC_HELP_STRING([--enable-universal],
                             [build Universal binaries on OSX]),
              [if test x"$host_cpu" = x"powerpc"; then
                CXXFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk"
               fi

               CXXFLAGS="-arch i386 -arch ppc -arch x86_64 $CXXFLAGS"])
FORTRAN_EXAMPLES=false
AC_ARG_ENABLE([fortran-examples],
	AC_HELP_STRING([--enable-fortran-examples],
				   [Build the fortran (F77) examples]),
				   [if test "$enableval" = "yes"; then
				   		AC_SUBST([FILEREADER_FORTRAN], ['filereader_compat_f${EXEEXT}'])
				   		FORTRAN_EXAMPLES=true
				   	fi
				   ],)
			
TRACER_ENABLED=false	   
AC_ARG_ENABLE([tracer],
	AC_HELP_STRING([--enable-tracer],
				   [Build the Kameleon fieldline tracer]),
				   [if test "$enableval" = "yes"; then
				   		AC_SUBST([TRACER_LIB], ['libkameleon-tracer.la'])
				   		AC_SUBST([TRACER_HEADERS],['Kameleon-Tracer.h Point3f.h'])
				   		AC_SUBST([TRACER],['tracer'])
				   		AC_SUBST([TRACER_C],['tracer_c'])
				   		TRACER_ENABLED=true
				   	fi
				   ],)
#AM_CONDITIONAL([FORTRAN_EXAMPLES], [testx$fortran-examples = xtrue])
#AC_ARG_WITH([boost],
#            [AC_HELP_STRING([--with-boost=<dir>],
#                            [specify the location of the boost program_options library])],
#            [LDFLAGS="-L$withval $LDFLAGS"])
AC_ARG_WITH([cdf],
            [AC_HELP_STRING([--with-cdf=<dir>],
                            [specify the location of the CDF library])],
            [LDFLAGS="-L$withval $LDFLAGS"])                
AC_ARG_WITH([boost-includes],
			[AC_HELP_STRING([--with-boost-includes=<dir>],
                            [specify the location of the boost header files])],
            [CXXFLAGS="-I$withval $CXXFLAGS"])


AC_CHECK_LIB([cdf],
             [main],,
             [AC_MSG_ERROR([The CDF library was not found. Specify the directory using the --with-cdf option],1)])
#AC_CHECK_LIB([boost_program_options],[main],[C,
#			[AC_MSG_ERROR([The boost program_options library was not found.  Specify the directory using the --with-boost option],1)])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER([string.h])
AC_CHECK_HEADER([boost/unordered_map.hpp],,
					AC_MSG_ERROR(Cannot find boost/unordered_map.hpp. Specify the boost include directory using --with-boost-includes=<dir>))
# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])


AC_CONFIG_FILES(
    [
        Makefile
        src/Makefile
        src/example/Makefile
        src/ccmc/Makefile
        
    ]
)

AC_MSG_NOTICE( )
AC_MSG_NOTICE([------------------------------------------------------------])
AC_MSG_NOTICE([|                        Summary                           |])
AC_MSG_NOTICE([------------------------------------------------------------])
AC_MSG_NOTICE([ LIBS: $LIBS])
AC_MSG_NOTICE([ CFLAGS: $CFLAGS]) 
AC_MSG_NOTICE([ CXXFLAGS: $CXXFLAGS])

if test x$TRACER_ENABLED = xtrue; then
	AC_MSG_NOTICE([ Tracer: included])
else
	AC_MSG_NOTICE([ Tracer: not included])
fi

if test x$FORTRAN_EXAMPLES = xtrue; then
	AC_MSG_NOTICE([ Fortran examples: included])
else
	AC_MSG_NOTICE([ Fortran examples: not included])
fi
AC_MSG_NOTICE([------------------------------------------------------------])
AC_MSG_NOTICE( )
AC_OUTPUT

           

