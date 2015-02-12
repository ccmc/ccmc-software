# top level CMakeLists.txt (kameleon-plus/)

cmake_minimum_required (VERSION 2.8) 
project (Kameleon)

#Set version number
set(Kameleon_VERSION_MAJOR "6")
set(Kameleon_VERSION_MINOR "0.0")
set(KAMELEON_PLUS_VERSION ${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR})
message(STATUS "Kameleon_plus_version " ${KAMELEON_PLUS_VERSION})


# Set Kameleon paths
set(KAMELEON_BASE_DIR "${PROJECT_SOURCE_DIR}")
set(KAMELEON_SRC_DIR "${KAMELEON_BASE_DIR}/src")
set(KAMELEON_EXT_DIR "${KAMELEON_BASE_DIR}/ext")
set(KAMELEON_LIB_DIR "${KAMELEON_BASE_DIR}/lib")

# add path to external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${KAMELEON_EXT_DIR})

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 


find_package(Boost 1.54.0 COMPONENTS python)
# find_package(Boost 1.54.0  REQUIRED COMPONENTS thread )
message(STATUS "BOOST_ROOT=" ${BOOST_ROOT})

# HDF5
option(HDF5_USE_STATIC_LIBRARIES "Use HDF5 static libraries" OFF)
find_package(HDF5 REQUIRED COMPONENTS C CXX)

#Desired functionality: If the HDF5 library is installed, build by default with HDF5
#Optionally, turn off buildling HDF5 even if it is installed
option(BUILD_HDF5 "Build with HDF5 library" ON)

if(BUILD_HDF5)
	if(HDF5_FOUND)
		message(STATUS "HDF5 Found: Building Kameleon with HDF 5")
		add_definitions(-DHAVE_HDF5)
	else()
		message(STATUS "HDF 5 was not found. Please install HDF5")
		set(BUILD_HDF5 OFF)
	endif(HDF5_FOUND)
else()
	message(STATUS "Not Building HDF5 - CDF files only supported")
endif(BUILD_HDF5)

# CDF
set(CDF_USE_STATIC_LIBS ON)
# set(CDF_PATH "/Applications/cdf35_0-dist")
find_package(CDF REQUIRED)

configure_file (
	${PROJECT_SOURCE_DIR}/kameleon-plus-Config.h.in
	${PROJECT_BINARY_DIR}/kameleon-plus-Config.h
)

# pyreaders
# add_subdirectory(${KAMELEON_SRC_DIR}/ccmc/pyreaders)

# CCMC
set(CCMC_ROOT_DIR "${KAMELEON_SRC_DIR}/ccmc")
add_subdirectory(${CCMC_ROOT_DIR})


set(EXAMPLES_BIN_DIR "${PROJECT_SOURCE_DIR}/bin/examples")
set(EXAMPLES_ROOT_DIR "${KAMELEON_BASE_DIR}/src/example")
FIND_PACKAGE( PythonLibs 2.7 REQUIRED)
add_subdirectory(${EXAMPLES_ROOT_DIR}/c++)
add_subdirectory(${EXAMPLES_ROOT_DIR}/c)
add_subdirectory(${EXAMPLES_ROOT_DIR}/fortran)
add_subdirectory(${EXAMPLES_ROOT_DIR}/python)




