# C examples (kameleon-plus/src/example/c)
set(msg_prefix " ")
message("")
message("=========== c examples(src/example/c) ===========")
message(STATUS ${msg_prefix} "building c examples")

if (NOT KAMELEON_EXAMPLES_DIR)
    set(KAMELEON_EXAMPLES_DIR ${PROJECT_SOURCE_DIR})
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_BIN_DIR}/c")

add_executable(kameleon_compat_test Kameleon_compatibility_test.c)
add_executable(tracer_c tracer_prog.c)
add_executable(time_interp_c time_interpolator_test.c)
add_executable(filereader_compat_test GeneralFileReader_compatibility_test.c)

# include ccmc
include_directories(${KAMELEON_SRC_DIR}) 

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


# link ccmc_wrapper_c
target_link_libraries(kameleon_compat_test ccmc ccmc_wrapper_c)
target_link_libraries(tracer_c ccmc ccmc_wrapper_c)
target_link_libraries(time_interp_c ccmc ccmc_wrapper_c)
target_link_libraries(filereader_compat_test ccmc ccmc_wrapper_c)


if(UNIX)
set_target_properties(  kameleon_compat_test 
                        tracer_c 
                        time_interp_c 
                        filereader_compat_test
                PROPERTIES INSTALL_RPATH "\$ORIGIN/../../../../lib/ccmc/;\$ORIGIN/../../../../lib/"
                )

endif(UNIX)

if(APPLE)
set_target_properties(	kameleon_compat_test 
                        tracer_c 
                        time_interp_c 
                        filereader_compat_test
				PROPERTIES INSTALL_RPATH "@loader_path/../../../../lib/ccmc/;@loader_path/../../../../lib/"
				)

endif(APPLE)

###################### INSTALL #######################
install(TARGETS kameleon_compat_test 
                tracer_c 
                time_interp_c 
                filereader_compat_test
		DESTINATION bin/ccmc/examples/c COMPONENT Runtime )
		# PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ WORLD_EXECUTE WORLD_WRITE WORLD_READ)

