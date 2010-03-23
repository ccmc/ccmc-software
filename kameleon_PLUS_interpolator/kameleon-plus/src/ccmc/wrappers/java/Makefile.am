lib_LTLIBRARIES = libkameleon-plus-wrapper-java.la
libkameleon_plus_wrapper_java_la_SOURCES = ccmc_wrap.cxx
libkameleon_plus_wrapper_java_la_LDFLAGS = -module -avoid-version @JNI_EXTRA_LDFLAGS@ @builddir@/../../../ccmc/.libs/libkameleon-plus.a
#libkameleon_plus_wrapper_java_la_LDFLAGS = -release @RELEASE@ @JNI_EXTRA_LDFLAGS@
libkameleon_plus_wrapper_java_la_CXXFLAGS = -fPIC
libkameleon_plus_wrapper_java_la_CPPFLAGS = -I@JNI_INCLUDE_DIRS@ -I@srcdir@/../../..
libkameleon_plus_wrapper_java_la_LIBTOOLFLAGS = --tag=disable-static

jarfile = kameleon-plus-wrapper-@RELEASE@.jar

jardir = $(datadir)/java
pkgpath = @srcdir@
jar_DATA = $(jarfile)
JAVAROOT= @srcdir@

CLEANFILES = $(jarfile) $(pkgpath)/ccmc/wrappers/*.class

dist_noinst_JAVA = 	Attribute.java \
					CCMC.java \
					CCMCJNI.java \
					Fieldline.java \
					FileReader.java \
					Interpolator.java \
					Kameleon.java \
					Point3f.java \
					Position.java \
					SWIGTYPE_p_boost__unordered_mapT_int_ccmc__FileReader_p_t.java \
					SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Interpolator_p_t.java \
					SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Kameleon_p_t.java \
					SWIGTYPE_p_float.java \
					SWIGTYPE_p_int.java \
					SWIGTYPE_p_std__string.java \
					SWIGTYPE_p_void.java \
					Time.java \
					Tracer.java \
					vector_fieldline.java \
					vector_float.java \
					vector_int.java \
					vector_point3f.java \
					vector_string.java
					



kameleon-plus-wrapper-@RELEASE@.jar: $(dist_noinst_JAVA)
	@JAR@ cf $@ ccmc/wrappers/*.class; 