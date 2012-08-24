lib_LTLIBRARIES = libkameleon_plus-wrapper-java.la
libkameleon_plus_wrapper_java_la_SOURCES = ccmc_wrap.cxx
libkameleon_plus_wrapper_java_la_LDFLAGS = -module -avoid-version @JNI_EXTRA_LDFLAGS@ @builddir@/../../../ccmc/libkameleon_plus.la
#libkameleon_plus_wrapper_java_la_LDFLAGS = -release @RELEASE@ @JNI_EXTRA_LDFLAGS@
libkameleon_plus_wrapper_java_la_CXXFLAGS = -fPIC
libkameleon_plus_wrapper_java_la_CPPFLAGS = -I@JNI_INCLUDE_DIRS@ -I@srcdir@/../../..
libkameleon_plus_wrapper_java_la_LIBTOOLFLAGS = --tag=disable-static

jarfile = kameleon_plus_wrapper_@RELEASE@.jar

jardir = $(datadir)/java
pkgpath = @builddir@
jar_DATA = $(jarfile)
JAVAROOT= @builddir@

#CLEANFILES = $(pkgpath)/*.jar $(pkgpath)/gov/
CLEANFILES = $(pkgpath)/*.jar
AM_JAVACFLAGS=-nowarn

dist_noinst_JAVA = 	Attribute.java \
					BATSRUS.java \
					BATSRUSInterpolator.java \
					CDFFileReader.java \
					CCMC.java \
					CCMCJNI.java \
					ENLIL.java \
					ENLILInterpolator.java \
					Fieldline.java \
					FileReader.java \
					CDFFileReader.java \
					GeneralFileReader.java \
					Interpolator.java \
					Kameleon.java \
					KameleonInterpolator.java \
					MAS.java \
					MASInterpolator.java \
					Model.java \
					OpenGGCM.java \
					OpenGGCMInterpolator.java \
					Point3f.java \
					Position.java \
					SWIGTYPE_p_float.java \
					SWIGTYPE_p_int.java \
					SWIGTYPE_p_std__string.java \
					Time.java \
					Tracer.java \
					vector_fieldline.java \
					vector_float.java \
					vector_int.java \
					vector_point3f.java \
					vector_string.java \
					SWIGTYPE_p_boost__unordered_mapT_int_ccmc__GeneralFileReader_p_t.java \
					SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Interpolator_p_t.java \
					SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Kameleon_p_t.java \
					SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Tracer_p_t.java \
					SWMFIono.java \
					SWMFIonoInterpolator.java

#					HDF5FileReader.java 
					

EXTRA_DIST = SWIG_COMMAND
					

.NOTPARALLEL:

clean-local:
	-rm -rf $(pkgpath)/gov

$(jarfile): $(dist_noinst_JAVA)
	echo $(dist_noinst_SWIG_COMMAND)
	jar cf $@ $(pkgpath)/gov/nasa/gsfc/ccmc/*.class; 
	
