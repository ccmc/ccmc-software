/*******************************************************************************
 *    NAME:       kpvt_attributes.h                                            *
 *                                                                             *
 *    VERSION:    3.2                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    centralized location for registerd kpvt file                 *
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
 *    11.28.2006  Maddox, Marlo - Initial Implementaion                        *
 *                                                                             *
 ******************************************************************************/

/*** update total number of registered kpvt attributes ***/

#define NUMBER_KPVT_ATTR   16

/*** update total number of registered kpvt variable attributes if any need to be defined ***/

#define NUMBER_KPTV_VARIABLE_ATTR 0

/******************* define ucla_ggcm attribute names ******************************/

#define KPVT_ATTR_1_NAME            "BITPIX"
#define KPVT_ATTR_1_DATA_TYPE        "int"

#define KPVT_ATTR_2_NAME            "NAXIS"
#define KPVT_ATTR_2_DATA_TYPE        "int"

#define KPVT_ATTR_3_NAME            "NAXIS1"
#define KPVT_ATTR_3_DATA_TYPE        "int"

#define KPVT_ATTR_4_NAME            "NAXIS2"
#define KPVT_ATTR_4_DATA_TYPE        "int"

#define KPVT_ATTR_5_NAME            "NAXIS3"
#define KPVT_ATTR_5_DATA_TYPE        "int"

#define KPVT_ATTR_6_NAME            "telescope"
#define KPVT_ATTR_6_DATA_TYPE        "char"

#define KPVT_ATTR_7_NAME            "instrument"
#define KPVT_ATTR_7_DATA_TYPE        "char"

#define KPVT_ATTR_8_NAME            "observation_site"
#define KPVT_ATTR_8_DATA_TYPE        "char"

#define KPVT_ATTR_9_NAME            "carrington_rotation"
#define KPVT_ATTR_9_DATA_TYPE        "int"

#define KPVT_ATTR_10_NAME            "equator_of_map"
#define KPVT_ATTR_10_DATA_TYPE    "float"

#define KPVT_ATTR_11_NAME            "c_lon_of_map_center"
#define KPVT_ATTR_11_DATA_TYPE    "float"

#define KPVT_ATTR_12_NAME            "sine_lat_of_equator"
#define KPVT_ATTR_12_DATA_TYPE    "float"

#define KPVT_ATTR_13_NAME            "lamda_for_lat_coord"
#define KPVT_ATTR_13_DATA_TYPE    "float"

#define KPVT_ATTR_14_NAME            "carrington_lon_step"
#define KPVT_ATTR_14_DATA_TYPE    "float"

#define KPVT_ATTR_15_NAME            "sine_lat_step"
#define KPVT_ATTR_15_DATA_TYPE    "float"

#define KPVT_ATTR_16_NAME            "carrington_step"
#define KPVT_ATTR_16_DATA_TYPE    "float"

struct kpvt_attribute_name_type
{
   char *kpvt_attr_name;
   char *kpvt_attr_data_type;

}static kpvt_attribute_name_type_list[] =
{
      KPVT_ATTR_1_NAME,
      KPVT_ATTR_1_DATA_TYPE,
      KPVT_ATTR_2_NAME,
      KPVT_ATTR_2_DATA_TYPE,
      KPVT_ATTR_3_NAME,
      KPVT_ATTR_3_DATA_TYPE,
      KPVT_ATTR_4_NAME,
      KPVT_ATTR_4_DATA_TYPE,
      KPVT_ATTR_5_NAME,
      KPVT_ATTR_5_DATA_TYPE,
      KPVT_ATTR_6_NAME,
      KPVT_ATTR_6_DATA_TYPE,
      KPVT_ATTR_7_NAME,
      KPVT_ATTR_7_DATA_TYPE,
      KPVT_ATTR_8_NAME,
      KPVT_ATTR_8_DATA_TYPE,
      KPVT_ATTR_9_NAME,
      KPVT_ATTR_9_DATA_TYPE,
      KPVT_ATTR_10_NAME,
      KPVT_ATTR_10_DATA_TYPE,
      KPVT_ATTR_11_NAME,
      KPVT_ATTR_11_DATA_TYPE,
      KPVT_ATTR_12_NAME,
      KPVT_ATTR_12_DATA_TYPE,
      KPVT_ATTR_13_NAME,
      KPVT_ATTR_13_DATA_TYPE,
      KPVT_ATTR_14_NAME,
      KPVT_ATTR_14_DATA_TYPE,
      KPVT_ATTR_15_NAME,
      KPVT_ATTR_15_DATA_TYPE,
      KPVT_ATTR_16_NAME,
      KPVT_ATTR_16_DATA_TYPE };

