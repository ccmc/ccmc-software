/*******************************************************************************
*                                                                              *
*    NAME:           ucla_ggcm_attributes.h                                    *
*                                                                              *
*    VERSION:        3.2                                                       *
*                                                                              *
*    AUTHOR:         Marlo M. H. Maddox                                        *
*                    NASA-GSFC-AETD-ISD-ADMA(587)                              *
*                    Marlo.Maddox@nasa.gov                                     *
*                    (301) 286 - 5202                                          *
*                                                                              *
*    PURPOSE:        centralized location for registerd ucla_ggcm file         *
*                    attribute names. To be used for data format               *
*                    standarization through conversion software                *
*                                                                              *
*    INPUTS:         NONE                                                      *
*                                                                              *
*    OUTPUTS:        NONE                                                      *
*                                                                              *
*    FUNCTIONS:      NONE                                                      *
*                                                                              *
*                                                                              *
*    VARIABLES:      - attr names of the form ucla_ggcm_attr_1_name ..         *
*                    ucla_ggcm_attr_n_name                                     *
*                    - NUMBER_UCLA_GGCM_ATTR - number of registered            *
*                    ucla_ggcm_attributes                                      *
*                    - ucla_ggcm_attribute_array - array of atribute           *
*                    name strings                                              *
*                                                                              *
*                                                                              *
*    MODIFICATION                                                              *
*    HISTORY:                                                                  *
*                                                                              *
*    03.26.2004      Maddox, Marlo                                             *
*                    Initial Implementaion                                     *
*                                                                              *
*    04.19.2004      Maddox, Marlo                                             *
*                    added 16 - 18                                             *
*                                                                              *
*    04.27.2004      Maddox, Marlo                                             *
*                    added 19 - 22                                             *
*                                                                              *
*    02.08.2005      Maddox, Marlo                                             *
*                    Added elapsed_time_in_seconds gAttribute                  *
*                                                                              *
*    03.24.2005      Maddox, Marlo                                             *
*                    added 23-25                                               *
*                                                                              *
*    05.04.2006      Maddox, Marlo                                             *
*                    removed ucla_ggcm_attribute_array - no longer needed      *
*                    with new design and name->type structure                  *
*                                                                              *
*    11.29.2006      Maddox, Marlo                                             *
*                    added #define NUMBER_UCLA_GGCM_VARIABLE_ATTR        0     *
*******************************************************************************/

/*** update total number of registered ucla_ggcm attributes ***/

#define NUMBER_UCLA_GGCM_ATTR   25

/*** update total number of registered ucla_ggcm variable attributes if any need to be defined ***/

#define NUMBER_UCLA_GGCM_VARIABLE_ATTR        0

/******************* define ucla_ggcm attribute names ******************************/

#define UCLA_GGCM_ATTR_1_NAME        "grid_system_2_number_of_dimensions"
#define UCLA_GGCM_ATTR_1_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_2_NAME        "grid_system_2_dimension_1_size"
#define UCLA_GGCM_ATTR_2_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_3_NAME        "grid_system_2_dimension_2_size"
#define UCLA_GGCM_ATTR_3_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_4_NAME        "grid_system_2_dimension_3_size"
#define UCLA_GGCM_ATTR_4_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_5_NAME        "grid_system_2"
#define UCLA_GGCM_ATTR_5_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_6_NAME        "grid_system_3_number_of_dimensions"
#define UCLA_GGCM_ATTR_6_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_7_NAME        "grid_system_3_dimension_1_size"
#define UCLA_GGCM_ATTR_7_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_8_NAME        "grid_system_3_dimension_2_size"
#define UCLA_GGCM_ATTR_8_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_9_NAME        "grid_system_3_dimension_3_size"
#define UCLA_GGCM_ATTR_9_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_10_NAME        "grid_system_3"
#define UCLA_GGCM_ATTR_10_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_11_NAME        "grid_system_4_number_of_dimensions"
#define UCLA_GGCM_ATTR_11_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_12_NAME        "grid_system_4_dimension_1_size"
#define UCLA_GGCM_ATTR_12_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_13_NAME        "grid_system_4_dimension_2_size"
#define UCLA_GGCM_ATTR_13_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_14_NAME        "grid_system_4_dimension_3_size"
#define UCLA_GGCM_ATTR_14_DATA_TYPE    "int"

#define UCLA_GGCM_ATTR_15_NAME        "grid_system_4"
#define UCLA_GGCM_ATTR_15_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_16_NAME        "grid_2_type"
#define UCLA_GGCM_ATTR_16_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_17_NAME        "grid_3_type"
#define UCLA_GGCM_ATTR_17_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_18_NAME        "grid_4_type"
#define UCLA_GGCM_ATTR_18_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_19_NAME        "r_body"
#define UCLA_GGCM_ATTR_19_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_20_NAME        "r_currents"
#define UCLA_GGCM_ATTR_20_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_21_NAME        "dipole_time"
#define UCLA_GGCM_ATTR_21_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_22_NAME        "elapsed_time_in_seconds"
#define UCLA_GGCM_ATTR_22_DATA_TYPE    "float"

#define UCLA_GGCM_ATTR_23_NAME        "dipole_update"
#define UCLA_GGCM_ATTR_23_DATA_TYPE    "char"

#define UCLA_GGCM_ATTR_24_NAME        "dipole_tilt"
#define UCLA_GGCM_ATTR_24_DATA_TYPE    "float"

#define UCLA_GGCM_ATTR_25_NAME        "dipole_tilt_y"
#define UCLA_GGCM_ATTR_25_DATA_TYPE    "float"


struct ucla_ggcm_attribute_name_type
{
    char *ucla_ggcm_attr_name;
    char *ucla_ggcm_attr_data_type;

} static ucla_ggcm_attribute_name_type_list[] = {
                        UCLA_GGCM_ATTR_1_NAME,  UCLA_GGCM_ATTR_1_DATA_TYPE,
                        UCLA_GGCM_ATTR_2_NAME,  UCLA_GGCM_ATTR_2_DATA_TYPE,
                        UCLA_GGCM_ATTR_3_NAME,  UCLA_GGCM_ATTR_3_DATA_TYPE,
                        UCLA_GGCM_ATTR_4_NAME,  UCLA_GGCM_ATTR_4_DATA_TYPE,
                        UCLA_GGCM_ATTR_5_NAME,  UCLA_GGCM_ATTR_5_DATA_TYPE,
                        UCLA_GGCM_ATTR_6_NAME,  UCLA_GGCM_ATTR_6_DATA_TYPE,
                        UCLA_GGCM_ATTR_7_NAME,  UCLA_GGCM_ATTR_7_DATA_TYPE,
                        UCLA_GGCM_ATTR_8_NAME,  UCLA_GGCM_ATTR_8_DATA_TYPE,
                        UCLA_GGCM_ATTR_9_NAME,  UCLA_GGCM_ATTR_9_DATA_TYPE,
                        UCLA_GGCM_ATTR_10_NAME, UCLA_GGCM_ATTR_10_DATA_TYPE,
                        UCLA_GGCM_ATTR_11_NAME, UCLA_GGCM_ATTR_11_DATA_TYPE,
                        UCLA_GGCM_ATTR_12_NAME, UCLA_GGCM_ATTR_12_DATA_TYPE,
                        UCLA_GGCM_ATTR_13_NAME, UCLA_GGCM_ATTR_13_DATA_TYPE,
                        UCLA_GGCM_ATTR_14_NAME, UCLA_GGCM_ATTR_14_DATA_TYPE,
                        UCLA_GGCM_ATTR_15_NAME, UCLA_GGCM_ATTR_15_DATA_TYPE,
                        UCLA_GGCM_ATTR_16_NAME, UCLA_GGCM_ATTR_16_DATA_TYPE,
                        UCLA_GGCM_ATTR_17_NAME, UCLA_GGCM_ATTR_17_DATA_TYPE,
                        UCLA_GGCM_ATTR_18_NAME, UCLA_GGCM_ATTR_18_DATA_TYPE,
                        UCLA_GGCM_ATTR_19_NAME, UCLA_GGCM_ATTR_19_DATA_TYPE,
                        UCLA_GGCM_ATTR_20_NAME, UCLA_GGCM_ATTR_20_DATA_TYPE,
                        UCLA_GGCM_ATTR_21_NAME, UCLA_GGCM_ATTR_21_DATA_TYPE,
                        UCLA_GGCM_ATTR_22_NAME, UCLA_GGCM_ATTR_22_DATA_TYPE,
                        UCLA_GGCM_ATTR_23_NAME, UCLA_GGCM_ATTR_23_DATA_TYPE,
                        UCLA_GGCM_ATTR_24_NAME, UCLA_GGCM_ATTR_24_DATA_TYPE,
                        UCLA_GGCM_ATTR_25_NAME, UCLA_GGCM_ATTR_25_DATA_TYPE
                        };


