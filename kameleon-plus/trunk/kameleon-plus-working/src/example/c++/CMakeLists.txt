# C++ examples (kameleon-plus/src/examples/c++)

if (NOT KAMELEON_EXAMPLES_DIR)
    set(KAMELEON_EXAMPLES_DIR ${PROJECT_SOURCE_DIR})
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_BIN_DIR}/c++")

add_executable(adapt3d_test adapt3d_test.cpp)
add_executable(CDFReader CDFReader.cpp)
add_executable(coordinate_transformation_test coordinate_transformation_test.cpp)
add_executable(enlil_test enlil_test.cpp)
# add_executable(HDF5Reader HDF5Reader.cpp)
add_executable(IMFTest IMFTest.cpp)
add_executable(integrator_prog integrator_prog.cpp)
add_executable(kameleon_prog kameleon_prog.cpp)
add_executable(lfm_test lfm_test.cpp)
add_executable(magnetogram_test magnetogram_test.cpp)
add_executable(mas_test mas_test.cpp)
add_executable(open_ggcm_test open_ggcm_test.cpp)
add_executable(swmf_iono_test swmf_iono_test.cpp)
add_executable(time_interp time_interp.cpp)
add_executable(tracer_prog tracer_prog.cpp)
add_executable(pyModel_prog pyModel_prog.cpp)

# include ccmc
include_directories(${KAMELEON_SRC_DIR}) 

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

# python embedding
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
message(STATUS "c++/ Include python embedding directory: " ${PYTHON_INCLUDE_DIRS})

# link ccmc
target_link_libraries(adapt3d_test ccmc)
target_link_libraries(CDFReader ccmc)
target_link_libraries(coordinate_transformation_test ccmc)
target_link_libraries(enlil_test ccmc)
# target_link_libraries(HDF5Reader ccmc)
target_link_libraries(IMFTest ccmc)
target_link_libraries(integrator_prog ccmc)
target_link_libraries(kameleon_prog ccmc)
target_link_libraries(lfm_test ccmc)
target_link_libraries(magnetogram_test ccmc)
target_link_libraries(mas_test ccmc)
target_link_libraries(open_ggcm_test ccmc)
target_link_libraries(swmf_iono_test ccmc)
target_link_libraries(time_interp ccmc)
target_link_libraries(tracer_prog ccmc)
target_link_libraries(pyModel_prog ccmc)

