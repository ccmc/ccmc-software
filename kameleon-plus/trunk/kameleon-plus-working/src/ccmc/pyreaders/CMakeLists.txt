cmake_minimum_required(VERSION 2.8)
PROJECT (boost_python_test)
set(msg_prefix " ")
message("")
message("=========== pyKameleon(src/ccmc/pyreader) ===========")

message("setting up python...")
FIND_PACKAGE(PythonInterp)
FIND_PACKAGE( PythonLibs 2.7 REQUIRED)
message(STATUS ${msg_prefix} "PYTHON_LIBRARIES:" ${PYTHON_LIBRARIES})
message(STATUS ${msg_prefix} "PYTHON_INCLUDE_DIRS:" ${PYTHON_INCLUDE_DIRS})
message(STATUS ${msg_prefix} "PYTHON_INCLUDE_PATH:" ${PYTHON_INCLUDE_PATH})
message(STATUS ${msg_prefix} "PYTHON_EXECUTABLE:" ${PYTHON_EXECUTABLE})


INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

# getting Boost.python to link with the right python libraries on a mac can be tricky
# if you've installed boost with homebrew. Look at these instructions:
# http://brianfosterallen.com/devblog/blog/2013/06/13/Using-Homebrew-s-Python-with-boost-python-on-OSX/

#the following c++ flags have to do with compatibility between clang and gcc compilers on mac
# set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++") 


# Boost 
# FIND_PACKAGE(Boost 1.54.0 COMPONENTS python)

message("setting up boost.python")
INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)

# set(Boost_USE_STATIC_LIBS OFF) #was set to OFF
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)

message(STATUS ${msg_prefix} "Boost_LIBRARIES:" ${Boost_LIBRARIES})
message(STATUS ${msg_prefix} "BOOST_INCLUDEDIR:" ${BOOST_INCLUDEDIR})


set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so") #allows macs to import 


# create the shared library for pyKameleon, to be imported by python modules
add_library(pyKameleon SHARED ../Attribute.h 
							../Attribute.cpp 
							../FileReader.h 
							../FileReader.cpp 
							generated/pyKameleon.cpp)

target_link_libraries(pyKameleon ${Boost_LIBRARIES}  ${PYTHON_LIBRARIES})

# set install_rpath
if(UNIX)
set_target_properties(pyKameleon
        PROPERTIES INSTALL_RPATH "\$ORIGIN/../"
        )
endif(UNIX)
if(APPLE)
set_target_properties(pyKameleon
        PROPERTIES INSTALL_RPATH "@loader_path/../"
        )
endif(APPLE)


install(TARGETS pyKameleon 
        DESTINATION lib/ccmc COMPONENT libraries 
        )

# make pyreaders directory in lib/ccmc
file(MAKE_DIRECTORY lib/ccmc/pyreaders) #keep a copy in src directory

# gather pyreader scripts
file(GLOB PYREADER_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/*.py" )

# configure and move the pyreader scripts
FOREACH(infileName ${PYREADER_SCRIPTS})
    MESSAGE(STATUS ${msg_prefix} "Process file: ${infileName}")
    configure_file(${infileName} lib/ccmc/pyreaders NEWLINE_STYLE UNIX)
ENDFOREACH(infileName)

# on install, copy scripts to lib directory
install(FILES ${PYREADER_SCRIPTS} DESTINATION lib/ccmc/pyreaders COMPONENT libraries ) 

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}../)

# remove this line?
INCLUDE_DIRECTORIES("/Users/apembrok/git/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/build")

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)







