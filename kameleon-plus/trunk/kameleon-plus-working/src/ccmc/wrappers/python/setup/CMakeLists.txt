#generates setup file for installation
message(STATUS "wrappers/python/setup")

############# setup.py ##############
# This code generates setup.py using setup.py.in as a template, with variable values supplied by CMakeLists file.
# This code follows the example here: http://bloerg.net/2012/11/10/cmake-and-distutils.html

# """ The target that we add to the build system needs a way to tell when it’s time to run. 
# For this, the add_custom_command specifies one or more outputs on which the target depends. 
# But the output from python setup.py build is not a good candidate because it is placed in architecture- 
# and platform-specific directories like lib.linux-x86_64-2.6. And you don’t want to test on that in a sane and portable way.
# What we can do is to generate a file in the build root, each time our source dependencies change. 
# The simplest way to do this, is to touch a stamp file and thus update its modification time. The dependency graph then looks like this:
#
# 						Target --depends-> Stamp --depends-> Source
#
# Last but not least, we install the package with the CODE form of the install command. 
# install(CODE) is tied to the install target and expects a CMake command. For our purposes this 
# is execute_process that in turn runs python setup.py install. """

# find_program(PYTHON "python")
# if (PYTHON)
#     set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
#     set(SETUP_PY    "${KAMELEON_LIB_DIR}/ccmc/python/setup.py")
#     # set(DEPS        "${CMAKE_CURRENT_SOURCE_DIR}/CCMC/__init__.py" )
#     set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

#     configure_file(${SETUP_PY_IN} ${SETUP_PY})

#     add_custom_command(OUTPUT ${OUTPUT}
#                        COMMAND ${PYTHON} ${SETUP_PY} build 
#                        COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
#                        DEPENDS ${DEPS})

#     add_custom_target(target ALL DEPENDS ${OUTPUT})

#     install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install)")
# endif()