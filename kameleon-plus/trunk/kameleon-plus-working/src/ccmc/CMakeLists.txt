# core kameleon library (kameleon-plus/src/ccmc)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc) 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc)

if (NOT CCMC_ROOT_DIR)
    set(CCMC_ROOT_DIR ${PROJECT_SOURCE_DIR})
endif ()

file(GLOB CCMC_SOURCE
    ${CCMC_ROOT_DIR}/*.cpp ${CCMC_ROOT_DIR}/*.c
)
file(GLOB CCMC_HEADER
    ${CCMC_ROOT_DIR}/*.h
)

source_group(Ccmc FILES ${CCMC_SOURCE})  
source_group(Ccmc FILES ${CCMC_HEADER})  

set(ALL_SOURCE ${CCMC_SOURCE})
set(ALL_HEADER ${CCMC_HEADER})

add_library(ccmc ${ALL_SOURCE} ${ALL_HEADER})

get_property(ccmc_lib_loc TARGET ccmc PROPERTY LOCATION)
message (STATUS "ccmc library location = " ${ccmc_lib_loc})

# Boost
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    target_link_libraries(ccmc ${Boost_LIBRARIES})  
    message(STATUS "ccmc/ - Include boost: " ${Boost_INCLUDE_DIRS})    
    message(STATUS "ccmc/ - Link boost: " ${Boost_LIBRARIES})                                                                                          
endif()


# CDF
include_directories(${CDF_INCLUDES})
# include_directories(${CDF_INCLUDE_DIR})
target_link_libraries(ccmc ${CDF_LIBRARIES})
message(STATUS "ccmc/ - Include CDF: " ${CDF_INCLUDES})
message(STATUS "ccmc/ - link CDF: " ${CDF_LIBRARIES})

# point to kameleon-plus-Config.h
include_directories(${PROJECT_BINARY_DIR})

# hdf5
if(BUILD_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
    add_definitions(-DHAVE_HDF5)
	message(STATUS "ccmc/ - Include HDF5: " ${HDF5_INCLUDE_DIR}) 
	message(STATUS "ccmc/ - Link HDF5_LIBRARIES: " ${HDF5_LIBRARIES}) 
	target_link_libraries(ccmc ${HDF5_LIBRARIES})
endif(BUILD_HDF5)

# Wrappers
set(WRAPPERS_ROOT_DIR "${CCMC_ROOT_DIR}/wrappers")
add_subdirectory(${WRAPPERS_ROOT_DIR}/c)
add_subdirectory(${WRAPPERS_ROOT_DIR}/fortran)
add_subdirectory(${WRAPPERS_ROOT_DIR}/python)
add_subdirectory(${WRAPPERS_ROOT_DIR}/java)

