/*******************************************************************************
 *                                                                             *
 *      NAME:       read_msfc_tvm.h                                            *
 *                                                                             *
 *      VERSION:    1.0                                                        *
 *                                                                             *
 *      AUTHOR:     Marlo Maddox                                               *
 *                  NASA/GSFC/AETD/ADMA/CCMC                                   *
 *                  Marlo.Maddox@nasa.gov                                      *
 *                  (301) 286 - 5202                                           *
 *                                                                             *
 *      PURPOSE:    Marshall Space Flight Center Tower Vector Magnetograph     *
 *                  Telescope                                                  *
 *                  Magnetogram Data Ingestion                                 *
 *                                                                             *
 *      INPUTS:     *                                                          *
 *                                                                             *
 *      OUTPUTS:    *                                                          *
 *                                                                             *
 *      FUNCTIONS:  *                                                          *
 *                                                                             *
 *      VARIABLES:  *                                                          *
 *                                                                             *
 *                                                                             *
 *      MODIFICATION                                                           *
 *      HISTORY:                                                               *
 *                                                                             *
 *      2006 December 4th   Maddox, Marlo                                      *
 *                          initial development started                        *
 *                                                                             *
 *                          NOTES: aux files hardcoded for quick AGU prototype *
 *                          remember to change                                 *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG_FLAG 1

#include "read_msfc_tvm.h"

#ifdef FITS
#include "fitsio.h"
#endif

int read_msfc_tvm(char *input_file_name, char *aux_file, int verbose_flag,
      int minmax_flag)
{

   int status = 0;

   /* If the Compiler macro FITS is not defined, that means the makefile
    * determined that libcfitsio.a was not available.  Instead of shutting
    * down the entire converter Lets just disable any read routines that
    * depend on the FITS library */

#ifdef FITS

   extern int linear_minmax_search(float *, int, float *, float *);

   /************* hard coded aux files for quick agu prototype...
    * add file name parsing later ********/

   fitsfile *fits_filePtr, *fits_filePtr_aux1, *fits_filePtr_aux2;
   char *input_file_name_aux1 = "t2003_0929_142335v.01.bt.fits",
         *input_file_name_aux2="t2003_0929_142335v.50.raz.fits";

   char header_buffer[FLEN_CARD ];

   float sin_phi, cos_phi, sun_angle_in_radians;
   double degrees_in_radians = 0.0174532925;
   double pi = 3.14159265;
   double radian_in_degrees = 57.2957795;
   double arcmin_in_radians = 0.00029;
   double arcmin_at_solar_dist_in_km = 43000.0;
   double solar_radius_in_km = 700000.0;
   double number_of_arc_min = 7.0;
   double pixel_size; /*** = number_of_arc_min / naxis1 ***/
   double pixel_length; /*** = pixel_size * 43,000km **/
   double coefficient; /** = pixel_length / solar_radius_in_km **/

   int count =0, more=1, prompt;

   /* use the wildcard just in case they fixx the typo SUNAGNLE to SUNANGLE */
   char *sun_angle_keyname = "SUN*LE";

   int i, j;
   int number_of_fits_header_keys;
   int number_of_fits_hdus;
   int hdu_type;
   int bit_pix;
   long naxes[10];
   int maxdim = 10;
   long fpixel[10];
   float missing_val = 256*256*256;
   int anynul;

   /** open fits file **/

   fits_open_file( &fits_filePtr, input_file_name, READONLY, &status);
   fits_open_file( &fits_filePtr_aux1, input_file_name_aux1, READONLY, &status);
   fits_open_file( &fits_filePtr_aux2, input_file_name_aux2, READONLY, &status);

   /** get number_of_fits_header_keys **/

   fits_get_hdrspace(fits_filePtr, &number_of_fits_header_keys, NULL, &status);

   /** get the number of hdu's in the current file **/

   fits_get_num_hdus(fits_filePtr, &number_of_fits_hdus, &status);

   printf("FITS File: %s has %d hdu's\n", input_file_name, number_of_fits_hdus);

   /** for every hdu **/

   for (i=0; i<number_of_fits_hdus; i++)
   {
      /** select hdu by number **/

      fits_movabs_hdu(fits_filePtr, number_of_fits_hdus, &hdu_type, &status);

      /** there are three type of hdu's
       * IMAGE_HDU
       * ACSII_TBL
       * BINARY_TBL
       * ********************************/

      if (hdu_type == IMAGE_HDU)
      {
         printf("Current HDU Type is IMAGE\n");

         /*** get general info about the current image file ***/

         fits_get_img_param(fits_filePtr, maxdim, &bit_pix, &naxis, naxes,
               &status);

         printf("bitpix = %d\nnaxis = %d\n", bit_pix, naxis);

         /*** get a specific KEYWORD value from header -
          * SUNAGNLE <------ actually spelled wrong in the header file */

         fits_read_key(fits_filePtr, TFLOAT, sun_angle_keyname, &sun_angle,
         NULL, &status);

         printf("SUNANGLE keyword value = %f\n", sun_angle);

         for (i = 0; i<naxis; i++)
            printf("naxis[%d] = %ld\n", i, naxes[i]);

         naxis1 = naxes[0];
         naxis2 = naxes[1];

         number_of_elements = naxis1 * naxis2;

         /*******************************************************************
          *                                                                 *
          * LOCAL BLOCK TO DECLARE STATIC ARRAYS BASED ON HEADER INPUT      *
          *                                                                 *
          * ****************************************************************/

         if (number_of_elements > 0)
         {

            float x_2d[naxis2][naxis1];
            float y_2d[naxis2][naxis1];
            float theta_2d[naxis2][naxis1];
            float phi_2d[naxis2][naxis1];
            float b_l_2d_static[naxis2][naxis1];
            float b_t_2d_static[naxis2][naxis1];
            float raz_2d_static[naxis2][naxis1];
            float *longitude_2, *latitude_2;
            float *longitude_3, *latitude_3;

            float x_2d_rev[naxis1][naxis2];
            float y_2d_rev[naxis1][naxis2];
            float theta_2d_rev[naxis1][naxis2];
            float phi_2d_rev[naxis1][naxis2];
            float b_l_2d_static_rev[naxis1][naxis2];
            float b_t_2d_static_rev[naxis1][naxis2];
            float raz_2d_static_rev[naxis1][naxis2];

            /** dynamically allocate space for the image grid **/

            longitude = malloc(naxis1 * sizeof(float));
            latitude = malloc(naxis2 * sizeof(float));
            longitude_2 = malloc(naxis1 * sizeof(float));
            latitude_2 = malloc(naxis2 * sizeof(float));
            longitude_3 = malloc(naxis1 * sizeof(float));
            latitude_3 = malloc(naxis2 * sizeof(float));
            theta = malloc(naxis2 * sizeof(float));
            phi = malloc(naxis1 * sizeof(float));

            /*** dynamically allocate space for flat 2d grid rad & deg **/

            theta_rad = malloc(number_of_elements * sizeof(float));
            theta_deg = malloc(number_of_elements * sizeof(float));
            phi_rad = malloc(number_of_elements * sizeof(float));
            phi_deg = malloc(number_of_elements * sizeof(float));

            /** dynamically allocate space for the flat variable arrays **/

            b_l = malloc(number_of_elements * sizeof(float));
            b_t = malloc(number_of_elements * sizeof(float));
            raz = malloc(number_of_elements * sizeof(float));

            /*********** process coordinates using supplied formula...
             *
             */
            sun_angle_in_radians = sun_angle * degrees_in_radians;
            sin_phi = sinf(sun_angle_in_radians);
            cos_phi = cosf(sun_angle_in_radians);

            pixel_size = number_of_arc_min / ( float )naxis1;

            printf("pixel_size = %f / %f = %f\n", number_of_arc_min,
                  ( float )naxis1, pixel_size);
            pixel_length = (pixel_size * arcmin_at_solar_dist_in_km );
            coefficient = pixel_length / solar_radius_in_km;

            printf("pixel_length = %f * %f = %f\n", pixel_size,
                  arcmin_at_solar_dist_in_km, pixel_length);
            printf("coefficient = %f / %f = %f\n", pixel_length,
                  solar_radius_in_km, coefficient);

            if ( DEBUG_FLAG )
               printf(
                     "DEBUG\tsun_angle_in_radians = %f\nsin_phi = %f\ncos_phi = %f\n",
                     sun_angle_in_radians, sin_phi, cos_phi);

            i=0;

            printf("i = %d\ni + 1.0 = %f\n", i, i+1.0);

            /*** build new grid ***/

            for (i=0; i<naxis1; i++)
            {
               for (j=0; j<naxis2; j++)
               {

                  x_2d_rev[i][j] = cos_phi * ( (float )i + 1.0 ) + sin_phi
                        * ( (float )j + 1.0 );
                  y_2d_rev[i][j] = ( -1.0 * (sin_phi * ( (float )i + 1.0 ) ) )
                        + cos_phi * ( (float )j + 1.0 );

                  theta_2d_rev[i][j] = asinf(y_2d_rev[i][j] * coefficient);
                  phi_2d_rev[i][j] = asinf( (coefficient ) * (x_2d_rev[i][j]
                        / cosf(theta_2d_rev[i][j]) ));
                  /*
                   printf("x[%d][%d]\t=%f\n", i,j, x_2d_rev[i][j] );
                   printf("y[%d][%d]\t=%f\n\n", i,j, y_2d_rev[i][j] );

                   printf("theta[%d][%d] = %f\n",i,j, theta_2d_rev[i][j] );
                   printf("phi[%d][%d] = %f\n\n", i,j, phi_2d_rev[i][j] );
                   */

               }
            }

            /*** test first328 phi values ***/

            for (i = 0; i<naxis1; i++)
            {
               printf("phi_2d_rev[%d][%d] = %f\n", i, 0, phi_2d_rev[i][0]);
               /*    printf("phi_2d_rev[%d][%d] = %f\n", 0,i, phi_2d_rev[0][i] ); */

            }

            /*** test first258 theta values ***/

            for (i = 0; i<naxis2; i++)
            {
               printf("theta_2d_rev[%d][%d] = %f\n", i, 0, theta_2d_rev[i][0]);
               /*printf("theta_2d_rev[%d][%d] = %f\n", 0,i, theta_2d_rev[0][i] ); */

            }

            printf("\n\n\n____________________________ GRID SHOULD BE BUILT BY NOW ____________________\n\n\n");

            /*** read in the image data ***/

            fpixel[0] = fpixel[1] = fpixel[2]= 1; /** this is the starting point for the read - slice one - net flux **/

            printf(
                  "\nfpixel[0] = %ld\nfpixel[1] = %ld\nfpixel[2] = %ld\nnumber_of_elements = %ld\n\n",
                  fpixel[0], fpixel[1], fpixel[2], number_of_elements);

            /******** read default fits_filePtr along with aux_1 and aux_2- hard coded as b_t and raz respectiveley ***/
            /*
             *
             */

            fits_read_pix(fits_filePtr, TFLOAT, fpixel, number_of_elements,
                  &missing_val, b_l_2d_static, &anynul, &status);
            fits_read_pix(fits_filePtr_aux1, TFLOAT, fpixel,
                  number_of_elements, &missing_val, b_t_2d_static, &anynul,
                  &status);
            fits_read_pix(fits_filePtr_aux2, TFLOAT, fpixel,
                  number_of_elements, &missing_val, raz_2d_static, &anynul,
                  &status);
            /*
             *
             *
             *
             */

            /**** check contents of b_l_2d_static ****/
            count=0;
            for (i=0; i<naxis2; i++)
            {
               for (j=0; j<naxis1; j++)
               {
                  /* good 12.8.06 @ 1AM
                   printf("b_l_2d_static[%d][%d] = %f\n", i,j, b_l_2d_static[i][j] );

                   count++;

                   if( count==more)
                   {    printf("Continue how may iterations? ");
                   scanf("%d", &more );
                   if( count == 0 ) exit(0);
                   count=0;
                   }
                   */
               }
            }

            /** convert all to c style arrays **/

            for (i=0; i<naxis1; i++)
            {
               for (j=0; j<naxis2; j++)
               {

                  x_2d[j][i] = x_2d_rev[i][j];
                  y_2d[j][i] = y_2d_rev[i][j];

                  theta_2d[j][i] = theta_2d_rev[i][j];
                  phi_2d[j][i] = phi_2d_rev[i][j];

               }
            }

            /** flatten out array **/

            for (i=0; i<naxis2; i++)
            {
               for (j=0; j<naxis1; j++)
               {
                  b_l[ j + naxis1 * i] = b_l_2d_static[i][j];
                  b_t[ j + naxis1 * i] = b_t_2d_static[i][j];
                  raz[ j + naxis1 * i] = raz_2d_static[i][j];

                  /*
                   b_l[ i * naxis1 + j] = b_l_2d_static[i][j];
                   b_t[ i * naxis1 + j] = b_t_2d_static[i][j];
                   raz[ i * naxis1 + j] = raz_2d_static[i][j];
                   */

                  phi_rad[ j + naxis1 * i] = phi_2d[i][j];
                  phi_deg[ j + naxis1 * i] = phi_2d[i][j] * radian_in_degrees;
                  /*
                   phi_rad[ i * naxis1 + j] = phi_2d_rev[i][j];
                   phi_deg[ i * naxis1 + j] = phi_2d_rev[i][j]* radian_in_degrees;
                   */

                  theta_rad[j + naxis1 * i] = theta_2d[i][j];
                  theta_deg[ j + naxis1 * i] = theta_2d[i][j]
                        * radian_in_degrees;

                  /*
                   theta_rad[ i * naxis1 + j] = theta_2d_rev[i][j];
                   theta_deg[ i * naxis1 + j] = theta_2d_rev[i][j]* radian_in_degrees;
                   */

                  theta[i] = theta_2d_rev[i][0];
                  phi[j] = phi_2d_rev[j][0];

               }
            }

            /**** check contents of flat b_l array vs b_l_2d_static ****/
            count=0;
            for (i=0; i<naxis2; i++)
            {
               for (j=0; j<naxis1; j++)
               {
                  /** good 12.8.06 @ 1AM
                   printf("b_l_2d_static[%d][%d] = %f\n", i,j, b_l_2d_static[i][j] );
                   printf("b_l FLAT[%d][%d] = %f\n", i,j, b_l[i * naxis1 + j] );

                   count++;

                   if( count==more)
                   {    printf("Continue how may iterations? ");
                   scanf("%d", &more );
                   if( count == 0 ) exit(0);
                   count=0;
                   }
                   */
               }
            }

            /**** check contents of flat theta_rad array vs theta_2d_rev  ****/
            count=0;
            for (i=0; i<naxis2; i++)
            {
               for (j=0; j<naxis1; j++)
               {
                  /* good...
                   printf("theta_rad[%d][%d] = %f\n", i,j, theta_rad[i * naxis1 + j] );
                   printf("theta_2d_rev[%d][%d] = %f\n", i,j, theta_2d_rev[i][j] );
                   count++;

                   if( count==more)
                   {    printf("Continue how may iterations? ");
                   scanf("%d", &more );
                   if( count == 0 ) exit(0);
                   count=0;
                   }
                   */
               }
            }

            /**** check contents of flat phi_rad array vs phi_2d_rev  ****/
            count=0;
            for (i=0; i<naxis2; i++)
            {
               for (j=0; j<naxis1; j++)
               {
                  /* good...
                   printf("phi_rad[%d][%d] = %f\n", i,j, phi_rad[i * naxis1 + j] );
                   printf("phi_2d_rev[%d][%d] = %f\n", i,j, phi_2d_rev[i][j] );
                   count++;

                   if( count==more)
                   {    printf("Continue how may iterations? ");
                   scanf("%d", &more );
                   if( count == 0 ) exit(0);
                   count=0;
                   }
                   */
               }
            }

            /**** check contents of flat phi_rad, theta_rad, and b_l line up in i,j  ****/
            count=0;
            for (i=0; i<naxis2; i++)
            {
               for (j=0; j<naxis1; j++)
               {
                  /** good
                   printf("phi_rad[%d][%d] = %f\n", i,j, phi_rad[i * naxis1 + j] );
                   printf("theta_rad[%d][%d] = %f\n", i,j, theta_rad[i * naxis1 + j] );
                   printf("b_l[%d][%d] = %f\n", i,j, b_l[i * naxis1 + j] );
                   count++;

                   if( count==more)
                   {    printf("Continue how may iterations? ");
                   scanf("%d", &more );
                   if( count == 0 ) exit(0);
                   count=0;
                   }
                   **/
               }
            }

            /*** test flat theta vs unique theta ***/

            for (i = 0; i<naxis2; i++)
            {

               printf("theta_2d[%d][%d] = %f\n", 0, i, theta_2d[0][i]);
               printf("theta_2d_rev[%d][%d] = %f\n", i, 0, theta_2d_rev[i][0]);

               printf("flat theta_rad[%d][%d] = %f\n", i, 0, theta_rad[i]);
               printf("flat small theta[%d] = %f\n", i, theta[i]);

            }

            /*** test flat phi vs unique phi ***/

            for (i = 0; i<naxis1; i++)
            {

               printf("phi_2d[%d][%d] = %f\n", 0, i, phi_2d[0][i]);
               printf("phi_2d_rev[%d][%d] = %f\n", i, 0, phi_2d_rev[i][0]);
               /*printf("phi_2d_rev rev ij [%d][%d] = %f\n", 0,i, phi_2d_rev[0][i]);*/
               printf("flat phi_rad[%d][%d] = %f\n", i, 0, phi_rad[i]);
               printf("flat small phi[%d] = %f\n", i, phi[i]);

            }

            /*** replace lat & lon with theta and phi values **/

            for (i=0; i<naxis1; i++)
            {
               longitude[i] = phi[i];
            }

            for (i=0; i<naxis2; i++)
            {
               latitude[i] = theta[i];
            }

            /***** test all, lon & lat 1 - 3 ******/
            printf("!!!!!!!!!!!!!!!!!!!!!!!!\n\nTEST flat log lat 3 \n\n");

            for (i=0; i<naxis1; i++)
            {

               printf("lon[%d] =\t\t%f\n", i, longitude[i]);
               printf("phi[%d] =\t\t%f\n", i, phi[i]);
               printf("phi_rad[%d] =\t\t%f\n", i, phi_rad[i]);
               printf("phi_deg[%d] =\t\t%f\n\n", i, phi_deg[i]);

            }

            for (i=0; i<naxis2; i++)
            {

               printf("lat[%d] =\t\t%f\n", i, latitude[i]);
               printf("theta[%d] =\t\t%f\n", i, theta[i]);
               printf("theta_rad[%d] =\t\t%f\n", i, theta_rad[i]);
               printf("theta_deg[%d] =\t\t%f\n\n", i, theta_deg[i]);

            }

            /*** DEBUG PRINT OUT OF VALUES ***/

            if ( DEBUG_FLAG)
            {
               for (i=0; i<naxis2; i+=30)
               {
                  for (j=0; j<naxis1; j+=30)
                  {
                     /*
                      printf( "b_l[%d][%d] = %-50f\n", i,j, b_l[i * naxis1 + j] );
                      printf( "b_l_2d_static[%d][%d] = %-50f\n", i,j, b_l_2d_static[i][j] );
                      */
                  }
               }

            }

            /** slice validation **/
            /*
             for( i=0;i<naxis2;i++)
             */

            printf("\n\n\n--------------- phi = 14\n\n");

            for (j=0; j<naxis1; j++)
            { /*
             printf("2d\t%f\t%f\t%f\n", phi_2d[257][0], theta_2d[0][j], b_l_2d_static[257][j]);
             printf("flat\t%f\t%f\t%f\n", phi[257], theta[j], b_l[257 * naxis1 + j]);
             */
               /*
                printf("phi14-%f\t%f\t%f\n", phi[14], theta[j], b_l[14 * naxis1 + j]);
                */

            }

            printf("\n\n\n--------------- phi = 89\n\n");

            for (j=89; j<naxis1; j++)
            { /*
             printf("2d\t%f\t%f\t%f\n", phi_2d[257][0], theta_2d[0][j], b_l_2d_static[257][j]);
             printf("flat\t%f\t%f\t%f\n", phi[257], theta[j], b_l[257 * naxis1 + j]);
             */
               /*
                printf("phi89-%f\t%f\t%f\n", phi[0], theta[j], b_l[89 * naxis1 + j]);
                */

            }

            printf("\n\n\n--------------- phi = 229\n\n");

            for (j=0; j<naxis1; j++)
            { /*
             printf("2d\t%f\t%f\t%f\n", phi_2d[257][0], theta_2d[0][j], b_l_2d_static[257][j]);
             printf("flat\t%f\t%f\t%f\n", phi[257], theta[j], b_l[257 * naxis1 + j]);
             */
               /*
                printf("phi229-%f\t%f\t%f\n", phi[0], theta[j], b_l[229 * naxis1 + j]);
                */

            }

         } /** end of LOCAL BLOCK FOR STATIC ARRAYS **/

      }
      else if (hdu_type == ASCII_TBL)
      {
         printf("Current HDU Type is ACSII TABLE\n");
      }
      else if (hdu_type == BINARY_TBL)
      {
         printf("Current HDU Type is BINARY TABLE\n");
      }

      /*** get the header info for the current hdu ***/

      for (i = 0; i < number_of_fits_header_keys; i++)
      {
         fits_read_record(fits_filePtr, i, header_buffer, &status);
         printf("%s\n", header_buffer);
      }

   }

   /** close fits file **/

   fits_close_file(fits_filePtr, &status);

   if (status)
   {
      fits_report_error( stderr, status);
   }

   /*********************  calcluate actual min/max values for each ariable unless -nominmax flag was specified *************/

   /**** add more error handling for each linear_minmax_search function call ***/

   if ( !minmax_flag) /*** if -nominmax option was NOT specified ***/
   {

      if (verbose_flag)
      {
         printf("\ncalculating actual minimum & maximum values for each variable ...\n");
      }

      if (verbose_flag)
      {
         printf("%-25s%-25s%-25s\n", "", "min", "max");
      }

      linear_minmax_search(longitude, naxis1, &longitude_actual_min,
            &longitude_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "lon", longitude_actual_min,
               longitude_actual_max);
      }

      linear_minmax_search(latitude, naxis2, &latitude_actual_min,
            &latitude_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "lat", latitude_actual_min,
               latitude_actual_max);
      }

      linear_minmax_search(b_l, number_of_elements, &b_l_actual_min,
            &b_l_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "b_l", b_l_actual_min, b_l_actual_max);
      }

      linear_minmax_search(b_t, number_of_elements, &b_t_actual_min,
            &b_t_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "b_t", b_t_actual_min, b_t_actual_max);
      }

      linear_minmax_search(raz, number_of_elements, &raz_actual_min,
            &raz_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "raz", raz_actual_min, raz_actual_max);
      }

   }

#endif

#ifndef FITS
   printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
   printf("!! WARNING: from %s line [%d]. Conversion Software was not compiled and linked with netCDF libraries ( libcfitsio.a ).  No .fts FITS files will be ingested... \n", __FILE__, __LINE__ );
   return EXIT_FAILURE;
#endif

   return status;

}

