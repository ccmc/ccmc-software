# PYTHON wrapper
set(msg_prefix " ")
message("")
message("=========== python wrappers(src/ccmc/wrappers/python) ===========")
message(${msg_prefix} "setting up python wrappers..")
#Output directories
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/python/CCMC) 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/python)
SET(CMAKE_SWIG_OUTDIR ${KAMELEON_LIB_DIR}/ccmc/python/CCMC)


message(STATUS ${msg_prefix} "SWIG_USE_FILE: " ${SWIG_USE_FILE})
INCLUDE(${SWIG_USE_FILE})
SET(CMAKE_SWIG_FLAGS "")

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Kameleon includes
INCLUDE_DIRECTORIES(${KAMELEON_SRC_DIR}) 
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}) 

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


# ############# SWIG_ADD_MODULE ########
# # This code uses cmake's swig commands to generate wrapper code, compile and link with ccmc
SET_SOURCE_FILES_PROPERTIES(../ccmc.i PROPERTIES CPLUSPLUS ON)
#SWIG flag -builtin mostly supported in python 2.3 or 2.4, fully supported in 2.5+
SET_SOURCE_FILES_PROPERTIES(../ccmc.i PROPERTIES SWIG_FLAGS "-builtin")

SWIG_ADD_MODULE(CCMC python ../ccmc.i)

SWIG_LINK_LIBRARIES(CCMC ${PYTHON_LIBRARIES})
SWIG_LINK_LIBRARIES(CCMC ccmc) # link ccmc

set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup/setup.py.in")
set(SETUP_PY    "${KAMELEON_LIB_DIR}/ccmc/python/setup.py")
configure_file(${SETUP_PY_IN} ${SETUP_PY})
# naviage to where python module was created and run python install from command line
# message(STATUS "Python exe: " ${PYTHON_EXECUTABLE})
# install(CODE "
# 	execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install WORKING_DIRECTORY ${KAMELEON_LIB_DIR}/ccmc/python/)
# 	" COMPONENT python)
 
# add_subdirectory(${WRAPPERS_ROOT_DIR}/python/setup)
if(UNIX)
set_target_properties(_CCMC
		PROPERTIES INSTALL_RPATH "\$ORIGIN/../../../ccmc/;\$ORIGIN/../../../"
		)
endif(UNIX)

if(APPLE)
set_target_properties(_CCMC
		PROPERTIES INSTALL_RPATH "@loader_path/../../../ccmc/;@loader_path/../../../"
		)
endif(APPLE)
############# install in Kameleon distribution #####################
# This code installs the python module in ${KAMELEON_LIB_DIR}/ccmc/python
# install(TARGETS _CCMC DESTINATION lib/ccmc COMPONENT libraries)
# install(FILES ${CMAKE_SWIG_OUTDIR}/CCMC.py DESTINATION lib/ccmc COMPONENT headers)


if(INSTALL_CCMC_PYTHON_MODULE)

	############ system install ################################
	# This code determines the machine python library location by running python, then installs the CCMC python library there
	message(STATUS ${PYTHON_EXECUTABLE})
	message(STATUS "will install ccmc module to " ${PYTHON_SITE_PACKAGES})
	install(TARGETS _CCMC PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ DESTINATION ${PYTHON_SITE_PACKAGES}/ccmc)
	install(FILES ${CMAKE_SWIG_OUTDIR}/CCMC.py PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ DESTINATION ${PYTHON_SITE_PACKAGES}/ccmc)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ DESTINATION ${PYTHON_SITE_PACKAGES}/ccmc)
endif(INSTALL_CCMC_PYTHON_MODULE)



