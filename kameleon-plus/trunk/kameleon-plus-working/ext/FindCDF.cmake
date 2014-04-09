
if(NOT CDF_PATH)
	set(CDF_PATH "/Applications/cdf35_0-dist")
endif()

find_path(CDF_INCLUDES cdf.h
            HINTS "${CDF_PATH}/include")

if(CDF_USE_STATIC_LIBS)
    find_library(CDF_LIB NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}cdf${CMAKE_STATIC_LIBRARY_SUFFIX}
                 HINTS "${CDF_PATH}/lib")
else()
    find_library(CDF_LIB NAMES ${CMAKE_SHARED_LIBRARY_PREFIX}cdf${CMAKE_SHARED_LIBRARY_SUFFIX}
                 HINTS "${CDF_PATH}/lib")
endif(CDF_USE_STATIC_LIBS)

set(CDF_LIBRARIES "${CDF_LIB}")