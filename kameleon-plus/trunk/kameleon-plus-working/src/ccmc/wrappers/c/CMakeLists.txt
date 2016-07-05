# c wrapper library
set(msg_prefix "")
message("")
message("=========== c wrappers(src/ccmc/wrappers/c) ===========")
message(STATUS ${msg_prefix} "initializing c wrappers")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/c) 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/c)
file(GLOB CCMC_WRAPPER_C_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB CCMC_WRAPPER_C_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
# set(CMAKE_CXX_FLAGS "-fPIC")

set(ALL_SOURCE ${CCMC_WRAPPER_C_SOURCES})
set(ALL_HEADER ${CCMC_WRAPPER_C_HEADER})

if(BUILD_SHARED_LIBS)
    message(STATUS ${msg_prefix} "will build SHARED ccmc_wrapper_c library")
    add_library(ccmc_wrapper_c SHARED ${ALL_SOURCE} ${ALL_HEADER})

	install(TARGETS ccmc_wrapper_c 
        DESTINATION lib/ccmc COMPONENT libraries 
        )
    install(FILES ${ALL_HEADER}
        DESTINATION include/ccmc COMPONENT libraries)
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    message(STATUS ${msg_prefix} "will build STATIC ccmc_wrapper_c library")
    add_library(ccmc_wrapper_c_static STATIC ${ALL_SOURCE} ${ALL_HEADER})
    
    install(TARGETS ccmc_wrapper_c_static
        DESTINATION lib/ccmc COMPONENT libraries 
        )
endif(BUILD_STATIC_LIBS)


    

# include ccmc
include_directories(${KAMELEON_SRC_DIR})

target_link_libraries(ccmc_wrapper_c ccmc)

# if(APPLE)
#     set_property(TARGET ccmc_wrapper_c PROPERTY MACOSX_RPATH TRUE)
# endif(APPLE)