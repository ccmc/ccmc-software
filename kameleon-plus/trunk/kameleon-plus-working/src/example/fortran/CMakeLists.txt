# fortran examples (kameleon-plus/src/examples/fortran)
enable_language (Fortran)

if (NOT KAMELEON_EXAMPLES_DIR)
    set(KAMELEON_EXAMPLES_DIR ${PROJECT_SOURCE_DIR})
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_BIN_DIR}/fortran")

set(CMAKE_Fortran_FLAGS "-lstdc++ -v")

#bin_PROGRAMS = generalfilereader_compat_f kameleon_compat_f timeinterp_compat_f #tracer_compat_f 

add_executable(generalfilereader_compat_f GeneralFileReader_compatibility_test_f.f)
add_executable(kameleon_compat_f Kameleon_compatibility_test_f.f)
add_executable(timeinterp_compat_f timeinterp_compatibility_test_f.f)
add_executable(tracer_compat_f Tracer_test_f.f)

# include ccmc
include_directories(${KAMELEON_SRC_DIR}/ccmc)
include_directories(${KAMELEON_SRC_DIR}/ccmc/wrappers/c)
include_directories(${KAMELEON_SRC_DIR}/ccmc/wrappers/fortran)

# hdf5
if(BUILD_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
	add_definitions(-DHAVE_HDF5)
	message(STATUS "c++/ Include HDF5: " ${HDF5_INCLUDE_DIR})
endif(BUILD_HDF5)

# Boost
include_directories(${Boost_INCLUDE_DIRS})                                                                                          
message(STATUS "c++/ Include boost directories: " ${Boost_INCLUDE_DIRS})

# CDF
include_directories(${CDF_INCLUDES})
message(STATUS "c++/ Include CDF: " ${CDF_INCLUDES})


# link ccmc ccmc_wrapper_c ccmc_wrapper_fortran
target_link_libraries(generalfilereader_compat_f ccmc_wrapper_fortran)
target_link_libraries(kameleon_compat_f ccmc_wrapper_fortran)
target_link_libraries(timeinterp_compat_f ccmc_wrapper_fortran)
target_link_libraries(tracer_compat_f ccmc_wrapper_fortran)


