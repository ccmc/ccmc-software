# collect python files and copy to 

file(GLOB PYTHON_EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/*.py" )
message(STATUS "Python example files: " ${PYTHON_EXAMPLES} )
message(STATUS "Will be installed to: " ${EXAMPLES_BIN_DIR}/python)
file(COPY ${PYTHON_EXAMPLES} DESTINATION ${EXAMPLES_BIN_DIR}/python)

