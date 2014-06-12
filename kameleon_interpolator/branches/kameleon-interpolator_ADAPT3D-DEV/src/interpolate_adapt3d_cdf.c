#define LINEAR_INTERPOL
/*******************************************************************************
 *                                                                             *
 *    NAME:          interpolate_adapt3d_cdf.c                                 *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Peter MacNeice                                            *
 *                   based on template written by                              *
 *                   Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *
 *
 *
 *
 * UNDER CONSTRUCTION  2/24/2010   -pmn
 *
 *
 *
 *
 *
 *                                                                             *
 *    PURPOSE:       Retrun an interpolated variable value for any given       *
 *                   position in the simulation box.  Requires a file to have  *
 *                   alreday been opened with open_cdf call                    *
 *                                                                             *
 *    INPUTS:        (1)  variable name (2-4) x,y,z posotion (5)mem flag       *
 *                   (6)opt arg flag                                           *
 *                                                                             *
 *    OUTPUTS:       unit varibale attribute value                             *
 *                                                                             *
 *    FUNCTIONS:     *                                                         *
 *                                                                             *
 *    VARIABLES:     *                                                         *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    08.02.2006     Maddox, Marlo     Initial Development Started             *
 *                                                                             *
 *    10.05.2006     Rastaetter, Lutz  Periodic boundary conditions in "z"     *
 *                                     (longitude) valid data is returned for  *
 *                                     positions within one grid spacing beyond*
 *                                     longitude array                         *
 * *  10.05.2006    Maddox, Marlo      Adding a main_memory_flag check/overide *
 *                                     control. Function wil call              *
 *                                     is_var_in_memory() and set mem_flag     *
 *                                     based on return value.                  *
 *                                                                             *
 *    11.09.2006    Maddox, Marlo      Changed return value from float to     *
 *                                     float                                   *
 *                                                                             *
 *    02.22.2007    Maddox, Marlo      Added interpolate_enlil_cdf_2 that      *
 *                                     accepts float arguments. Not fully      *
 *                                     committed to this as default yet        *
 *                                                                             *
 *    02.05.2008  Maddox, Marlo        Changing optional arguments in          *
 *                                     interpolate_enlil_cdf_2 to floats...    *
 *                                                                             *
 *    02.06.2008  Maddox, Marlo        Changed the interpolate_in_block dx, dy *
 *                                     dz arguments from float to float and in*
 *                                     the interpolate_enlil_cdf() routine     *
 *                                     we make sure to copy our d*_float values*
 *                                     returned from interpolate in block into *
 *                                     the user defined float pointer space   *
 *                                                                             *
 *    08.04.2008  Maddox, Marlo        Changing interpolate_enlil_cdf() to use *
 *                                     floats just like the                    *
 *                                     interpolate_enlil_cdf()
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "adapt3d_cdf_interface_variables.h"
/* #include "cdf.h" */

#define MISSING        -256.*256.*256.*256.*256.
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9
#define DEBUG_FLAG    0

float interpolate_adapt3d_cdf(
      char *variable,
      float X_float_input,
      float Y_float_input,
      float Z_float_input,
      int main_memory_flag,
      int optional_argument_flag,
      ...)
{

   float rsun_in_meters = 7.0e8;

   float coord1[3];
   float unkno_local[NVARS_ADAPT3D];

   long counts[1] = { 0 };
   long intervals[1] = { 1 };

   float interpolated_value;
   float X = (float)X_float_input, Y= (float)Y_float_input, Z= (float)Z_float_input;

   int   intmat_in_main_memory_flag;
   int   coord_in_main_memory_flag;
   int   unkno_in_main_memory_flag;

   long cdf_varNum;

   float  *var_arrayPtr;
   int     array_size;
   int     istatus;
   int     ielem, unkno_index;

   char variable_name0[] = "intmat";
   char variable_name1[] = "coord";

   float missing = MISSING;


   void interpolate_adapt3d_solution(float *coord1,int ielem, float unkno_local[NVARS_ADAPT3D]);
   extern int is_var_in_memory( char *);

   /** lets see if required variables are in memory **/
   unkno_in_main_memory_flag  = is_var_in_memory("unkno");
   intmat_in_main_memory_flag = is_var_in_memory("intmat");
   coord_in_main_memory_flag  = is_var_in_memory("coord");

   /****     since the cdf data is stored in r[meters], radians, radians or r, phi theta
    *         but we are accepting input as r[AU], lon, lat - we must do some coordiante transformations
    */

   /* convert rsun to meters */
/*
   coord1[0] = X * rsun_in_meters;
   coord1[1] = Y * rsun_in_meters;
   coord1[2] = Z * rsun_in_meters;
*/
   coord1[0] = X;
   coord1[1] = Y;
   coord1[2] = Z;



   counts[0] = 0; /*reset values after once through */
   intervals[0] = 1;


   /* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */
   /* also select appropriate position arrays x, y , z for bx1, by1,bz1 */

   /* set default grid arrays and change if neccessary */


   char *filename="adapt3d_kameleon_soln.cdf";


   if ( unstructured_grid_setup_done != 1 ) {

       
#ifdef DEBUG
       printf("Calling setup_search_unstructured_grid \n");
#endif
       istatus=setup_search_unstructured_grid();
#ifdef DEBUG
       printf("Exited setup_search_unstructured_grid: istatus=%d \n",istatus);
#endif


#ifdef DEBUG
       printf("\nCalling  smart_search_setup \n");
#endif
       smart_search_setup();
#ifdef DEBUG
       printf("Exited smart_search_setup \n");
#endif

       last_element_found = -1;
       unstructured_grid_setup_done=1;
   }

   if ( !strcmp(
         variable,
         "bx") || !strcmp(
         variable,
         "b1") )
   {
      cdf_varNum = bx_cdf_varNum;
      unkno_index=5;
   }
   else if ( !strcmp(
         variable,
         "by") || !strcmp(
         variable,
         "b2") )
   {
      cdf_varNum = by_cdf_varNum;
      unkno_index=6;
   }
   else if ( !strcmp(
         variable,
         "bz") || !strcmp(
         variable,
         "b3"))
   {
      cdf_varNum = bz_cdf_varNum;
      unkno_index=7;
   }
   else if ( !strcmp(
         variable,
         "ux") || !strcmp(
         variable,
         "u1"))
   {
      cdf_varNum = ux_cdf_varNum;
      unkno_index=1;
   }
   else if ( !strcmp(
         variable,
         "uy") || !strcmp(
         variable,
         "u2"))
   {
      cdf_varNum = uy_cdf_varNum;
      unkno_index=2;
   }
   else if ( !strcmp(
         variable,
         "uz") || !strcmp(
         variable,
         "u3"))
   {
      cdf_varNum = uz_cdf_varNum;
      unkno_index=3;
   }
   else if ( !strcmp(
         variable,
         "rho") )
   {
      cdf_varNum = rho_cdf_varNum;
      unkno_index=0;
   }
   else if ( !strcmp(
         variable,
         "p") || !strcmp(
         variable,
         "e"))
   {
      cdf_varNum = p_cdf_varNum;
      unkno_index=4;
   }
   else if ( !strcmp(
         variable,
         "temp") )
   {
      cdf_varNum = temp_cdf_varNum;
      unkno_index=8;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for ADAPT3D :\n bx OR b1 \n by OR b2 \n bz OR b3\n ux OR u1 \n uy OR u2 \n uz OR u3 \n p OR e \n OR temp\n----------------------------------------------\n"
            );
      return 0;
   }


/* if unkno is not in main memory then fetch it now */

   if ( !unkno_in_main_memory_flag)
   {
     var_arrayPtr = var_get(filename, variable, &array_size);
    }
   if ( !intmat_in_main_memory_flag)
   {
     var_arrayPtr = var_get(filename, variable_name0, &array_size);
    }
   if ( !coord_in_main_memory_flag)
   {
     var_arrayPtr = var_get(filename, variable_name1, &array_size);
    }


/* locate the grid element that contains the point coord1 */

       ielem = smart_search(coord1);

       interpolated_value=999.5;     /* test value */

       if(ielem > -1) {
         interpolate_adapt3d_solution(coord1, ielem, unkno_local);
#ifdef DEBUG
       printf("interpolate_adapt3d_cdf: unkno_local %e %e %e  %e %e %e  %e %e %e \n", unkno_local[0],unkno_local[1],unkno_local[2],unkno_local[3],unkno_local[4],
                          unkno_local[5],unkno_local[6],unkno_local[7],unkno_local[8]);
#endif
         interpolated_value = unkno_local[unkno_index];
         last_element_found = ielem;
       } else {
         printf("Failed to find point in grid\n");
         last_element_found = -1;
       }


      /*  return interpolated_value  */

      if (interpolated_value < MIN_RANGE || interpolated_value > MAX_RANGE)
      {
         return (float)missing;
      }
      else
      {
         return (float)interpolated_value;
      }

}


void interpolate_adapt3d_solution(float *coord1,int ielem, float unkno_local[NVARS_ADAPT3D])
{
/*
 * Interpolate values of unkno to position coord in element ielem
*/


       int ipa,ipb,ipc,ipd;
       int iv;
       float x1,y1,z1;
       float x2,y2,z2;
       float x3,y3,z3;
       float x4,y4,z4;
       float vol,vol6;
       float a1,b1,c1,d1;
       float a2,b2,c2,d2;
       float a3,b3,c3,d3;
       float a4,b4,c4,d4;
       float f1,f2,f3,f4;
       float x,y,z;

       ipa = intmat[ index_2d_to_1d(ielem,0,nelem,4) ]-1;
       ipb = intmat[ index_2d_to_1d(ielem,1,nelem,4) ]-1;
       ipc = intmat[ index_2d_to_1d(ielem,2,nelem,4) ]-1;
       ipd = intmat[ index_2d_to_1d(ielem,3,nelem,4) ]-1;

       x1 = coord[ index_2d_to_1d(0,ipa,0,npoin) ];
       y1 = coord[ index_2d_to_1d(1,ipa,0,npoin) ];
       z1 = coord[ index_2d_to_1d(2,ipa,0,npoin) ];
       x2 = coord[ index_2d_to_1d(0,ipb,0,npoin) ];
       y2 = coord[ index_2d_to_1d(1,ipb,0,npoin) ];
       z2 = coord[ index_2d_to_1d(2,ipb,0,npoin) ];
       x3 = coord[ index_2d_to_1d(0,ipc,0,npoin) ];
       y3 = coord[ index_2d_to_1d(1,ipc,0,npoin) ];
       z3 = coord[ index_2d_to_1d(2,ipc,0,npoin) ];
       x4 = coord[ index_2d_to_1d(0,ipd,0,npoin) ];
       y4 = coord[ index_2d_to_1d(1,ipd,0,npoin) ];
       z4 = coord[ index_2d_to_1d(2,ipd,0,npoin) ];

       x = coord1[0];
       y = coord1[1];
       z = coord1[2];

#ifdef TEST_CASE1
       x1=0.;
       y1=0.;
       z1=0.;
       x2=1.;
       y2=0.;
       z2=0.;
       x3=0.;
       y3=2.;
       z3=0.;
       x4=0.;
       y4=0.;
       z4=3.;
#endif

#ifdef LINEAR_INTERPOL
       a1 = x2*(y3*z4-z3*y4)+y2*(z3*x4-z4*x3)+z2*(x3*y4-x4*y3);
       b1 = - ( y3*z4-z3*y4 + y2*(z3-z4) + z2*(y4-y3) );
       c1 = - ( x2*(z4-z3) + (z3*x4-z4*x3) + z2*(x3-x4) );
       d1 = - ( x2*(y3-y4) + y2*(x4-x3) + (x3*y4-y3*x4) );

       a2 = x3*(y4*z1-z4*y1)+y3*(z4*x1-z1*x4)+z3*(x4*y1-x1*y4);
       b2 = - ( y4*z1-z4*y1 + y3*(z4-z1) + z3*(y1-y4) );
       c2 = - ( x3*(z1-z4) + (z4*x1-z1*x4) + z3*(x4-x1) );
       d2 = - ( x3*(y4-y1) + y3*(x1-x4) + (x4*y1-y4*x1) );

       a3 = x4*(y1*z2-z1*y2)+y4*(z1*x2-z2*x1)+z4*(x1*y2-x2*y1);
       b3 = - ( y1*z2-z1*y2 + y4*(z1-z2) + z4*(y2-y1) );
       c3 = - ( x4*(z2-z1) + (z1*x2-z2*x1) + z4*(x1-x2) );
       d3 = - ( x4*(y1-y2) + y4*(x2-x1) + (x1*y2-y1*x2) );

       a4 = x1*(y2*z3-z2*y3)+y1*(z2*x3-z3*x2)+z1*(x2*y3-x3*y2);
       b4 = - ( y2*z3-z2*y3 + y1*(z2-z3) + z1*(y3-y2) );
       c4 = - ( x1*(z3-z2) + (z2*x3-z3*x2) + z1*(x2-x3) );
       d4 = - ( x1*(y2-y3) + y1*(x3-x2) + (x2*y3-y2*x3) );

       vol6 = a1 + x1*b1 + y1*c1 + z1*d1;
       vol  = vol6/6.;

#ifdef TEST_CASE1
       printf("Volume = %e\n",vol);
       printf("Correct Volume should be 1.0\n");
#endif
       
       for ( iv=0; iv<NVARS_ADAPT3D; iv++) {

         unkno_local[iv] = 0.;
         f1 =  (a1 + b1*x + c1*y + d1*z)/vol6;
         f2 = -(a2 + b2*x + c2*y + d2*z)/vol6;
         f3 =  (a3 + b3*x + c3*y + d3*z)/vol6;
         f4 = -(a4 + b4*x + c4*y + d4*z)/vol6;

         unkno_local[iv] = f1*unkno[ index_2d_to_1d(ipa,iv,npoin,NVARS_ADAPT3D) ]+f2*unkno[ index_2d_to_1d(ipb,iv,npoin,NVARS_ADAPT3D) ]
                          +f3*unkno[ index_2d_to_1d(ipc,iv,npoin,NVARS_ADAPT3D) ]+f4*unkno[ index_2d_to_1d(ipd,iv,npoin,NVARS_ADAPT3D) ] ;

       }
#endif 


/*       end subroutine interpolate_solution  */

}


