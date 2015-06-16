cmake_minimum_required(VERSION 2.8)
PROJECT (boost_python_test_sub)

# FIND_PACKAGE(PythonInterp)
FIND_PACKAGE( PythonLibs 2.7 REQUIRED)


# INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})


# getting Boost.python to link with the right python libraries on a mac can be tricky
# if you've installed boost with homebrew. Look at these instructions:
# http://brianfosterallen.com/devblog/blog/2013/06/13/Using-Homebrew-s-Python-with-boost-python-on-OSX/

#the following c++ flags have to do with compatibility between clang and gcc compilers on mac
# set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++") 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../) # points to ccmc/


# Boost 
FIND_PACKAGE(Boost 1.54.0 COMPONENTS python)
IF (${Boost_FOUND})
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	

	add_executable(testDerived testDerivedpy.cpp 
								../../Attribute.h 
								../../Attribute.cpp 
								../../FileReader.h 
								../../FileReader.cpp 
								)

	target_link_libraries(testDerived 
							${PYTHON_LIBRARIES}
							${Boost_LIBRARIES}  
							) 

	INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

ELSE()

ENDIF(${Boost_FOUND})



