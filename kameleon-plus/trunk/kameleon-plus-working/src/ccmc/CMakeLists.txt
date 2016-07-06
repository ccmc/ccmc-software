# core kameleon library (kameleon-plus/src/ccmc)
set(msg_prefix "src/ccmc>")
message(" ")
message("============ ccmc library(" ${msg_prefix} ") ==============")
set(msg_prefix " ")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc) 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc)

if (NOT CCMC_ROOT_DIR)
    set(CCMC_ROOT_DIR ${PROJECT_SOURCE_DIR})
endif ()

file(GLOB CCMC_SOURCE
    ${CCMC_ROOT_DIR}/*.cpp ${CCMC_ROOT_DIR}/*.c
)
file(GLOB CCMC_HEADER
    ${CCMC_ROOT_DIR}/*.h ${CCMC_ROOT_DIR}/*.hpp
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

# Fix up the install names
# INSTALL_NAME=`otool -D ${PYTHON_LIBRARIES} | tail -n1 | tr -d '\n'`
# install_name_tool -change ${INSTALL_NAME} ${PYTHON_LIBRARIES} ${LIB_FILENAME}

set_target_properties(ccmc PROPERTIES SOVERSION ${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR})
#set the install_name of the lib relative to the executables 

#let the user know where the ccmc library will end up
get_property(ccmc_lib_loc TARGET ccmc PROPERTY LOCATION)
message (STATUS ${msg_prefix} "ccmc library output location = " ${ccmc_lib_loc})


############ Boost #############
if(Boost_FOUND)
    message(STATUS ${msg_prefix} "Include boost: " ${BOOST_INCLUDEDIR})    
    message(STATUS ${msg_prefix} "Boost library directory: " ${BOOST_LIBRARYDIR})
    message(STATUS ${msg_prefix} "Link boost: " ${Boost_LIBRARIES})    

    include_directories(${BOOST_INCLUDEDIR}) 

    if(BUILD_SHARED_LIBS)
        target_link_libraries(ccmc ${Boost_LIBRARIES}) 
    endif(BUILD_SHARED_LIBS)    

    if(BUILD_STATIC_LIBS)
        target_link_libraries(ccmc_static ${Boost_LIBRARIES})
    endif(BUILD_STATIC_LIBS)
endif()


############## CDF #############
INCLUDE_DIRECTORIES(${CDF_INCLUDES})
if(BUILD_SHARED_LIBS)
    target_link_libraries(ccmc ${CDF_LIB}) 
endif(BUILD_SHARED_LIBS) 

if(BUILD_STATIC_LIBS)
    target_link_libraries(ccmc_static ${CDF_LIB})
endif(BUILD_STATIC_LIBS)

message(STATUS ${msg_prefix} "Include CDF: " ${CDF_INCLUDES})
message(STATUS ${msg_prefix} "link CDF: " ${CDF_LIB})


############## hdf5 #############
if(BUILD_HDF5)
    INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR})
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


############# python ############
message(STATUS ${msg_prefix} "cmake framework path: " ${CMAKE_FRAMEWORK_PATH})
# FIND_PACKAGE(PythonInterp 2.7)
# FIND_PACKAGE(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)
if(PYTHONINTERP_FOUND)
    message(STATUS ${msg_prefix} "Python Interpreter:" ${PYTHON_EXECUTABLE})
    add_definitions(-DHAVE_PYTHON) # preprocessor to insert python interpreter calls
endif()

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS} )
target_link_libraries(ccmc  ${PYTHON_LIBRARIES})

############## embedded python #######
if(Boost_PYTHON_LIBRARY_RELEASE)
    message(STATUS ${msg_prefix} "boost.python library found, creating embedded readers..")   
    add_subdirectory(pyreaders)

    # make sure pyKameleon gets built when ccmc library is built
    if(BUILD_SHARED_LIBS)
        target_link_libraries(ccmc pyKameleon) 
    endif(BUILD_SHARED_LIBS)    

    if(BUILD_STATIC_LIBS)
        target_link_libraries(ccmc_static pyKameleon)
    endif(BUILD_STATIC_LIBS)    
endif(Boost_PYTHON_LIBRARY_RELEASE)


# point to kameleon-plus-Config.h
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})


############ installer ############



#install the library 
if(BUILD_SHARED_LIBS)
    if(UNIX)
    set_target_properties(ccmc
            PROPERTIES INSTALL_RPATH "\$ORIGIN/../;\$ORIGIN/"
            )
    endif(UNIX)
    if(APPLE)
    set_target_properties(ccmc
            PROPERTIES INSTALL_RPATH "@loader_path/../;@loader_path/"
            )
    endif(APPLE)

    install(TARGETS ccmc 
        DESTINATION lib/ccmc COMPONENT libraries 
        )
    install(FILES ${ALL_HEADER}
        DESTINATION include/ccmc COMPONENT libraries)
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

# Wrappers
set(WRAPPERS_ROOT_DIR "${CCMC_ROOT_DIR}/wrappers")


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

    if(BUILD_JAVA)
        FIND_PACKAGE(Java)
        FIND_PACKAGE(JNI)
        IF(JAVA_FOUND AND JNI_FOUND)
            MESSAGE(STATUS ${msg_prefix} "Java and JNI libraries found. Will build Java wrappers")
            add_subdirectory(${WRAPPERS_ROOT_DIR}/java)
        ELSE()
            MESSAGE(STATUS ${msg_prefix} "Java and JNI libraries not found.. cannot build Java wrappers")
            MESSAGE(STATUS "java flag: " Java_FOUND " JNI flag:" JNI_FOUND)
        ENDIF(JAVA_FOUND AND JNI_FOUND)
    endif(BUILD_JAVA)
endif(SWIG_FOUND)

add_subdirectory(${WRAPPERS_ROOT_DIR}/c)
add_subdirectory(${WRAPPERS_ROOT_DIR}/fortran)

