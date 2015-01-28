# top level CMakeLists.txt (kameleon-plus/)

cmake_minimum_required (VERSION 2.8) 
project (Kameleon)

#Set version number
set(Kameleon_VERSION_MAJOR "6")
set(Kameleon_VERSION_MINOR "0.0")
set(KAMELEON_PLUS_VERSION ${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR})

set(msg_prefix "./> ")
message(STATUS ${msg_prefix} "entered top level CMakeLists.txt file.")
message(STATUS ${msg_prefix} "Kameleon-plus " ${KAMELEON_PLUS_VERSION} " build for " ${CMAKE_SYSTEM})

# Set Kameleon paths
set(KAMELEON_BASE_DIR "${PROJECT_SOURCE_DIR}")
set(KAMELEON_SRC_DIR "${KAMELEON_BASE_DIR}/src")
set(KAMELEON_EXT_DIR "${KAMELEON_BASE_DIR}/ext")
set(KAMELEON_LIB_DIR "${KAMELEON_BASE_DIR}/lib")

# Set whether to build shared or static
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(USE_STATIC_LIBS "Use static libraries" OFF)

if(BUILD_SHARED_LIBS)
	set(CMAKE_MACOSX_RPATH 1) #Turning this off ensures that fixup_bundle finds the ccmc library correctly
endif(BUILD_SHARED_LIBS)

# add path to external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${KAMELEON_EXT_DIR})

############Boost #############
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
find_package(Boost 1.54.0 COMPONENTS thread program_options filesystem system REQUIRED)
message(STATUS ${msg_prefix} "BOOST_ROOT=" ${BOOST_ROOT})
message(STATUS ${msg_prefix} "Boost includes: " ${Boost_INCLUDE_DIRS})
message(STATUS ${msg_prefix} "Boost library directory: " ${Boost_LIBRARY_DIR})   
message(STATUS ${msg_prefix} "Link boost: " ${Boost_LIBRARIES})      

##################### HDF5 ##############
#Desired functionality: If the HDF5 library is installed, build by default with HDF5
#Optionally, turn off buildling HDF5 even if it is installed
option(BUILD_HDF5 "Build with HDF5 library" ON)

if(BUILD_HDF5)
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
		message(STATUS ${msg_prefix} "HDF5_CXX_LIBRARIES:" ${HDF5_CXX_LIBRARIES})
		add_definitions(-DHAVE_HDF5)
	else()
		message(STATUS ${msg_prefix} "HDF5 was not found. Please install HDF5")
		set(BUILD_HDF5 OFF)
	endif(HDF5_FOUND)
else()
	message(STATUS ${msg_prefix} "Will not build Kameleon with HDF5 - CDF files only supported")
endif(BUILD_HDF5)


##################### CDF ####################
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

######### Set Config variables #############
# This code generates kameleon-plus-Config.h using kameleon-plus-Config.h.in as a template, with variable values supplied by this CMakeLists file.
configure_file (
	${PROJECT_SOURCE_DIR}/kameleon-plus-Config.h.in
	${PROJECT_BINARY_DIR}/kameleon-plus-Config.h
)

######### Set up packaging environment #####
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/ccmc)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/ccmc/c++)
# file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/package_with_dependencies)
# set(PACKAGE_LOC ${CMAKE_INSTALL_PREFIX}/bin/ccmc/MacOS)
# message(STATUS ${msg_prefix} "Package location will be: " ${PACKAGE_LOC})
# file(MAKE_DIRECTORY ${PACKAGE_LOC}/bin)
# file(MAKE_DIRECTORY ${PACKAGE_LOC}/lib)
# file(MAKE_DIRECTORY ${PACKAGE_LOC}/inc)

######### Run other CMakeLists.txt files in subdirectories #####
# CCMC
set(CCMC_ROOT_DIR "${KAMELEON_SRC_DIR}/ccmc")
add_subdirectory(${CCMC_ROOT_DIR})


set(EXAMPLES_BIN_DIR "${PROJECT_SOURCE_DIR}/bin/examples")
set(EXAMPLES_ROOT_DIR "${KAMELEON_BASE_DIR}/src/example")

add_subdirectory(${EXAMPLES_ROOT_DIR}/c++)
add_subdirectory(${EXAMPLES_ROOT_DIR}/c)
add_subdirectory(${EXAMPLES_ROOT_DIR}/fortran)
add_subdirectory(${EXAMPLES_ROOT_DIR}/python)




