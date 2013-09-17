/*******************************************************************************
*                                                                              *
*    NAME:        batsrus_attributes.h                                         *
*                                                                              *
*    VERSION:     4.1                                                          *
*                                                                              *
*    AUTHOR:      Marlo M. H. Maddox                                           *
*                 NASA-GSFC-AETD-ISD-ADMA(587)                                 *
*                 Marlo.Maddox@nasa.gov                                        *
*                 (301) 286 - 5202                                             *
*                                                                              *
*    PURPOSE:     centralized location for registerd batrus file               *
*                 attribute names. To be used for data format                  *
*                 standarization through conversion software                   *
*                                                                              *
*    INPUTS:      NONE                                                         *
*                                                                              *
*    OUTPUTS:     NONE                                                         *
*                                                                              *
*    FUNCTIONS:   NONE                                                         *
*                                                                              *
*    VARIABLES:   -attr names of the form batsrus_attr_1_name ..               *
*                 batsrus_attr_n_name                                          *
*                 - NUMBER_BATSRUS_ATTR - number of registered                 *
*                 batsrus_attributes                                           *
*                 - batsrus_attribute_array - array of atribute name strings   *
*                                                                              *
*                                                                              *
*    MODIFICATION                                                              *
*    HISTORY:                                                                  *
*                                                                              *
*    09.11.2003   Maddox, Marlo                                                *
*                 initial development started                                  *
*                                                                              *
*    11.06.2003   Maddox, Marlo                                                *
*                 added attributes for detailed Grid Description               *
*    03.26.2004   Maddox, Marlo                                                *
*                 changed array declaration to static                          *
*    04.27.2004   Maddox, Marlo                                                *
*                 added 27 - 30                                                *
*    03.22.2005   Maddox, Marlo                                                *
*                 added 31 - 33                                                *
*    04.05.2005   Maddox, Marlo                                                *
*                 changed flat array to array of structures containing name    *
*                 data type pairs                                              *
*                                                                              *
*******************************************************************************/



#define NUMBER_BATSRUS_ATTR        33  /*** update total number of registered attributes ***/

#define NUMBER_BATSRUS_VARIABLE_ATTR     0 /*** update total number of registered batsrus variable attributes if any need to be defined ***/

/******************* define batrus attribute names ******************************/

#define    BATSRUS_ATTR_1_NAME            "elapsed_time_in_seconds"
#define    BATSRUS_ATTR_1_DATA_TYPE    "float"

#define BATSRUS_ATTR_2_NAME            "number_of_dimensions"
#define    BATSRUS_ATTR_2_DATA_TYPE    "int"

#define BATSRUS_ATTR_3_NAME            "special_parameter_g"
#define    BATSRUS_ATTR_3_DATA_TYPE    "float"

#define BATSRUS_ATTR_4_NAME            "special_parameter_c"
#define    BATSRUS_ATTR_4_DATA_TYPE    "float"

#define BATSRUS_ATTR_5_NAME            "special_parameter_th"
#define    BATSRUS_ATTR_5_DATA_TYPE    "float"

#define BATSRUS_ATTR_6_NAME            "special_parameter_P1"
#define    BATSRUS_ATTR_6_DATA_TYPE    "float"

#define BATSRUS_ATTR_7_NAME            "special_parameter_P2"
#define    BATSRUS_ATTR_7_DATA_TYPE    "float"

#define BATSRUS_ATTR_8_NAME            "special_parameter_P3"
#define    BATSRUS_ATTR_8_DATA_TYPE    "float"

#define BATSRUS_ATTR_9_NAME            "special_parameter_R"
#define    BATSRUS_ATTR_9_DATA_TYPE    "float"

#define BATSRUS_ATTR_10_NAME        "special_parameter_NX"
#define    BATSRUS_ATTR_10_DATA_TYPE    "float"

#define BATSRUS_ATTR_11_NAME        "special_parameter_NY"
#define    BATSRUS_ATTR_11_DATA_TYPE    "float"

#define BATSRUS_ATTR_12_NAME        "special_parameter_NZ"
#define    BATSRUS_ATTR_12_DATA_TYPE    "float"

#define BATSRUS_ATTR_13_NAME        "x_dimension_size"
#define    BATSRUS_ATTR_13_DATA_TYPE    "int"

#define BATSRUS_ATTR_14_NAME        "y_dimension_size"
#define    BATSRUS_ATTR_14_DATA_TYPE    "int"

#define BATSRUS_ATTR_15_NAME        "z_dimension_size"
#define    BATSRUS_ATTR_15_DATA_TYPE    "int"

#define BATSRUS_ATTR_16_NAME        "current_iteration_step"
#define    BATSRUS_ATTR_16_DATA_TYPE    "int"

#define BATSRUS_ATTR_17_NAME        "global_x_min"
#define    BATSRUS_ATTR_17_DATA_TYPE    "float"

#define BATSRUS_ATTR_18_NAME        "global_x_max"
#define    BATSRUS_ATTR_18_DATA_TYPE    "float"

#define BATSRUS_ATTR_19_NAME        "global_y_min"
#define    BATSRUS_ATTR_19_DATA_TYPE    "float"

#define BATSRUS_ATTR_20_NAME        "global_y_max"
#define    BATSRUS_ATTR_20_DATA_TYPE    "float"

#define BATSRUS_ATTR_21_NAME        "global_z_min"
#define    BATSRUS_ATTR_21_DATA_TYPE    "float"

#define BATSRUS_ATTR_22_NAME        "global_z_max"
#define    BATSRUS_ATTR_22_DATA_TYPE    "float"

#define BATSRUS_ATTR_23_NAME        "max_amr_level"
#define    BATSRUS_ATTR_23_DATA_TYPE    "float"

#define BATSRUS_ATTR_24_NAME        "number_of_cells"
#define    BATSRUS_ATTR_24_DATA_TYPE    "int"

#define BATSRUS_ATTR_25_NAME        "number_of_blocks"
#define    BATSRUS_ATTR_25_DATA_TYPE    "int"

#define BATSRUS_ATTR_26_NAME        "smallest_cell_size"
#define    BATSRUS_ATTR_26_DATA_TYPE    "float"

#define BATSRUS_ATTR_27_NAME        "r_body"
#define    BATSRUS_ATTR_27_DATA_TYPE    "char"

#define BATSRUS_ATTR_28_NAME        "r_currents"
#define    BATSRUS_ATTR_28_DATA_TYPE    "char"

#define BATSRUS_ATTR_29_NAME        "b_dipole"
#define    BATSRUS_ATTR_29_DATA_TYPE    "char"

#define BATSRUS_ATTR_30_NAME        "dipole_time"
#define    BATSRUS_ATTR_30_DATA_TYPE    "char"

#define BATSRUS_ATTR_31_NAME        "dipole_update"
#define    BATSRUS_ATTR_31_DATA_TYPE    "char"

#define BATSRUS_ATTR_32_NAME        "dipole_tilt"
#define    BATSRUS_ATTR_32_DATA_TYPE    "float"

#define BATSRUS_ATTR_33_NAME        "dipole_tilt_y"
#define    BATSRUS_ATTR_33_DATA_TYPE    "float"

/********************* load all batrus attribute names & data type into batsrus_atribute_name_type_list array of structures **********/

struct batsrus_attribute_name_type
{
    char *batsrus_attr_name;
    char *batsrus_attr_data_type;

} static batsrus_attribute_name_type_list[] = {
                        BATSRUS_ATTR_1_NAME,  BATSRUS_ATTR_1_DATA_TYPE,
                        BATSRUS_ATTR_2_NAME,  BATSRUS_ATTR_2_DATA_TYPE,
                        BATSRUS_ATTR_3_NAME,  BATSRUS_ATTR_3_DATA_TYPE,
                        BATSRUS_ATTR_4_NAME,  BATSRUS_ATTR_4_DATA_TYPE,
                        BATSRUS_ATTR_5_NAME,  BATSRUS_ATTR_5_DATA_TYPE,
                        BATSRUS_ATTR_6_NAME,  BATSRUS_ATTR_6_DATA_TYPE,
                        BATSRUS_ATTR_7_NAME,  BATSRUS_ATTR_7_DATA_TYPE,
                        BATSRUS_ATTR_8_NAME,  BATSRUS_ATTR_8_DATA_TYPE,
                        BATSRUS_ATTR_9_NAME,  BATSRUS_ATTR_9_DATA_TYPE,
                        BATSRUS_ATTR_10_NAME, BATSRUS_ATTR_10_DATA_TYPE,
                        BATSRUS_ATTR_11_NAME, BATSRUS_ATTR_11_DATA_TYPE,
                        BATSRUS_ATTR_12_NAME, BATSRUS_ATTR_12_DATA_TYPE,
                        BATSRUS_ATTR_13_NAME, BATSRUS_ATTR_13_DATA_TYPE,
                        BATSRUS_ATTR_14_NAME, BATSRUS_ATTR_14_DATA_TYPE,
                        BATSRUS_ATTR_15_NAME, BATSRUS_ATTR_15_DATA_TYPE,
                        BATSRUS_ATTR_16_NAME, BATSRUS_ATTR_16_DATA_TYPE,
                        BATSRUS_ATTR_17_NAME, BATSRUS_ATTR_17_DATA_TYPE,
                        BATSRUS_ATTR_18_NAME, BATSRUS_ATTR_18_DATA_TYPE,
                        BATSRUS_ATTR_19_NAME, BATSRUS_ATTR_19_DATA_TYPE,
                        BATSRUS_ATTR_20_NAME, BATSRUS_ATTR_20_DATA_TYPE,
                        BATSRUS_ATTR_21_NAME, BATSRUS_ATTR_21_DATA_TYPE,
                        BATSRUS_ATTR_22_NAME, BATSRUS_ATTR_22_DATA_TYPE,
                        BATSRUS_ATTR_23_NAME, BATSRUS_ATTR_23_DATA_TYPE,
                        BATSRUS_ATTR_24_NAME, BATSRUS_ATTR_24_DATA_TYPE,
                        BATSRUS_ATTR_25_NAME, BATSRUS_ATTR_25_DATA_TYPE,
                        BATSRUS_ATTR_26_NAME, BATSRUS_ATTR_26_DATA_TYPE,
                        BATSRUS_ATTR_27_NAME, BATSRUS_ATTR_27_DATA_TYPE,
                        BATSRUS_ATTR_28_NAME, BATSRUS_ATTR_28_DATA_TYPE,
                        BATSRUS_ATTR_29_NAME, BATSRUS_ATTR_29_DATA_TYPE,
                        BATSRUS_ATTR_30_NAME, BATSRUS_ATTR_30_DATA_TYPE,
                        BATSRUS_ATTR_31_NAME, BATSRUS_ATTR_31_DATA_TYPE,
                        BATSRUS_ATTR_32_NAME, BATSRUS_ATTR_32_DATA_TYPE,
                        BATSRUS_ATTR_33_NAME, BATSRUS_ATTR_33_DATA_TYPE
                        };

