SUBDIRS = . wrappers
#EXTRA_LTLIBRARIES = libkameleon-tracer.la
lib_LTLIBRARIES = libkameleon_plus.la

libkameleon_plus_la_SOURCES = Attribute.h \
						Attribute.cpp \
						BATSRUS.h \
						BATSRUS.cpp \
						BATSRUSInterpolator.h \
						BATSRUSInterpolator.cpp \
						CCMCTime.h \
						CCMCTime.cpp \
						Cell3D.h \
						CDFFileReader.h \
						CDFFileReader.cpp \
						cdf.h \
						Constants.h \
						CTIP.h \
						CTIP.cpp \
						ENLIL.h \
						ENLIL.cpp \
						ENLILInterpolator.h \
						ENLILInterpolator.cpp \
						GeneralFileReader.h \
						GeneralFileReader.cpp \
						KameleonInterpolator_compute_etaj.cpp \
						KameleonInterpolator_compute_beta.cpp \
						KameleonInterpolator_compute_e.cpp \
						KameleonInterpolator_compute_edotj.cpp \
						KameleonInterpolator_compute_en.cpp \
						KameleonInterpolator_compute_exb.cpp \
						KameleonInterpolator_compute_jpar.cpp \
						KameleonInterpolator_compute_jxb.cpp \
						KameleonInterpolator_compute_n.cpp \
						KameleonInterpolator_compute_nv.cpp \
						KameleonInterpolator_compute_p.cpp \
						KameleonInterpolator_compute_polb.cpp \
						KameleonInterpolator_compute_pram.cpp \
						KameleonInterpolator_compute_s.cpp \
						KameleonInterpolator_compute_scaleByRadius.cpp \
						KameleonInterpolator_compute_temp.cpp \
						Kameleon_initializeListOfRequiredVariablesForComponentsAndVectors.cpp \
						Kameleon_initializeUnits.cpp \
						Kameleon_initializeVariableAliases.cpp \
						KameleonInterpolator_compute_gradient.cpp \
						KameleonInterpolator_compute_magnitude.cpp \
						KameleonInterpolator_interpolateSimple.cpp \
						KameleonInterpolator_initializeCalculationMethods.cpp \
						Kameleon_open.cpp \
						Kameleon.cpp \
						Kameleon.h \
						KameleonInterpolator.cpp \
						KameleonInterpolator.h \
						FileReader.cpp \
						FileReader.h \
						Interpolator.cpp \
						Interpolator.h \
						LFM.h\
						LFM.cpp\
						LFMInterpolator.h\
						LFMInterpolator.cpp\
						Magnetogram.h \
						Magnetogram.cpp \
						MagnetogramInterpolator.h \
						MagnetogramInterpolator.cpp \
						MAS.h \
						MAS.cpp \
						MASInterpolator.h \
						MASInterpolator.cpp \
						Model.h \
						Model.cpp \
						OpenGGCM.h \
						OpenGGCM.cpp \
						OpenGGCMInterpolator.h \
						OpenGGCMInterpolator.cpp \
						Point.h \
						Utils.h \
						StringConstants.h \
						Vector.h \
						cxform.h \
						cxform-auto.c \
						cxform-manual.c \
						Point3f.h \
						Point3f.cpp \
						Fieldline.h \
						Fieldline.cpp \
						TimeInterpolator.h \
						TimeInterpolator.cpp \
						Tracer.h \
						Tracer.cpp \
						Adapt3D.h \
						Adapt3D.cpp \
						Adapt3DInterpolator.h \
						Adapt3DInterpolator.cpp \
						MathHelper.h \
						MathHelper.cpp \
						HDF5FileReader.h \
						HDF5FileReader.cpp \
						SWMFIono.h \
						SWMFIono.cpp \
						SWMFIonoInterpolator.h \
						SWMFIonoInterpolator.cpp \
						Polyhedron.h \
						pointcloud.h \
						nanoflann.hpp
						
											
						
libkameleon_plus_la_LDFLAGS = -release @RELEASE@
#-enable-runtime-pseudo-reloc --enable-auto-import
libkameleon_plus_la_CFLAGS = -fPIC

pkgincludedir = $(includedir)/ccmc
pkginclude_HEADERS = Tracer.h Fieldline.h Point3f.h Attribute.h FileReader.h \
					 Kameleon.h Interpolator.h KameleonInterpolator.h Constants.h cxform.h\
		 			 Model.h Point.h BATSRUS.h OpenGGCM.h MAS.h ENLIL.h CTIP.h \
		 			 Adapt3D.h Adapt3DInterpolator.h MathHelper.h LFM.h \
		 			 TimeInterpolator.h nanoflann.hpp pointcloud.h Polyhedron.h
