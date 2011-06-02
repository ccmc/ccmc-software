/*******************************************************************************
 *                                                                             *
 *       NAME:           read_ctip.h                                           *
 *                                                                             *
 *       VERSION:        1.0                                                   *
 *                                                                             *
 *       AUTHOR:         Lutz Rastaetter                                       *
 *                       CUA at NASA-GSFC-(612.3)                              *
 *                       lr@waipio.gsfc.nasa.gov                               *
 *                       (301) 286 - 1085                                      *
 *                                                                             *
 *       PURPOSE:        CTIP 3D data ingest                                   *
 *                                                                             *
 *       INPUTS:         *                                                     *
 *                                                                             *
 *       OUTPUTS:        *                                                     *
 *                                                                             *
 *       FUNCTIONS:      *                                                     *
 *                                                                             *
 *       VARIABLES:      *                                                     *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       03.23.2005     Rastaetter, Lutz                                       *
 *                      initial development started                            *
 *                                                                             *
 *       08.11.2005     Maddox, Marlo                                          *
 *                      added ctip_NH variable to stoe size of H array         *
 *                                                                             *
 ******************************************************************************/

#define CTIP_MAX_VARIABLE_NAME_STRING    300
#define CTIP_MAX_UNITS_STRING        300
#define CTIP_MAX_HEADER_STRING        300

FILE *ctip_input_filePtr;

int ctip_byte_value;
int ctip_byte_value_2;

int ctip_flip_endian, ctip_is_double;
int ctip_NX, ctip_NY, ctip_NZ, ctip_NH;
int ctip_number_of_cells;

/******************** header file ****************/
char ctip_unit_string[CTIP_MAX_UNITS_STRING];
char ctip_headerline[CTIP_MAX_HEADER_STRING];
int ctip_x_dimension_size;
int ctip_y_dimension_size;
int ctip_z_dimension_size;
char ctip_variable_name_string[CTIP_MAX_VARIABLE_NAME_STRING];

/********************** record 6 ***************/
float *ctip_ip_arrayPtr;
float *ctip_colat_arrayPtr;
float *ctip_lon_arrayPtr;
float CTIP_MISSING;

/* some global attribute values */
#define CTIP_MODEL_DESCRIPTION "Model Description: The Comprehensive Thermosphere-Ionosphere-Plasmasphere (CTIP) model was develped under the leadership of Tim Fuller-Rowell at the National Center for Atmosphereic Research (NCAR). Its plasma and neutral partivle chemistry is defined on a geographic grid in spherical coordiantes with the pressure level (IP) as the vertical coordinate. The altitude grid is Lagrangian and varies with time (height variable can be used instead of IP). Inputs to the CTIP model include the solar wind conditions, solar irradiance (F10.7), a high-latitude electric field model (such as Weimer model), and neutral wind patterns caused by the tilt of Earth's axis during the seasions. Model outputs are written every 72 minutes, corresponding to the Earth's rotation over one longitudinal grid cell (20 per day). Outputs include 3D plasma and neutral composition, velocities, conductivities, 2D conductances, electric potentials, and conditions along field lines in the plasmasphere."

#define CTIP_COORDINATE_DISCRIPTION "CTIP model data comes in spherical coordinates (height or pressure level, co-latitude, longitude). The longitude coordiante is periodic and the height of a given pressure level varies with time and column position in longitude and colatitude"

#define CTIP_DISCIPLINE "ionosphere/thermosphere"

#define CCMC_USE_POLICY "For tracking purposes for our government sponsors, we ask that you notify the CCMC whenever you use CCMC results in a scientific publications or presentations: ccmc@ccmc.gsfc.nasa.gov "
/* variable attributes */

