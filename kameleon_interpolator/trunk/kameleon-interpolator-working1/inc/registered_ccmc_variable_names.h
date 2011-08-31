/*******************************************************************************
 *                                                                             *
 *    NAME:          registered_ccmc_variable_names.h                          *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Definitions all available ccmc defined variables - A pre  *
 *                   validation test to verify variable names used as input to *
 *                   the open_cdf call for main memory loading requests        *
 *                                                                             *
 *    INPUTS:                                                                  *
 *                                                                             *
 *    OUTPUTS:       used by open_cdf.c                                        *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    11.30.2006     Maddox, Marlo                                             *
 *                   Adding kptv variables: net_flux, total_flux, weights      *
 *                                                                             *
 *    12.07.2006     Maddox, Marlo                                             *
 *                   Adding msfc_tvm variables: b_l, b_t, raz                  *
 *                                                                             *
 *    02.20.2007     Maddox, Marlo                                             *
 *                   Adding mas variables: br,bphi,btheta, ur,uphi,utheta,jr,  *
 *                   jphi,jtheta,                                              *
 *                                                                             *
 *    07.29.2008     Maddox, Marlo                                             *
 *                   Adding eta variable for ucla_ggcm/open_ggcm               *
 *                                                                             *
 *    08.01.2008     Maddox, Marlo                                             *
 *                   Adding new ENLIL r,phi, theta based variable names        *
 *                                                                             *
 ******************************************************************************/

static char *bx_name = "bx", *by_name = "by", *bz_name = "bz";

static char *br_name = "br", *bphi_name = "bphi", *btheta_name = "btheta";

static char *br_1_name = "br_1", *bphi_1_name = "bphi_1", *btheta_1_name = "btheta_1";
static char *br_2_name = "br_2", *bphi_2_name = "bphi_2", *btheta_2_name = "btheta_2";
static char *br_3_name = "br_3", *bphi_3_name = "bphi_3", *btheta_3_name = "btheta_3";
static char *br_4_name = "br_4", *bphi_4_name = "bphi_4", *btheta_4_name = "btheta_4";

static char *b1r_name = "b1r", *b1phi_name = "b1phi", *b1theta_name = "b1theta";

static char *b1x_name = "b1x", *b1y_name = "b1y", *b1z_name = "b1z";

static char *bx1_name = "bx1", *by1_name = "by1", *bz1_name = "bz1";

static char *ux_name = "ux", *uy_name = "uy", *uz_name = "uz";
static char *ur_name = "ur", *uphi_name = "uphi", *utheta_name = "utheta";

static char *ur_1_name = "ur_1", *uphi_1_name = "uphi_1";
static char *ur_2_name = "ur_2", *uphi_2_name = "uphi_2";
static char *ur_3_name = "ur_3", *uphi_3_name = "uphi_3";
static char *ur_4_name = "ur_4", *uphi_4_name = "uphi_4";

static char *bats_status_name = "status";

static char *Vn_x_name = "Vn_x", *Vn_y_name = "Vn_y", *Vn_z_name = "Vn_z";
static char *Vi_x_name = "Vi_x", *Vi_y_name = "Vi_y";

static char *jx_name = "jx", *jy_name = "jy", *jz_name = "jz";
static char *jr_name = "jr", *jphi_name = "jphi", *jtheta_name = "jtheta";

static char *rho_name = "rho", *p_name = "p", *e_name = "e";
static char *temp_name = "temp";

static char *rho_1_name = "rho_1";
static char *rho_2_name = "rho_2";
static char *rho_3_name = "rho_3";
static char *rho_4_name = "rho_4";

static char *N_e_name = "N_e", *N_O_name = "N_O", *N_O2_name = "N_O2",
      *N_N2_name = "N_N2";

static char *Rmt_name = "Rmt", *Tn_name = "Tn";

static char *sigmaP_name = "sigmaP", *sigmaH_name = "sigmaH";

static char *T_name = "T", *dp_name = "dp", *bp_name = "bp";

static char *T_1_name = "T_1";
static char *T_2_name = "T_2";
static char *T_3_name = "T_3";
static char *T_4_name = "T_4";

static char *bp_1_name = "bp_1";
static char *bp_2_name = "bp_2";
static char *bp_3_name = "bp_3";
static char *bp_4_name = "bp_4";

static char *net_flux_name = "net_flux", *total_flux_name = "total_flux",
      *weights_name = "weights";

static char *b_l_name = "b_l", *b_t_name = "b_t", *raz_name = "raz";

static char *eta_name = "eta";

static char *time_1_name = "time_1";
static char *time_2_name = "time_2";
static char *time_3_name = "time_3";
static char *time_4_name = "time_4";

static char *time_step_1_name = "time_step_1";
static char *time_step_2_name = "time_step_2";
static char *time_step_3_name = "time_step_3";
static char *time_step_4_name = "time_step_4";

static char *numerical_step_1_name = "numerical_step_1";
static char *numerical_step_2_name = "numerical_step_2";
static char *numerical_step_3_name = "numerical_step_3";
static char *numerical_step_4_name = "numerical_step_4";

/* MAGIC entries */
static char *x_name = "x";
static char *y_name = "y";
static char *z_name = "z";
static char *time_range_name = "time_range";
static char *longitude_name = "longitude";
static char *latitude_name = "latitude";
static char *b_x_name = "b_x";
static char *b_y_name = "b_y";
static char *b_z_name = "b_z";
static char *v_x_name = "v_x";
static char *v_y_name = "v_y";
static char *v_z_name = "v_z";
static char *b_x_BSpline_Coeff_name = "b_x_BSpline_Coeff";
static char *b_y_BSpline_Coeff_name = "b_y_BSpline_Coeff";
static char *b_z_BSpline_Coeff_name = "b_z_BSpline_Coeff";
static char *v_x_BSpline_Coeff_name = "v_x_BSpline_Coeff";
static char *v_y_BSpline_Coeff_name = "v_y_BSpline_Coeff";
static char *v_z_BSpline_Coeff_name = "v_z_BSpline_Coeff";

/* ADAPT3D entries */
static char *coord_name = "coord";
static char *unkno_name = "unkno";
static char *intmat_name = "intmat";
static char *bconi_name = "bconi";
