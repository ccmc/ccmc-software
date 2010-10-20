/*******************************************************************************
*                                                                              *
*    NAME:       ccmc_attributes.h                                             *
*                                                                              *
*    VERSION:    3.2                                                           *
*                                                                              *
*    AUTHOR:     Marlo M. H. Maddox                                            *
*                NASA-GSFC-AETD-ISD-ADMA(587)                                  *
*                Marlo.Maddox@nasa.gov                                         *
*                (301) 286 - 5202                                              *
*                                                                              *
*    PURPOSE:    centralized location for registerd CCMC data file             *
*                attribute names. To be used for data format                   *
*                standarization through conversion software                    *
*                                                                              *
*    INPUTS:     NONE                                                          *
*                                                                              *
*    OUTPUTS:    NONE                                                          *
*                                                                              *
*    FUNCTIONS:  NONE                                                          *
*                                                                              *
*    VARIABLES:  - attributes names of the form ccmc_attr_1_name ..            *
*                ccmc_attr_n_name                                              *
*                - NUMBER_CCMC_ATTR - number of registered attributes          *
*                - ccmc_attribute_array - array of atribute name strings       *
*                                                                              *
*                                                                              *
*    MODIFICATION                                                              *
*    HISTORY:                                                                  *
*                                                                              *
*    09.11.2003  Maddox, Marlo                                                 *
*                initial development started                                   *
*    03.26.2004  Maddox, Marlo                                                 *
*                made array declarations static                                *
*    03.31.2004  Maddox, Marlo                                                 *
*                added aditional attrubutes for grid description               *
*    04.19.2004  Maddox, Marlo                                                 *
*                added 17 & 18                                                 *
*    08.31.2004  Maddox, Marlo                                                 *
*                added 19-20                                                   *
*    03.22.2005  Maddox, Marlo                                                 *
*                added 21                                                      *
*    04.05.2005  Maddox, Marlo                                                 *
*                changed flat array to array of structures containing name     *
*                data type pairs                                               *
*    05.17.2007  Maddox, Marlo                                                 *
*                adding kameleon_version gattribute                            *
*                                                                              *
*******************************************************************************/

/*********** update total number of registered attributes ***********/

#define NUMBER_CCMC_ATTR        22


/******************* define attribute names ******************************/

#define CCMC_ATTR_1_NAME         "README"
#define CCMC_ATTR_1_DATA_TYPE    "char"

#define CCMC_ATTR_2_NAME         "README_visualization"
#define CCMC_ATTR_2_DATA_TYPE    "char"

#define    CCMC_ATTR_3_NAME      "model_name"
#define CCMC_ATTR_3_DATA_TYPE    "char"

#define CCMC_ATTR_4_NAME         "model_type"
#define CCMC_ATTR_4_DATA_TYPE    "char"

#define CCMC_ATTR_5_NAME         "generation_date"
#define CCMC_ATTR_5_DATA_TYPE    "char"

#define CCMC_ATTR_6_NAME         "original_output_file_name"
#define CCMC_ATTR_6_DATA_TYPE    "char"

#define CCMC_ATTR_7_NAME         "run_registration_number"
#define CCMC_ATTR_7_DATA_TYPE    "char"

#define CCMC_ATTR_8_NAME         "generated_by"
#define CCMC_ATTR_8_DATA_TYPE    "char"

#define CCMC_ATTR_9_NAME         "terms_of_usage"
#define CCMC_ATTR_9_DATA_TYPE    "char"

#define CCMC_ATTR_10_NAME        "grid_system_count"
#define CCMC_ATTR_10_DATA_TYPE   "int"

#define CCMC_ATTR_11_NAME        "grid_system_1_number_of_dimensions"
#define CCMC_ATTR_11_DATA_TYPE   "int"

#define CCMC_ATTR_12_NAME        "grid_system_1_dimension_1_size"
#define CCMC_ATTR_12_DATA_TYPE   "int"

#define CCMC_ATTR_13_NAME        "grid_system_1_dimension_2_size"
#define CCMC_ATTR_13_DATA_TYPE   "int"

#define CCMC_ATTR_14_NAME        "grid_system_1_dimension_3_size"
#define CCMC_ATTR_14_DATA_TYPE   "int"

#define CCMC_ATTR_15_NAME        "grid_system_1"
#define CCMC_ATTR_15_DATA_TYPE   "char"

#define CCMC_ATTR_16_NAME        "output_type"
#define CCMC_ATTR_16_DATA_TYPE   "char"

#define CCMC_ATTR_17_NAME        "standard_grid_target"
#define CCMC_ATTR_17_DATA_TYPE   "char"

#define CCMC_ATTR_18_NAME        "grid_1_type"
#define CCMC_ATTR_18_DATA_TYPE   "char"

#define CCMC_ATTR_19_NAME        "start_time"
#define CCMC_ATTR_19_DATA_TYPE   "char"

#define CCMC_ATTR_20_NAME        "end_time"
#define CCMC_ATTR_20_DATA_TYPE   "char"

#define CCMC_ATTR_21_NAME        "run_type"
#define CCMC_ATTR_21_DATA_TYPE   "char"

#define CCMC_ATTR_22_NAME        "kameleon_version"
#define CCMC_ATTR_22_DATA_TYPE   "char"

/********* load all attribute names & data types into atribute array **********/

struct ccmc_attribute_name_type
{
    char *ccmc_attr_name;
    char *ccmc_attr_data_type;

}static ccmc_attribute_name_type_list[] = {
                        CCMC_ATTR_1_NAME,  CCMC_ATTR_1_DATA_TYPE,
                        CCMC_ATTR_2_NAME,  CCMC_ATTR_2_DATA_TYPE,
                        CCMC_ATTR_3_NAME,  CCMC_ATTR_3_DATA_TYPE,
                        CCMC_ATTR_4_NAME,  CCMC_ATTR_4_DATA_TYPE,
                        CCMC_ATTR_5_NAME,  CCMC_ATTR_5_DATA_TYPE,
                        CCMC_ATTR_6_NAME,  CCMC_ATTR_6_DATA_TYPE,
                        CCMC_ATTR_7_NAME,  CCMC_ATTR_7_DATA_TYPE,
                        CCMC_ATTR_8_NAME,  CCMC_ATTR_8_DATA_TYPE,
                        CCMC_ATTR_9_NAME,  CCMC_ATTR_9_DATA_TYPE,
                        CCMC_ATTR_10_NAME, CCMC_ATTR_10_DATA_TYPE,
                        CCMC_ATTR_11_NAME, CCMC_ATTR_11_DATA_TYPE,
                        CCMC_ATTR_12_NAME, CCMC_ATTR_12_DATA_TYPE,
                        CCMC_ATTR_13_NAME, CCMC_ATTR_13_DATA_TYPE,
                        CCMC_ATTR_14_NAME, CCMC_ATTR_14_DATA_TYPE,
                        CCMC_ATTR_15_NAME, CCMC_ATTR_15_DATA_TYPE,
                        CCMC_ATTR_16_NAME, CCMC_ATTR_16_DATA_TYPE,
                        CCMC_ATTR_17_NAME, CCMC_ATTR_17_DATA_TYPE,
                        CCMC_ATTR_18_NAME, CCMC_ATTR_18_DATA_TYPE,
                        CCMC_ATTR_19_NAME, CCMC_ATTR_19_DATA_TYPE,
                        CCMC_ATTR_20_NAME, CCMC_ATTR_20_DATA_TYPE,
                        CCMC_ATTR_21_NAME, CCMC_ATTR_21_DATA_TYPE,
                        CCMC_ATTR_22_NAME, CCMC_ATTR_22_DATA_TYPE
                        };
