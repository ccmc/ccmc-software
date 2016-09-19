# top level CMakeLists.txt (kameleon-plus/)

cmake_minimum_required (VERSION 2.8) 
project (Kameleon)
cmake_policy(SET CMP0026 OLD)

#Set version number
set(Kameleon_VERSION_MAJOR "6")
set(Kameleon_VERSION_MINOR "1.0.0")
set(KAMELEON_PLUS_VERSION ${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR})

set(msg_prefix " ")
message("")
message("============= Top level CMakeLists.txt ==============")
message(STATUS ${msg_prefix} "Kameleon-plus " ${KAMELEON_PLUS_VERSION} " build for " ${CMAKE_SYSTEM})

# Set Kameleon paths
set(KAMELEON_BASE_DIR "${PROJECT_SOURCE_DIR}")
set(KAMELEON_SRC_DIR "${KAMELEON_BASE_DIR}/src")
set(KAMELEON_EXT_DIR "${KAMELEON_BASE_DIR}/ext")
set(KAMELEON_LIB_DIR "${KAMELEON_BASE_DIR}/lib")
set(CCMC_DIR "${KAMELEON_SRC_DIR}/ccmc/")

# Set whether to build shared or static
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(USE_STATIC_LIBS "Use static libraries" OFF)

# # Install rpath
# option(CMAKE_INSTALL_RPATH "install prefix for Rpath" "${CMAKE_INSTALL_PREFIX}/lib")

# # UNIX
# if(UNIX)
#     set(CMAKE_SKIP_BUILD_RPATH FALSE)
#     set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
#     set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# # below sets the install path but only if it is in a non-system directory
# #    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
# #    if("${isSystemDir}" STREQUAL "-1")
# #        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# #    endif()
# endif()

# OSX
if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()


# add path to external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${KAMELEON_EXT_DIR})

# voice debug settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

#============ python ============
option(INSTALL_CCMC_PYTHON_MODULE "Install ccmc python module" OFF)
FIND_PACKAGE(PythonInterp 2.7)
FIND_PACKAGE(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)
message(STATUS ${msg_prefix} "Python Interpreter:" ${PYTHON_EXECUTABLE})
if(INSTALL_CCMC_PYTHON_MODULE)
	execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; from distutils.sysconfig import get_python_lib; print get_python_lib().split(sys.prefix)[-1][1:]"
					OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
endif(INSTALL_CCMC_PYTHON_MODULE)
#============ Boost =============
message("Getting BOOST....")
# Boost shared or static?
if(USE_STATIC_LIBS)
	set(Boost_USE_STATIC_LIBS ON)
	# set(Boost_USE_STATIC_RUNTIME OFF)
else()
	set(Boost_USE_STATIC_LIBS OFF)
	# set(Boost_USE_STATIC_RUNTIME ON)
endif(USE_STATIC_LIBS)


set(Boost_USE_MULTITHREADED ON)  
# find_package(Boost 1.54.0)
find_package(Boost 1.54.0 COMPONENTS thread program_options filesystem system python REQUIRED)
message(STATUS ${msg_prefix} "BOOST_ROOT=" ${BOOST_ROOT})
message(STATUS ${msg_prefix} "Boost includes: " ${BOOST_INCLUDEDIR})
message(STATUS ${msg_prefix} "Boost library directory: " ${BOOST_LIBRARYDIR})   
message(STATUS ${msg_prefix} "Link boost: " ${Boost_LIBRARIES})      

#============= HDF5 ==============
#Desired functionality: If the HDF5 library is installed, build by default with HDF5
#Optionally, turn off buildling HDF5 even if it is installed
option(BUILD_HDF5 "Build with HDF5 library" ON)

if(BUILD_HDF5)
	message("Getting HDF5...")
	# if(HDF5_ROOT) #needs to be passed to FindHDF5 as an environment variable
	# 	set(ENV{HDF5_ROOT} ${HDF5_ROOT})
	# endif(HDF5_ROOT)

	# HDF5 shared or static?
	if(USE_STATIC_LIBS)
		set(HDF5_USE_STATIC_LIBRARIES ON)
	else()
		set(HDF5_USE_STATIC_LIBRARIES OFF)
	endif(USE_STATIC_LIBS)

	find_package(HDF5 COMPONENTS CXX)

	if(HDF5_FOUND)
		message(STATUS ${msg_prefix} "HDF5 Found: will build Kameleon with HDF 5")
		message(STATUS ${msg_prefix} "HDF_LIBRARY_DIRS:" ${HDF5_LIBRARY_DIRS})
		message(STATUS ${msg_prefix} "HDF5 includes:" ${HDF5_INCLUDE_DIR})
		message(STATUS ${msg_prefix} "HDF5_CXX_LIBRARIES:" ${HDF5_CXX_LIBRARIES})
		add_definitions(-DHAVE_HDF5)
	else()
		message(STATUS ${msg_prefix} "HDF5 was not found. Please install HDF5")
		set(BUILD_HDF5 OFF)
	endif(HDF5_FOUND)
else()
	message(${msg_prefix} "Will NOT build Kameleon with HDF5.")
endif(BUILD_HDF5)

option(BUILD_JAVA "Build java wrappers" ON)
if(BUILD_JAVA)
	message(STATUS ${msg_prefix} "Will build java wrappers")
else()
	message(STATUS ${msg_prefix} "Will NOT build java wrappers")
endif()


#============= CDF ==============
message("Getting CDF..")
# CDF shared or static?
if(USE_STATIC_LIBS)
	message(STATUS ${msg_prefix} "retrieving static CDF libraries")
	set(CDF_USE_STATIC_LIBS ON)
else()
	# note: on osx, had to change the install name of my cdf library like this
	# install_name_tool -id /Applications/cdf35_0-dist/lib/libcdf.dylib libcdf.dylib
	message(STATUS ${msg_prefix} "retrieving shared CDF libraries")
	set(CDF_USE_STATIC_LIBS OFF)
endif(USE_STATIC_LIBS)

find_package(CDF REQUIRED)

message(STATUS ${msg_prefix} "Include CDF: " ${CDF_INCLUDES})
message(STATUS ${msg_prefix} "link CDF: " ${CDF_LIB})
message(STATUS ${msg_prefix} "CDF Path:" ${CDF_PATH}/lib)

#============ Set Config variables =============
# This code generates kameleon-plus-Config.h using kameleon-plus-Config.h.in as a template, with variable values supplied by this CMakeLists file.
configure_file (
	${PROJECT_SOURCE_DIR}/kameleon-plus-Config.h.in
	${PROJECT_BINARY_DIR}/kameleon-plus-Config.h
)


#============ Set up packaging environment ==========
install(DIRECTORY DESTINATION bin/ccmc)
install(DIRECTORY DESTINATION bin/ccmc/examples/c++)

# file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/package_with_dependencies)
# set(PACKAGE_LOC ${CMAKE_INSTALL_PREFIX}/bin/ccmc/MacOS)
# message(STATUS ${msg_prefix} "Package location will be: " ${PACKAGE_LOC})
# file(MAKE_DIRECTORY ${PACKAGE_LOC}/bin)
# file(MAKE_DIRECTORY ${PACKAGE_LOC}/lib)
# file(MAKE_DIRECTORY ${PACKAGE_LOC}/inc)

#============ Run other CMakeLists.txt files in subdirectories ==========
# CCMC
set(CCMC_ROOT_DIR "${KAMELEON_SRC_DIR}/ccmc")
add_subdirectory(${CCMC_ROOT_DIR})

message("")
message("============ Top level CMakeLists.txt ============")
set(EXAMPLES_BIN_DIR "${PROJECT_SOURCE_DIR}/bin/ccmc/examples")
file(MAKE_DIRECTORY ${EXAMPLES_BIN_DIR}/python)
set(EXAMPLES_ROOT_DIR "${KAMELEON_BASE_DIR}/src/example")
# FIND_PACKAGE( PythonLibs 2.7 REQUIRED)
add_subdirectory(${EXAMPLES_ROOT_DIR}/c++)
add_subdirectory(${EXAMPLES_ROOT_DIR}/c)
add_subdirectory(${EXAMPLES_ROOT_DIR}/fortran)
add_subdirectory(${EXAMPLES_ROOT_DIR}/python)

