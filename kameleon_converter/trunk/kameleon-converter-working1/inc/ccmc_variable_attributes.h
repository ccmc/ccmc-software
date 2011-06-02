/*******************************************************************************
 *                                                                             *
 *    NAME:       ccmc_variable_attributes.h                                   *
 *                                                                             *
 *    VERSION:    4.1.1                                                        *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    centralized location for registerd CCMC variable attribute   *
 *                names and data types                                         *
 *                                                                             *
 *    INPUTS:     NONE                                                         *
 *                                                                             *
 *    OUTPUTS:    NONE                                                         *
 *                                                                             *
 *    FUNCTIONS:  NONE                                                         *
 *                                                                             *
 *    VARIABLES:  - MACRO variable attributes names of the form                *
 *                ccmc_variable_attr_1_name .. ccmc_variable_attr_n_name       *
 *                - MACRO variable attributes data types of the form           *
 *                ccmc_variable_attr_1_data_type ..                            *
 *                ccmc_variable_attr_n_data_type                               *
 *                - NUMBER_CCMC_VARIABLE_ATTR - number of registered variable  *
 *                attributes                                                   *
 *                - ccmc_variable_attribute_name_type_list - array of          *
 *                structures containg variable_atribute names - data_type pairs*
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.11.2003  Maddox, Marlo                                                *
 *                initial development started                                  *
 *                                                                             *
 *    03.26.2004  Maddox, Marlo                                                *
 *                made array declarations static                               *
 *    03.32.2004  Maddox, Marlo                                                *
 *                added additional attributes                                  *
 *                                                                             *
 *    04.21.2004  Maddox, marlo                                                *
 *                added 7 - 9 now                                              *
 *                                                                             *
 *    08.31.2004  Maddox, Marlo                                                *
 *                adding valid min/max & actual min/max                        *
 *                                                                             *
 *    04.052005   Maddox, Marlo                                                *
 *                changed flat array to array of structures containing name    *
 *                data type pairs                                              *
 *                                                                             *
 ******************************************************************************/

#define NUMBER_CCMC_VARIABLE_ATTR    11  /*** update total number of registered attributes ***/

/******************* define variable_attribute names and coorespoding data types ****************/

#define    CCMC_VARIABLE_ATTR_1_NAME        "valid_min"
#define    CCMC_VARIABLE_ATTR_1_DATA_TYPE  "float"

#define CCMC_VARIABLE_ATTR_2_NAME        "valid_max"
#define    CCMC_VARIABLE_ATTR_2_DATA_TYPE  "float"

#define CCMC_VARIABLE_ATTR_3_NAME        "units"
#define    CCMC_VARIABLE_ATTR_3_DATA_TYPE  "char"

#define CCMC_VARIABLE_ATTR_4_NAME        "grid_system"
#define    CCMC_VARIABLE_ATTR_4_DATA_TYPE  "char"

#define CCMC_VARIABLE_ATTR_5_NAME        "mask"
#define    CCMC_VARIABLE_ATTR_5_DATA_TYPE  "float"

#define CCMC_VARIABLE_ATTR_6_NAME        "description"
#define    CCMC_VARIABLE_ATTR_6_DATA_TYPE  "char"

#define CCMC_VARIABLE_ATTR_7_NAME        "is_vector_component"
#define    CCMC_VARIABLE_ATTR_7_DATA_TYPE  "int"

#define CCMC_VARIABLE_ATTR_8_NAME        "position_grid_system"
#define    CCMC_VARIABLE_ATTR_8_DATA_TYPE  "char"

#define CCMC_VARIABLE_ATTR_9_NAME        "data_grid_system"
#define    CCMC_VARIABLE_ATTR_9_DATA_TYPE  "char"

#define    CCMC_VARIABLE_ATTR_10_NAME        "actual_min"
#define    CCMC_VARIABLE_ATTR_10_DATA_TYPE "float"

#define CCMC_VARIABLE_ATTR_11_NAME        "actual_max"
#define    CCMC_VARIABLE_ATTR_11_DATA_TYPE "float"

/********************* load all variable attribute names and data types into variable atribute array of struts **********/

struct ccmc_variable_attribute_name_type
{
   char *ccmc_var_attr_name;
   char *ccmc_var_attr_data_type;
}static ccmc_variable_attribute_name_type_list[] =
{
      CCMC_VARIABLE_ATTR_1_NAME,
      CCMC_VARIABLE_ATTR_1_DATA_TYPE,
      CCMC_VARIABLE_ATTR_2_NAME,
      CCMC_VARIABLE_ATTR_2_DATA_TYPE,
      CCMC_VARIABLE_ATTR_3_NAME,
      CCMC_VARIABLE_ATTR_3_DATA_TYPE,
      CCMC_VARIABLE_ATTR_4_NAME,
      CCMC_VARIABLE_ATTR_4_DATA_TYPE,
      CCMC_VARIABLE_ATTR_5_NAME,
      CCMC_VARIABLE_ATTR_5_DATA_TYPE,
      CCMC_VARIABLE_ATTR_6_NAME,
      CCMC_VARIABLE_ATTR_6_DATA_TYPE,
      CCMC_VARIABLE_ATTR_7_NAME,
      CCMC_VARIABLE_ATTR_7_DATA_TYPE,
      CCMC_VARIABLE_ATTR_8_NAME,
      CCMC_VARIABLE_ATTR_8_DATA_TYPE,
      CCMC_VARIABLE_ATTR_9_NAME,
      CCMC_VARIABLE_ATTR_9_DATA_TYPE,
      CCMC_VARIABLE_ATTR_10_NAME,
      CCMC_VARIABLE_ATTR_10_DATA_TYPE,
      CCMC_VARIABLE_ATTR_11_NAME,
      CCMC_VARIABLE_ATTR_11_DATA_TYPE };
