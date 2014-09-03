cmake_minimum_required(VERSION 2.8)
PROJECT (boost_python_test)

FIND_PACKAGE( PythonLibs 2.7 REQUIRED)
FIND_PACKAGE(PythonInterp)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

# getting Boost.python to link with the right python libraries on a mac can be tricky
# if you've installed boost with homebrew. Look at these instructions:
# http://brianfosterallen.com/devblog/blog/2013/06/13/Using-Homebrew-s-Python-with-boost-python-on-OSX/

#the following c++ flags have to do with compatibility between clang and gcc compilers on mac
# set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++") 


# Boost 
FIND_PACKAGE(Boost 1.54.0 COMPONENTS python)
IF (${Boost_FOUND})
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)

	set(Boost_USE_STATIC_LIBS OFF) #was set to OFF
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)

	message(STATUS "Boost_LIBRARIES:" ${Boost_LIBRARIES})
	message(STATUS "Boost includes:" ${Boost_INCLUDE_DIRS})
	message(STATUS "Python Libraries:" ${PYTHON_LIBRARIES})
	message(STATUS "Python includes:" ${PYTHON_INCLUDE_PATH})
	message(STATUS "Python interpreter:" ${PYTHON_EXECUTABLE})


	set(CMAKE_SHARED_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_LIBRARY_SUFFIX ".so") #allows macs to import 
	# add_library(Baz SHARED testVirtual.cpp ) #python module
	# target_link_libraries(Baz ${Boost_LIBRARIES}  ${PYTHON_LIBRARIES})


	# create the shared library for pyKameleon, to be imported by python modules
	add_library(pyKameleon SHARED ../Attribute.h 
								../Attribute.cpp 
								../FileReader.h 
								../FileReader.cpp 
								generated/pyKameleon.cpp)
	# set_target_properties( pyKameleon PROPERTIES IMPORT_SUFFIX ".so")

	# not sure why I was linking to pyKameleon to Baz, uncommenting to see if it works
	target_link_libraries(pyKameleon ${Boost_LIBRARIES}  ${PYTHON_LIBRARIES})


	add_executable(testDerived testDerivedpy.cpp ../Attribute.h 
								../Attribute.cpp 
								../FileReader.h 
								../FileReader.cpp 
								generated/pyKameleon.cpp)

	target_link_libraries(testDerived ${Boost_LIBRARIES}  ${PYTHON_LIBRARIES}) 
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})



ELSE()

ENDIF(${Boost_FOUND})



