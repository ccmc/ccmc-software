/*******************************************************************************
 *                                                                             *
 *    NAME:       date_formatter.c                                             *
 *                                                                             *
 *    VERSION:    3.2                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    Converts a date string into cdf_epoch and then encodes that  *
 *                into a cdfEPOCH3 string to be written into cdf - add case    *
 *                when new HDF5 implementation etc..to create same format:     *
 *                                                                             *
 *                             yyyy-mm-ddThh:mm:ss.cccZ                        *
 *                                                                             *
 *    INPUTS:     *                                                            *
 *                                                                             *
 *    OUTPUTS:        set input array to formatted date string:                *
 *                                                                             *
 *                             yyyy-mm-ddThh:mm:ss.cccZ                        *
 *                                                                             *
 *    FUNCTIONS:  *                                                            *
 *                                                                             *
 *    VARIABLES:  *                                                            *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    2004 September 13th  Maddox, Marlo                                       *
 *                         added date/time_cdf_epoch3 starage for date/time    *
 *                         variables request_date & start/end_time             *
 *                                                                             *
 *    2005 February 7th    Maddox, Marlo                                       *
 *                         adding open_ggcm_init_time support                  *
 *                                                                             *
 *    2008 July 24th       Maddox, Marlo                                       *
 *                         Added EXIT_FAILURE & EXIT_SUCCESS to qualifying     *
 *                         return and exit calls                               *
 *                                                                             *
 *    2008 August 15th     Maddox, Marlo                                       *
 *                         Added MAX_BUFFER_SIZE for buffer and fixed          *
 *                         termination for char arrays in open_ggcm block      *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdf.h"

#define DEBUG_FLAG 1

/** #include <time.h> for future reference **/

#define MAX_BUFFER_SIZE 50

void date_formatter(
      char *original_date_string,
      char *date_entry_name,
      char cdf_epoch3_date_string[25],
      int verbose_flag)
{

   char *string_token;
   char *space = " ";
   char *slash = "/";
   char *colon = ":";
   char *dot = ".";

   char left_half_date[11];
   char right_half_date[6];
   char buffer[MAX_BUFFER_SIZE];

   char YYYY_char_string[5];
   char MM_char_string[3];
   char DD_char_string[3];
   char hh_char_string[3];
   char mm_char_string[3];
   char ss_char_string1[6]; /*   for open_ggcm_init_time format y
    yyy:mm:dd:hh:mm:ss.ss - holds the ss.ss */
   char ss_char_string2[3]; /* just store whole seconds */
   char dummy_buf[2];

   long year, month, day, hour, minute, second = 0, msec = 0;

   double cdf_epoch_time;

   /******* define database entry names to search for in DatabaseInfo file ****/

   char *request_date_entry = "request_date";
   char *start_time_entry = "start_time";
   char *end_time_entry = "end_time";
   char *dipole_time_entry = "dipole_time";
   char *open_ggcm_init_time_entry = "open_ggcm_init_time";

   if ( DEBUG_FLAG)
   {
      printf(
            "\n\nDEBUG:\t%s - entering date formatter...\n",
__FILE__            );
         }

         if ( !strcmp(
               date_entry_name,
               start_time_entry) || !strcmp(
               date_entry_name,
               end_time_entry) || !strcmp(
               date_entry_name,
               dipole_time_entry) )
         {

            if ( DEBUG_FLAG)
            {
               printf(
               "DEBUG:\tattempting to format -->%s<--...\n",
               original_date_string);
            }

            /*** input date string is either start_time|end_time of the form
             * yyyy/mm/dd hh:mm ***/

            /*** spilt start/end_time date sting into halfs delimited my the space ***/

            string_token = strtok(
            original_date_string,
            space);
            strcpy(
            left_half_date,
            string_token);
            left_half_date[10] = '\0';

            string_token = strtok( NULL, space);
            strcpy(
            right_half_date,
            string_token);
            right_half_date[5] = '\0';

            /*** spilt left half of date string into seperate yyyy mm dd by using slash
             * delimiter ***/

            string_token = strtok(
            left_half_date,
            slash);
            strcpy(
            YYYY_char_string,
            string_token);
            YYYY_char_string[4] = '\0';

            string_token = strtok( NULL, slash);
            strcpy(
            MM_char_string,
            string_token);
            MM_char_string[2] = '\0';

            string_token = strtok( NULL, slash);
            strcpy(
            DD_char_string,
            string_token);
            DD_char_string[2] = '\0';

            /*** spilt right half of date string into seperate hh mm by using colon
             * delimiter ***/

            string_token = strtok(
            right_half_date,
            colon);
            strcpy(
            hh_char_string,
            string_token);
            hh_char_string[2] = '\0';

            string_token = strtok( NULL, colon);
            strcpy(
            mm_char_string,
            string_token);
            mm_char_string[2] = '\0';

         }
         else if ( !strcmp(
               date_entry_name,
               request_date_entry) )
         {

            if ( DEBUG_FLAG)
            {
               printf(
               "DEBUG:\tattempting to format -->%s<--...\n",
               original_date_string);
            }

            /*** date string is of the form YYMMDD ***/

            /** first set the last two digits of the year **/

            YYYY_char_string[2] = original_date_string[0];
            YYYY_char_string[3] = original_date_string[1];

            dummy_buf[0] = YYYY_char_string[2];
            dummy_buf[1] = '\0';

            if (strcmp(
                  dummy_buf,
                  "9") )
            {
               YYYY_char_string[0] = '2';
               YYYY_char_string[1] = '0';
            }
            else
            {
               YYYY_char_string[0] = '1';
               YYYY_char_string[1] = '9';
            }

            MM_char_string[0] = original_date_string[2];
            MM_char_string[1] = original_date_string[3];

            DD_char_string[0] = original_date_string[4];
            DD_char_string[1] = original_date_string[5];

            hh_char_string[0] = '0';
            hh_char_string[1] = '0';
            mm_char_string[0] = '0';
            mm_char_string[1] = '0';

            YYYY_char_string[4] = '\0';
            MM_char_string[2] = '\0';
            DD_char_string[2] = '\0';
            hh_char_string[2] = '\0';
            mm_char_string[2] = '\0';

         }
         else if ( !strcmp(
               date_entry_name,
               open_ggcm_init_time_entry) )
         {

            int input_string_length;

            if ( DEBUG_FLAG)
            {
               printf(
               "DEBUG:\tattempting to format -->%s<--...\n",
               original_date_string);
            }

            /**** date string is of the form YYYY:MM:DD:hh:mm:ss.ss **********/

            /*** separate all feilds using colon delimiter ***/
            input_string_length = strlen(original_date_string);

            printf(
            "original_date_string = --->%s<---\n",
            original_date_string);
            printf(
            "original_date_string length = %d \n",
            input_string_length);

            strncpy(
            buffer,
            original_date_string,
            input_string_length);

            /** don't trust strncpy, force null terminate **/

            buffer[input_string_length] = '\0';

            printf(
            "buffer = --->%s<---\n",
            buffer);

            string_token = strtok(
            buffer,
            colon);
            strcpy(
            YYYY_char_string,
            string_token);
            YYYY_char_string[4] = '\0';

            printf(
            "YYYY_char_string = --->%s<---\n",
            YYYY_char_string);

            string_token = strtok( NULL, colon);
            strcpy(
            MM_char_string,
            string_token);
            MM_char_string[2] = '\0';

            printf(
            "MM_char_string = --->%s<---\n",
            MM_char_string);

            string_token = strtok( NULL, colon);
            strcpy(
            DD_char_string,
            string_token);
            DD_char_string[2] = '\0';

            printf(
            "DD_char_string = --->%s<---\n",
            DD_char_string);

            string_token = strtok( NULL, colon);
            strcpy(
            hh_char_string,
            string_token);
            hh_char_string[2] = '\0';

            printf(
            "hh_char_string = --->%s<---\n",
            hh_char_string);

            string_token = strtok( NULL, colon);
            strcpy(
            mm_char_string,
            string_token);
            mm_char_string[2] = '\0';

            printf(
            "mm_char_string = --->%s<---\n",
            mm_char_string);

            /****** NOTE CHANGE FROM COLON TO DOT *****/

            string_token = strtok( NULL, colon);
            strcpy(
            ss_char_string1,
            string_token);
            ss_char_string1[3] = '\0';

            printf(
            "ss_char_string1 = --->%s<---\n",
            ss_char_string1);

            /****** NOTE CHANGE FROM COLON TO DOT *****/

            string_token = strtok(
            ss_char_string1,
            dot);
            strcpy(
            ss_char_string2,
            string_token);
            ss_char_string2[2] = '\0';

            printf(
            "ss_char_string2 = --->%s<---\n",
            ss_char_string2);

            second = atol(ss_char_string2);

         }

         if (verbose_flag)
         {
            printf(
            "\noriginal extracted %s entry: %s\n",
            date_entry_name,
            original_date_string);
         }

         if (verbose_flag)
         {
            printf(
            "parsed date elements:\t%s %s %s %s %s\n",
            YYYY_char_string,
            MM_char_string,
            DD_char_string,
            hh_char_string,
            mm_char_string);
         }

         /*** converts extracted strings to their cooresponding long int values and
          * store ***/

         year = atol(YYYY_char_string);
         month = atol(MM_char_string);
         day = atol(DD_char_string);
         hour = atol(hh_char_string);
         minute = atol(mm_char_string);

         cdf_epoch_time = computeEPOCH(
         year,
         month,
         day,
         hour,
         minute,
         second,
         msec);

         encodeEPOCH3(
         cdf_epoch_time,
         cdf_epoch3_date_string);

         return;

      }
