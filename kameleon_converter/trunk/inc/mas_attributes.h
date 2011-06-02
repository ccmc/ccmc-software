/*******************************************************************************
 *    NAME:       mas_attributes.h                                             *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    centralized location for registerd mas file                  *
 *                attribute names. To be used for data format                  *
 *                standarization through conversion software                   *
 *                                                                             *
 *    INPUTS:     NONE                                                         *
 *                                                                             *
 *    OUTPUTS:    NONE                                                         *
 *                                                                             *
 *    FUNCTIONS:  NONE                                                         *
 *                                                                             *
 *    VARIABLES:  *                                                            *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    02.09.2007  Maddox, Marlo - Initial Implementaion                        *
 *                                                                             *
 *    04.06.2007  Maddox, Marlo - Adding extra offset grid support             *
 *                                                                             *
 ******************************************************************************/

/*** update total number of registered mas attributes ***/

#define NUMBER_MAS_ATTR   18

/*** update total number of registered mas variable attributes if any need to be defined ***/

#define NUMBER_MAS_VARIABLE_ATTR 0

/******************* define MAS attribute names ******************************/

#define MAS_ATTR_1_NAME        "grid_system_2_number_of_dimensions"
#define MAS_ATTR_1_DATA_TYPE    "int"

#define MAS_ATTR_2_NAME        "grid_system_2_dimension_1_size"
#define MAS_ATTR_2_DATA_TYPE    "int"

#define MAS_ATTR_3_NAME        "grid_system_2_dimension_2_size"
#define MAS_ATTR_3_DATA_TYPE    "int"

#define MAS_ATTR_4_NAME        "grid_system_2_dimension_3_size"
#define MAS_ATTR_4_DATA_TYPE    "int"

#define MAS_ATTR_5_NAME        "grid_system_2"
#define MAS_ATTR_5_DATA_TYPE    "char"

#define MAS_ATTR_6_NAME        "grid_system_3_number_of_dimensions"
#define MAS_ATTR_6_DATA_TYPE    "int"

#define MAS_ATTR_7_NAME        "grid_system_3_dimension_1_size"
#define MAS_ATTR_7_DATA_TYPE    "int"

#define MAS_ATTR_8_NAME        "grid_system_3_dimension_2_size"
#define MAS_ATTR_8_DATA_TYPE    "int"

#define MAS_ATTR_9_NAME        "grid_system_3_dimension_3_size"
#define MAS_ATTR_9_DATA_TYPE    "int"

#define MAS_ATTR_10_NAME        "grid_system_3"
#define MAS_ATTR_10_DATA_TYPE    "char"

#define MAS_ATTR_11_NAME        "grid_system_4_number_of_dimensions"
#define MAS_ATTR_11_DATA_TYPE    "int"

#define MAS_ATTR_12_NAME        "grid_system_4_dimension_1_size"
#define MAS_ATTR_12_DATA_TYPE    "int"

#define MAS_ATTR_13_NAME        "grid_system_4_dimension_2_size"
#define MAS_ATTR_13_DATA_TYPE    "int"

#define MAS_ATTR_14_NAME        "grid_system_4_dimension_3_size"
#define MAS_ATTR_14_DATA_TYPE    "int"

#define MAS_ATTR_15_NAME        "grid_system_4"
#define MAS_ATTR_15_DATA_TYPE    "char"

#define MAS_ATTR_16_NAME        "grid_2_type"
#define MAS_ATTR_16_DATA_TYPE    "char"

#define MAS_ATTR_17_NAME        "grid_3_type"
#define MAS_ATTR_17_DATA_TYPE    "char"

#define MAS_ATTR_18_NAME        "grid_4_type"
#define MAS_ATTR_18_DATA_TYPE    "char"

struct mas_attribute_name_type
{
   char *mas_attr_name;
   char *mas_attr_data_type;

}static mas_attribute_name_type_list[] =
{
      MAS_ATTR_1_NAME,
      MAS_ATTR_1_DATA_TYPE,
      MAS_ATTR_2_NAME,
      MAS_ATTR_2_DATA_TYPE,
      MAS_ATTR_3_NAME,
      MAS_ATTR_3_DATA_TYPE,
      MAS_ATTR_4_NAME,
      MAS_ATTR_4_DATA_TYPE,
      MAS_ATTR_5_NAME,
      MAS_ATTR_5_DATA_TYPE,
      MAS_ATTR_6_NAME,
      MAS_ATTR_6_DATA_TYPE,
      MAS_ATTR_7_NAME,
      MAS_ATTR_7_DATA_TYPE,
      MAS_ATTR_8_NAME,
      MAS_ATTR_8_DATA_TYPE,
      MAS_ATTR_9_NAME,
      MAS_ATTR_9_DATA_TYPE,
      MAS_ATTR_10_NAME,
      MAS_ATTR_10_DATA_TYPE,
      MAS_ATTR_11_NAME,
      MAS_ATTR_11_DATA_TYPE,
      MAS_ATTR_12_NAME,
      MAS_ATTR_12_DATA_TYPE,
      MAS_ATTR_13_NAME,
      MAS_ATTR_13_DATA_TYPE,
      MAS_ATTR_14_NAME,
      MAS_ATTR_14_DATA_TYPE,
      MAS_ATTR_15_NAME,
      MAS_ATTR_15_DATA_TYPE,
      MAS_ATTR_16_NAME,
      MAS_ATTR_16_DATA_TYPE,
      MAS_ATTR_17_NAME,
      MAS_ATTR_17_DATA_TYPE,
      MAS_ATTR_18_NAME,
      MAS_ATTR_18_DATA_TYPE };
