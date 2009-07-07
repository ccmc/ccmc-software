#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT([ccmc_derived], [1.0.0], [David.H.Berrios@nasa.gov], [kameleon-plus])
AM_INIT_AUTOMAKE([-Wall -Werror no-dependencies dist-bzip2 dist-lzma dist-zip])
AC_CONFIG_SRCDIR([src/ccmc/Kameleon.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LIBTOOL

# FIXME: Replace `main' with a function in `-lc':
AC_ARG_ENABLE([universal],
              AC_HELP_STRING([--enable-universal],
                             [build Universal binaries on OSX]),
              [if test x"$host_cpu" = x"powerpc"; then
                CPPFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk"
               fi

               CPPFLAGS="-arch i386 -arch ppc -arch x86_64 -arch ppc64 $CPPFLAGS"])
AC_ARG_WITH([kameleon],
            [AC_HELP_STRING([--with-kameleon=<dir>],
                            [specify the location of the kameleon library])],
            [LDFLAGS="-L$withval $LDFLAGS"])
AC_ARG_WITH([cdf],
            [AC_HELP_STRING([--with-cdf=<dir>],
                            [specify the location of the CDF library])],
            [LDFLAGS="-L$withval $LDFLAGS"])                
AC_ARG_WITH([cdf-includes],
			[AC_HELP_STRING([--with-cdf-includes=<dir>],
                            [specify the location of the cdf header files])],
            [CPPFLAGS="-I$withval $CPPFLAGS"])

AC_ARG_WITH([boost-includes],
			[AC_HELP_STRING([--with-boost-includes=<dir>],
                            [specify the location of the boost header files])],
            [CPPFLAGS="-I$withval $CPPFLAGS"])
    

AC_CHECK_LIB([cdf],
             [main],,
             [AC_MSG_ERROR([The CDF library was not found. Specify the directory using the --with-cdf option],1)])
AC_CHECK_LIB([kameleon],[main],,
             [AC_MSG_ERROR([The kameleon library was not found. Specify the directory using the --with-kameleon option],1)])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h])

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
        src/ccmc/kameleon-plus.pc
    ]
)
AC_OUTPUT
           

