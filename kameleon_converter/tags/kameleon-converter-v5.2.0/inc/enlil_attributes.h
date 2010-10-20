/*******************************************************************************
 *                                                                             *
 *    NAME:       enlil_attributes.h                                           *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    centralized location for registerd enlil file                *
 *                attribute names. To be used for data format                  *
 *                standarization through conversion software                   *
 *                                                                             *
 *    INPUTS:     NONE                                                         *
 *                                                                             *
 *    OUTPUTS:    NONE                                                         *
 *                                                                             *
 *    FUNCTIONS:  NONE                                                         *
 *                                                                             *
 *    VARIABLES:  attr names of the form enlil_attr_1_name .. enlil_attr_n_name*
 *                NUMBER_ENLIL_ATTR - number of registered batsrus_attributes  *
 *                enlil_attribute_array - array of atribute name strings       *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    04.05.2006  Maddox, Maddox                                               *
 *                Created.                                                     *
 *    10.16.2006  Maddox, Marlo                                                *
 *                Adding grid_system_1_dimension_4_size    attribute           *
 *                                                                             *
 *                Fixing type for some attributes                              *
 *                                                                             *
 *                Adding time_physical_time attribute                          *
 *                                                                             *
 *    10.19.2006  Maddox, Marlo                                                *
 *                The ev* time variables need to remain variables.  Remove     *
 *                from enlil_attribute_name_type_list:                         *
 *                                                                             *
 *                ENLIL_ATTR_31_NAME         "evh_physical_time"               *
 *                ENLIL_ATTR_31_DATA_TYPE    "float"                           *
 *                ENLIL_ATTR_32_NAME         "evh_physical_time_step"          *
 *                ENLIL_ATTR_32_DATA_TYPE    "float"                           *
 *                ENLIL_ATTR_33_NAME         "evh_numerical_time_step"         *
 *                ENLIL_ATTR_33_DATA_TYPE    "int"                             *
 *                ENLIL_ATTR_55_NAME         "evl_physical_time"               *
 *                ENLIL_ATTR_55_DATA_TYPE    "float"                           *
 *                ENLIL_ATTR_56_NAME         "evl_physical_time_step"          *
 *                ENLIL_ATTR_56_DATA_TYPE    "float"                           *
 *                ENLIL_ATTR_57_NAME         "evl_numerical_time_step"         *
 *                ENLIL_ATTR_57_DATA_TYPE    "int"                             *
 *                ENLIL_ATTR_79_NAME         "evg_physical_time"               *
 *                ENLIL_ATTR_79_DATA_TYPE    "float"                           *
 *                ENLIL_ATTR_80_NAME         "evg_physical_time_step"          *
 *                ENLIL_ATTR_80_DATA_TYPE    "float"                           *
 *                ENLIL_ATTR_81_NAME         "evg_numerical_time_step"         *
 *                ENLIL_ATTR_81_DATA_TYPE    "int"                             *
 *                ENLIL_ATTR_103_NAME        "evp_physical_time"               *
 *                ENLIL_ATTR_103_DATA_TYPE    "float"                          *
 *                ENLIL_ATTR_104_NAME        "evp_physical_time_step"          *
 *                ENLIL_ATTR_104_DATA_TYPE    "float"                          *
 *                ENLIL_ATTR_105_NAME        "evp_numerical_time_step"         *
 *                ENLIL_ATTR_105_DATA_TYPE    "int"                            *
 ******************************************************************************/

/*** NUMBER_ENLIL_ATTR is actually 123 but we removed the
 * ev*[physical_time],[physical_time_step], and[numerical_time_step]
 * attributes from the list - they will be stored as variables...*/

#define NUMBER_ENLIL_ATTR        111  /*** update total number of registered attributes ***/

#define NUMBER_ENLIL_VARIABLE_ATTR    0        /*** update total number of registered enlil variable attributes if any need to be defined ***/

/******************* define enlil attribute names ******************************/

/** first define all of the model specific global attributes for the enlil tim.<rrrr>.nc files **/

/** second grid for time file **/

#define    ENLIL_ATTR_1_NAME        "grid_system_2"
#define    ENLIL_ATTR_1_DATA_TYPE    "char"

#define    ENLIL_ATTR_2_NAME        "grid_2_type"
#define    ENLIL_ATTR_2_DATA_TYPE    "char"

#define    ENLIL_ATTR_3_NAME        "grid_system_2_number_of_dimensions"
#define    ENLIL_ATTR_3_DATA_TYPE    "int"

#define    ENLIL_ATTR_4_NAME        "grid_system_2_dimension_1_size"
#define    ENLIL_ATTR_4_DATA_TYPE    "int"

#define    ENLIL_ATTR_5_NAME        "grid_system_2_dimension_2_size"
#define    ENLIL_ATTR_5_DATA_TYPE    "int"

#define    ENLIL_ATTR_6_NAME        "grid_system_2_dimension_3_size"
#define    ENLIL_ATTR_6_DATA_TYPE    "int"

#define    ENLIL_ATTR_7_NAME        "grid_system_2_dimension_4_size"
#define    ENLIL_ATTR_7_DATA_TYPE    "int"

/** all time fie global attributes **/

#define    ENLIL_ATTR_8_NAME        "time_physical_time_step"
#define    ENLIL_ATTR_8_DATA_TYPE    "float"

#define    ENLIL_ATTR_9_NAME        "time_numerical_time_step"
#define    ENLIL_ATTR_9_DATA_TYPE    "int"

#define    ENLIL_ATTR_10_NAME        "time_type"
#define    ENLIL_ATTR_10_DATA_TYPE    "char"

#define    ENLIL_ATTR_11_NAME        "time_title"
#define    ENLIL_ATTR_11_DATA_TYPE    "char"

#define    ENLIL_ATTR_12_NAME        "time_name"
#define    ENLIL_ATTR_12_DATA_TYPE    "char"

#define    ENLIL_ATTR_13_NAME        "time_project"
#define    ENLIL_ATTR_13_DATA_TYPE    "char"

#define    ENLIL_ATTR_14_NAME        "time_initial"
#define    ENLIL_ATTR_14_DATA_TYPE    "char"

#define    ENLIL_ATTR_15_NAME        "time_resume"
#define    ENLIL_ATTR_15_DATA_TYPE    "char"

#define    ENLIL_ATTR_16_NAME        "time_boundary"
#define    ENLIL_ATTR_16_DATA_TYPE    "char"

#define    ENLIL_ATTR_17_NAME        "time_passage"
#define    ENLIL_ATTR_17_DATA_TYPE    "char"

#define    ENLIL_ATTR_18_NAME        "time_grid"
#define    ENLIL_ATTR_18_DATA_TYPE    "char"

#define    ENLIL_ATTR_19_NAME        "time_geometry"
#define    ENLIL_ATTR_19_DATA_TYPE    "char"

#define    ENLIL_ATTR_20_NAME        "time_code"
#define    ENLIL_ATTR_20_DATA_TYPE    "char"

#define    ENLIL_ATTR_21_NAME        "time_parameters"
#define    ENLIL_ATTR_21_DATA_TYPE    "char"

#define    ENLIL_ATTR_22_NAME        "time_refdate_mjd"
#define    ENLIL_ATTR_22_DATA_TYPE    "double"

#define    ENLIL_ATTR_23_NAME        "time_gamma"
#define    ENLIL_ATTR_23_DATA_TYPE    "double"

#define    ENLIL_ATTR_24_NAME        "time_xalpha"
#define    ENLIL_ATTR_24_DATA_TYPE    "double"

#define    ENLIL_ATTR_25_NAME        "time_history"
#define    ENLIL_ATTR_25_DATA_TYPE    "char"

/**  define all of the model specific global attributes for the enlil evh.nc files **/

/** third grid for evh file **/

#define    ENLIL_ATTR_26_NAME        "grid_system_3"
#define    ENLIL_ATTR_26_DATA_TYPE    "char"

#define    ENLIL_ATTR_27_NAME        "grid_3_type"
#define    ENLIL_ATTR_27_DATA_TYPE    "char"

#define    ENLIL_ATTR_28_NAME        "grid_system_3_number_of_dimensions"
#define    ENLIL_ATTR_28_DATA_TYPE    "int"

#define    ENLIL_ATTR_29_NAME        "grid_system_3_dimension_1_size"
#define    ENLIL_ATTR_29_DATA_TYPE    "int"

#define    ENLIL_ATTR_30_NAME        "grid_system_3_dimension_2_size"
#define    ENLIL_ATTR_30_DATA_TYPE    "int"

/** all evh fie global attributes **/

#define    ENLIL_ATTR_31_NAME        "evh_physical_time"
#define    ENLIL_ATTR_31_DATA_TYPE    "float"

#define    ENLIL_ATTR_32_NAME        "evh_physical_time_step"
#define    ENLIL_ATTR_32_DATA_TYPE    "float"

#define    ENLIL_ATTR_33_NAME        "evh_numerical_time_step"
#define    ENLIL_ATTR_33_DATA_TYPE    "int"

#define    ENLIL_ATTR_34_NAME        "evh_type"
#define    ENLIL_ATTR_34_DATA_TYPE    "char"

#define    ENLIL_ATTR_35_NAME        "evh_title"
#define    ENLIL_ATTR_35_DATA_TYPE    "char"

#define    ENLIL_ATTR_36_NAME        "evh_name"
#define    ENLIL_ATTR_36_DATA_TYPE    "char"

#define    ENLIL_ATTR_37_NAME        "evh_project"
#define    ENLIL_ATTR_37_DATA_TYPE    "char"

#define    ENLIL_ATTR_38_NAME        "evh_initial"
#define    ENLIL_ATTR_38_DATA_TYPE    "char"

#define    ENLIL_ATTR_39_NAME        "evh_resume"
#define    ENLIL_ATTR_39_DATA_TYPE    "char"

#define    ENLIL_ATTR_40_NAME        "evh_boundary"
#define    ENLIL_ATTR_40_DATA_TYPE    "char"

#define    ENLIL_ATTR_41_NAME        "evh_passage"
#define    ENLIL_ATTR_41_DATA_TYPE    "char"

#define    ENLIL_ATTR_42_NAME        "evh_grid"
#define    ENLIL_ATTR_42_DATA_TYPE    "char"

#define    ENLIL_ATTR_43_NAME        "evh_geometry"
#define    ENLIL_ATTR_43_DATA_TYPE    "char"

#define    ENLIL_ATTR_44_NAME        "evh_code"
#define    ENLIL_ATTR_44_DATA_TYPE    "char"

#define    ENLIL_ATTR_45_NAME        "evh_parameters"
#define    ENLIL_ATTR_45_DATA_TYPE    "char"

#define    ENLIL_ATTR_46_NAME        "evh_refdate_mjd"
#define    ENLIL_ATTR_46_DATA_TYPE    "double"

#define    ENLIL_ATTR_47_NAME        "evh_gamma"
#define    ENLIL_ATTR_47_DATA_TYPE    "double"

#define    ENLIL_ATTR_48_NAME        "evh_xalpha"
#define    ENLIL_ATTR_48_DATA_TYPE    "double"

#define    ENLIL_ATTR_49_NAME        "evh_history"
#define    ENLIL_ATTR_49_DATA_TYPE    "char"

/**  define all of the model specific global attributes for the enlil evl.nc files **/

/** fourth grid for evl file **/

#define    ENLIL_ATTR_50_NAME        "grid_system_4"
#define    ENLIL_ATTR_50_DATA_TYPE    "char"

#define    ENLIL_ATTR_51_NAME        "grid_4_type"
#define    ENLIL_ATTR_51_DATA_TYPE    "char"

#define    ENLIL_ATTR_52_NAME        "grid_system_4_number_of_dimensions"
#define    ENLIL_ATTR_52_DATA_TYPE    "int"

#define    ENLIL_ATTR_53_NAME        "grid_system_4_dimension_1_size"
#define    ENLIL_ATTR_53_DATA_TYPE    "int"

#define    ENLIL_ATTR_54_NAME        "grid_system_4_dimension_2_size"
#define    ENLIL_ATTR_54_DATA_TYPE    "int"

/** all evl fie global attributes **/

#define    ENLIL_ATTR_55_NAME        "evl_physical_time"
#define    ENLIL_ATTR_55_DATA_TYPE    "float"

#define    ENLIL_ATTR_56_NAME        "evl_physical_time_step"
#define    ENLIL_ATTR_56_DATA_TYPE    "float"

#define    ENLIL_ATTR_57_NAME        "evl_numerical_time_step"
#define    ENLIL_ATTR_57_DATA_TYPE    "int"

#define    ENLIL_ATTR_58_NAME        "evl_type"
#define    ENLIL_ATTR_58_DATA_TYPE    "char"

#define    ENLIL_ATTR_59_NAME        "evl_title"
#define    ENLIL_ATTR_59_DATA_TYPE    "char"

#define    ENLIL_ATTR_60_NAME        "evl_name"
#define    ENLIL_ATTR_60_DATA_TYPE    "char"

#define    ENLIL_ATTR_61_NAME        "evl_project"
#define    ENLIL_ATTR_61_DATA_TYPE    "char"

#define    ENLIL_ATTR_62_NAME        "evl_initial"
#define    ENLIL_ATTR_62_DATA_TYPE    "char"

#define    ENLIL_ATTR_63_NAME        "evl_resume"
#define    ENLIL_ATTR_63_DATA_TYPE    "char"

#define    ENLIL_ATTR_64_NAME        "evl_boundary"
#define    ENLIL_ATTR_64_DATA_TYPE    "char"

#define    ENLIL_ATTR_65_NAME        "evl_passage"
#define    ENLIL_ATTR_65_DATA_TYPE    "char"

#define    ENLIL_ATTR_66_NAME        "evl_grid"
#define    ENLIL_ATTR_66_DATA_TYPE    "char"

#define    ENLIL_ATTR_67_NAME        "evl_geometry"
#define    ENLIL_ATTR_67_DATA_TYPE    "char"

#define    ENLIL_ATTR_68_NAME        "evl_code"
#define    ENLIL_ATTR_68_DATA_TYPE    "char"

#define    ENLIL_ATTR_69_NAME        "evl_parameters"
#define    ENLIL_ATTR_69_DATA_TYPE    "char"

#define    ENLIL_ATTR_70_NAME        "evl_refdate_mjd"
#define    ENLIL_ATTR_70_DATA_TYPE    "double"

#define    ENLIL_ATTR_71_NAME        "evl_gamma"
#define    ENLIL_ATTR_71_DATA_TYPE    "double"

#define    ENLIL_ATTR_72_NAME        "evl_xalpha"
#define    ENLIL_ATTR_72_DATA_TYPE    "double"

#define    ENLIL_ATTR_73_NAME        "evl_history"
#define    ENLIL_ATTR_73_DATA_TYPE    "char"

/**  define all of the model specific global attributes for the enlil evg.nc files **/

/** fifth grid for evg file **/

#define    ENLIL_ATTR_74_NAME        "grid_system_5"
#define    ENLIL_ATTR_74_DATA_TYPE    "char"

#define    ENLIL_ATTR_75_NAME        "grid_5_type"
#define    ENLIL_ATTR_75_DATA_TYPE    "char"

#define    ENLIL_ATTR_76_NAME        "grid_system_5_number_of_dimensions"
#define    ENLIL_ATTR_76_DATA_TYPE    "int"

#define    ENLIL_ATTR_77_NAME        "grid_system_5_dimension_1_size"
#define    ENLIL_ATTR_77_DATA_TYPE    "int"

#define    ENLIL_ATTR_78_NAME        "grid_system_5_dimension_2_size"
#define    ENLIL_ATTR_78_DATA_TYPE    "int"

/** all evg fie global attributes **/

#define    ENLIL_ATTR_79_NAME        "evg_physical_time"
#define    ENLIL_ATTR_79_DATA_TYPE    "float"

#define    ENLIL_ATTR_80_NAME        "evg_physical_time_step"
#define    ENLIL_ATTR_80_DATA_TYPE    "float"

#define    ENLIL_ATTR_81_NAME        "evg_numerical_time_step"
#define    ENLIL_ATTR_81_DATA_TYPE    "int"

#define    ENLIL_ATTR_82_NAME        "evg_type"
#define    ENLIL_ATTR_82_DATA_TYPE    "char"

#define    ENLIL_ATTR_83_NAME        "evg_title"
#define    ENLIL_ATTR_83_DATA_TYPE    "char"

#define    ENLIL_ATTR_84_NAME        "evg_name"
#define    ENLIL_ATTR_84_DATA_TYPE    "char"

#define    ENLIL_ATTR_85_NAME        "evg_project"
#define    ENLIL_ATTR_85_DATA_TYPE    "char"

#define    ENLIL_ATTR_86_NAME        "evg_initial"
#define    ENLIL_ATTR_86_DATA_TYPE    "char"

#define    ENLIL_ATTR_87_NAME        "evg_resume"
#define    ENLIL_ATTR_87_DATA_TYPE    "char"

#define    ENLIL_ATTR_88_NAME        "evg_boundary"
#define    ENLIL_ATTR_88_DATA_TYPE    "char"

#define    ENLIL_ATTR_89_NAME        "evg_passage"
#define    ENLIL_ATTR_89_DATA_TYPE    "char"

#define    ENLIL_ATTR_90_NAME        "evg_grid"
#define    ENLIL_ATTR_90_DATA_TYPE    "char"

#define    ENLIL_ATTR_91_NAME        "evg_geometry"
#define    ENLIL_ATTR_91_DATA_TYPE    "char"

#define    ENLIL_ATTR_92_NAME        "evg_code"
#define    ENLIL_ATTR_92_DATA_TYPE    "char"

#define    ENLIL_ATTR_93_NAME        "evg_parameters"
#define    ENLIL_ATTR_93_DATA_TYPE    "char"

#define    ENLIL_ATTR_94_NAME        "evg_refdate_mjd"
#define    ENLIL_ATTR_94_DATA_TYPE    "double"

#define    ENLIL_ATTR_95_NAME        "evg_gamma"
#define    ENLIL_ATTR_95_DATA_TYPE    "double"

#define    ENLIL_ATTR_96_NAME        "evg_xalpha"
#define    ENLIL_ATTR_96_DATA_TYPE    "double"

#define    ENLIL_ATTR_97_NAME        "evg_history"
#define    ENLIL_ATTR_97_DATA_TYPE    "char"

/**  define all of the model specific global attributes for the enlil evp.nc files **/

/** sixth grid for evp file **/

#define    ENLIL_ATTR_98_NAME        "grid_system_6"
#define    ENLIL_ATTR_98_DATA_TYPE    "char"

#define    ENLIL_ATTR_99_NAME        "grid_6_type"
#define    ENLIL_ATTR_99_DATA_TYPE    "char"

#define    ENLIL_ATTR_100_NAME        "grid_system_6_number_of_dimensions"
#define    ENLIL_ATTR_100_DATA_TYPE    "int"

#define    ENLIL_ATTR_101_NAME        "grid_system_6_dimension_1_size"
#define    ENLIL_ATTR_101_DATA_TYPE    "int"

#define    ENLIL_ATTR_102_NAME        "grid_system_6_dimension_2_size"
#define    ENLIL_ATTR_102_DATA_TYPE    "int"

/** all evg fie global attributes **/

#define    ENLIL_ATTR_103_NAME        "evp_physical_time"
#define    ENLIL_ATTR_103_DATA_TYPE    "float"

#define    ENLIL_ATTR_104_NAME        "evp_physical_time_step"
#define    ENLIL_ATTR_104_DATA_TYPE    "float"

#define    ENLIL_ATTR_105_NAME        "evp_numerical_time_step"
#define    ENLIL_ATTR_105_DATA_TYPE    "int"

#define    ENLIL_ATTR_106_NAME        "evp_type"
#define    ENLIL_ATTR_106_DATA_TYPE    "char"

#define    ENLIL_ATTR_107_NAME        "evp_title"
#define    ENLIL_ATTR_107_DATA_TYPE    "char"

#define    ENLIL_ATTR_108_NAME        "evp_name"
#define    ENLIL_ATTR_108_DATA_TYPE    "char"

#define    ENLIL_ATTR_109_NAME        "evp_project"
#define    ENLIL_ATTR_109_DATA_TYPE    "char"

#define    ENLIL_ATTR_110_NAME        "evp_initial"
#define    ENLIL_ATTR_110_DATA_TYPE    "char"

#define    ENLIL_ATTR_111_NAME        "evp_resume"
#define    ENLIL_ATTR_111_DATA_TYPE    "char"

#define    ENLIL_ATTR_112_NAME        "evp_boundary"
#define    ENLIL_ATTR_112_DATA_TYPE    "char"

#define    ENLIL_ATTR_113_NAME        "evp_passage"
#define    ENLIL_ATTR_113_DATA_TYPE    "char"

#define    ENLIL_ATTR_114_NAME        "evp_grid"
#define    ENLIL_ATTR_114_DATA_TYPE    "char"

#define    ENLIL_ATTR_115_NAME        "evp_geometry"
#define    ENLIL_ATTR_115_DATA_TYPE    "char"

#define    ENLIL_ATTR_116_NAME        "evp_code"
#define    ENLIL_ATTR_116_DATA_TYPE    "char"

#define    ENLIL_ATTR_117_NAME        "evp_parameters"
#define    ENLIL_ATTR_117_DATA_TYPE    "char"

#define    ENLIL_ATTR_118_NAME        "evp_refdate_mjd"
#define    ENLIL_ATTR_118_DATA_TYPE    "double"

#define    ENLIL_ATTR_119_NAME        "evp_gamma"
#define    ENLIL_ATTR_119_DATA_TYPE    "double"

#define    ENLIL_ATTR_120_NAME        "evp_xalpha"
#define    ENLIL_ATTR_120_DATA_TYPE    "double"

#define    ENLIL_ATTR_121_NAME        "evp_history"
#define    ENLIL_ATTR_121_DATA_TYPE    "char"

/********************** LATE ADDITIONS **************************/

#define    ENLIL_ATTR_122_NAME        "grid_system_1_dimension_4_size"
#define    ENLIL_ATTR_122_DATA_TYPE    "int"

#define    ENLIL_ATTR_123_NAME        "time_physical_time"
#define    ENLIL_ATTR_123_DATA_TYPE    "float"

/********************* load all enlil attribute names & data type into enlil_atribute_name_type_list array of structures **********/

struct enlil_attribute_name_type
{
   char *enlil_attr_name;
   char *enlil_attr_data_type;

}static enlil_attribute_name_type_list[] =
{
      ENLIL_ATTR_1_NAME,
      ENLIL_ATTR_1_DATA_TYPE,
      ENLIL_ATTR_2_NAME,
      ENLIL_ATTR_2_DATA_TYPE,
      ENLIL_ATTR_3_NAME,
      ENLIL_ATTR_3_DATA_TYPE,
      ENLIL_ATTR_4_NAME,
      ENLIL_ATTR_4_DATA_TYPE,
      ENLIL_ATTR_5_NAME,
      ENLIL_ATTR_5_DATA_TYPE,
      ENLIL_ATTR_6_NAME,
      ENLIL_ATTR_6_DATA_TYPE,
      ENLIL_ATTR_7_NAME,
      ENLIL_ATTR_7_DATA_TYPE,
      ENLIL_ATTR_8_NAME,
      ENLIL_ATTR_8_DATA_TYPE,
      ENLIL_ATTR_9_NAME,
      ENLIL_ATTR_9_DATA_TYPE,
      ENLIL_ATTR_10_NAME,
      ENLIL_ATTR_10_DATA_TYPE,
      ENLIL_ATTR_11_NAME,
      ENLIL_ATTR_11_DATA_TYPE,
      ENLIL_ATTR_12_NAME,
      ENLIL_ATTR_12_DATA_TYPE,
      ENLIL_ATTR_13_NAME,
      ENLIL_ATTR_13_DATA_TYPE,
      ENLIL_ATTR_14_NAME,
      ENLIL_ATTR_14_DATA_TYPE,
      ENLIL_ATTR_15_NAME,
      ENLIL_ATTR_15_DATA_TYPE,
      ENLIL_ATTR_16_NAME,
      ENLIL_ATTR_16_DATA_TYPE,
      ENLIL_ATTR_17_NAME,
      ENLIL_ATTR_17_DATA_TYPE,
      ENLIL_ATTR_18_NAME,
      ENLIL_ATTR_18_DATA_TYPE,
      ENLIL_ATTR_19_NAME,
      ENLIL_ATTR_19_DATA_TYPE,
      ENLIL_ATTR_20_NAME,
      ENLIL_ATTR_20_DATA_TYPE,
      ENLIL_ATTR_21_NAME,
      ENLIL_ATTR_21_DATA_TYPE,
      ENLIL_ATTR_22_NAME,
      ENLIL_ATTR_22_DATA_TYPE,
      ENLIL_ATTR_23_NAME,
      ENLIL_ATTR_23_DATA_TYPE,
      ENLIL_ATTR_24_NAME,
      ENLIL_ATTR_24_DATA_TYPE,
      ENLIL_ATTR_25_NAME,
      ENLIL_ATTR_25_DATA_TYPE,
      ENLIL_ATTR_26_NAME,
      ENLIL_ATTR_26_DATA_TYPE,
      ENLIL_ATTR_27_NAME,
      ENLIL_ATTR_27_DATA_TYPE,
      ENLIL_ATTR_28_NAME,
      ENLIL_ATTR_28_DATA_TYPE,
      ENLIL_ATTR_29_NAME,
      ENLIL_ATTR_29_DATA_TYPE,
      ENLIL_ATTR_30_NAME,
      ENLIL_ATTR_30_DATA_TYPE,
      ENLIL_ATTR_34_NAME,
      ENLIL_ATTR_34_DATA_TYPE,
      ENLIL_ATTR_35_NAME,
      ENLIL_ATTR_35_DATA_TYPE,
      ENLIL_ATTR_36_NAME,
      ENLIL_ATTR_36_DATA_TYPE,
      ENLIL_ATTR_37_NAME,
      ENLIL_ATTR_37_DATA_TYPE,
      ENLIL_ATTR_38_NAME,
      ENLIL_ATTR_38_DATA_TYPE,
      ENLIL_ATTR_39_NAME,
      ENLIL_ATTR_39_DATA_TYPE,
      ENLIL_ATTR_40_NAME,
      ENLIL_ATTR_40_DATA_TYPE,
      ENLIL_ATTR_41_NAME,
      ENLIL_ATTR_41_DATA_TYPE,
      ENLIL_ATTR_42_NAME,
      ENLIL_ATTR_42_DATA_TYPE,
      ENLIL_ATTR_43_NAME,
      ENLIL_ATTR_43_DATA_TYPE,
      ENLIL_ATTR_44_NAME,
      ENLIL_ATTR_44_DATA_TYPE,
      ENLIL_ATTR_45_NAME,
      ENLIL_ATTR_45_DATA_TYPE,
      ENLIL_ATTR_46_NAME,
      ENLIL_ATTR_46_DATA_TYPE,
      ENLIL_ATTR_47_NAME,
      ENLIL_ATTR_47_DATA_TYPE,
      ENLIL_ATTR_48_NAME,
      ENLIL_ATTR_48_DATA_TYPE,
      ENLIL_ATTR_49_NAME,
      ENLIL_ATTR_49_DATA_TYPE,
      ENLIL_ATTR_50_NAME,
      ENLIL_ATTR_50_DATA_TYPE,
      ENLIL_ATTR_51_NAME,
      ENLIL_ATTR_51_DATA_TYPE,
      ENLIL_ATTR_52_NAME,
      ENLIL_ATTR_52_DATA_TYPE,
      ENLIL_ATTR_53_NAME,
      ENLIL_ATTR_53_DATA_TYPE,
      ENLIL_ATTR_54_NAME,
      ENLIL_ATTR_54_DATA_TYPE,
      ENLIL_ATTR_58_NAME,
      ENLIL_ATTR_58_DATA_TYPE,
      ENLIL_ATTR_59_NAME,
      ENLIL_ATTR_59_DATA_TYPE,
      ENLIL_ATTR_60_NAME,
      ENLIL_ATTR_60_DATA_TYPE,
      ENLIL_ATTR_61_NAME,
      ENLIL_ATTR_61_DATA_TYPE,
      ENLIL_ATTR_62_NAME,
      ENLIL_ATTR_62_DATA_TYPE,
      ENLIL_ATTR_63_NAME,
      ENLIL_ATTR_63_DATA_TYPE,
      ENLIL_ATTR_64_NAME,
      ENLIL_ATTR_64_DATA_TYPE,
      ENLIL_ATTR_65_NAME,
      ENLIL_ATTR_65_DATA_TYPE,
      ENLIL_ATTR_66_NAME,
      ENLIL_ATTR_66_DATA_TYPE,
      ENLIL_ATTR_67_NAME,
      ENLIL_ATTR_67_DATA_TYPE,
      ENLIL_ATTR_68_NAME,
      ENLIL_ATTR_68_DATA_TYPE,
      ENLIL_ATTR_69_NAME,
      ENLIL_ATTR_69_DATA_TYPE,
      ENLIL_ATTR_70_NAME,
      ENLIL_ATTR_70_DATA_TYPE,
      ENLIL_ATTR_71_NAME,
      ENLIL_ATTR_71_DATA_TYPE,
      ENLIL_ATTR_72_NAME,
      ENLIL_ATTR_72_DATA_TYPE,
      ENLIL_ATTR_73_NAME,
      ENLIL_ATTR_73_DATA_TYPE,
      ENLIL_ATTR_74_NAME,
      ENLIL_ATTR_74_DATA_TYPE,
      ENLIL_ATTR_75_NAME,
      ENLIL_ATTR_75_DATA_TYPE,
      ENLIL_ATTR_76_NAME,
      ENLIL_ATTR_76_DATA_TYPE,
      ENLIL_ATTR_77_NAME,
      ENLIL_ATTR_77_DATA_TYPE,
      ENLIL_ATTR_78_NAME,
      ENLIL_ATTR_78_DATA_TYPE,
      ENLIL_ATTR_82_NAME,
      ENLIL_ATTR_82_DATA_TYPE,
      ENLIL_ATTR_83_NAME,
      ENLIL_ATTR_83_DATA_TYPE,
      ENLIL_ATTR_84_NAME,
      ENLIL_ATTR_84_DATA_TYPE,
      ENLIL_ATTR_85_NAME,
      ENLIL_ATTR_85_DATA_TYPE,
      ENLIL_ATTR_86_NAME,
      ENLIL_ATTR_86_DATA_TYPE,
      ENLIL_ATTR_87_NAME,
      ENLIL_ATTR_87_DATA_TYPE,
      ENLIL_ATTR_88_NAME,
      ENLIL_ATTR_88_DATA_TYPE,
      ENLIL_ATTR_89_NAME,
      ENLIL_ATTR_89_DATA_TYPE,
      ENLIL_ATTR_90_NAME,
      ENLIL_ATTR_90_DATA_TYPE,
      ENLIL_ATTR_91_NAME,
      ENLIL_ATTR_91_DATA_TYPE,
      ENLIL_ATTR_92_NAME,
      ENLIL_ATTR_92_DATA_TYPE,
      ENLIL_ATTR_93_NAME,
      ENLIL_ATTR_93_DATA_TYPE,
      ENLIL_ATTR_94_NAME,
      ENLIL_ATTR_94_DATA_TYPE,
      ENLIL_ATTR_95_NAME,
      ENLIL_ATTR_95_DATA_TYPE,
      ENLIL_ATTR_96_NAME,
      ENLIL_ATTR_96_DATA_TYPE,
      ENLIL_ATTR_97_NAME,
      ENLIL_ATTR_97_DATA_TYPE,
      ENLIL_ATTR_98_NAME,
      ENLIL_ATTR_98_DATA_TYPE,
      ENLIL_ATTR_99_NAME,
      ENLIL_ATTR_99_DATA_TYPE,
      ENLIL_ATTR_100_NAME,
      ENLIL_ATTR_100_DATA_TYPE,
      ENLIL_ATTR_101_NAME,
      ENLIL_ATTR_101_DATA_TYPE,
      ENLIL_ATTR_102_NAME,
      ENLIL_ATTR_102_DATA_TYPE,
      ENLIL_ATTR_106_NAME,
      ENLIL_ATTR_106_DATA_TYPE,
      ENLIL_ATTR_107_NAME,
      ENLIL_ATTR_107_DATA_TYPE,
      ENLIL_ATTR_108_NAME,
      ENLIL_ATTR_108_DATA_TYPE,
      ENLIL_ATTR_109_NAME,
      ENLIL_ATTR_109_DATA_TYPE,
      ENLIL_ATTR_110_NAME,
      ENLIL_ATTR_110_DATA_TYPE,
      ENLIL_ATTR_111_NAME,
      ENLIL_ATTR_111_DATA_TYPE,
      ENLIL_ATTR_112_NAME,
      ENLIL_ATTR_112_DATA_TYPE,
      ENLIL_ATTR_113_NAME,
      ENLIL_ATTR_113_DATA_TYPE,
      ENLIL_ATTR_114_NAME,
      ENLIL_ATTR_114_DATA_TYPE,
      ENLIL_ATTR_115_NAME,
      ENLIL_ATTR_115_DATA_TYPE,
      ENLIL_ATTR_116_NAME,
      ENLIL_ATTR_116_DATA_TYPE,
      ENLIL_ATTR_117_NAME,
      ENLIL_ATTR_117_DATA_TYPE,
      ENLIL_ATTR_118_NAME,
      ENLIL_ATTR_118_DATA_TYPE,
      ENLIL_ATTR_119_NAME,
      ENLIL_ATTR_119_DATA_TYPE,
      ENLIL_ATTR_120_NAME,
      ENLIL_ATTR_110_DATA_TYPE,
      ENLIL_ATTR_121_NAME,
      ENLIL_ATTR_121_DATA_TYPE,
      ENLIL_ATTR_122_NAME,
      ENLIL_ATTR_122_DATA_TYPE,
      ENLIL_ATTR_123_NAME,
      ENLIL_ATTR_123_DATA_TYPE };

#ifndef ENLIL_ATTRIBUTES_H_
#define ENLIL_ATTRIBUTES_H_

#endif /*ENLIL_ATTRIBUTES_H_*/
