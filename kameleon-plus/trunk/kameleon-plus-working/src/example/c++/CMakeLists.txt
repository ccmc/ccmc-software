# C++ examples (kameleon-plus/src/examples/c++)
set(msg_prefix " ")
message("")
message("========== c++ examples(src/example/c++) ==========")
message(STATUS ${msg_prefix} "building c++ examples")
if (NOT KAMELEON_EXAMPLES_DIR)
    set(KAMELEON_EXAMPLES_DIR ${PROJECT_SOURCE_DIR})
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_BIN_DIR}/c++")

add_executable(adapt3d_test adapt3d_test.cpp)
add_executable(CDFReader CDFReader.cpp)
add_executable(coordinate_transformation_test coordinate_transformation_test.cpp)
add_executable(enlil_test enlil_test.cpp)
# add_executable(HDF5Reader HDF5Reader.cpp)
# add_executable(IMFTest IMFTest.cpp)
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
	message(STATUS ${msg_prefix} "Include HDF5: " ${HDF5_INCLUDE_DIR})
endif(BUILD_HDF5)

# Boost
include_directories(${BOOST_INCLUDEDIR})                                                                                          
message(STATUS ${msg_prefix} "Include boost directories: " ${BOOST_INCLUDEDIR})

# CDF
include_directories(${CDF_INCLUDES})
message(STATUS ${msg_prefix} "Include CDF: " ${CDF_INCLUDES})

# python embedding
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
message(STATUS "Include python embedding directory: " ${PYTHON_INCLUDE_DIRS})

# link ccmc
target_link_libraries(adapt3d_test ccmc)
target_link_libraries(CDFReader ccmc)
target_link_libraries(coordinate_transformation_test ccmc)
target_link_libraries(enlil_test ccmc)
# target_link_libraries(HDF5Reader ccmc)
# target_link_libraries(IMFTest ccmc)
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

if(UNIX)
set_target_properties(  kameleon_prog 
                                                adapt3d_test 
                                                CDFReader 
                                                coordinate_transformation_test
                                                enlil_test
                                                integrator_prog
                                                lfm_test
                                                magnetogram_test
                                                mas_test
                                                open_ggcm_test
                                                swmf_iono_test
                                                time_interp
                                                tracer_prog
                PROPERTIES INSTALL_RPATH "\$ORIGIN/../../../../lib/ccmc/;\$ORIGIN/../../../../lib/"
                )

endif(UNIX)

if(APPLE)
set_target_properties(	kameleon_prog 
						adapt3d_test 
						CDFReader 
						coordinate_transformation_test
						enlil_test
						integrator_prog
						lfm_test
						magnetogram_test
						mas_test
						open_ggcm_test
						swmf_iono_test
						time_interp
						tracer_prog
		PROPERTIES INSTALL_RPATH "@loader_path/../../../../lib/ccmc/;@loader_path/../../../../lib/"
		)

endif(APPLE)

###################### INSTALL #######################
install(TARGETS kameleon_prog 
				adapt3d_test 
				CDFReader 
				coordinate_transformation_test
				enlil_test
				integrator_prog
				lfm_test
				magnetogram_test
				mas_test
				open_ggcm_test
				swmf_iono_test
				time_interp
				tracer_prog
		DESTINATION bin/ccmc/examples/c++ COMPONENT Runtime )
		# PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ WORLD_EXECUTE WORLD_WRITE WORLD_READ)


# ### MACOSX_BUNDLE hack: copy required dynamic libaries and change their install names
# # set this as a target property for an executable, and cmake will produce a bundle application 
# # you can then use cpack's packagemaker to create a distribution
# add_executable(main_test MACOSX_BUNDLE main_test.cpp) # executable in main_test.app/Contents/Resources/bin
# target_link_libraries(main_test ccmc) #links to ccmc which links to required dylibs  
# set_target_properties(main_test PROPERTIES INSTALL_RPATH "@loader_path/../../../lib/ccmc")

## We need to copy any required libraries to the directory we want to fixup:

# install(FILES ${CDF_LIB} ${Boost_LIBRARIES}  ${HDF5_LIBRARIES} DESTINATION ${PACKAGE_LOC}/lib COMPONENT package_deps)
# install(FILES ${Boost_SYSTEM_LIBRARY} DESTINATION ${PACKAGE_LOC}/bin/ COMPONENT package)

# set(APPS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main_test.app)  # paths to executables
# set(APPS \${CMAKE_INSTALL_PREFIX}/bin/ccmc/MacOS/kameleon_prog)  # paths to executables
# set(DIRS ${PACKAGE_LOC}/lib ${PACKAGE_LOC}/bin)   # directories to search for prerequisites;
# set(LIBS ${CMAKE_INSTALL_PREFIX}/lib/ccmc/libccmc.dylib)
# set(LIBS ${CMAKE_INSTALL_PREFIX}/bin/ccmc/MacOS/_CCMC.so)
# set(DIRS ${CMAKE_INSTALL_PREFIX}/lib/ccmc)
# INSTALL(CODE "
#    include(BundleUtilities)
# 	fixup_bundle(\"${APPS}\" \"${LIBS}\" \"${DIRS}\")
#    " COMPONENT Runtime) #trying to prevent fixup from running at package time
# 

# install(TARGETS main_test BUNDLE DESTINATION bin COMPONENT examples)

# #Now copy the fixed up libaries into lib/ccmc (this avoids conflicts with the original libraries)
# install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main_test.app/Contents/MacOS/ DESTINATION lib/ccmc
#         PATTERN "main_test" EXCLUDE PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

##### CPACK BUNDLE generator ####### 
# This generator creates a disk image containing a Mac Bundle, but it doesn't fix_up any of the targets. From their TODO:
#  * Detect attempts to use the bundle generator with MACOSX_BUNDLE.
#  * Generate a default Info.plist file if CPACK_BUNDLE_PLIST is not defined.
#  * Support fixing-up binaries with install_name_tool, eliminating the need to run a script that sets DYLD_LIBRARY_PATH.
#  * Add arbitrary files (such as background images, READMEs, etc) to the disk image - this use-case is distinct from adding files to the bundle with INSTALL().
#  * Provide an option or alternative to CPACK_BUNDLE_STARTUP_COMMAND that simply executes one of the files installed in the bundle. 
#  * Presumably, this should be a symlink to a binary or script. 

set(CPACK_PACKAGE_FILE_NAME CCMC)
set(CPACK_PACKAGE_ICON ${KAMELEON_EXT_DIR}/CCMC-logo-DISK.png)
set(CPACK_BUNDLE_NAME Kameleon-plus)
set(CPACK_BUNDLE_ICON ${KAMELEON_EXT_DIR}/CCMC-logo.png)
set(CPACK_BUNDLE_PLIST ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main_test.app/Contents/Info.plist)
set(CPACK_BUNDLE_STARTUP_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/main_test.sh)
set(CPACK_BUNDLE_NAME "Kameleon-plus-${CMAKE_SYSTEM_NAME}-${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR}")

############# CPACK #################
set(CPACK_COMPONENTS_ALL Runtime) #libraries will force ccmc.dylib into the tar's /lib directory, runtime will place exes in /bin
set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "CCMC Example Programs")
set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "CCMC Example Programs")
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "CCMC libraries")
set(CPACK_COMPONENT_fixedUpPackage_DISPlAY_NAME "standalone example program")


set(CPACK_PACKAGE_NAME "CCMC")
set(CPACK_PACKAGE_VENDOR "ccmc.gsfc.nasa.gov/")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CCMC Kameleon-plus installer")
set(CPACK_PACKAGE_VERSION ${Kameleon_VERSION_MAJOR}.${Kameleon_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_MAJOR ${Kameleon_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Kameleon_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

# SET(CPACK_GENERATOR "Bundle")
# SET(CPACK_GENERATOR "DragNDrop")
############# CPACK TGZ Generator ########
# This builds an archive populated with any Install commands. It does not fix_up the libraries 
SET(CPACK_GENERATOR "TGZ")

INCLUDE(CPack)


