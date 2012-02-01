#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([kameleon-plus], [0.1.30], [David.H.Berrios@nasa.gov], [kameleon-plus])
AM_INIT_AUTOMAKE([-Wall -Werror no-dependencies tar-ustar dist-bzip2 dist-zip])
AC_CONFIG_SRCDIR([src/ccmc/Kameleon.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_SUBST([RELEASE],[0.1.30])
AC_LANG([C++])
# Checks for programs.
AC_PROG_CXX
AC_PROG_F77
AC_PROG_CC
AM_PROG_LIBTOOL
AM_PROG_CC_C_O
AM_PATH_PYTHON([2.5],,[:])
AX_BOOST_BASE([1.40])
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_THREAD

AC_ARG_ENABLE([universal],
              AC_HELP_STRING([--enable-universal],
                             [build Universal binaries on OSX]),
              [if test x"$host_cpu" = x"powerpc"; then
                CPPFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk"
               fi

               CXXFLAGS="-arch i386 -arch ppc -arch x86_64 $CXXFLAGS"
               CFLAGS="-arch i386 -arch ppc -arch x86_64 $CFLAGS"])

ENABLE_C=false
AC_ARG_ENABLE([c],
	AC_HELP_STRING([--enable-c],
				   [Build the C-language wrappers]),
				   [if test "$enableval" = "yes"; then
				   		AC_SUBST([FILEREADER_C], ['filereader_compat${EXEEXT}'])
				   		AC_SUBST([C_WRAPPER],['c'])
				   		ENABLE_C=true
				   		
				   	fi
				   ],)
				   
AC_ARG_ENABLE([fortran],
	AC_HELP_STRING([--enable-fortran],
				   [Build the fortran (F90 compatible) wrappers]),
				   [if test "$enableval" = "yes"; then
				   		if test x$ENABLE_C = xtrue; then
				   			AC_SUBST([FILEREADER_FORTRAN], ['filereader_compat_f${EXEEXT}'])
				   			AC_SUBST([FORTRAN_WRAPPER],['fortran'])
				   		else
				   			AC_MSG_ERROR([The C-language wrappers must also be enabled when enabling the Fortran-language wrappers. Use the --enable-c option],1)
				   		fi
				   		
				   		if test x"$F77" == x""; then
				   			AC_MSG_ERROR([F77 must point to a valid Fortran compiler. Set the environment variable prior to running configure])
				   		fi
				   	fi
				   ],)

ENABLE_JAVA=false
AC_ARG_ENABLE([java],
	AC_HELP_STRING([--enable-java],
				[Build the Java wrappers]),
				[if test "$enableval" = "yes"; then
					AX_PROG_JAVAC
					AX_JNI_INCLUDE_DIR
					#AX_PROG_JAR
					AC_SUBST(JNI_INCLUDE_DIRS)
					AC_SUBST([JAVA_WRAPPER],['java'])
					for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
				    do
				           CPPFLAGS="$CPPFLAGS -I$JNI_INCLUDE_DIR"
				    done
				    ENABLE_JAVA=true
					
				 fi
				],)

ENABLE_PYTHON=false
AC_ARG_ENABLE([python],
	AC_HELP_STRING([--enable-python],
				[Build the Python wrappers]),
				[if test "$enableval" = "yes"; then
					AC_SUBST([PYTHON_WRAPPER],['python'])
					ENABLE_PYTHON=true
				 fi
				],)

ENABLE_HDF5=false
AC_ARG_ENABLE([hdf5],
	AC_HELP_STRING([--enable-hdf5],
				[Build HDF5 support]),
				[if test "$enableval" = "yes"; then
					AC_SUBST([PYTHON_WRAPPER],['hdf5'])
					ENABLE_HDF5=true
				 fi
				],)

AC_ARG_WITH([cdf],
            [AC_HELP_STRING([--with-cdf=<dir>],
                            [specify the location of the CDF library])],
            [LDFLAGS="-L$withval $LDFLAGS"])                
AC_ARG_WITH([boost-includes],
			[AC_HELP_STRING([--with-boost-includes=<dir>],
                            [specify the location of the boost header files. This is the directory that contains the boost directory.])],
            [CPPFLAGS="-I$withval $CPPFLAGS"])
            
if $ENABLE_HDF5; then
	AC_ARG_WITH([hdf5],
			[AC_HELP_STRING([--with-hdf5=<dir>],
							[specify the location of the HDF5 library])],
			[LDFLAGS="-L$withval $LDFLAGS"])
	AC_ARG_WITH([hdf5-includes],
			[AC_HELP_STRING([--with-hdf5-includes=<dir>],
							[specify the location of the HDF5 header files. This is the directory that contains the H5Cpp.h file.])],
			[CPPFLAGS="-I$withval $CPPFLAGS"])
fi

HAVE_PYTHON= false;
AC_PYTHON_INCLUDES
AC_CHECK_LIB([cdf],
             [main],,
             [AC_MSG_ERROR([The CDF library was not found. Specify the directory using the --with-cdf option],1)])

if $ENABLE_HDF5; then
	AC_CHECK_LIB([hdf5_cpp],
			 [main],,
			 [AC_MSG_ERROR([The HDF5 library was not found. Specify the directory using the --with-hdf5 option],1)])
fi
# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER([string.h])
AC_CHECK_HEADER([boost/unordered_map.hpp],,
					AC_MSG_ERROR(Cannot find boost/unordered_map.hpp. Specify the boost include directory using --with-boost-includes=<dir>))
#AC_CHECK_HEADER([H5Cpp.h],,
#					AC_MSG_ERROR(Cannot find H5Cpp.h. Specify the HDF5 include directory using --with-hdf5-includes=<dir>))
# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/ccmc/Makefile])
AC_CONFIG_FILES([src/ccmc/wrappers/Makefile])
AC_CONFIG_FILES([src/ccmc/wrappers/fortran/Makefile])
AC_CONFIG_FILES([src/ccmc/wrappers/java/Makefile])
AC_CONFIG_FILES([src/ccmc/wrappers/python/Makefile])
AC_CONFIG_FILES([src/example/Makefile])
AC_CONFIG_FILES([src/example/c/Makefile])
AC_CONFIG_FILES([src/example/c++/Makefile])
AC_CONFIG_FILES([src/example/fortran/Makefile])

AC_MSG_NOTICE( )
AC_MSG_NOTICE([------------------------------------------------------------])
AC_MSG_NOTICE([|                        Summary                           |])
AC_MSG_NOTICE([------------------------------------------------------------])
AC_MSG_NOTICE([ LDFLAGS: $LDFLAGS])
AC_MSG_NOTICE([ LIBS: $LIBS])
AC_MSG_NOTICE([ CFLAGS: $CFLAGS]) 
AC_MSG_NOTICE([ CXXFLAGS: $CXXFLAGS])
AC_MSG_NOTICE([ JAVAC: $JAVAC])
AC_MSG_NOTICE([ F77: $F77])
AC_MSG_NOTICE([ JNI_INCLUDE_DIRS: $JNI_INCLUDE_DIRS])
AC_MSG_NOTICE([ Python: $PYTHON_VERSION])
AC_MSG_NOTICE([ pyexecdir: $pyexecdir])
AC_MSG_NOTICE([ pkgpyexecdir: $pkgpyexecdir])
AC_MSG_NOTICE()
AC_MSG_NOTICE()



if test x$ENABLE_FORTRAN = xtrue; then
	AC_MSG_NOTICE([ Fortran wrappers: included])
else
	AC_MSG_NOTICE([ Fortran wrappers: not included])
fi

if test x$ENABLE_C = xtrue; then
	AC_MSG_NOTICE([ C wrappers: included])
else
	AC_MSG_NOTICE([ C wrappers: not included])
fi

case $host_os in
	darwin*) 	JNI_EXTRA_LDFLAGS="-shrext .jnilib" ;;
	*) 			JNI_EXTRA_LDFLAGS="" ;;
esac

AC_SUBST(JNI_EXTRA_LDFLAGS)
	
if $ENABLE_JAVA; then
	AC_MSG_NOTICE([ Java wrappers: included])

else
	AC_MSG_NOTICE([ Java wrappers: not included])
fi

if $ENABLE_PYTHON; then
	AC_MSG_NOTICE([ Python wrappers: included])
	AC_MSG_NOTICE([ Python include dir: $PYTHON_INCLUDES])

else
	AC_MSG_NOTICE([ Python wrappers: not included])
fi

if $ENABLE_HDF5; then
	AC_MSG_NOTICE([ HDF5 support: included])
else
	AC_MSG_NOTICE([ HDF5 support: not included])
fi
AC_MSG_NOTICE([------------------------------------------------------------])
AC_MSG_NOTICE( )
AC_OUTPUT

           

