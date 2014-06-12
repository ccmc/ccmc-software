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
 *									       *
 *    06.06.2011  Maddox, Marlo -Adding additional grid specifications for     *
 *    		  for the four(4) new grids			               *
 *    		  						               *
 *    		  This comes as a result of the new MAS version adding an      *
 *    		  offset phi grid which now creates four more parameter-grid   *
 *    		  combinations						       *
 *                                                                             *
 ******************************************************************************/

/*** update total number of registered mas attributes ***/

#define NUMBER_MAS_ATTR   41

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

#define MAS_ATTR_19_NAME        "grid_system_5_number_of_dimensions"
#define MAS_ATTR_19_DATA_TYPE    "int"

#define MAS_ATTR_20_NAME        "grid_system_5_dimension_1_size"
#define MAS_ATTR_20_DATA_TYPE    "int"

#define MAS_ATTR_21_NAME        "grid_system_5_dimension_2_size"
#define MAS_ATTR_21_DATA_TYPE    "int"

#define MAS_ATTR_22_NAME        "grid_system_5_dimension_3_size"
#define MAS_ATTR_22_DATA_TYPE    "int"

#define MAS_ATTR_23_NAME        "grid_system_5"
#define MAS_ATTR_23_DATA_TYPE    "char"

#define MAS_ATTR_24_NAME        "grid_system_6_number_of_dimensions"
#define MAS_ATTR_24_DATA_TYPE    "int"

#define MAS_ATTR_25_NAME        "grid_system_6_dimension_1_size"
#define MAS_ATTR_25_DATA_TYPE    "int"

#define MAS_ATTR_26_NAME        "grid_system_6_dimension_2_size"
#define MAS_ATTR_26_DATA_TYPE    "int"

#define MAS_ATTR_27_NAME        "grid_system_6_dimension_3_size"
#define MAS_ATTR_27_DATA_TYPE    "int"

#define MAS_ATTR_28_NAME        "grid_system_6"
#define MAS_ATTR_28_DATA_TYPE    "char"

#define MAS_ATTR_29_NAME        "grid_system_7_number_of_dimensions"
#define MAS_ATTR_29_DATA_TYPE    "int"

#define MAS_ATTR_30_NAME        "grid_system_7_dimension_1_size"
#define MAS_ATTR_30_DATA_TYPE    "int"

#define MAS_ATTR_31_NAME        "grid_system_7_dimension_2_size"
#define MAS_ATTR_31_DATA_TYPE    "int"

#define MAS_ATTR_32_NAME        "grid_system_7_dimension_3_size"
#define MAS_ATTR_32_DATA_TYPE    "int"

#define MAS_ATTR_33_NAME        "grid_system_8_number_of_dimensions"
#define MAS_ATTR_33_DATA_TYPE    "int"

#define MAS_ATTR_34_NAME        "grid_system_8_dimension_1_size"
#define MAS_ATTR_34_DATA_TYPE    "int"

#define MAS_ATTR_35_NAME        "grid_system_8_dimension_2_size"
#define MAS_ATTR_35_DATA_TYPE    "int"

#define MAS_ATTR_36_NAME        "grid_system_8_dimension_3_size"
#define MAS_ATTR_36_DATA_TYPE    "int"

#define MAS_ATTR_37_NAME        "grid_system_8"
#define MAS_ATTR_37_DATA_TYPE    "char"

#define MAS_ATTR_38_NAME        "grid_5_type"
#define MAS_ATTR_38_DATA_TYPE    "char"

#define MAS_ATTR_39_NAME        "grid_6_type"
#define MAS_ATTR_39_DATA_TYPE    "char"

#define MAS_ATTR_40_NAME        "grid_7_type"
#define MAS_ATTR_40_DATA_TYPE    "char"

#define MAS_ATTR_41_NAME        "grid_8_type"
#define MAS_ATTR_41_DATA_TYPE    "char"


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
      MAS_ATTR_18_DATA_TYPE,
      MAS_ATTR_19_NAME,
      MAS_ATTR_19_DATA_TYPE,
      MAS_ATTR_20_NAME,
      MAS_ATTR_20_DATA_TYPE,
      MAS_ATTR_21_NAME,
      MAS_ATTR_21_DATA_TYPE,
      MAS_ATTR_22_NAME,
      MAS_ATTR_22_DATA_TYPE,
      MAS_ATTR_23_NAME,
      MAS_ATTR_23_DATA_TYPE,
      MAS_ATTR_24_NAME,
      MAS_ATTR_24_DATA_TYPE,
      MAS_ATTR_25_NAME,
      MAS_ATTR_25_DATA_TYPE,
      MAS_ATTR_26_NAME,
      MAS_ATTR_26_DATA_TYPE,
      MAS_ATTR_27_NAME,
      MAS_ATTR_27_DATA_TYPE,
      MAS_ATTR_28_NAME,
      MAS_ATTR_28_DATA_TYPE,
      MAS_ATTR_29_NAME,
      MAS_ATTR_29_DATA_TYPE,
      MAS_ATTR_30_NAME,
      MAS_ATTR_30_DATA_TYPE,
      MAS_ATTR_31_NAME,
      MAS_ATTR_31_DATA_TYPE,
      MAS_ATTR_32_NAME,
      MAS_ATTR_32_DATA_TYPE,
      MAS_ATTR_33_NAME,
      MAS_ATTR_33_DATA_TYPE,
      MAS_ATTR_34_NAME,
      MAS_ATTR_34_DATA_TYPE,
      MAS_ATTR_35_NAME,
      MAS_ATTR_35_DATA_TYPE,
      MAS_ATTR_36_NAME,
      MAS_ATTR_36_DATA_TYPE,
      MAS_ATTR_37_NAME,
      MAS_ATTR_37_DATA_TYPE,
      MAS_ATTR_38_NAME,
      MAS_ATTR_38_DATA_TYPE,
      MAS_ATTR_39_NAME,
      MAS_ATTR_39_DATA_TYPE,
      MAS_ATTR_40_NAME,
      MAS_ATTR_40_DATA_TYPE,
      MAS_ATTR_41_NAME,
      MAS_ATTR_41_DATA_TYPE};
