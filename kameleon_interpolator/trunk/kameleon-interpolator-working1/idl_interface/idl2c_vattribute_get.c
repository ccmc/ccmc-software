/**************************************************************************************************
*                                                                                               	*
*       NAME:           idl2c_vattribute_get.c													*
*                                                                                               	*
*       VERSION:        1.0                                                                     	*
*                                                                                               	*
*		AUTHOR:			Marlo M. H. Maddox                                                      	*
*						NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                                       	*
*						Marlo.Maddox@nasa.gov                                                   	*
*                       	(301) 286 - 5202                                                        	*
*                                                                                               	*
*       PURPOSE:        	This code is used tot create an IDL system routine that returns a 		*
* 						variable attribute for a specified file and variable					*
* 						It essentially provides the same functionality as the vattribute_get	*
* 						routine.  This module is added to a shared library and linked to IDL	*
* 						with LINKIMAGE...( DLM's are also possible )							*
*                                                                                               	*
*       INPUTS:         	(1) full path file name (2) variable name (3) variable attribute name	*
*                                                                                               	*
*       OUTPUTS:        variable attribute value												*
*                                                                                               	*
*       FUNCTIONS:      *
*																							*
*		VARIABLES:		*
*																							*
* *																							*
*       MODIFICATION                                                                            	*
*       HISTORY:        																		*
* 																							*
* 						Maddox, Marlo	09/2006		Initial development started				*
* 						Maddox, Marlo	10.10.2006	Using export.h exclusivley				*
* 						Maddox, Marlo	10.11.2006	Removed kameleon library cdf_open &		*
* 													cdf_close calls and replaced them with		*
* 													native CDFopen and CDFclose calls			*
* 																							*
* 													had to add SELECT_, gEntry to CDFLIB call	*
* 													during attribute inquire step				*
* 																							*
* 																							*
* 						Maddox, Marlo	10.25.2006 	we chaged CDFid id to current_file_id in	*
* 													the cdf_interface_variables.h file but		*
* 													forgot to update the extern declaration	*
* 													here										*
*																							*
**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "cdf.h"
#include "export.h"
/*
#include "idl_export.h"
*/

#define MAX_MESSAGE_BUFFER_SIZE 500


IDL_VPTR idl2c_vattribute_get( int argc, IDL_VPTR argv[] )
{

extern CDFid current_file_id;
CDFstatus status;

long var_attrNum, varNum, data_type, num_elements;
static void *value;
static float tmp_float_value;
static int tmp_int_value;

/***************** declared local pointers to store returned vattribute value pointers ************/

char *char_vattribute_valuePtr;
int *int_vattribute_valuePtr;
float *float_vattribute_valuePtr;

/****** idl2c interface variables ******/

IDL_VPTR input_file_name, variable_name, vattribute_name;
IDL_VPTR idl_local_float_value, idl_local_int_value;
IDL_VPTR idl_return_float_value, idl_return_int_value;

char message_buffer1[MAX_MESSAGE_BUFFER_SIZE];

int         tmp_ndims;
IDL_LONG    tmp_dims[IDL_MAX_ARRAY_DIM];


/* check our argc count first */

if( argc != 3 ) 
{
	IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, "ERROR:program requires 3 arguments: (1) full path and file name of data file (2) variable name (3) variable attribute name");
}

/** get a local copies of idl2c function arguments **/

input_file_name=argv[0];
variable_name=argv[1];
vattribute_name=argv[2];

/**************************************
 * OPEN CDF FILE AND AQUIRE CDFid		*
**************************************/

/**** lets see if the file even exists first ****/

if( ! access( IDL_VarGetString(input_file_name), R_OK) )
{

	status = CDFopen( IDL_VarGetString(input_file_name), &current_file_id );
	
	if(status != CDF_OK)
	{
	
		/** if we couldn't access file, return contron with message to IDL **/
	
		StatusHandler (status);
		snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "ERROR: cannot read \t%s\n", IDL_VarGetString( input_file_name ) );
		IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
	
	}
	
}
else
{
	/** if we couldn't access file, return contron with message to IDL **/
	
	snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "ERROR: cannot read \t%s\n", IDL_VarGetString( input_file_name ) );
	IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
	
}

sprintf( message_buffer1, "Open %s complete...\n", IDL_VarGetString(input_file_name) );

IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_INFO, message_buffer1 );

/** now lets work on identifying the specified vattribute of interest **/

status = CDFlib (	GET_, 		ATTR_NUMBER_, 		IDL_VarGetString( vattribute_name ), 	&var_attrNum, 	NULL_ );

/** if we couldn't find the vattribute number, return with message to calling idl program **/

if(status != CDF_OK)
{
	StatusHandler (status);
	sprintf( message_buffer1, "Couldn't find specified vattribute = %s\n", IDL_VarGetString(vattribute_name) );
	IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1);
}

/** so the attribute exists, now lets find the variable **/

/**************************************
 * NOW LETS locate the variable id	*
**************************************/

status = CDFlib (	GET_, 		zVAR_NUMBER_, 		IDL_VarGetString( variable_name ), 	&varNum, 	NULL_ );

/** if we couldn't find the variable number, return with message to calling idl program **/

if(status != CDF_OK)
{
	StatusHandler (status);
	sprintf( message_buffer1, "Couldn't find specified variable = %s\n", IDL_VarGetString(variable_name) );
	IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1);
	}

/** get some information about the requested vattribute **/

/** the vattribute data type **/

status = CDFlib ( 	SELECT_,		ATTR_,				var_attrNum,
					SELECT_, 	zENTRY_NAME_,		IDL_VarGetString( variable_name ),
					GET_,		zENTRY_DATATYPE_,	&data_type,
					NULL_
				);
				
if(status != CDF_OK) StatusHandler (status);

/** the number of vattribute elements for the current variable **/

status = CDFlib ( 	
			SELECT_,		ATTR_,				var_attrNum,
			SELECT_, 	zENTRY_NAME_,		IDL_VarGetString( variable_name ),
			GET_,		zENTRY_NUMELEMS_,	&num_elements,
			NULL_
		);
		
if(status != CDF_OK) StatusHandler (status);



	if( data_type == CDF_CHAR )
	{

	value = ( char * ) malloc( num_elements + 1 );
		
	status = CDFlib ( 	
			SELECT_,	ATTR_,			var_attrNum,
			SELECT_, 	zENTRY_NAME_,		IDL_VarGetString( variable_name ),
			GET_,		zENTRY_DATA_,		value,
			NULL_
		);
	if(status != CDF_OK) StatusHandler (status);

	((char *) value)[num_elements] = '\0';
	
	char_vattribute_valuePtr = value;

	status = CDFclose ( current_file_id );
	if(status != CDF_OK) { StatusHandler (status); }

	return IDL_StrToSTRING( char_vattribute_valuePtr );
	
	}
	else if( data_type == CDF_INT4 )
	{
		
	tmp_ndims   = 1;
 	tmp_dims[0] = 1;
	
	status = CDFlib ( 	
			SELECT_,	ATTR_,			var_attrNum,
			SELECT_, 	zENTRY_NAME_,	IDL_VarGetString( variable_name ),
			GET_,		zENTRY_DATA_,	&tmp_int_value,
			NULL_
		);
		
	if(status != CDF_OK) StatusHandler (status);

	/** Create a temporary IDL variable and have our local pointer point to it **/

	int_vattribute_valuePtr = (int *) IDL_MakeTempArray( IDL_TYP_LONG, tmp_ndims, tmp_dims, IDL_ARR_INI_ZERO, &idl_local_int_value );
	
	/** using our local pointer, modify the contents of the IDL Variable **/
	
	int_vattribute_valuePtr[0] = tmp_int_value;
		
	/*fprintf(stderr, "WTF4.25 *int_attribute_valuePtr = %d\n", *int_attribute_valuePtr );*/

	status = CDFclose ( current_file_id );
	if(status != CDF_OK) { StatusHandler (status); }
	
	return idl_local_int_value;
	
	}
	else if( data_type == CDF_FLOAT )
	{

	tmp_ndims   = 1;
 	tmp_dims[0] = 1;

	status = CDFlib ( 	
			SELECT_,	ATTR_,			var_attrNum,
			SELECT_, 	zENTRY_NAME_,	IDL_VarGetString( variable_name ),
			GET_,		zENTRY_DATA_,	&tmp_float_value,
			NULL_
		);
		
	if(status != CDF_OK) StatusHandler (status);
	
	/** Create a temporary IDL variable and have our local pointer point to it **/

	float_vattribute_valuePtr = (float *) IDL_MakeTempArray( IDL_TYP_FLOAT, tmp_ndims, tmp_dims, IDL_ARR_INI_NOP, &idl_local_float_value );
	
	/** using our local pointer, modify the contents of the IDL Variable **/
	
	float_vattribute_valuePtr[0] = tmp_float_value;
	
	status = CDFclose ( current_file_id );
	if(status != CDF_OK) { StatusHandler (status); }
	
	return idl_local_float_value;

	}
	else
	{
	
	IDL_VPTR no_value = NULL;
	
	/*** code will NOT get here until we capture CDF's error of NO_SUCH_ATTR:..., and return control instead of dying ***/
	
	status = CDFclose ( current_file_id );
	if(status != CDF_OK) { StatusHandler (status); }

	snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "ERROR:\tCouldn't determine data type for %s\n", IDL_VarGetString( vattribute_name ) );

	IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_INFO, message_buffer1 );
	
	return no_value;
	}
	
/*** program should never reach here ***/	

}


