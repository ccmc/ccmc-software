static char const Ident[] = 
     "@(#) $Id: cxform-manual.c,v 1.1.1.1 2005/02/25 20:41:41 rboller Exp $ ";
/*
** cxform-manual.c  --  manually coded functions for coordinate transforms
**
** This file is part of Ed's "cxform" Coordinate Transformation package.
** It contains the hand-coded functions for converting between some
** coordinate systems.  
**
** All code in here is derived from Mike Hapgood <M.Hapgood@rl.ac.uk>'s
** excellent introduction to coordinate transformations, along with
** Markus Fraenz' and Christopher Russell's work:
**
**	http://sspg1.bnsc.rl.ac.uk/Share/Coordinates/ct_home.htm
**      http://www.space-plasma.qmul.ac.uk/heliocoords/
**      http://www-ssc.igpp.ucla.edu/personnel/russell/papers/gct1.html/
**
** Converted to C by Ed Santiago <esm@pobox.com>
** Modified & maintained by Ryan Boller <Ryan.A.Boller@nasa.gov>
**
** Version history:
**   2000/06/21  v0.2  Ed Santiago:  Last released version from Ed.
**   2003/09/12  v0.3  Ryan Boller:  First modified version from Ryan.  
**                     Added RTN and GSEQ systems, IGRF model, and slightly
**                     different time manipulation.
**   2004/03/19  v0.4  Ryan Boller: Updated Makefile to auto-detect platform and
**                     to build under Mac OS X.  HEEQ system now implemented by
**                     Kristi Keller.
**   2004/05/21  v0.5  Ryan Boller: Corrected small discrepancy when calculating
**				       T0 and lambda0.  Results now match SSCWeb's output very
**					   closely when magnetic lat & lon are forced to their
**                     values (they haven't updated their IGRF coefficients).
**   2004/11/25  v0.6  Ryan Boller: Moved utility functions from main.c into shared
**                     library for convenience;  updated IGRF coefficients to the 9th
**                     generation which adds definitive values for 1995 & 2000
**	 2006/09/24  v0.7  Ryan Boller: Updated IGRF coefficients to 10th generation
** 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include "cxform.h"

#define X 0
#define Y 1
#define Z 2

/* MSVC++ math.h does not include M_PI or M_PI_2 */
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923
#endif


#define DEGREES_TO_RADIANS      (M_PI / 180.0)
#define RADIANS_TO_DEGREES      (180.0 / M_PI)
#define sind(x)                 sin((x) * DEGREES_TO_RADIANS)
#define cosd(x)                 cos((x) * DEGREES_TO_RADIANS)
#define tand(x)                 tan((x) * DEGREES_TO_RADIANS)
#define atand(z)				(atan(z) / DEGREES_TO_RADIANS)
#define atan2d(y,z)				(atan2(y,z) / DEGREES_TO_RADIANS)

#define	SECONDS_PER_CENTURY	(86400 * 365.25 * 100)

#define NUM_IGRF_YEARS_DEFINED 23

/* for debugging */
#define	DUMP_MAT	{ int i,j; for (i=0;i<3;i++) { for (j=0;j<3;j++) printf("%15lf ", mat[i][j]); printf("\n"); }}


/*****************************************************************************\
|*                                                                           *|
|*                          MATRIX OPERATIONS                                *|
|*                                                                           *|
|*  This section provides functions for defining & dealing with matrices.    *|
|*                                                                           *|
\*****************************************************************************/

/******************\
|* mat_times_mat  *|  multiplies two 3x3 matrices together.
\******************/
void mat_times_mat( Mat m1,  Mat m2, Mat m_out)
{
  int i,j;
  Mat m_tmp;

  /*
  ** Do the multiplication, but do so into a _temporary_ holding spot,
  ** in case we have been invoked with m_out == (m1 or m2).
  */
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++) {
      m_tmp[i][j] = m1[i][0]*m2[0][j] + m1[i][1]*m2[1][j] + m1[i][2]*m2[2][j];
    }
  }

  /*
  ** Now write into the caller's space.
  */
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++) {
      m_out[i][j] = m_tmp[i][j];
    }
  }
}


/******************\
|* mat_times_vec  *|  multiplies a 3x3 matrix by a 3-D vector
\******************/
void mat_times_vec(Mat m1, Vec v1, Vec v_out)
{
  int i;
  Vec v_tmp;

  for (i=0; i<3; i++) {
    v_tmp[i] = m1[i][0]*v1[0] + m1[i][1]*v1[1] + m1[i][2]*v1[2];
  }

  for (i=0; i<3; i++) {
    v_out[i] = v_tmp[i];
  }
}


/******************\
|* mat_transpose  *|  returns the transpose of a 3x3 matrix
\******************/
void mat_transpose(Mat m_in, Mat m_out)
{
  int i,j;
  Mat m_tmp;

  /*
  ** Do the transposition, but do so into a _temporary_ holding spot,
  ** in case we have been invoked with m_out == m_in
  */
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++) {
      m_tmp[i][j] = m_in[j][i];
    }
  }

  /*
  ** Now write into the caller's space.
  */
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++) {
      m_out[i][j] = m_tmp[i][j];
    }
  }
}



/*******************\
|* hapgood_matrix  *|  defines a rotation matrix for a given angle & axis
\*******************|
 *
 * Rotation matrices are a special case.  They can be defined by two 
 * parameters: an axis about which to rotate (X, Y, Z) and an angle.
 * Given those two, we can fill in all nine elements of a 3x3 matrix.
 *
 * See http://sspg1.bnsc.rl.ac.uk/Share/Coordinates/matrix.htm
 */
void hapgood_matrix(const double theta, int axis, Mat mat)
{
  int i,j;

  /* 1.calculate sin(zeta) and cos(zeta),  */
  double sin_theta = sind(theta);
  double cos_theta = cosd(theta);

  /* compute the indices for the other two axes (e.g., "X,Z" for Y) */
  int t1 = (axis+1) % 3;
  int t2 = (axis+2) % 3;
  if (t1 > t2) {
    int tmp;
    tmp = t1;
    t1  = t2;
    t2  = tmp;
  }
    

  /*
  ** 4.set the remaining off-diagonal terms to zero.
  */
  for (i=0; i<3; i++)
    for (j=0; j<3; j++)
      mat[i][j] = 0.0;

  /*
  ** 2.determine the matrix diagonal:
  **   1.put 1 in the Nth term, where N=1 if the rotation axis is X, etc
  **   2.put cos(zeta) in the other two terms
  */
  mat[axis][axis] = 1.0;
  mat[t1][t1]     = cos_theta;
  mat[t2][t2]     = cos_theta;

  /*
  ** 3.locate the two off-diagonal terms in the same columns and rows as 
  **   the cos(zeta) terms - put sin(zeta) in the term above the diagonal
  **   and -sin(zeta) in the term below,
  */
  mat[t1][t2]     =  sin_theta;
  mat[t2][t1]     = -sin_theta;
}


/*****************************************************************************\
|*                                                                           *|
|*                          ELEMENTAL FUNCTIONS                              *|
|*                                                                           *|
|*  This section provides functions required by the actual transformations   *|
|*                                                                           *|
\*****************************************************************************/

/*
** H
**
** time, in hours, since preceding UT midnight
*/
double
H(const double et)
{
  double jd    = (et / 86400.0) - 0.5;	/* Convert seconds to days */
  double dfrac = jd - (long)jd;
  double hh    = dfrac * 24.0;

  if (hh < 0.0)
    hh += 24.0;

  /* printf("hh= %15.10lf\n", hh);  */
  return hh;
}

/*
** MJD
**
** Modified Julian Date
*/
double
MJD(const double et)
{
  double jd = (et / 86400.0) - 0.5;

  return jd + 51545.0;
}


/*
** T0
**
** Julian Centuries from a certain time to 1 Jan 2000 12:00
**
** Note that this version takes into account hours, minutes, and seconds
*/
double
T0(const double et)
{
  /*  Old method
  double jd = (et/86400.0)-0.5;
  jd = (long)jd - 0.5;
  return jd / 36525.0;
  */

  /*  Seconds --> days --> centuries  */
  return (et / 86400.0)/36525.0;  
}


/*
** lambda0
**
** The Sun's ecliptic longitude (lambdaO) can be calculated using the 
** series of formulae:
**
**     M = 357.528 + 35999.050T0 + 0.04107H degrees 
**     Lambda = 280.460 + 36000.772T0 + 0.04107H degrees 
**     lambdaO = Lambda + (1.915 - 0.0048T0) sinM + 0.020 sin2M
**
** where T0 is the time in Julian centuries from 12:00 UT on 1 January 2000
** to the midnight Universal Time (UT) preceding the time of interest and 
** H is the time in hours since that preceding UT midnight. Formulae 
** derived from the Almanac for Computers. In the intermediate formulae, 
** M is the Sun's mean anomaly and Lambda its mean longitude.
*/
double
lambda0(const double et)
{
  double M, lambda;

  M      = 357.528 + 35999.050 * T0(et);  /* + 0.04107 * H(et); */
  lambda = 280.460 + 36000.772 * T0(et);  /* + 0.04107 * H(et); */

  /*  printf("lambda0: %10.15lf\n\n", lambda + (1.915 - 0.0048 * T0(et)) * sind(M) + 0.020 * sind(2 * M));  */

  return lambda + (1.915 - 0.0048 * T0(et)) * sind(M) + 0.020 * sind(2 * M);
}


/*
** The obliquity of the ecliptic (epsilon) can be calculated using the formula:
**
**	epsilon = 23.439 - 0.013T0 degrees
**
** where T0 is the time in Julian centuries from 12:00 UT on 1 January 2000
** to the midnight (UT) preceding the time of interest. Formula derived from
** the Almanac for Computers.
*/
double
epsilon(const double et)
{
  return 23.439 - 0.013 * T0(et);
}


/*
** The following functions calculate the first three coefficients of the IGRF,
** used in determining the location of the magnetic dipole.  The values
** are interpolated to the day of interest.
** 
** Inputs:
**   fracYearIndex:  The double-precision "array index" of the appropriate
**                   IGRF year [0, NUM_IGRF_YEARS_DEFINED], corresponding to
**                   the years 1900-2005.  This can be found by converting et to
**                   seconds past 1900, converting to years, and dividing by 5
**                   to get the appropriate array index.  I.e.,
**                   fracYearIndex = (et+3155803200.0) / 157788000.0
**   fracYear:  The remainder of the fracYearIndex [0, 1] for use during
**              interpolation.  
**
** The IGRF/DGRF coefficients are taken from the IGRF2000 (8th gen) model:
**   http://www.ngdc.noaa.gov/IAGA/wg8/igrf2000.html
**
** Note: the coefficients are now updated to the IGRF 9th generation model:
**   http://www.ngdc.noaa.gov/IAGA/vmod/igrf.html
**
** Note: IGRF coefficients are now updated to 10th generation
*/

double calcG01(double fracYearIndex, double fracYear)
{
	static int g01[NUM_IGRF_YEARS_DEFINED] =
		{-31543, -31464, -31354, -31212, -31060, -30926, -30805, -30715,
		 -30654, -30594, -30554, -30500, -30421, -30334, -30220, -30100,
		 -29992, -29873, -29775, -29692, -29619.4, -29556.8, -29512.8};
	
	return (g01[(int)floor(fracYearIndex)]*(1.0-fracYear) + 
		g01[(int)ceil(fracYearIndex)]*fracYear);
}

double calcG11(double fracYearIndex, double fracYear)
{
	static int g11[NUM_IGRF_YEARS_DEFINED] = 
		{-2298, -2298, -2297, -2306, -2317, -2318, -2316, -2306, -2292, -2285,
		 -2250, -2215, -2169, -2119, -2068, -2013, -1956, -1905, -1848, -1784,
		 -1728.2, -1671.8, -1617.8};
	
	return (g11[(int)floor(fracYearIndex)]*(1.0-fracYear) + 
		g11[(int)ceil(fracYearIndex)]*fracYear);
}

double calcH11(double fracYearIndex, double fracYear)
{
	static int h11[NUM_IGRF_YEARS_DEFINED] = 
		{5922, 5909, 5898, 5875, 5845, 5817, 5808, 5812, 5821, 5810, 5815,
		 5820, 5791, 5776, 5737, 5675, 5604, 5500, 5406, 5306, 5186.1, 5080.0, 
		 4973.5};
		 
	return (h11[(int)floor(fracYearIndex)]*(1.0-fracYear) + 
		h11[(int)ceil(fracYearIndex)]*fracYear);
}


/*
** Latitude and longitude of Earth's magnetic pole using above coefficients
*/

double mag_lon(double et)
{
  double fracYearIndex = (et+3155803200.0)/157788000.0;
  double fracYear = fmod(fracYearIndex, 1.0);
  double lambda0, g11, h11;

  if (fracYearIndex >= NUM_IGRF_YEARS_DEFINED)
  {
	/*  fprintf(stderr, "ERROR: Specified year is greater than IGRF implementation.  Exiting.");  */
    exit(EXIT_FAILURE);
  }

  g11 = calcG11(fracYearIndex, fracYear);
  h11 = calcH11(fracYearIndex, fracYear);

  /* lambda0 / longitude  */
  lambda0 = atan2(h11, g11) + M_PI;
  /* lambda0 = (360.0-71.115)*(M_PI/180.0);  // SSC year 1990 value  */
  /* lambda0 = (360.0-71.381)*(M_PI/180.0);  // SSC year 1995 value  */
  /* lambda0 = (360.0-71.647)*(M_PI/180.0);  // SSC year 2000 value  */
  /* printf("lambda: %f\n", 360.0-(lambda*(180.0/M_PI)));  */

  return lambda0;
}


double mag_lat(double et)
{
  double fracYearIndex = (et+3155803200.0)/157788000.0;
  double fracYear = fmod(fracYearIndex, 1.0);
  double phi0, lambda, g01, g11, h11;

  if (fracYearIndex >= NUM_IGRF_YEARS_DEFINED)
  {
    /*  fprintf(stderr, "ERROR: Specified year is greater than IGRF implementation.  Exiting.");  */
    exit(EXIT_FAILURE);
  }
  
  g01 = calcG01(fracYearIndex, fracYear);
  g11 = calcG11(fracYearIndex, fracYear);
  h11 = calcH11(fracYearIndex, fracYear);
  lambda = mag_lon(et);
  
  /*  phi0 / latitude  */
  phi0 = M_PI_2 - atan((g11*cos(lambda) + h11*sin(lambda))/g01);
  /* phi0 = (90.0-10.872)*(M_PI/180.0);   // SSC year 1990 value  */
  /* phi0 = (90.0-10.730)*(M_PI/180.0);   // SSC year 1995 value  */
  /* phi0 = (90.0-10.588)*(M_PI/180.0);   // SSC year 2000 value  */
  /* printf("phi: %f\n", 90.0-(phi*(180.0/M_PI)));  */
  
  return phi0;
}

/*****************************************************************************\
|*                                                                           *|
|*                    TRANSFORMATION MATRICES                                *|
|*                                                                           *|
|* Hapgood defines all his transformations in terms of various matrices T1,  *|
|* T2, ... Tn.  Since they're used in various places throughout the paper,   *|
|* it makes sense to define them as subroutines.                             *|
|*                                                                           *|
|* Comments preceding each function are extracted verbatim from Hapgood.     *|
|*                                                                           *|
\*****************************************************************************/

/*
** The GEI2000 to GEI transformation is given by the matrix 
**
**	P = <-zA,Z>*<thetaA,Y>*<-zetaA,Z>
**
** where the rotation angles zA, thetaA and zetaA are the precession 
** angles. This transformation is a precession correction as described
** by Hapgood (1995).
**
**     zA     = 0.64062 T0 + 0.00030 T0^2 degrees 
**     thetaA = 0.55675 T0 - 0.00012 T0^2 degrees 
**     zetaA  = 0.64062 T0 + 0.00008 T0^2 degrees
*/
void
mat_P(const double et, Mat mat)
{
  Mat mat_tmp;
  double t0 = T0(et);

  hapgood_matrix(-1.0*(0.64062 * t0  +  0.00030 * t0*t0), Z, mat);

  hapgood_matrix(0.55675 * t0  -  0.00012 * t0*t0, Y, mat_tmp);
  mat_times_mat(mat, mat_tmp, mat);

  hapgood_matrix(-1.0*(0.64062 * t0  +  0.00008 * t0*t0), Z, mat_tmp);
  mat_times_mat(mat, mat_tmp, mat);
}


/*
** The GEI to GEO transformation is given by the matrix
**
**	T1 = <theta,Z>
**
** where the rotation angle theta is the Greenwich mean sidereal time. This
** transformation is a rotation in the plane of the Earth's equator from 
** the First Point of Aries to the Greenwich meridian. 
**---
** The Greenwich Mean Sidereal Time (theta) can be calculated using the formula:
**
**  theta = 280.46061837 + 360.98564736629*d0 + 0.0003875*T0^2 - 
**          0.0000000258333*T0^3
**
**  where d0 is Julian days from J2000 and T0 is Julian centuries from J2000.
**  The forumla can be found in:
**    http://www.space-plasma.qmul.ac.uk/heliocoords/systems2art/
**    (Heliospheric Coordinate Systems, M. Franz, D. Harper)
**
**
** GST was formerly calculated as: 
** 
**	theta = 100.461 + 360000.770T0 + 15.04107H degrees
**
** where T0 is the time in Julian centuries from 12:00 UT on 1 January 2000
** to the midnight Universal Time (UT) preceding the time of interest and 
** H is the time in hours since that preceding UT midnight. Formula derived
** from the Almanac for Computers.
*/

void
mat_T1(const double et, Mat mat)
{
  double theta = 100.461 + 36000.770 * T0(et) + 360.0*(H(et)/24.0);  /* + 15.04107 * H(et);  */

  /*
  theta = fmod(theta, 360.0);
  if (theta < 0.0)
    theta += 360.0;
  */
  
  /*  printf("T0= %20.20lf, H= %15.10lf, theta= %15lf\n", T0(et), H(et), theta);  */

  hapgood_matrix(theta, Z, mat);
}


/*
** The GEI to GSE transformation is given by the matrix 
**
**	T2 = <lambdaO,Z>*<epsilon,X>
**
** where the rotation angle lambdaO is the Sun's ecliptic longitude and 
** the angle epsilon is the obliquity of the ecliptic. This transformation 
** is a rotation from the Earth's equator to the plane of the ecliptic
** followed by a rotation in the plane of the ecliptic from the First Point 
** of Aries to the Earth-Sun direction. 
*/
void
mat_T2(const double et, Mat mat)
{
  Mat mat_tmp;

  hapgood_matrix(lambda0(et), Z, mat);

  hapgood_matrix(epsilon(et), X, mat_tmp);
  mat_times_mat(mat, mat_tmp, mat);
}



/*
** vec_Qe
**
** don't ask.
*/
void
vec_Qe(double et, Vec Qe)
{
  double lat = mag_lat(et);
  double lon = mag_lon(et);

  double cos_lat = cos(lat);
  double sin_lat = sin(lat);
  double cos_lon = cos(lon);
  double sin_lon = sin(lon);

  Mat mat_tmp, mat;

  Vec Qg;

  Qg[0] = cos_lat * cos_lon;
  Qg[1] = cos_lat * sin_lon;
  Qg[2] = sin_lat;

  /* printf("lat=%lf  lon=%lf\n", 90.0 - lat, lon);*/

  mat_T2(et, mat);
  mat_T1(et, mat_tmp);
  mat_transpose(mat_tmp, mat_tmp);
  mat_times_mat(mat, mat_tmp, mat);
  mat_times_vec(mat, Qg, Qe);
}


/*
** The GSE to GSM transformation is given by the matrix
**
**	T3 = <-psi,X>
**
** where the rotation angle psi is the the GSE-GSM angle. This
** transformation is a rotation in the GSE YZ plane from the GSE Z axis
** to the GSM Z axis. 
*/
void
mat_T3(const double et, Mat mat)
{
  Vec Qe;
  double psi;

  vec_Qe(et, Qe);

  psi = atan2d(Qe[1], Qe[2]);

  hapgood_matrix(-psi, X, mat);
}


/*
** The GSM to SM transformation is given by the matrix
**	T4 = <- mu,Y>
**
** where the rotation angle mu is the dipole tilt. This transformation 
** is a rotation in the GSM XZ plane from the GSM Z axis to the 
** geomagnetic dipole axis. 
*/
void
mat_T4(const double et, Mat mat)
{
  Vec Qe;
  double mu;

  vec_Qe(et, Qe);

  mu = atan2d(Qe[0], sqrt(Qe[1]*Qe[1] + Qe[2]*Qe[2]));

  hapgood_matrix(-mu, Y, mat);
}


/*
** The GEO to MAG transformation is given by the matrix 
**
**	T5 = <lat-90,Y>*<long,Z>
**
** where the rotation angle lat is the latitude and angle long is the 
** longitude of the geomagnetic pole (as defined by the axis of the 
** dipole component of the geomagnetic field). This transformation is 
** a rotation in the plane of the Earth's equator from the Greenwich 
** meridian to the meridian containing the dipole axis, followed by 
** a rotation in that meridian from the rotation axis to the dipole axis. 
*/
void
mat_T5(const double et, Mat mat)
{
  Mat mat_tmp;
  
  hapgood_matrix((mag_lat(et)*RADIANS_TO_DEGREES) - 90.0, Y, mat);
  hapgood_matrix((mag_lon(et)*RADIANS_TO_DEGREES),      Z, mat_tmp);
  mat_times_mat(mat, mat_tmp, mat);
}

/*
** The GSE to GSEQ transformation is given by the matrix
**
**  T6 = <theta, X>
**
** where theta is the angle between the Y-axes in the two systems. A full
** description can be found at
**  http://www-ssc.igpp.ucla.edu/personnel/russell/papers/gct1.html/#s3.5
**  (Geophysical Coordinate Transformations, C. T. Russell 1971)
*/
void
mat_T6(const double et, Mat mat)
{
	Vec GSE_ES, GEI_ES, thetaD;
	double theta, thetaN, magThetaD;
	Mat matT2;
	
	/*  Get Earth-Sun vector in GEI  */
	GSE_ES[0] = 1.0;
	GSE_ES[1] = 0.0;
	GSE_ES[2] = 0.0;
	
	/*  Convert GSE --> GEI  */
	mat_T2(et, matT2);
	mat_transpose(matT2, matT2);
	mat_times_vec(matT2, GSE_ES, GEI_ES);
		
	/*  Rotation axis of the Sun (GEI): (1.217,- 0.424, 0.897)  */
	thetaN = GEI_ES[0]*(-0.032) + GEI_ES[1]*(-0.112) + GEI_ES[2]*(-0.048);
	thetaD[0] = (-0.424)*GEI_ES[2] - 0.897*GEI_ES[1];
	thetaD[1] = 0.897*GEI_ES[0] - 0.1217*GEI_ES[2];
	thetaD[2] = 0.1217*GEI_ES[1] - (-0.424)*GEI_ES[0];
	magThetaD = sqrt(pow(thetaD[0],2) + pow(thetaD[1], 2) + pow(thetaD[2], 2));
	
	theta = asin(thetaN/magThetaD);
	/*  printf("Theta: %f\n", theta);  */
	
	hapgood_matrix((theta*RADIANS_TO_DEGREES), X, mat);

	/*  TODO: Unknown why transpose is necessary to match previous results  */
	mat_transpose(mat,mat);
}


/**************  Heliocentric transformations ************/


/*
** The HAE to HEE transformation is given by the matrix
**
**	S1 = <lambdaO + 180,Z>
**
** where the rotation angle lambdaO is the Sun's ecliptic longitude.  
** This transformation is a rotation in the plane of the ecliptic from 
** the First Point of Aries to the Sun-Earth direction. 
*/
void
mat_S1(const double et, Mat mat)
{
  hapgood_matrix(lambda0(et) + 180.0, Z, mat);
}


/*
** The HAE to HEEQ transformation is given by the matrix
**
** 	S2 = <theta0,Z>*<i,X>*<Omega,Z>
**
** where the rotation angle theta0 is the the longitude of the Sun's
** central meridian, i is the the inclination of the Sun's equator and
** Omega is the the ecliptic longitude of the ascending node of the Sun's
** equator. This transformation comprises a rotation in the plane of the
** ecliptic from the First Point of Aries to the ascending node of the
** solar equator, then a rotation from the plane of the ecliptic to the 
** plane of the equator and finally a rotation in the plane of the solar
** equator from the ascending node to the central meridian. 
**
** Implemented by Kristi Keller on 2/2/2004
*/
void
mat_S2(const double et, Mat mat)
{
  Mat mat_tmp;
  double Omega = 73.6667+0.013958*(MJD(et)+3242)/365.25;
  double theta0 = atand(cosd(7.25) * tand(lambda0(et)-Omega));
  double angle_1 = lambda0(et)-Omega;
  angle_1=fmod(angle_1,360.0);
  if (angle_1 < 0.0) angle_1+=360.0;
  
  theta0=fmod(theta0,360.0);
  if (theta0 < 0.0) theta0+=360.0;
  if (angle_1 < 180.0) {
      if (theta0 < 180.0) theta0+=180.0;
       }
  if (angle_1 > 180.0) {
      if (theta0 > 180.0) theta0-=180.0;
       }
  hapgood_matrix(theta0, Z, mat);

  hapgood_matrix(7.25, X, mat_tmp);
  mat_times_mat(mat, mat_tmp, mat);

  hapgood_matrix(Omega, Z, mat_tmp);
  mat_times_mat(mat, mat_tmp, mat);
}



/*****************************************************************************\
|*                                                                           *|
|*                    TRANSFORMATIONS BEGIN HERE                             *|
|*                                                                           *|
|*                                                                           *|
\*****************************************************************************/


/*********************\
**  simple_rotation  **  utility function used by all the "twixt" functions
**********************
**
** This is basically what all the "twixt" functions do:
**
**    1) define a rotation matrix
**    2) If doing an inverse transformation, transpose that matrix.
**    3) multiply the rotation matrix by the input vector.
**
** To save all that work in the "twixt" functions, they just call this
** function, passing us a pointer to a function that defines the matrix.
*/
int
simple_rotation(const double et, Vec v_in, Vec v_out, Direction d, void (*m)())
{
  Mat mat;

  /*
  ** Call the user-specified function to get a rotation matrix.
  */
  (m)(et, mat);

  /*
  ** To do the inverse transformation, we use the transposition of the matrix
  */
  if (d == BACK)
    mat_transpose(mat, mat);

  /*
  ** Multiply the rotation matrix by the input vector, and that's it!
  */
  mat_times_vec(mat, v_in, v_out);

  return 0;
}


/*  Bo-ring.  Just call simple_rotation() with the appropriate matrix. */
int
j2000_twixt_gei(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_P);
}

int
gei_twixt_geo(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_T1);
}

int
geo_twixt_mag(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_T5);
}

int
gei_twixt_gse(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_T2);
}

int
gse_twixt_gsm(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_T3);
}

int
gsm_twixt_sm(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_T4);
}

/*
** Define conversion from GSE to Earth-centered RTN
**
** NOTE:  This coordinate system has not been tested and its results should be
** treated as such
*/
int
gse_twixt_rtn(const double et, Vec v_in, Vec v_out, Direction direction)
{  
  	/*  Convert time  */
	double dYear = (et/31557600.0) + 2000.0013689254;
	int iYear = (int)floor(dYear);
	int doy = (int)(fmod(dYear,1.0) * 365.25)+1;

	/*  Longitude of the ascending node for the year  */
	double alpha = 74.3666667 + 0.014*(iYear - 1900);
	
	/*  Angle between fall equinox and alpha  */
	double phi = DEGREES_TO_RADIANS*(alpha - (360.0*(doy-266.0)/365.25));
	
	/*  Rotation angle around GSE X / RTN R to change coordinates:  */
	double delta = atan(tan(DEGREES_TO_RADIANS*7.25)*cos(phi));
	
	/*  Rotation is the same, regardless of direction  */
	v_out[0] = -v_in[0];
	v_out[1] = (-v_in[1])*cos(delta) + v_in[2]*sin(delta);
	v_out[2] = v_in[1]*sin(delta) + v_in[2]*cos(delta);
  
  	return 0;
}


int
gse_twixt_gseq(const double et, Vec v_in, Vec v_out, Direction direction)
{
	return simple_rotation(et, v_in, v_out, direction, &mat_T6);
}




/*****************************************************************************\
|*              HELIOCENTRIC COORDINATE SYSTEMS                              *|
\*****************************************************************************/

/*
** Hapgood defines a transformation between GSE and HEE in his 1992
** paper (section 6), but this part isn't online.  
**
** The gist of it is, we rotate 180 degrees about Z, and then translate
** along X.
**
** But we also need to add "R", a constant vector defined by
**
**      R = [ Rsun, 0, 0 ]
**
** where 
**
**             r0 (1 - e^2)
**    Rsun =   ------------
**             1 + e cos(v)
**
**   r0 = 1.495985E8 km        	mean distance of the Sun from Earth.
**
**    e = 0.016709 - 0.0000418T0	eccentricity of the Sun's apparent
**					orbit around the Earth.
**
**    w = 282.94 + 1.72 T0		longitude of perigee of that orbit
**
**    v = lambda0 - w			(see lambda0 above)
**
**
** Implemented by Ed Santiago, Updated by Kristi Keller
*/
int
gse_twixt_hee(const double et, Vec v_in, Vec v_out, Direction direction)
{
  Mat mat;
  double r0,e, w,v, Rsun;
  hapgood_matrix(180, Z, mat);

  /*
  ** Note that there's no transposition here if the direction is "back";
  ** the operation works identically in both directions.
  */
  mat_times_vec(mat, v_in, v_out);

  /* Translate the X axis about the earth-sun distance */
  r0 = (double)1.495985e8;  
  e = 0.016709 - 0.0000418*T0(et);
  w = 282.94 + 1.72*T0(et);
  v = lambda0(et) - w; 
  Rsun = r0*(1-e*e)/(1.+e*cosd(v)); 
  /*  v_out[0] += (double)1.5e8;  */
  
  v_out[0] += Rsun;

  return 0;
}


int
hae_twixt_hee(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_S1);
}

int
hae_twixt_heeq(const double et, Vec v_in, Vec v_out, Direction direction)
{
  return simple_rotation(et, v_in, v_out, direction, &mat_S2);
}



/*****************************************************************************\
|*              UTILITY FUNCTIONS                                            *|
\*****************************************************************************/

/* gregorian_calendar_to_jd
**		Converts standard YYYY/MM/DD dates into its corresponding Julian day
**
** Written by Robin O'Leary  (http://pdc.ro.nu/jd-code.html)
** Modified by Ryan Boller to add hour, minute, second precision
*/
double gregorian_calendar_to_jd(int y, int m, int d, int h, int mi, int s)
{
	y+=8000;
	if(m<3) { y--; m+=12; }
	return (y*365) +(y/4) -(y/100) +(y/400) -1200820
	          +(m*153+3)/5-92
	          +d-1
	          +h/24.0
	          +mi/1440.0
	          +s/86400.0
	          -0.5  /* RAB */
	;
}


/* cxRound
**		Rounds a double to a long.  Built-in round function not present on
**		older compilers, so this one needs to be here.
**
** Modification History:
** 		2004/03/19: Ryan Boller - initial version
*/
long cxRound(double doub)
{
	if (doub < 0.0)  return (long)(doub - 0.5);
	else             return (long)(doub + 0.5);
}


/* date2es
** 		Converts a standard Gregorian date and UT (YYYY/MM/DD  HH:MM:SS) to 
** 		ephemeris seconds past J2000, as required by CXFORM
**
** Modification History:
** 		2003/11/11: Ryan Boller - initial version
*/
long date2es(int yyyy, int mm, int dd, int hh, int mm2, int ss)
{
	return cxRound((gregorian_calendar_to_jd(yyyy, mm, dd, hh, mm2, ss) - 2451545)
			* 86400);
}

