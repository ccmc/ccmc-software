# core kameleon library (kameleon-plus/src/ccmc)
set(msg_prefix "src/ccmc> ")
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
    message(STATUS ${msg_prefix} "ignoring HDF5FileReader.cpp  HDF5FileReader.h")
    list(REMOVE_ITEM CCMC_SOURCE ${CCMC_ROOT_DIR}/HDF5FileReader.cpp)
    list(REMOVE_ITEM CCMC_HEADER ${CCMC_ROOT_DIR}/HDF5FileReader.h)
    # message(STATUS "new list:" ${CCMC_SOURCE})
endif(NOT BUILD_HDF5)

source_group(Ccmc FILES ${CCMC_SOURCE}) 
source_group(Ccmc FILES ${CCMC_HEADER})  

set(ALL_SOURCE ${CCMC_SOURCE})
set(ALL_HEADER ${CCMC_HEADER})

if(BUILD_SHARED_LIBS)
    message(STATUS ${msg_prefix} "will build SHARED ccmc library")
    add_library(ccmc SHARED ${ALL_SOURCE} ${ALL_HEADER})
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    message(STATUS ${msg_prefix} "will build STATIC ccmc library")
    add_library(ccmc_static STATIC ${ALL_SOURCE} ${ALL_HEADER})
endif(BUILD_STATIC_LIBS)


set_target_properties(ccmc PROPERTIES SOVERSION ${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR})
#set the install_name of the lib relative to the executables 

get_property(ccmc_lib_loc TARGET ccmc PROPERTY LOCATION)
message (STATUS ${msg_prefix} "ccmc library location = " ${ccmc_lib_loc})


############ Boost #############
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 

    if(BUILD_SHARED_LIBS)
        target_link_libraries(ccmc ${Boost_LIBRARIES}) 
    endif(BUILD_SHARED_LIBS)    

    if(BUILD_STATIC_LIBS)
        target_link_libraries(ccmc_static ${Boost_LIBRARIES})
    endif(BUILD_STATIC_LIBS)

    message(STATUS ${msg_prefix} "Include boost: " ${Boost_INCLUDE_DIRS})    
    message(STATUS ${msg_prefix} "Boost library directory: " ${Boost_LIBRARY_DIR})
    message(STATUS ${msg_prefix} "Link boost: " ${Boost_LIBRARIES})    
endif()


############## CDF #############
include_directories(${CDF_INCLUDES})

if(BUILD_SHARED_LIBS)
    target_link_libraries(ccmc ${CDF_LIB}) 
endif(BUILD_SHARED_LIBS) 

if(BUILD_STATIC_LIBS)
    target_link_libraries(ccmc_static ${CDF_LIB})
endif(BUILD_STATIC_LIBS)

message(STATUS ${msg_prefix} "Include CDF: " ${CDF_INCLUDES})
message(STATUS ${msg_prefix} "link CDF: " ${CDF_LIB})
# point to kameleon-plus-Config.h
include_directories(${PROJECT_BINARY_DIR})

############## hdf5 #############
if(BUILD_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
    add_definitions(-DHAVE_HDF5)
	message(STATUS ${msg_prefix} "Include HDF5: " ${HDF5_INCLUDE_DIR}) 
	message(STATUS ${msg_prefix} "Link HDF5_LIBRARIES: " ${HDF5_LIBRARIES}) 

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

FIND_PACKAGE(SWIG)
if(SWIG_FOUND)
    MESSAGE(STATUS ${msg_prefix} "SWIG installed. Will attempt to build python and java wrappers.")
    FIND_PACKAGE(PythonLibs)
    FIND_PACKAGE(PythonInterp)
    IF(NOT PYTHONLIBS_FOUND OR NOT PYTHON_EXECUTABLE)
        MESSAGE(STATUS ${msg_prefix} "Could not find python. You need Python to build Python binding")
    ELSE()
        MESSAGE(STATUS ${msg_prefix} "PythonLibs and python executable found. Will build python wrapper.")
        add_subdirectory(${WRAPPERS_ROOT_DIR}/python)
    ENDIF(NOT PYTHONLIBS_FOUND OR NOT PYTHON_EXECUTABLE)

    FIND_PACKAGE(Java)
    FIND_PACKAGE(JNI)
    IF(JAVA_FOUND AND JNI_FOUND)
        MESSAGE(STATUS ${msg_prefix} "Java and JNI libraries found. Will build Java wrappers")
        add_subdirectory(${WRAPPERS_ROOT_DIR}/java)
    ELSE()
        MESSAGE(STATUS ${msg_prefix} "Java and JNI libraries not found.. cannot build Java wrappers")
        MESSAGE(STATUS "java flag: " Java_FOUND " JNI flag:" JNI_FOUND)
    ENDIF(JAVA_FOUND AND JNI_FOUND)
endif(SWIG_FOUND)


