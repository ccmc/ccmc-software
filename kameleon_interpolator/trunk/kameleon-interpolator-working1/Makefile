########## Kameleon Interpolator Makefile 10.22.2007

########## set your compilers here

CC = gcc
F77 = gfortran

CC_FULL_PATH =
F77_FULL_PATH =

########## Default Compiler Flags

#CFLAGS = -D$(F77) -fPIC -ansi -Wall -Wno-unused-variable
CFLAGS = -D$(F77) -fPIC

FFLAGS = -fno-second-underscore

########## OS specific system commands

### suns regular grep /usr/bin/grep doesn't support the -e option so lets use 
### sed instead

SUN_GREP = sed 's/ .*//' 

######## main directory structure of the interpolation software distribution.
######## libkameleon.a & libkameleon.so libraries will be created in ./lib

SRC_DIR = ./src
INC_DIR = ./inc
LIB_DIR = ./lib
BIN_DIR = ./bin

F77_SRC_DIR = ./fortran_interface

EXAMPLE_DIR = ./examples

IDL_INTERFACE_DIR = ./idl_interface

KAMELEON_INC = ${INC_DIR}/kameleon.h

######### set the location of the static cdf library. ie. 
######### LIB_CDF = /home/mmaddox/cdf/lib/libcdf.a

LIB_CDF_DIR = ${LIB_DIR}
LIB_CDF = ${LIB_CDF_DIR}/libcdf.a


STATIC_INTERP_LIB = ${LIB_DIR}/libkameleon.a

IDL_BUNDLE_PLUGGIN = ${LIB_DIR}/idl2kameleon.bundle
IDL_SHARED_LIBRARY = ${LIB_DIR}/idl2kameleon.so

######## SYSTEM SPECIFIC FLAGS

## set the location of any extra library paths that are reguired for things like -lm -lg2c ...
## because many systms have unconventional ( read weird ) layouts.

EXTRA_LIB_PATHS = -L/usr/lib/gcc-lib/x86_64-redhat-linux/3.2.3

FORTRAN_LIB = -L/usr/local/lib/x86_64

## set the location of any extra include paths that are reguired for things like cdf.h, export.h, idl_export.h ...
## because many systmes have unconventional ( read weird ) layouts.

IDL_INC_DIR = ${IDL_INTERFACE_DIR}/idl_inc

CDF_INC = ${INC_DIR}

CDF_INC_FILE = ${CDF_INC}/cdf.h

######## OS Specific Flags

#DARWIN_SPECIFIC_FLAGS = -L/sw/lib -lc -lm -lg2c -lSystemStubs
DARWIN_SPECIFIC_FLAGS = -Wall -L/usr/local/lib -lc -lm -lSystemStubs -Wno-unused-variable
#SUNOS_SPECIFIC_FLAGS = -L/opt/SUNWspro/lib -lF77 -lg2c -lsunmath -lfsu -lfai -lm -lc -lc2
SUNOS_SPECIFIC_FLAGS = -L/opt/SUNWspro/lib -L/usr/SUNWspro/lib -lsunmath -lfsu -lfai -lm -lc -lc2

LINUX_SPECIFIC_FLAGS = -Wall -Wno-unused-variable -lc -lm ${EXTRA_LIB_PATHS} 

DARWIN_LIBRARY_FLAGS = -flat_namespace -undefined suppress -bundle
NON_DARWIN_LIBRARY_FLAGS = -shared

######## default to darwin since im developing on a mac ;)

OS_SPECIFIC_FLAGS = ${DARWIN_SPECIFIC_FLAGS}

IDL_2_KAMELEON_LIBRARY_FLAGS = ${DARWIN_LIBRARY_FLAGS}

IDL_2_KAMELEON_LIBRARY = ${IDL_BUNDLE_PLUGGIN}

######## List of all required object files for interpolator

INTERP_OBJ = cdf_status_handler.o get_batsrus_cdf_info.o get_ucla_ggcm_cdf_info.o hunt.o \
interpolate_batsrus_cdf.o interpolate_ucla_ggcm_cdf.o open_cdf.o get_units.o \
close_cdf.o close_batsrus_cdf.o close_ucla_ggcm_cdf.o gattribute_char_get.o \
gattribute_float_get.o tree_structure_toolkit.o init_time.o time_interpolate.o \
vattribute_get.o gattribute_get.o interpolate_ctip_cdf.o get_ctip_cdf_info.o \
close_ctip_cdf.o get_enlil_cdf_info.o interpolate_enlil_cdf.o close_enlil_cdf.o \
var_get.o list_structure_toolkit.o is_var_in_memory.o var_exists.o attribute_exists.o \
get_kpvt_cdf_info.o interpolate_kpvt_cdf.o get_msfc_tvm_cdf_info.o interpolate_msfc_tvm_cdf.o \
get_mas_cdf_info.o interpolate_mas_cdf.o close_mas_cdf.o

FORTRAN_INTERFACE_OBJ = f2c_open_cdf.o f2c_close_cdf.o f2c_interp_bats_cdf.o f2c_interp_open_ggcm_cdf.o \
f2c_init_time.o f2c_time_interpolate.o f2c_gattribute_char_get.o f2c_gattribute_float_get.o \
f2c_gattribute_int_get.o f2c_vattribute_char_get.o f2c_vattribute_float_get.o f2c_vattribute_int_get.o \
f2c_string_convert.o f2c_interp_ctip_cdf.o f2c_interp_enlil_cdf.o f2c_interp_kpvt_cdf.o f2c_interp_msfc_tvm_cdf.o

IDL_INTERFACE_OBJ = idl2c_gattribute_get.o idl2c_vattribute_get.o idl2c_var_get.o

####### List of sample driver programs

C_EXAMPLE_PROGRAMS = ${EXAMPLE_DIR}/attr_get_test.c ${EXAMPLE_DIR}/basic_ctip_interp_example.c \
${EXAMPLE_DIR}/basic_bats_interp_example.c ${EXAMPLE_DIR}/basic_ucla_ggcm_interp_example.c \
${EXAMPLE_DIR}/interp_bats_mem_delta.c ${EXAMPLE_DIR}/interp_bats_mem.c ${EXAMPLE_DIR}/interp_bats_no_mem_delta.c \
${EXAMPLE_DIR}/interp_bats_no_mem.c ${EXAMPLE_DIR}/interp_ucla_mem_delta.c ${EXAMPLE_DIR}/interp_ucla_mem.c \
${EXAMPLE_DIR}/interp_ucla_no_mem_delta.c ${EXAMPLE_DIR}/interp_ucla_no_mem.c ${EXAMPLE_DIR}/time_interp_example.c \
${EXAMPLE_DIR}/interp_ctip_mem_delta.c ${EXAMPLE_DIR}/interp_ctip_mem.c ${EXAMPLE_DIR}/interp_ctip_no_mem_delta.c \
${EXAMPLE_DIR}/interp_ctip_no_mem.c ${EXAMPLE_DIR}/interp_enlil_mem_delta.c \
${EXAMPLE_DIR}/interp_enlil_mem_delta.c ${EXAMPLE_DIR}/interp_enlil_mem.c ${EXAMPLE_DIR}/interp_enlil_no_mem_delta.c \
${EXAMPLE_DIR}/interp_enlil_no_mem.c ${EXAMPLE_DIR}/interp_enlil_mem_delta.c ${EXAMPLE_DIR}/basic_enlil_interp_example.c \
${EXAMPLE_DIR}/var_get_example.c ${EXAMPLE_DIR}/multi_open_interp_example.c ${EXAMPLE_DIR}/interp_kpvt_no_mem_delta.c \
${EXAMPLE_DIR}/interp_msfc_tvm_no_mem_delta.c ${EXAMPLE_DIR}/interp_mas_no_mem_delta.c ${EXAMPLE_DIR}/interp_mas_mem_delta.c

FORTRAN_EXAMPLE_PROGRAMS = ${EXAMPLE_DIR}/f2c_interp_batsrus.f ${EXAMPLE_DIR}/f2c_interp_open_ggcm.f \
${EXAMPLE_DIR}/f2c_4D_interpolation.f ${EXAMPLE_DIR}/f2c_attribute_get_example.f \
${EXAMPLE_DIR}/f2c_interp_ctip.f ${EXAMPLE_DIR}/f2c_interp_enlil.f ${EXAMPLE_DIR}/f2c_interp_kpvt.f \
${EXAMPLE_DIR}/f2c_interp_msfc_tvm.f

####### Explicit list of executable examples that can be created from examples target. Use list to clean instead of bin/* ( make me nervous )

EXECUTABLE_EXAMPLE_LIST = ${BIN_DIR}/attribute_get_test ${BIN_DIR}/basic_ctip_interp_example ${BIN_DIR}/basic_bats_interp_example \
${BIN_DIR}/basic_ucla_ggcm_interp_example ${BIN_DIR}/interpolate_batsrus_memory_deltas ${BIN_DIR}/interpolate_batsrus_memory \
${BIN_DIR}/interpolate_batsrus_no_memory_deltas ${BIN_DIR}/interpolate_batsrus_no_memory ${BIN_DIR}/interpolate_open_ggcm_memory_deltas \
${BIN_DIR}/interpolate_open_ggcm_memory ${BIN_DIR}/interpolate_open_ggcm_no_memory_deltas ${BIN_DIR}/interpolate_open_ggcm_no_memory \
${BIN_DIR}/4D_interpolation ${BIN_DIR}/fortran_interpolate_batsrus_no_memory ${BIN_DIR}/fortran_interpolate_open_ggcm_no_memory \
${BIN_DIR}/fortran_4D_interpolator ${BIN_DIR}/fortran_attribute_get ${BIN_DIR}/interpolate_ctip_memory_deltas \
${BIN_DIR}/interpolate_ctip_memory ${BIN_DIR}/interpolate_ctip_no_memory_deltas ${BIN_DIR}/interpolate_ctip_no_memory \
${BIN_DIR}/interpolate_enlil_memory_deltas ${BIN_DIR}/interpolate_enlil_memory ${BIN_DIR}/interpolate_enlil_no_memory_deltas \
${BIN_DIR}/interpolate_enlil_no_memory ${BIN_DIR}/basic_enlil_interp_example ${BIN_DIR}/var_get_example \
${BIN_DIR}/fortran_interpolate_ctip_no_memory ${BIN_DIR}/fortran_interpolate_enlil_no_memory ${BIN_DIR}/multi_open_interp_example \
${BIN_DIR}/interpolate_kpvt_no_memory_deltas ${BIN_DIR}/fortran_interpolate_kpvt_no_memory ${BIN_DIR}/fortran_interpolate_msfc_tvm_no_memory \
${BIN_DIR}/interpolate_msfc_tvm_no_memory_deltas ${BIN_DIR}/interpolate_mas_no_memory_deltas ${BIN_DIR}/interpolate_mas_memory_deltas


####### automatically determine OS to set specific FLAGS

first :
	@echo "Automatically Determining Operating System to set OS Specific Flags: "`uname`
	@case `uname` in \
	"Darwin") make all-static "OS_SPECIFIC_FLAGS=${DARWIN_SPECIFIC_FLAGS}" "IDL_2_KAMELEON_LIBRARY_FLAGS=${DARWIN_LIBRARY_FLAGS}" "IDL_2_KAMELEON_LIBRARY=${IDL_BUNDLE_PLUGGIN}" "CC_FULL_PATH = `which ${CC} | grep -v -e ^no`" "F77_FULL_PATH = `which ${F77} | grep -v -e ^no`";;\
	"Linux") make all "OS_SPECIFIC_FLAGS = ${LINUX_SPECIFIC_FLAGS}" "IDL_2_KAMELEON_LIBRARY_FLAGS = ${NON_DARWIN_LIBRARY_FLAGS}" "IDL_2_KAMELEON_LIBRARY = ${IDL_SHARED_LIBRARY}" "CC_FULL_PATH = `which ${CC} | grep -v -e ^no`" "F77_FULL_PATH = `which ${F77} | grep -v -e ^no`";;\
	"SunOS") make -e all "OS_SPECIFIC_FLAGS = ${SUNOS_SPECIFIC_FLAGS}" "IDL_2_KAMELEON_LIBRARY_FLAGS = ${NON_DARWIN_LIBRARY_FLAGS}" "IDL_2_KAMELEON_LIBRARY = ${IDL_SHARED_LIBRARY}" "FFLAGS= " "CC_FULL_PATH = `which ${CC} | ${SUN_GREP}`" "F77_FULL_PATH = `which ${F77} | ${SUN_GREP}`";;\
	CYGWIN*) make all "OS_SPECIFIC_FLAGS = ${LINUX_SPECIFIC_FLAGS}" "IDL_2_KAMELEON_LIBRARY_FLAGS = ${NON_DARWIN_LIBRARY_FLAGS}" "IDL_2_KAMELEON_LIBRARY = ${IDL_SHARED_LIBRARY}" "CC_FULL_PATH = `which ${CC} | grep -v -e ^no`" "F77_FULL_PATH = `which ${F77} | grep -v -e ^no`";;\
	*) echo "Current Operating System is NOT supported" ;;\
	esac

####### all

all : libkameleon.a libkameleon.so idl2kameleon all-examples

####### all-static

all-static : libkameleon.a idl2kameleon all-examples

####### all-shared

all-shared : libkameleon.so idl2kameleon all-examples

####### examples

all-examples : c-examples-static fortran-examples-static examples-shared ${KAMELEON_INC}

#c-examples-static : ${C_EXAMPLE_PROGRAMS} ${LIB_DIR}/libkameleon.a ${LIB_CDF}
c-examples-static : ${CC} ${LIB_CDF} ${CDF_INC_FILE} ${LIB_DIR}/libkameleon.a
	${CC} ${CFLAGS} -o ${BIN_DIR}/attribute_get_test ${EXAMPLE_DIR}/attr_get_test.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/basic_ctip_interp_example ${EXAMPLE_DIR}/basic_ctip_interp_example.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/basic_enlil_interp_example ${EXAMPLE_DIR}/basic_enlil_interp_example.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/basic_bats_interp_example ${EXAMPLE_DIR}/basic_bats_interp_example.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/basic_ucla_ggcm_interp_example ${EXAMPLE_DIR}/basic_ucla_ggcm_interp_example.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_batsrus_memory_deltas ${EXAMPLE_DIR}/interp_bats_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_batsrus_memory ${EXAMPLE_DIR}/interp_bats_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_batsrus_no_memory_deltas ${EXAMPLE_DIR}/interp_bats_no_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_batsrus_no_memory ${EXAMPLE_DIR}/interp_bats_no_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_open_ggcm_memory_deltas ${EXAMPLE_DIR}/interp_ucla_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_open_ggcm_memory ${EXAMPLE_DIR}/interp_ucla_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_open_ggcm_no_memory_deltas ${EXAMPLE_DIR}/interp_ucla_no_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_open_ggcm_no_memory ${EXAMPLE_DIR}/interp_ucla_no_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/4D_interpolation ${EXAMPLE_DIR}/time_interp_example.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_ctip_memory_deltas ${EXAMPLE_DIR}/interp_ctip_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_ctip_memory ${EXAMPLE_DIR}/interp_ctip_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_ctip_no_memory_deltas ${EXAMPLE_DIR}/interp_ctip_no_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_ctip_no_memory ${EXAMPLE_DIR}/interp_ctip_no_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_enlil_memory_deltas ${EXAMPLE_DIR}/interp_enlil_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_enlil_memory ${EXAMPLE_DIR}/interp_enlil_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_enlil_no_memory_deltas ${EXAMPLE_DIR}/interp_enlil_no_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_enlil_no_memory ${EXAMPLE_DIR}/interp_enlil_no_mem.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/var_get_example ${EXAMPLE_DIR}/var_get_example.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/multi_open_interp_example ${EXAMPLE_DIR}/multi_open_interp_example.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_kpvt_no_memory_deltas ${EXAMPLE_DIR}/interp_kpvt_no_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_msfc_tvm_no_memory_deltas ${EXAMPLE_DIR}/interp_msfc_tvm_no_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_mas_no_memory_deltas ${EXAMPLE_DIR}/interp_mas_no_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}
	${CC} ${CFLAGS} -o ${BIN_DIR}/interpolate_mas_memory_deltas ${EXAMPLE_DIR}/interp_mas_mem_delta.c ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS} -I${INC_DIR} -I${CDF_INC}

#fortran-examples-static : ${FORTRAN_EXAMPLE_PROGRAMS} ${LIB_DIR}/libkameleon.a ${LIB_CDF}
fortran-examples-static : ${F77} ${LIB_CDF} ${CDF_INC_FILE} ${LIB_DIR}/libkameleon.a
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_interpolate_batsrus_no_memory ${EXAMPLE_DIR}/f2c_interp_batsrus.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_interpolate_open_ggcm_no_memory ${EXAMPLE_DIR}/f2c_interp_open_ggcm.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_4D_interpolator ${EXAMPLE_DIR}/f2c_4D_interpolation.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_attribute_get ${EXAMPLE_DIR}/f2c_attribute_get_example.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_interpolate_ctip_no_memory ${EXAMPLE_DIR}/f2c_interp_ctip.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_interpolate_enlil_no_memory ${EXAMPLE_DIR}/f2c_interp_enlil.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_interpolate_kpvt_no_memory ${EXAMPLE_DIR}/f2c_interp_kpvt.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}
	${F77} ${FFLAGS} -o ${BIN_DIR}/fortran_interpolate_msfc_tvm_no_memory ${EXAMPLE_DIR}/f2c_interp_msfc_tvm.f ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}

examples-shared :
	@echo ""
	@echo "*********************************** INFO ***********************************"
	@echo "Static example/driver programs automatically compiled for quick use..."
	@echo " "
	@echo "SEE compile_examples.txt file located in the examples directory for info on"
	@echo "linking to the shared .so libraries. Building here is tedious and would make"
	@echo "the local bin directory crowded. IN a nutshell though, all you need to do is"
	@echo "update your LD_LIBRARY_PATH environment variable to include the directory"
	@echo "containing the libkameleon.so file. Then instead of linking against "
	@echo "./lib/libkameleon.a, use -lkameleon -L./lib..."
	@echo "*********************************** INFO ***********************************"


############# idl2kameleon

idl2kameleon : ${CC} ${LIB_CDF} ${CDF_INC_FILE} ${IDL_INTERFACE_OBJ}
	${CC} ${IDL_2_KAMELEON_LIBRARY_FLAGS} -o ${IDL_2_KAMELEON_LIBRARY} ${IDL_INTERFACE_OBJ} ${STATIC_INTERP_LIB} ${LIB_CDF} ${OS_SPECIFIC_FLAGS}

##### Check for presence of CDF library

${LIB_CDF} :
	@echo "ERROR!!!!!!!!!! $@ not found in LIB_CDF macro as currently set ---> ${LIB_CDF}"
	@echo "YOU MAY NEED TO INSTALL CDF library or set LIB_CDF macro ie. make LIB_CDF=/Applications/cdf31-dist/lib/libcdf.a"
	@echo ""
	$(error ERROR!!!!!!!!!! $@ not found in LIB_CDF macro as currently set --->${LIB_CDF}<--- ... YOU MAY NEED TO INSTALL CDF library or set LIB_CDF macro ie. make LIB_CDF=/Applications/cdf31-dist/lib/libcdf.a)

##### Check for presence of CDF include file

${CDF_INC_FILE} :
	@echo "ERROR!!!!!!!!!! cdf.h not found in CDF_INC directory as currently set ---> ${CDF_INC}"
	@echo "YOU MAY NEED TO INSTALL CDF library or set CDF_INC macro ie. make CDF_INC=/Applications/cdf31-dist/include"
	@echo ""
	$(error ERROR!!!!!!!!!! cdf.h not found in CDF_INC directory as currently set --->${CDF_INC}<--- YOU MAY NEED TO INSTALL CDF library or set LIB_CDF macro ie. make CDF_INC=/Applications/cdf31-dist/include)


###### Check for presence of specified CC compiler

# [ -x "${CC_FULL_PATH}" ] ; \

${CC} :
	@echo ""
	@echo "checking for c compiler as curently set --->${CC}<---"
	@echo "verifying full path as found using 'which ${CC}' --->`which ${CC}`<---"
	@if \
	[ ! -x "`which ${CC} | ${SUN_GREP}`" ] ; \
	then \
	make c_compiler_error ; \
	else \
	echo "${CC} compiler OK..." ; \
	echo "" ; \
	fi

###### Check for presence of specified FORTRAN compiler

# [ -z "${F77_FULL_PATH}" ] ; \

${F77} :
	@echo ""
	@echo "checking for fortran compiler as curently set --->${F77}<---"
	@echo "verifying full path as found using 'which ${F77}' --->`which ${F77}`<---"
	@if \
	[ ! -x `which ${F77} | ${SUN_GREP}` ] ; \
	then \
	make fortran_compiler_error ; \
	else \
	echo "${F77} compiler OK..." ; \
	echo "" ; \
	fi

c_compiler_error :
	@echo "ERROR!!!!!!!!!! ${CC} compiler not found as currently set ---> ${CC_FULL_PATH}"
	@echo "YOU MAY NEED TO set CC macro ie. make CC=cc"
	@echo ""
	$(error ERROR!!!!!!!!!! ${CC} compiler not found as currently set ---> ${CC_FULL_PATH} <--- you may need to set CC macro ie. make CC=gcc)

fortran_compiler_error :
	@echo ""
	@echo "ERROR!!!!!!!!!! ${F77} compiler not found as currently set ---> ${F77_FULL_PATH} <--"
	@echo "YOU MAY NEED TO set F77 macro ie. make F77=gfortran"
	@echo ""
	@echo "**NOTE: Kameleon can still build successfully without a fortran compiler. The only thing that should be affected is the sample fortran example programs..."
	@echo ""

##################### Static Library Rules #######################

libkameleon.a : ${CC} ${LIB_CDF} ${CDF_INC_FILE} ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ}
	ar rc ${LIB_DIR}/$@ ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ}
	ranlib ${LIB_DIR}/$@
#	rm -f ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ}

##################### Shared Library Rules #######################

libkameleon.so : ${CC} ${LIB_CDF} ${CDF_INC_FILE} ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ}
	${CC} -shared -o ${LIB_DIR}/$@ ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ} ${LIB_CDF}
#	rm -f ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ}

################### INTER_OBJ components

cdf_status_handler.o : ${SRC_DIR}/cdf_status_handler.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/cdf_status_handler.c -I${INC_DIR} -I${CDF_INC}

get_batsrus_cdf_info.o : ${SRC_DIR}/get_batsrus_cdf_info.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_batsrus_cdf_info.c -I${INC_DIR} -I${CDF_INC}

get_ucla_ggcm_cdf_info.o : ${SRC_DIR}/get_ucla_ggcm_cdf_info.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_ucla_ggcm_cdf_info.c -I${INC_DIR} -I${CDF_INC}

hunt.o : ${SRC_DIR}/hunt.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/hunt.c -I${INC_DIR} -I${CDF_INC}

interpolate_batsrus_cdf.o : ${SRC_DIR}/interpolate_batsrus_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/interpolate_batsrus_cdf.c -I${INC_DIR} -I${CDF_INC}

interpolate_ucla_ggcm_cdf.o : ${SRC_DIR}/interpolate_ucla_ggcm_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/interpolate_ucla_ggcm_cdf.c -I${INC_DIR} -I${CDF_INC}
	
open_cdf.o : ${SRC_DIR}/open_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/open_cdf.c -I${INC_DIR} -I${CDF_INC}

get_units.o : ${SRC_DIR}/get_units.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_units.c -I${INC_DIR} -I${CDF_INC}

close_cdf.o : ${SRC_DIR}/close_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/close_cdf.c -I${INC_DIR} -I${CDF_INC}

close_batsrus_cdf.o : ${SRC_DIR}/close_batsrus_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/close_batsrus_cdf.c -I${INC_DIR} -I${CDF_INC}

close_ucla_ggcm_cdf.o : ${SRC_DIR}/close_ucla_ggcm_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/close_ucla_ggcm_cdf.c -I${INC_DIR} -I${CDF_INC}

gattribute_char_get.o : ${SRC_DIR}/gattribute_char_get.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/gattribute_char_get.c -I${INC_DIR} -I${CDF_INC}

gattribute_float_get.o : ${SRC_DIR}/gattribute_float_get.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/gattribute_float_get.c -I${INC_DIR} -I${CDF_INC}

tree_structure_toolkit.o : ${SRC_DIR}/tree_structure_toolkit.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/tree_structure_toolkit.c -I${INC_DIR} -I${CDF_INC}

init_time.o : ${SRC_DIR}/init_time.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/init_time.c -I${INC_DIR} -I${CDF_INC}

time_interpolate.o : ${SRC_DIR}/time_interpolate.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/time_interpolate.c -I${INC_DIR} -I${CDF_INC}

vattribute_get.o : ${SRC_DIR}/vattribute_get.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/vattribute_get.c -I${INC_DIR} -I${CDF_INC}

gattribute_get.o : ${SRC_DIR}/gattribute_get.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/gattribute_get.c -I${INC_DIR} -I${CDF_INC}

interpolate_ctip_cdf.o : ${SRC_DIR}/interpolate_ctip_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/interpolate_ctip_cdf.c -I${INC_DIR} -I${CDF_INC}

get_ctip_cdf_info.o : ${SRC_DIR}/get_ctip_cdf_info.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_ctip_cdf_info.c -I${INC_DIR} -I${CDF_INC}

close_ctip_cdf.o : ${SRC_DIR}/close_ctip_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/close_ctip_cdf.c -I${INC_DIR} -I${CDF_INC}

get_enlil_cdf_info.o : ${SRC_DIR}/get_enlil_cdf_info.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_enlil_cdf_info.c -I${INC_DIR} -I${CDF_INC}
	
interpolate_enlil_cdf.o : ${SRC_DIR}/interpolate_enlil_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/interpolate_enlil_cdf.c -I${INC_DIR} -I${CDF_INC}

close_enlil_cdf.o : ${SRC_DIR}/close_enlil_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/close_enlil_cdf.c -I${INC_DIR} -I${CDF_INC}

var_get.o : ${SRC_DIR}/var_get.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/var_get.c -I${INC_DIR} -I${CDF_INC}

list_structure_toolkit.o : ${SRC_DIR}/list_structure_toolkit.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/list_structure_toolkit.c -I${INC_DIR} -I${CDF_INC}

is_var_in_memory.o : ${SRC_DIR}/is_var_in_memory.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/is_var_in_memory.c -I${INC_DIR} -I${CDF_INC}

var_exists.o : ${SRC_DIR}/var_exists.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/var_exists.c -I${INC_DIR} -I${CDF_INC}

attribute_exists.o : ${SRC_DIR}/attribute_exists.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/attribute_exists.c -I${INC_DIR} -I${CDF_INC}

get_kpvt_cdf_info.o : ${SRC_DIR}/get_kpvt_cdf_info.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_kpvt_cdf_info.c -I${INC_DIR} -I${CDF_INC}

interpolate_kpvt_cdf.o : ${SRC_DIR}/interpolate_kpvt_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/interpolate_kpvt_cdf.c -I${INC_DIR} -I${CDF_INC}

get_msfc_tvm_cdf_info.o : ${SRC_DIR}/get_msfc_tvm_cdf_info.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_msfc_tvm_cdf_info.c -I${INC_DIR} -I${CDF_INC}

interpolate_msfc_tvm_cdf.o : ${SRC_DIR}/interpolate_msfc_tvm_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/interpolate_msfc_tvm_cdf.c -I${INC_DIR} -I${CDF_INC}

get_mas_cdf_info.o : ${SRC_DIR}/get_mas_cdf_info.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/get_mas_cdf_info.c -I${INC_DIR} -I${CDF_INC}

interpolate_mas_cdf.o : ${SRC_DIR}/interpolate_mas_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/interpolate_mas_cdf.c -I${INC_DIR} -I${CDF_INC}

close_mas_cdf.o : ${SRC_DIR}/close_mas_cdf.c
	${CC} ${CFLAGS} -c ${SRC_DIR}/close_mas_cdf.c -I${INC_DIR} -I${CDF_INC}

################# FORTRAN_INTERFACE_OBJ components

f2c_open_cdf.o : ${F77_SRC_DIR}/f2c_open_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_open_cdf.c

f2c_close_cdf.o : ${F77_SRC_DIR}/f2c_close_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_close_cdf.c

f2c_interp_bats_cdf.o : ${F77_SRC_DIR}/f2c_interp_bats_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_interp_bats_cdf.c

f2c_interp_open_ggcm_cdf.o : ${F77_SRC_DIR}/f2c_interp_open_ggcm_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_interp_open_ggcm_cdf.c

f2c_init_time.o : ${F77_SRC_DIR}/f2c_init_time.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_init_time.c

f2c_time_interpolate.o : ${F77_SRC_DIR}/f2c_time_interpolate.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_time_interpolate.c

f2c_gattribute_char_get.o : ${F77_SRC_DIR}/f2c_gattribute_char_get.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_gattribute_char_get.c

f2c_gattribute_float_get.o : ${F77_SRC_DIR}/f2c_gattribute_float_get.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_gattribute_float_get.c

f2c_gattribute_int_get.o : ${F77_SRC_DIR}/f2c_gattribute_int_get.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_gattribute_int_get.c

f2c_vattribute_char_get.o : ${F77_SRC_DIR}/f2c_vattribute_char_get.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_vattribute_char_get.c

f2c_vattribute_float_get.o : ${F77_SRC_DIR}/f2c_vattribute_float_get.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_vattribute_float_get.c

f2c_vattribute_int_get.o : ${F77_SRC_DIR}/f2c_vattribute_int_get.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_vattribute_int_get.c

f2c_string_convert.o : ${F77_SRC_DIR}/f2c_string_convert.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_string_convert.c

f2c_interp_ctip_cdf.o : ${F77_SRC_DIR}/f2c_interp_ctip_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_interp_ctip_cdf.c
	
f2c_interp_enlil_cdf.o : ${F77_SRC_DIR}/f2c_interp_enlil_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_interp_enlil_cdf.c

f2c_interp_kpvt_cdf.o : ${F77_SRC_DIR}/f2c_interp_kpvt_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_interp_kpvt_cdf.c

f2c_interp_msfc_tvm_cdf.o : ${F77_SRC_DIR}/f2c_interp_msfc_tvm_cdf.c
	${CC} ${CFLAGS} -c ${F77_SRC_DIR}/f2c_interp_msfc_tvm_cdf.c


################# IDL_INTERFACE_OBJ components

idl2c_vattribute_get.o : ${IDL_INTERFACE_DIR}/idl2c_vattribute_get.c
	${CC} ${CFLAGS} -c ${IDL_INTERFACE_DIR}/idl2c_vattribute_get.c -I${IDL_INC_DIR} -I${INC_DIR} -I${CDF_INC}

idl2c_gattribute_get.o : ${IDL_INTERFACE_DIR}/idl2c_gattribute_get.c
	${CC} ${CFLAGS} -c ${IDL_INTERFACE_DIR}/idl2c_gattribute_get.c -I${IDL_INC_DIR} -I${INC_DIR} -I${CDF_INC}

idl2c_var_get.o : ${IDL_INTERFACE_DIR}/idl2c_var_get.c
	${CC} ${CFLAGS} -c ${IDL_INTERFACE_DIR}/idl2c_var_get.c -I${IDL_INC_DIR} -I${INC_DIR} -I${CDF_INC}

###################### CLEANING ####################################

clean :
	rm -f ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ} ${IDL_INTERFACE_OBJ}

real_clean :
	rm -f ${INTERP_OBJ} ${FORTRAN_INTERFACE_OBJ} ${IDL_INTERFACE_OBJ} ${LIB_DIR}/libkameleon.so ${LIB_DIR}/libkameleon.a ${EXECUTABLE_EXAMPLE_LIST} ${IDL_BUNDLE_PLUGGIN} ${IDL_SHARED_LIBRARY}


####################### HELP ########################################

help :
	@echo ""
	@echo "*********************************** HELP ***********************************"
	@echo ""
	@echo "You can set several Makefile variables during invocation of make..."
	@echo ""
	@echo "Required?     Description         Variable        Example"
	@echo "--------     -----------------   ------------    ---------------------------------------------"
	@echo "  YES         CDF Library         LIB_CDF         LIB_CDF=/Applications/cdf31-dist/lib/libcdf.a"
	@echo "  YES         CDF Include Files   CDF_INC         CDF_INC=/Applications/cdf31-dist/include/"
	@echo "  NO          C Compiler          CC              CC=gcc"
	@echo "  NO          FORTRAN Compiler    F77             F77=gfortran"
	@echo "  NO          FORTRAN Libraries   FORTRAN_LIB     FORTRAN_LIB=-L/usr/local/lib/x86_64"
	@echo "  NO          IDL Include Files   IDL_INC_DIR     IDL_INC_DIR=/Applications/itt/idl64/external"
	@echo "----------------------------------------------------------------------------------------------"
	@echo "Example:"
	@echo ""
	@echo "  make CC=gcc F77=gfortran CDF_INC=/Applications/cdf31-dist/include/ LIB_CDF=/Applications/cdf31-dist/lib/libcdf.a IDL_INC_DIR=/Applications/itt/idl64/external"
	@echo ""
	@echo "*********************************** HELP ***********************************"
	@echo ""

