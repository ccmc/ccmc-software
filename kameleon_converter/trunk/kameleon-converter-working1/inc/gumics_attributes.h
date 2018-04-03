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
*                 - NUMBER_GUMICS_ATTR - number of registered                 *
*                 batsrus_attributes                                           *
*                 - batsrus_attribute_array - array of atribute name strings   *
*                                                                              *
*                                                                              *
*    MODIFICATION                                                              *
*    HISTORY:                                                                  *
*                                                                              *
*    2018/03/30   Lutz Rastaetter                                              *
*                 derived from batsrus_attributes.h (version of 2005/4/5)      *
*                                                                              *
*******************************************************************************/



#define NUMBER_GUMICS_ATTR        33  /*** update total number of registered attributes ***/

#define NUMBER_GUMICS_VARIABLE_ATTR     0 /*** update total number of registered batsrus variable attributes if any need to be defined ***/

/******************* define batrus attribute names ******************************/

#define    GUMICS_ATTR_1_NAME            "elapsed_time_in_seconds"
#define    GUMICS_ATTR_1_DATA_TYPE    "float"

#define GUMICS_ATTR_2_NAME            "number_of_dimensions"
#define    GUMICS_ATTR_2_DATA_TYPE    "int"

#define GUMICS_ATTR_3_NAME            "special_parameter_g"
#define    GUMICS_ATTR_3_DATA_TYPE    "float"

#define GUMICS_ATTR_4_NAME            "special_parameter_c"
#define    GUMICS_ATTR_4_DATA_TYPE    "float"

#define GUMICS_ATTR_5_NAME            "special_parameter_th"
#define    GUMICS_ATTR_5_DATA_TYPE    "float"

#define GUMICS_ATTR_6_NAME            "special_parameter_P1"
#define    GUMICS_ATTR_6_DATA_TYPE    "float"

#define GUMICS_ATTR_7_NAME            "special_parameter_P2"
#define    GUMICS_ATTR_7_DATA_TYPE    "float"

#define GUMICS_ATTR_8_NAME            "special_parameter_P3"
#define    GUMICS_ATTR_8_DATA_TYPE    "float"

#define GUMICS_ATTR_9_NAME            "special_parameter_R"
#define    GUMICS_ATTR_9_DATA_TYPE    "float"

#define GUMICS_ATTR_10_NAME        "special_parameter_NX"
#define    GUMICS_ATTR_10_DATA_TYPE    "float"

#define GUMICS_ATTR_11_NAME        "special_parameter_NY"
#define    GUMICS_ATTR_11_DATA_TYPE    "float"

#define GUMICS_ATTR_12_NAME        "special_parameter_NZ"
#define    GUMICS_ATTR_12_DATA_TYPE    "float"

#define GUMICS_ATTR_13_NAME        "x_dimension_size"
#define    GUMICS_ATTR_13_DATA_TYPE    "int"

#define GUMICS_ATTR_14_NAME        "y_dimension_size"
#define    GUMICS_ATTR_14_DATA_TYPE    "int"

#define GUMICS_ATTR_15_NAME        "z_dimension_size"
#define    GUMICS_ATTR_15_DATA_TYPE    "int"

#define GUMICS_ATTR_16_NAME        "current_iteration_step"
#define    GUMICS_ATTR_16_DATA_TYPE    "int"

#define GUMICS_ATTR_17_NAME        "global_x_min"
#define    GUMICS_ATTR_17_DATA_TYPE    "float"

#define GUMICS_ATTR_18_NAME        "global_x_max"
#define    GUMICS_ATTR_18_DATA_TYPE    "float"

#define GUMICS_ATTR_19_NAME        "global_y_min"
#define    GUMICS_ATTR_19_DATA_TYPE    "float"

#define GUMICS_ATTR_20_NAME        "global_y_max"
#define    GUMICS_ATTR_20_DATA_TYPE    "float"

#define GUMICS_ATTR_21_NAME        "global_z_min"
#define    GUMICS_ATTR_21_DATA_TYPE    "float"

#define GUMICS_ATTR_22_NAME        "global_z_max"
#define    GUMICS_ATTR_22_DATA_TYPE    "float"

#define GUMICS_ATTR_23_NAME        "max_amr_level"
#define    GUMICS_ATTR_23_DATA_TYPE    "float"

#define GUMICS_ATTR_24_NAME        "number_of_cells"
#define    GUMICS_ATTR_24_DATA_TYPE    "int"

#define GUMICS_ATTR_25_NAME        "number_of_blocks"
#define    GUMICS_ATTR_25_DATA_TYPE    "int"

#define GUMICS_ATTR_26_NAME        "smallest_cell_size"
#define    GUMICS_ATTR_26_DATA_TYPE    "float"

#define GUMICS_ATTR_27_NAME        "r_body"
#define    GUMICS_ATTR_27_DATA_TYPE    "char"

#define GUMICS_ATTR_28_NAME        "r_currents"
#define    GUMICS_ATTR_28_DATA_TYPE    "char"

#define GUMICS_ATTR_29_NAME        "b_dipole"
#define    GUMICS_ATTR_29_DATA_TYPE    "char"

#define GUMICS_ATTR_30_NAME        "dipole_time"
#define    GUMICS_ATTR_30_DATA_TYPE    "char"

#define GUMICS_ATTR_31_NAME        "dipole_update"
#define    GUMICS_ATTR_31_DATA_TYPE    "char"

#define GUMICS_ATTR_32_NAME        "dipole_tilt"
#define    GUMICS_ATTR_32_DATA_TYPE    "float"

#define GUMICS_ATTR_33_NAME        "dipole_tilt_y"
#define    GUMICS_ATTR_33_DATA_TYPE    "float"

/********************* load all batrus attribute names & data type into batsrus_atribute_name_type_list array of structures **********/

struct gumics_attribute_name_type
{
    char *batsrus_attr_name;
    char *batsrus_attr_data_type;

} static gumics_attribute_name_type_list[] = {
                        GUMICS_ATTR_1_NAME,  GUMICS_ATTR_1_DATA_TYPE,
                        GUMICS_ATTR_2_NAME,  GUMICS_ATTR_2_DATA_TYPE,
                        GUMICS_ATTR_3_NAME,  GUMICS_ATTR_3_DATA_TYPE,
                        GUMICS_ATTR_4_NAME,  GUMICS_ATTR_4_DATA_TYPE,
                        GUMICS_ATTR_5_NAME,  GUMICS_ATTR_5_DATA_TYPE,
                        GUMICS_ATTR_6_NAME,  GUMICS_ATTR_6_DATA_TYPE,
                        GUMICS_ATTR_7_NAME,  GUMICS_ATTR_7_DATA_TYPE,
                        GUMICS_ATTR_8_NAME,  GUMICS_ATTR_8_DATA_TYPE,
                        GUMICS_ATTR_9_NAME,  GUMICS_ATTR_9_DATA_TYPE,
                        GUMICS_ATTR_10_NAME, GUMICS_ATTR_10_DATA_TYPE,
                        GUMICS_ATTR_11_NAME, GUMICS_ATTR_11_DATA_TYPE,
                        GUMICS_ATTR_12_NAME, GUMICS_ATTR_12_DATA_TYPE,
                        GUMICS_ATTR_13_NAME, GUMICS_ATTR_13_DATA_TYPE,
                        GUMICS_ATTR_14_NAME, GUMICS_ATTR_14_DATA_TYPE,
                        GUMICS_ATTR_15_NAME, GUMICS_ATTR_15_DATA_TYPE,
                        GUMICS_ATTR_16_NAME, GUMICS_ATTR_16_DATA_TYPE,
                        GUMICS_ATTR_17_NAME, GUMICS_ATTR_17_DATA_TYPE,
                        GUMICS_ATTR_18_NAME, GUMICS_ATTR_18_DATA_TYPE,
                        GUMICS_ATTR_19_NAME, GUMICS_ATTR_19_DATA_TYPE,
                        GUMICS_ATTR_20_NAME, GUMICS_ATTR_20_DATA_TYPE,
                        GUMICS_ATTR_21_NAME, GUMICS_ATTR_21_DATA_TYPE,
                        GUMICS_ATTR_22_NAME, GUMICS_ATTR_22_DATA_TYPE,
                        GUMICS_ATTR_23_NAME, GUMICS_ATTR_23_DATA_TYPE,
                        GUMICS_ATTR_24_NAME, GUMICS_ATTR_24_DATA_TYPE,
                        GUMICS_ATTR_25_NAME, GUMICS_ATTR_25_DATA_TYPE,
                        GUMICS_ATTR_26_NAME, GUMICS_ATTR_26_DATA_TYPE,
                        GUMICS_ATTR_27_NAME, GUMICS_ATTR_27_DATA_TYPE,
                        GUMICS_ATTR_28_NAME, GUMICS_ATTR_28_DATA_TYPE,
                        GUMICS_ATTR_29_NAME, GUMICS_ATTR_29_DATA_TYPE,
                        GUMICS_ATTR_30_NAME, GUMICS_ATTR_30_DATA_TYPE,
                        GUMICS_ATTR_31_NAME, GUMICS_ATTR_31_DATA_TYPE,
                        GUMICS_ATTR_32_NAME, GUMICS_ATTR_32_DATA_TYPE,
                        GUMICS_ATTR_33_NAME, GUMICS_ATTR_33_DATA_TYPE
                        };

