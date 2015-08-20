# collect python files and copy to bin
set(msg_prefix " ")
message("")
message("=========== python examples(src/example/python) ===========")
message(STATUS ${msg_prefix} "building python examples")
file(GLOB PYTHON_EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/*.py" )
message(STATUS ${msg_prefix} "Python example files will be installed to: " ${EXAMPLES_BIN_DIR}/python)

FOREACH(infileName ${PYTHON_EXAMPLES})
    MESSAGE(STATUS ${msg_prefix} "Process file: ${infileName}")
    configure_file(${infileName} ${EXAMPLES_BIN_DIR}/python)
ENDFOREACH(infileName)

