#CCMC_PYTHON = CCMC.py
#CCMCdir = $(datadir)/python
pkgpython_PYTHON = CCMC.py
pkgpyexec_LTLIBRARIES = _CCMC.la
_CCMC_la_SOURCES = ccmc_wrap.cxx
_CCMC_la_LDFLAGS = -module -avoid-version 
_CCMC_la_LIBADD = @builddir@/../../../ccmc/libkameleon-plus.la
_CCMC_la_CXXFLAGS = -fPIC
_CCMC_la_CPPFLAGS = @PYTHON_INCLUDES@ -I@srcdir@/../../..
_CCMC_la_LIBTOOLFLAGS = --tag=disable-static
