lib_LTLIBRARIES = libkameleon-plus.la
libkameleon_plus_la_SOURCES = Attribute.h \
						Attribute.cpp \
						BATSRUS.h \
						BATSRUS.cpp \
						BATSRUSInterpolator.h \
						BATSRUSInterpolator.cpp \
						Cell3D.h \
						Kameleon_compatibility.h \
						Kameleon_compute_etaj.cpp \
						Kameleon_compute_beta.cpp \
						Kameleon_compute_e.cpp \
						Kameleon_compute_edotj.cpp \
						Kameleon_compute_en.cpp \
						Kameleon_compute_exb.cpp \
						Kameleon_compute_jpar.cpp \
						Kameleon_compute_jxb.cpp \
						Kameleon_compute_n.cpp \
						Kameleon_compute_nv.cpp \
						Kameleon_compute_p.cpp \
						Kameleon_compute_polb.cpp \
						Kameleon_compute_pram.cpp \
						Kameleon_compute_s.cpp \
						Kameleon_compute_scaleByRadius.cpp \
						Kameleon_compute_temp.cpp \
						Kameleon_initializeConversionFactorsToVis.cpp \
						Kameleon_initializeListOfRequiredVariablesForComponentsAndVectors.cpp \
						Kameleon_initializeUnits.cpp \
						Kameleon_initializeVariableAliases.cpp \
						KameleonInterpolator_compute_magnitude.cpp \
						KameleonInterpolator_interpolateSimple.cpp \
						KameleonInterpolator_initializeCalculationMethods.cpp \
						Kameleon_loadVariable.cpp \
						Kameleon_loadVectorVariable.cpp \
						Kameleon_open.cpp \
						Kameleon.cpp \
						Kameleon.h \
						KameleonInterpolator.cpp \
						KameleonInterpolator.h \
						FileReader.cpp \
						FileReader.h \
						Interpolator.h \
						Interpolator.cpp \
						kameleon_adjusted.h \
						Model.h \
						Model.cpp \
						OpenGGCM.h \
						OpenGGCM.cpp \
						OpenGGCMInterpolator.h \
						OpenGGCMInterpolator.cpp \
						Point.h \
						#Shue.h \
						#Shue.cpp \
						Vector.h 
libkameleon_plus_la_LDFLAGS = -lkameleon -lcdf -release 1.0.0
libkameleon_plus_la_CFLAGS = -fPIC

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = kameleon-plus.pc

pkgincludedir = $(includedir)/ccmc
pkginclude_HEADERS = Attribute.h FileReader.h Kameleon.h Kameleon_compatibility.h Model.h Point.h #Shue.h
