# PYTHON wrapper
FIND_PACKAGE(SWIG REQUIRED)
FIND_PACKAGE(PythonLibs REQUIRED)
message(STATUS "wrappers/ SWIG_USE_FILE: " ${SWIG_USE_FILE})
INCLUDE(${SWIG_USE_FILE})
SET(CMAKE_SWIG_FLAGS "")

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Kameleon includes
INCLUDE_DIRECTORIES(${KAMELEON_SRC_DIR}) 
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}) # kameleon-plus-Config.h

# hdf5
if(BUILD_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
	add_definitions(-DHAVE_HDF5)
	message(STATUS "wrappers/python/ Include HDF5: " ${HDF5_INCLUDE_DIR})
endif(BUILD_HDF5)

# Boost
include_directories(${Boost_INCLUDE_DIRS})                                                                                          
message(STATUS "wrappers/python/ Include boost directories: " ${Boost_INCLUDE_DIRS})

# CDF
include_directories(${CDF_INCLUDES})
message(STATUS "wrappers/python/ Include CDF: " ${CDF_INCLUDES})


SET_SOURCE_FILES_PROPERTIES(../ccmc.i PROPERTIES CPLUSPLUS ON)
#SWIG flag -builtin mostly supported in python 2.3 or 2.4, fully supported in 2.5+
SET_SOURCE_FILES_PROPERTIES(../ccmc.i PROPERTIES SWIG_FLAGS "-builtin")

SWIG_ADD_MODULE(CCMC python ../ccmc.i)

SWIG_LINK_LIBRARIES(CCMC ${PYTHON_LIBRARIES})
SWIG_LINK_LIBRARIES(CCMC ccmc) # link ccmc

execute_process(COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
				OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
install(TARGETS _CCMC DESTINATION ${PYTHON_SITE_PACKAGES})
install(FILES CCMC.py DESTINATION ${PYTHON_SITE_PACKAGES})

