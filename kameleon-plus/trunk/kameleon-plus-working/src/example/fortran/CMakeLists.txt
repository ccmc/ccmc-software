# fortran examples (kameleon-plus/src/example/fortran)
set(msg_prefix " ")
message("")
message("========== fortran examples(src/example/fortran) ==========")
message(STATUS ${msg_prefix} "building FORTRAN examples")
enable_language (Fortran)

if (NOT KAMELEON_EXAMPLES_DIR)
    set(KAMELEON_EXAMPLES_DIR ${PROJECT_SOURCE_DIR})
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_BIN_DIR}/fortran")

set(CMAKE_Fortran_FLAGS "-lstdc++ -v")

#bin_PROGRAMS = generalfilereader_compat_f kameleon_compat_f timeinterp_compat_f #tracer_compat_f 

add_executable(generalfilereader_compat_f GeneralFileReader_compatibility_test_f.f)
add_executable(kameleon_compat_f Kameleon_compatibility_test_f.f)
add_executable(time_series_test Time_Series_test.f)
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
	message(STATUS ${msg_prefix} "Include HDF5: " ${HDF5_INCLUDE_DIR})
endif(BUILD_HDF5)

# Boost
include_directories(${BOOST_INCLUDEDIR})                                                                                          
message(STATUS ${msg_prefix} "Include boost directories: " ${BOOST_INCLUDEDIR})

# CDF
include_directories(${CDF_INCLUDES})
message(STATUS ${msg_prefix} "Include CDF: " ${CDF_INCLUDES})

# embedded python
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

# link ccmc ccmc_wrapper_c ccmc_wrapper_fortran
target_link_libraries(generalfilereader_compat_f ccmc_wrapper_fortran)
target_link_libraries(kameleon_compat_f ccmc_wrapper_fortran)
target_link_libraries(time_series_test ccmc_wrapper_fortran)
target_link_libraries(timeinterp_compat_f ccmc_wrapper_fortran)
target_link_libraries(tracer_compat_f ccmc_wrapper_fortran)


if(UNIX)
set_target_properties(  generalfilereader_compat_f 
                        kameleon_compat_f 
                        time_series_test 
                        timeinterp_compat_f
                        tracer_compat_f
                PROPERTIES INSTALL_RPATH "\$ORIGIN/../../../../lib/ccmc/;\$ORIGIN/../../../../lib/"
                )

endif(UNIX)

if(APPLE)
set_target_properties(	generalfilereader_compat_f 
                        kameleon_compat_f 
                        time_series_test 
                        timeinterp_compat_f
                        tracer_compat_f
				PROPERTIES INSTALL_RPATH "@loader_path/../../../../lib/ccmc/;@loader_path/../../../../lib/"
				)

endif(APPLE)

###################### INSTALL #######################
install(TARGETS generalfilereader_compat_f 
                        kameleon_compat_f 
                        time_series_test 
                        timeinterp_compat_f
                        tracer_compat_f
		DESTINATION bin/ccmc/examples/fortran COMPONENT Runtime )
		# PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ WORLD_EXECUTE WORLD_WRITE WORLD_READ)


