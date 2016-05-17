# collect python files and copy to bin
set(msg_prefix " ")
message("")
message("=========== python examples(src/example/python) ===========")
message(STATUS ${msg_prefix} "building python examples")
file(GLOB PYTHON_EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/*.py" )

option(APPEND_PYTHON_PATHS "Include path to CCMC module in examples" ON)

#include path ccmc module kept in source
if(APPEND_PYTHON_PATHS)
	set(CCMC_SRC_PATH_STR "sys.path.append('${CCMC_DIR}')") #unrelocatable src path!!
	set(CCMC_LIB_PATH_STR "sys.path.append('${KAMELEON_LIB_DIR}/ccmc/')")
	set(CCMC_MODULE_PATH_STR "sys.path.append('${KAMELEON_LIB_DIR}/ccmc/python/CCMC/')")
endif(APPEND_PYTHON_PATHS)
# configure for source
FOREACH(infileName ${PYTHON_EXAMPLES})
    MESSAGE(STATUS ${msg_prefix} "Process file: ${infileName}")
    configure_file(${infileName} ${EXAMPLES_BIN_DIR}/python NEWLINE_STYLE UNIX)
ENDFOREACH(infileName)

message(STATUS ${msg_prefix} "Python example files will be kept in: " ${EXAMPLES_BIN_DIR}/python)


# configure for install
if(INSTALL_CCMC_PYTHON_MODULE)
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/python_examples/python)
	set(CCMC_SRC_PATH_STR "") #empty since pyreaders will be installed to lib/ccmc
	set(CCMC_MODULE_PATH_STR "sys.path.append('../../../../${PYTHON_SITE_PACKAGES}/ccmc/')")
	set(CCMC_LIB_PATH_STR "sys.path.append('../../../../lib/ccmc/')")
	message(STATUS ${msg_prefix} "Using CCMC module install path in scripts: " ${CCMC_MODULE_PATH_STR})
	FOREACH(infileName ${PYTHON_EXAMPLES})
	    MESSAGE(STATUS ${msg_prefix} "Process file: ${infileName}")
	    MESSAGE(STATUS ${msg_prefix} "... will install to: ${CMAKE_INSTALL_PREFIX}/bin/ccmc/examples/python/")
	    configure_file(${infileName} ${CMAKE_BINARY_DIR}/python_examples/python NEWLINE_STYLE UNIX)
	ENDFOREACH(infileName)
	install(DIRECTORY ${CMAKE_BINARY_DIR}/python_examples/python DESTINATION bin/ccmc/examples)
endif(INSTALL_CCMC_PYTHON_MODULE)
