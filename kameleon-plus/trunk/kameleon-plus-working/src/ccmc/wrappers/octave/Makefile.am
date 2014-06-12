#CCMC_OCTAVE = CCMC.oct
#CCMCdir = $(datadir)/octave
pkgoctave_OCTAVE = CCMC.oct
pkgoctaveexec_LTLIBRARIES = _CCMC.la
_CCMC_la_SOURCES = ccmc_wrap.cxx
_CCMC_la_LDFLAGS = -module -avoid-version 
_CCMC_la_LIBADD = @builddir@/../../../ccmc/libkameleon_plus.la
_CCMC_la_CXXFLAGS = -fPIC
_CCMC_la_CPPFLAGS = @OCTAVE_INCLUDES@ -I@srcdir@/../../..
_CCMC_la_LIBTOOLFLAGS = --tag=disable-static

EXTRA_DIST = SWIG_COMMAND
