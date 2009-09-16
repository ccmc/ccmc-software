#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT([kameleon-interpolator], [0.1.0], [David.H.Berrios@nasa.gov], [kameleon-interpolator])
AM_INIT_AUTOMAKE([-Wall -Werror no-dependencies tar-ustar dist-bzip2 dist-lzma dist-zip])
AC_CONFIG_SRCDIR([src/ccmc/Kameleon.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])


AC_LANG([C++])
# Checks for programs.
AC_PROG_CXX
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
AC_ARG_WITH([boost],
            [AC_HELP_STRING([--with-boost=<dir>],
                            [specify the location of the boost program_options library])],
            [LDFLAGS="-L$withval $LDFLAGS"])
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
AC_OUTPUT
           

