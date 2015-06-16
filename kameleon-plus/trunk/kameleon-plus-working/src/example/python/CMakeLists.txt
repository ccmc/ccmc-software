# collect python files and copy to bin
set(msg_prefix " ")
message("###########-python examples(src/example/python)-###########")
message(STATUS ${msg_prefix} "building python examples")
file(GLOB PYTHON_EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/*.py" )
message(STATUS ${msg_prefix} "Python example files: " ${PYTHON_EXAMPLES} )
message(STATUS ${msg_prefix} "will be installed to: " ${EXAMPLES_BIN_DIR}/python)
file(COPY ${PYTHON_EXAMPLES} DESTINATION ${EXAMPLES_BIN_DIR}/python)

