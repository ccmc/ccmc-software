# fortran wrapper library
set(msg_prefix " ")
message("")
message("========== FORTRAN wrappers(src/ccmc/wrappers/fortran) ==========")
message(STATUS ${msg_prefix} "building fortran wrappers")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/fortran) 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/fortran)

file(GLOB CCMC_WRAPPER_FORTRAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB CCMC_WRAPPER_FORTRAN_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
# set(CMAKE_CXX_FLAGS "-fPIC")

set(ALL_SOURCE ${CCMC_WRAPPER_FORTRAN_SOURCES})
set(ALL_HEADER ${CCMC_WRAPPER_FORTRAN_HEADER})

add_library(ccmc_wrapper_fortran ${ALL_SOURCE} ${ALL_HEADER})

include_directories(${KAMELEON_SRC_DIR})

target_link_libraries(ccmc_wrapper_fortran ccmc_wrapper_c ccmc)

install(TARGETS ccmc_wrapper_fortran 
    DESTINATION lib/ccmc COMPONENT libraries 
    )
install(FILES ${ALL_HEADER}
    DESTINATION include/ccmc COMPONENT libraries)


# if(APPLE)
#     set_property(TARGET ccmc_wrapper_fortran PROPERTY MACOSX_RPATH TRUE)
# endif(APPLE)