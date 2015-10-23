set(msg_prefix " ")
message("")
message("========== Java wrappers(src/ccmc/wrappers/java) ==========")
message(STATUS ${msg_prefix} "building java wrappers")
# java wrappers (kameleon-plus/src/ccmc/wrappers/java)
# JAVA wrappers - the goal is to mimic Makefile to produce the following
# libkameleon_plus-wrapper-java.jnilib
# kameleon_plus-native-macosx-universal-0.3.15.jar	
# kameleon_plus-wrapper-0.3.15.jar	

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/java) 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/java)

include(UseJava) #enables add_java
include(UseSWIG) #enables SWIG_ADD_MODULE and SWIG_LINK_LIBRARIES

message(STATUS ${msg_prefix} "SWIG_USE_FILE: " ${SWIG_USE_FILE})
INCLUDE(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Kameleon includes
INCLUDE_DIRECTORIES(${KAMELEON_SRC_DIR}) 

# hdf5
if(BUILD_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
	add_definitions(-DHAVE_HDF5)
	# message(STATUS ${msg_prefix} "Include HDF5: " ${HDF5_INCLUDE_DIR})
endif(BUILD_HDF5)

# Boost
include_directories(${BOOST_INCLUDEDIR})                                                                                          
# message(STATUS ${msg_prefix} "Include boost directories: " ${BOOST_INCLUDEDIR})

# CDF
include_directories(${CDF_INCLUDES})
# message(STATUS ${msg_prefix} "Include CDF: " ${CDF_INCLUDES})

# JNI
INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})
# message(STATUS ${msg_prefix} "Include JNI: " ${JNI_INCLUDE_DIRS})

#SWIG
SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
SET_SOURCE_FILES_PROPERTIES(../ccmcJava.i PROPERTIES CPLUSPLUS ON)
SET(CMAKE_SWIG_FLAGS -Wall -package gov.nasa.gsfc.ccmc)

#create libkameleon_plus-wrapper-java-1.0.0.jnilib on mac
SWIG_ADD_MODULE(kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION} java ../ccmcJava.i) 
get_property(java_wrapped_mod_name TARGET kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION} PROPERTY LOCATION)
message (STATUS ${msg_prefix} "kameleon_plus-wrapper-java location will be = ${java_wrapped_mod_name}")
# link ccmc library
SWIG_LINK_LIBRARIES(kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION} ccmc) 

## The library name expected by the JNI relative to the system:
##   MacOS  : libLIBRARY.jnilib -> libkameleon_plus-wrapper-java.jnilib
##   Windows: LIBRARY.dll
##   Linux  : libLIBRARY.so
##
## where LIBRARY is used in java to load the library:
##      System.loadLibrary("LIBRARY");

set_source_files_properties(${SWIG_JAVA_SOURCES} PROPERTIES GENERATED TRUE)

add_custom_command(OUTPUT _GeneratedJavaSources
					COMMAND ls ${CMAKE_CURRENT_BINARY_DIR}/*.java)

# can't get POST_BUILD globbing to work..
# add_custom_command(TARGET kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION} POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -P "file( GLOB _GeneratedJavaSources ${CMAKE_CURRENT_BINARY_DIR}/*.java )"
# 	COMMAND ${CMAKE_COMMAND} -P "message(warning 'generated java sources:'${_GeneratedJavaSources})"
# 	)

# Since POST_BUILD won't work, have to list source files explicitly
set(_GeneratedJavaSources
	${CMAKE_CURRENT_BINARY_DIR}/Attribute.java
	${CMAKE_CURRENT_BINARY_DIR}/BATSRUS.java
	${CMAKE_CURRENT_BINARY_DIR}/BATSRUSInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/CCMC.java
	${CMAKE_CURRENT_BINARY_DIR}/CCMCJNI.java
	${CMAKE_CURRENT_BINARY_DIR}/CDFFileReader.java
	${CMAKE_CURRENT_BINARY_DIR}/ENLIL.java
	${CMAKE_CURRENT_BINARY_DIR}/ENLILInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/Fieldline.java
	${CMAKE_CURRENT_BINARY_DIR}/FileReader.java
	${CMAKE_CURRENT_BINARY_DIR}/GeneralFileReader.java
	${CMAKE_CURRENT_BINARY_DIR}/Interpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/Kameleon.java
	${CMAKE_CURRENT_BINARY_DIR}/KameleonInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/LFMInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/MAS.java
	${CMAKE_CURRENT_BINARY_DIR}/MASInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/Model.java
	${CMAKE_CURRENT_BINARY_DIR}/OpenGGCM.java
	${CMAKE_CURRENT_BINARY_DIR}/OpenGGCMInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/Point3f.java
	${CMAKE_CURRENT_BINARY_DIR}/Position.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_PointCloudT_float_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_boost__unordered_mapT_int_ccmc__GeneralFileReader_p_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Interpolator_p_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Kameleon_p_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_boost__unordered_mapT_int_ccmc__TimeInterpolator_p_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_boost__unordered_mapT_int_ccmc__Tracer_p_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_ccmc__PolyhedronT_float_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_clock_t.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_float.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_int.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_long.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_p_char.java
	${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__string.java
	${CMAKE_CURRENT_BINARY_DIR}/SWMFIono.java
	${CMAKE_CURRENT_BINARY_DIR}/SWMFIonoInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/Time.java
	${CMAKE_CURRENT_BINARY_DIR}/TimeInterpolator.java
	${CMAKE_CURRENT_BINARY_DIR}/Tracer.java
	${CMAKE_CURRENT_BINARY_DIR}/eucl_vector_float.java
	${CMAKE_CURRENT_BINARY_DIR}/point_float.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_eucl_vector_float.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_fieldline.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_float.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_int.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_point3f.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_point_float.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_string.java
	${CMAKE_CURRENT_BINARY_DIR}/vector_time.java
	)


# generate kameleon_plus-wrapper-java.jar (doesn't include manifest..)
set(CMAKE_JAVA_TARGET_OUTPUT_NAME kameleon_plus-wrapper-java)
set(CMAKE_JAVA_TARGET_VERSION ${KAMELEON_PLUS_VERSION})
add_jar(kameleon_plus_jni ${_GeneratedJavaSources})

get_property(_WRAPPED_CLASSES TARGET kameleon_plus_jni PROPERTY CLASSDIR)
    
add_dependencies(kameleon_plus_jni kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION})

add_custom_command(TARGET kameleon_plus_jni POST_BUILD
			COMMAND jar cvfm kameleon_plus-native-${CMAKE_SYSTEM_NAME}-${KAMELEON_PLUS_VERSION}.jar ${CMAKE_CURRENT_SOURCE_DIR}/manifest-native.mf ${java_wrapped_mod_name}
			COMMAND cp kameleon_plus-native-${CMAKE_SYSTEM_NAME}-${KAMELEON_PLUS_VERSION}.jar ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
			# COMMAND jar cvfm kameleon_plus-wrapper-${KAMELEON_PLUS_VERSION}.jar ${CMAKE_CURRENT_SOURCE_DIR}/manifest.mf -C ${_WRAPPED_CLASSES} .



