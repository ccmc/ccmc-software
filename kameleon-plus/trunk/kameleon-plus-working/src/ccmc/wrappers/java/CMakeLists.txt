# java wrappers (kameleon-plus/src/ccmc/wrappers/java)
# JAVA wrappers - the goal is to mimic Makefile to produce the following
# libkameleon_plus-wrapper-java.jnilib
# kameleon_plus-native-macosx-universal-0.3.15.jar	
# kameleon_plus-wrapper-0.3.15.jar	



FIND_PACKAGE(SWIG REQUIRED)
FIND_PACKAGE(Java REQUIRED)
FIND_PACKAGE(JNI REQUIRED)

include(UseJava) #enables add_java
include(UseSWIG) #enables SWIG_ADD_MODULE and SWIG_LINK_LIBRARIES

# message(STATUS "wrappers/java/ SWIG_USE_FILE: "${SWIG_USE_FILE})
INCLUDE(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Kameleon includes
INCLUDE_DIRECTORIES(${KAMELEON_SRC_DIR}) 

# hdf5
if(BUILD_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
	add_definitions(-DHAVE_HDF5)
	# message(STATUS "wrappers/java/ Include HDF5: " ${HDF5_INCLUDE_DIR})
endif(BUILD_HDF5)

# Boost
include_directories(${Boost_INCLUDE_DIRS})                                                                                          
# message(STATUS "wrappers/java/ Include boost directories: " ${Boost_INCLUDE_DIRS})

# CDF
include_directories(${CDF_INCLUDES})
# message(STATUS "wrappers/java/ Include CDF: " ${CDF_INCLUDES})

# JNI
INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})
# message(STATUS "wrappers/java/ Include JNI: " ${JNI_INCLUDE_DIRS})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

SET_SOURCE_FILES_PROPERTIES(../ccmcJava.i PROPERTIES CPLUSPLUS ON)
SET(CMAKE_SWIG_FLAGS -Wall -package gov.nasa.gsfc.ccmc)

#create libkameleon_plus-wrapper-java-1.0.0.jnilib on mac
SWIG_ADD_MODULE(kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION} java ../ccmcJava.i) 
get_property(java_wrapped_mod_name TARGET kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION} PROPERTY LOCATION)
message (STATUS "kameleon_plus-wrapper-java location == ${java_wrapped_mod_name}")
# link ccmc library
SWIG_LINK_LIBRARIES(kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION} ccmc) 

## The library name expected by the JNI relative to the system:
##   MacOS  : libLIBRARY.jnilib -> libkameleon_plus-wrapper-java.jnilib
##   Windows: LIBRARY.dll
##   Linux  : libLIBRARY.so
##
## where LIBRARY is used in java to load the library:
##      System.loadLibrary("LIBRARY");


# generate kameleon_plus-wrapper-java.jar (doesn't include manifest..)
set(CMAKE_JAVA_TARGET_OUTPUT_NAME kameleon_plus-wrapper-java)
set(CMAKE_JAVA_TARGET_VERSION ${KAMELEON_PLUS_VERSION})
add_jar(kameleon_plus_jni ${CMAKE_CURRENT_SOURCE_DIR}/*.java) # * wildcard may not work on windows 
get_property(_WRAPPED_CLASSES TARGET kameleon_plus_jni PROPERTY CLASSDIR)


add_dependencies(kameleon_plus_jni kameleon_plus-wrapper-java-${KAMELEON_PLUS_VERSION})

add_custom_command(TARGET kameleon_plus_jni POST_BUILD
			COMMAND jar cvfm kameleon_plus-native-${CMAKE_SYSTEM_NAME}-${KAMELEON_PLUS_VERSION}.jar ${CMAKE_CURRENT_SOURCE_DIR}/manifest-native.mf ${java_wrapped_mod_name})
			# COMMAND jar cvfm kameleon_plus-wrapper-${KAMELEON_PLUS_VERSION}.jar ${CMAKE_CURRENT_SOURCE_DIR}/manifest.mf -C ${_WRAPPED_CLASSES} .



