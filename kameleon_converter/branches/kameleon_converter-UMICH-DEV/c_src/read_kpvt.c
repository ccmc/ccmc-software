/*******************************************************************************
 *                                                                             *
 *      NAME:       read_kpvt.h                                                *
 *                                                                             *
 *      VERSION:    1.0                                                        *
 *                                                                             *
 *      AUTHOR:     Marlo Maddox                                               *
 *                  NASA/GSFC/AETD/ADMA/CCMC                                   *
 *                  Marlo.Maddox@nasa.gov                                      *
 *                  (301) 286 - 5202                                           *
 *                                                                             *
 *      PURPOSE:    Kitt Peak Vacuum Telescope Magnetogram Data Ingestion      *
 *                                                                             *
 *      INPUTS:     *                                                          *
 *                                                                             *
 *      OUTPUTS:    *                                                          *
 *                                                                             *
 *      FUNCTIONS:  *                                                          *
 *                                                                             *
 *       VARIABLES: *                                                          *
 *                                                                             *
 *                                                                             *
 *      MODIFICATION                                                           *
 *      HISTORY:                                                               *
 *                                                                             *
 *      2006 November 27th  Maddox, Marlo                                      *
 *                          initial development started                        *
 *      2007 February 6th   Maddox, Marlo                                      *
 *                          changing ccmc_metadata_structure.h to              *
 *                          ccmc_structure_manager.h                           *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "read_kpvt.h"

#ifdef FITS
#include "fitsio.h"
#endif

#define DEBUG_FLAG 0

/*

 #include "ccmc_attributes.h"
 #include "ccmc_registered_variables.h"
 #include "ccmc_structure_manager.h"

 */

/*******************************************************************************
 *                                MAIN                                         *
 ******************************************************************************/

/*
 int main( int argc, char *argv[])
 {
 int read_kptv( char *, char *, int, int );

 int return_status;

 return_status = read_kptv( argv[1], argv[2], 1, 0 );

 return return_status;
 }
 */

int read_kpvt(
      char *input_file_name,
      char *aux_file,
      int verbose_flag,
      int minmax_flag)
{

   int status = 0;

   /*
    * If the Compiler macro FITS is not defined, that means the makefile
    * determined that libcfitsio.a was not available.  Instead of shutting down
    * the entire converter Lets just disable any read routines thatt depend on
    * the FITS library
    *
    */

#ifdef FITS

   extern int linear_minmax_search(float *, int, float *, float *);

   fitsfile *fits_filePtr;
   char header_buffer[FLEN_CARD ];

   float operand1;

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
   float net_flux_2d_static[180][360];
   float net_flux_2d_static_flat[64800];
   float total_flux_2d_static[180][360];
   float total_flux_2d_static_flat[64800];
   float weights_2d_static[180][360];
   float weights_2d_static_flat[64800];

   /** open fits file **/

   fits_open_file( &fits_filePtr, input_file_name, READONLY, &status);

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

         fits_get_img_param(
               fits_filePtr,
               maxdim,
               &bit_pix,
               &naxis,
               naxes,
               &status);

         printf("bitpix = %d\nnaxis = %d\n", bit_pix, naxis);

         if (DEBUG_FLAG)
         {
            for (i = 0; i<naxis; i++)
               printf("naxis[%d] = %ld\n", i, naxes[i]);
         }

         naxis1 = naxes[0];
         naxis2 = naxes[1];

         number_of_elements = naxis1 * naxis2;

         /** dynamically allocate space for the image grid **/

         carrington_longitiude = malloc(naxis1 * sizeof(float));
         carrington_sine_latitude = malloc(naxis2 * sizeof(float));

         /** dynamically allocate space for the flat variable arrays **/

         net_flux = malloc(number_of_elements * sizeof(float));
         total_flux = malloc(number_of_elements * sizeof(float));
         weights = malloc(number_of_elements * sizeof(float));

         /** manually insert sequential grid values from 1 - naxes[*] **/

         operand1 = 2.0 / naxis2;
         if (DEBUG_FLAG)
         {
            printf(
                  "\n\n\n\n\n\nWTF: operand1 = %f\nnaxis2 = %d\n",
                  operand1,
                  naxis2);
         }

         for (i=0; i < naxis2; i++)
         {

            /** convert to sine latidude **/
            /*
             carrington_sine_latitude[i] = i+1;
             */
            carrington_sine_latitude[i] = (-1.0)
                  + (operand1 * ( (i + 1 ) - .5 ) );

            if (DEBUG_FLAG)
               printf("%d deg = %f\n", i+1, carrington_sine_latitude[i]);

         }

         for (i=0; i < naxis1; i++)
         {
            carrington_longitiude[i] = i+1;

            if (DEBUG_FLAG)
               printf("%d deg = %f deg\n", i+1, carrington_longitiude[i]);
         }

         /** print position values **/

         for (i=0; i<naxis2; i+=30)
         {
            for (j=0; j<naxis1; j+=30)
            {
               if (DEBUG_FLAG)
               {
                  printf(
                        "position[%f][%f]\n",
                        carrington_sine_latitude[i],
                        carrington_longitiude[j]);
               }
            }
         }

         /** print c_lon values **/

         for (i=0; i<naxis1; i++)
         {

            if( DEBUG_FLAG ) printf("c_lon[%d] = %f  ", i, carrington_longitiude[i]);

         }

         /** print c_sine_lat values **/

         for (i=0; i<naxis2; i++)
         {

            if( DEBUG_FLAG ) printf("c_sine_lat[%d] = %f  ", i, carrington_sine_latitude[i]);

         }

         /** dynamically allocate memory for the three image slices that we
          * expect - we must reverse the majority since the fits api expects
          * array[column][row] **/

         net_flux_2d = malloc(naxis1 * sizeof(float *));
         for (i=0; i< naxis1; i++)
         {
            net_flux_2d[i] = malloc(naxis2 * sizeof(float));
         }

         total_flux_2d = malloc(naxis2 * sizeof(float *));
         for (i=0; i< naxis2; i++)
         {
            total_flux_2d[i] = malloc(naxis1 * sizeof(float));
         }

         weights_2d = malloc(naxis2 * sizeof(float *));
         for (i=0; i< naxis2; i++)
         {
            weights_2d[i] = malloc(naxis1 * sizeof(float));
         }

         /*** read in the image data ***/

         /** this is the starting point for the read - slice one - net flux **/

         fpixel[0] = fpixel[1] = fpixel[2]= 1;
         if( DEBUG_FLAG ) printf(
               "\nfpixel[0] = %ld\nfpixel[1] = %ld\nfpixel[2] = %ld\nnumber_of_elements = %ld\n\n",
               fpixel[0],
               fpixel[1],
               fpixel[2],
               number_of_elements);
         /*
          fits_read_pix( fits_filePtr, TFLOAT, fpixel, number_of_elements,
          &missing_val, *net_flux_2d, &anynul, &status );
          */
         fits_read_pix(
               fits_filePtr,
               TFLOAT,
               fpixel,
               number_of_elements,
               &missing_val,
               net_flux_2d_static,
               &anynul,
               &status);

         /** this is the starting point for the read - slice two - total flux */

         fpixel[2] = 2;
         if( DEBUG_FLAG )  printf(
               "fpixel[0] = %ld\nfpixel[1] = %ld\nfpixel[2] = %ld\nnumber_of_elements = %ld\n\n",
               fpixel[0],
               fpixel[1],
               fpixel[2],
               number_of_elements);
         /*
          * fits_read_pix( fits_filePtr, TFLOAT, fpixel, number_of_elements,
          * &missing_val, *total_flux_2d, &anynul, &status );
          *
          */

         fits_read_pix(
               fits_filePtr,
               TFLOAT,
               fpixel,
               number_of_elements,
               &missing_val,
               total_flux_2d_static,
               &anynul,
               &status);

         /** this is the starting point for the read - slice three - weights **/

         fpixel[2] = 3;
         if( DEBUG_FLAG )printf(
               "fpixel[0] = %ld\nfpixel[1] = %ld\nfpixel[2] = %ld\nnumber_of_elements = %ld\n\n",
               fpixel[0],
               fpixel[1],
               fpixel[2],
               number_of_elements);
         /*
          * fits_read_pix( fits_filePtr, TFLOAT, fpixel, number_of_elements,
          * &missing_val, *weights_2d, &anynul, &status );
          *
          */

         fits_read_pix(
               fits_filePtr,
               TFLOAT,
               fpixel,
               number_of_elements,
               &missing_val,
               weights_2d_static,
               &anynul,
               &status);

         /** flatten out arrays **/

         for (i=0; i<naxis2; i++)
         {
            for (j=0; j<naxis1; j++)
            {
               /*
                net_flux[ i * naxis1 + j] = net_flux_2d_static[i][j];
                total_flux[ i * naxis1 + j] = total_flux_2d[i][j];
                weights[ i * naxis1 + j] = weights_2d[i][j];
                */
               net_flux[ i * naxis1 + j] = net_flux_2d_static[i][j];
               total_flux[ i * naxis1 + j] = total_flux_2d_static[i][j];
               weights[ i * naxis1 + j] = weights_2d_static[i][j];

            }
         }

         /*** DEBUG PRINT OUT OF VALUES ***/

         /*
          for( i=0;i<naxis2;i+=20)
          {
          for( j=0;j<naxis1;j+=20)
          {

          printf( "net_flux[%d][%d] = %f\n", i,j, net_flux[i * naxis1 + j] );
          printf( "net_flux_2d[%d][%d] = %f\n", i,j, net_flux_2d[i][j] );
          printf( "net_flux_2d_static[%d][%d] = %f\n", i,j, net_flux_2d_static[i][j] );
          printf( "net_flux_2d_static_flat[%d][%d] = %f\n", i,j, net_flux_2d_static_flat[i * naxis1 + j] );
          printf( "total_flux[%d][%d] = %f\n", i,j, total_flux[i * naxis1 + j] );
          printf( "total_flux_2d[%d][%d] = %f\n", i,j, total_flux_2d[i][j] );
          printf( "weights[%d][%d] = %f\n", i,j, weights[i * naxis1 + j] );
          printf( "weights_2d[%d][%d] = %f\n\n", i,j, weights_2d[i][j] );


          }
          }
          */

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

   /*
    * calcluate actual min/max values for each ariable unless -nominmax flag was
    * specified
    *
    */

   /** add more error handling for each linear_minmax_search function call **/

   /*** if -nominmax option was NOT specified ***/

   if ( !minmax_flag)
   {

      if (verbose_flag)
      {
         printf("\ncalculating actual minimum & maximum values for each variable ...\n");
      }

      if (verbose_flag)
      {
         printf("%-25s%-25s%-25s\n", "", "min", "max");
      }

      linear_minmax_search(
            carrington_longitiude,
            naxis1,
            &carrington_longitiude_actual_min,
            &carrington_longitiude_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "c_lon",
               carrington_longitiude_actual_min,
               carrington_longitiude_actual_max);
      }

      linear_minmax_search(
            carrington_sine_latitude,
            naxis2,
            &carrington_sine_latitude_actual_min,
            &carrington_sine_latitude_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "c_sine_lat",
               carrington_sine_latitude_actual_min,
               carrington_sine_latitude_actual_max);
      }

      linear_minmax_search(
            net_flux,
            number_of_elements,
            &net_flux_actual_min,
            &net_flux_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "net_flux",
               net_flux_actual_min,
               net_flux_actual_max);
      }

      linear_minmax_search(
            total_flux,
            number_of_elements,
            &total_flux_actual_min,
            &total_flux_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "total_flux",
               total_flux_actual_min,
               total_flux_actual_max);
      }

      linear_minmax_search(
            weights,
            number_of_elements,
            &weights_actual_min,
            &weights_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "weights",
               weights_actual_min,
               weights_actual_max);
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

