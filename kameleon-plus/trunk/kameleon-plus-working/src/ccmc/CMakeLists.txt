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

if(NOT BUILD_HDF5)
    message(STATUS "ignoring HDF5FileReader.cpp  HDF5FileReader.cpp")
    list(REMOVE_ITEM CCMC_SOURCE ${CCMC_ROOT_DIR}/HDF5FileReader.cpp)
    list(REMOVE_ITEM CCMC_HEADER ${CCMC_ROOT_DIR}/HDF5FileReader.h)
    message(STATUS "new list:" ${CCMC_SOURCE})
endif(NOT BUILD_HDF5)

source_group(Ccmc FILES ${CCMC_SOURCE}) 
source_group(Ccmc FILES ${CCMC_HEADER})  

set(ALL_SOURCE ${CCMC_SOURCE})
set(ALL_HEADER ${CCMC_HEADER})

if(BUILD_SHARED_LIBS)
    message(STATUS "building SHARED ccmc library")
    add_library(ccmc SHARED ${ALL_SOURCE} ${ALL_HEADER})
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    message(STATUS "building STATIC ccmc library")
    add_library(ccmc_static STATIC ${ALL_SOURCE} ${ALL_HEADER})
endif(BUILD_STATIC_LIBS)


set_target_properties(ccmc PROPERTIES SOVERSION ${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR})
#set the install_name of the lib relative to the executables 

get_property(ccmc_lib_loc TARGET ccmc PROPERTY LOCATION)
message (STATUS "ccmc library location = " ${ccmc_lib_loc})


############ Boost #############
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 

    if(BUILD_SHARED_LIBS)
        target_link_libraries(ccmc ${Boost_LIBRARIES}) 
    endif(BUILD_SHARED_LIBS)    

    if(BUILD_STATIC_LIBS)
        target_link_libraries(ccmc_static ${Boost_LIBRARIES})
    endif(BUILD_STATIC_LIBS)

    message(STATUS "ccmc/ - Include boost: " ${Boost_INCLUDE_DIRS})    
    message(STATUS "ccmc/ - Boost library directory: " ${Boost_LIBRARY_DIR})
    message(STATUS "ccmc/ - Link boost: " ${Boost_LIBRARIES})    
endif()


############## CDF #############
include_directories(${CDF_INCLUDES})

if(BUILD_SHARED_LIBS)
    target_link_libraries(ccmc ${CDF_LIB}) 
endif(BUILD_SHARED_LIBS) 

if(BUILD_STATIC_LIBS)
    target_link_libraries(ccmc_static ${CDF_LIB})
endif(BUILD_STATIC_LIBS)

message(STATUS "ccmc/ - Include CDF: " ${CDF_INCLUDES})
message(STATUS "ccmc/ - link CDF: " ${CDF_LIB})
# point to kameleon-plus-Config.h
include_directories(${PROJECT_BINARY_DIR})

############## hdf5 #############
if(BUILD_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
    add_definitions(-DHAVE_HDF5)
	message(STATUS "ccmc/ - Include HDF5: " ${HDF5_INCLUDE_DIR}) 
	message(STATUS "ccmc/ - Link HDF5_LIBRARIES: " ${HDF5_LIBRARIES}) 

    if(BUILD_SHARED_LIBS)
        target_link_libraries(ccmc ${HDF5_LIBRARIES}) 
    endif(BUILD_SHARED_LIBS)    
    if(BUILD_STATIC_LIBS)
        target_link_libraries(ccmc_static ${HDF5_LIBRARIES})
    endif(BUILD_STATIC_LIBS)

endif(BUILD_HDF5)

############ installer ############
#install the library 
if(BUILD_SHARED_LIBS)
    install(TARGETS ccmc 
        DESTINATION lib/ccmc COMPONENT libraries 
        )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    install(TARGETS ccmc_static
        DESTINATION lib/ccmc COMPONENT libraries 
        )
endif(BUILD_STATIC_LIBS)


# #######fixup_bundle - use for installing apps ######
# SET(plugins "")
# SET(library_directories "") #boost, cdf, hdf5 should be found automatically via otool -L
# install(CODE "
#     include(BundleUtilities)
#     fixup_bundle(\"${APPS}\" \"${plugins}\" \"${library_directories}\")
#     " COMPONENT libraries)

############# Wrappers ############ 
set(WRAPPERS_ROOT_DIR "${CCMC_ROOT_DIR}/wrappers")
add_subdirectory(${WRAPPERS_ROOT_DIR}/c)
add_subdirectory(${WRAPPERS_ROOT_DIR}/fortran)
add_subdirectory(${WRAPPERS_ROOT_DIR}/python)
add_subdirectory(${WRAPPERS_ROOT_DIR}/java)

