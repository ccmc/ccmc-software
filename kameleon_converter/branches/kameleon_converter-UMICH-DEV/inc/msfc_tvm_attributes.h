/********************************************************************************
 *    NAME:        msfc_tvm_attributes.h                                        *
 *                                                                              *
 *    VERSION:     3.2                                                          *
 *                                                                              *
 *    AUTHOR:      Marlo M. H. Maddox                                           *
 *                 NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                 Marlo.Maddox@nasa.gov                                        *
 *                 (301) 286 - 5202                                             *
 *                                                                              *
 *    PURPOSE:     centralized location for registerd msfc_tvm file             *
 *                 attribute names. To be used for data format                  *
 *                 standarization through conversion software                   *
 *                                                                              *
 *    INPUTS:      NONE                                                         *
 *                                                                              *
 *    OUTPUTS:     NONE                                                         *
 *                                                                              *
 *    FUNCTIONS:   NONE                                                         *
 *                                                                              *
 *    VARIABLES:   *                                                            *
 *                                                                              *
 *    MODIFICATION                                                              *
 *    HISTORY:                                                                  *
 *                                                                              *
 *     12.04.2006  Maddox, Marlo - Initial Implementaion                        *
 *                                                                              *
 ********************************************************************************/

/*** update total number of registered msfc_tvm attributes ***/

#define NUMBER_MSFC_TVM_ATTR   9

/*** update total number of registered msfc_tvm variable attributes if any need to be defined ***/

#define NUMBER_MSFC_TVM_VARIABLE_ATTR 0

/******************* define msfc_tvm attribute names **************************/

#define MSFC_TVM_ATTR_1_NAME            "BITPIX"
#define MSFC_TVM_ATTR_1_DATA_TYPE        "int"

#define MSFC_TVM_ATTR_2_NAME            "NAXIS"
#define MSFC_TVM_ATTR_2_DATA_TYPE        "int"

#define MSFC_TVM_ATTR_3_NAME            "NAXIS1"
#define MSFC_TVM_ATTR_3_DATA_TYPE        "int"

#define MSFC_TVM_ATTR_4_NAME            "NAXIS2"
#define MSFC_TVM_ATTR_4_DATA_TYPE        "int"

#define MSFC_TVM_ATTR_5_NAME            "NAXIS3"
#define MSFC_TVM_ATTR_5_DATA_TYPE        "int"

#define MSFC_TVM_ATTR_6_NAME            "telescope"
#define MSFC_TVM_ATTR_6_DATA_TYPE        "char"

#define MSFC_TVM_ATTR_7_NAME            "instrument"
#define MSFC_TVM_ATTR_7_DATA_TYPE        "char"

#define MSFC_TVM_ATTR_8_NAME            "observation_site"
#define MSFC_TVM_ATTR_8_DATA_TYPE        "char"

#define MSFC_TVM_ATTR_9_NAME            "sun_angle"
#define MSFC_TVM_ATTR_9_DATA_TYPE        "float"

struct msfc_tvm_attribute_name_type
{
   char *msfc_tvm_attr_name;
   char *msfc_tvm_attr_data_type;

}static msfc_tvm_attribute_name_type_list[] =
{
      MSFC_TVM_ATTR_1_NAME,
      MSFC_TVM_ATTR_1_DATA_TYPE,
      MSFC_TVM_ATTR_2_NAME,
      MSFC_TVM_ATTR_2_DATA_TYPE,
      MSFC_TVM_ATTR_3_NAME,
      MSFC_TVM_ATTR_3_DATA_TYPE,
      MSFC_TVM_ATTR_4_NAME,
      MSFC_TVM_ATTR_4_DATA_TYPE,
      MSFC_TVM_ATTR_5_NAME,
      MSFC_TVM_ATTR_5_DATA_TYPE,
      MSFC_TVM_ATTR_6_NAME,
      MSFC_TVM_ATTR_6_DATA_TYPE,
      MSFC_TVM_ATTR_7_NAME,
      MSFC_TVM_ATTR_7_DATA_TYPE,
      MSFC_TVM_ATTR_8_NAME,
      MSFC_TVM_ATTR_8_DATA_TYPE,
      MSFC_TVM_ATTR_9_NAME,
      MSFC_TVM_ATTR_9_DATA_TYPE };

