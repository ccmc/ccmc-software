/*
 * Adapt3D.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: David Berrios
 */

#include "Adapt3D.h"
#include "Point.h"
#include "Cell3D.h"
#include "Vector.h"
#include "Adapt3DInterpolator.h"
#include "StringConstants.h"


namespace ccmc
{
	/**
	 * Default constructor
	 */
	Adapt3D::Adapt3D()
	{
		ndimn=NDIMN_ADAPT3D;
		//NDIMN=NDIMN_ADAPT3D;
		nnode=NNODE_ADAPT3D;
		nvars=NVARS_ADAPT3D;

	}

	/**
	 * @copydoc Model::open()
	 */
	long Adapt3D::open(const std::string& filename)
	{

		long status;
		status = openFile(filename);

		if (status != FileReader::FILE_OK)
		   return status;

		counts[0] = 1; /* set values */
		intervals[0] = 1;
		this->unstructuredGridSetup = false;

		   /* get all of the cdf attribute/variable numbers that will be needed *******/

		if (this->doesVariableExist(ccmc::strings::variables::coord_) == FileReader::OK)
		{
//			status = this->loadVariable(ccmc::strings::variables::coord_);
//			if (status != FileReader::OK)
//				return FileReader::LOAD_FAILED;
		} else
			return FileReader::VARIABLE_DOES_NOT_EXIST;
		if (this->doesVariableExist(ccmc::strings::variables::unkno_) == FileReader::OK)
		{
//			status = this->loadVariable(ccmc::strings::variables::unkno_);
//			if (status != FileReader::OK)
//				return FileReader::LOAD_FAILED;
		} else
			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::rho_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::rho_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::bx_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::bx_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::by_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::by_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::bz_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::bz_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::ux_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::ux_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::uy_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::uy_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::uz_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::uz_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::p_) == FileReader::OK)
//		{
////			status = this->loadVariable(ccmc::strings::variables::p_);
////			if (status != FileReader::OK)
////				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
//		if (this->doesVariableExist(ccmc::strings::variables::temp_) == FileReader::OK)
//		{
//			status = this->loadVariable(ccmc::strings::variables::temp_);
//			if (status != FileReader::OK)
//				return FileReader::LOAD_FAILED;
//		} else
//			return FileReader::VARIABLE_DOES_NOT_EXIST;
		if (this->doesVariableExist(ccmc::strings::variables::intmat_) == FileReader::OK)
		{
			status = this->loadVariable(ccmc::strings::variables::intmat_);
			if (status != FileReader::OK)
				return FileReader::LOAD_FAILED;
		} else
			return FileReader::VARIABLE_DOES_NOT_EXIST;

		/***  Check the existence of necessary attributes.  Fail if any don't exist ***/
		if (doesAttributeExist(ccmc::strings::variables::grid_reg_no_) == FileReader::OK)
		{
			this->grid_reg_no = (this->getGlobalAttribute(ccmc::strings::variables::grid_reg_no_)).getAttributeInt();
		} else
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		if (doesAttributeExist(ccmc::strings::variables::ndimn_) == FileReader::OK)
		{
			this->ndimn = (this->getGlobalAttribute(ccmc::strings::variables::ndimn_)).getAttributeInt();
		} else
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;

		if (doesAttributeExist(ccmc::strings::variables::npoin_) == FileReader::OK)
		{
			this->npoin = (this->getGlobalAttribute(ccmc::strings::variables::npoin_)).getAttributeInt();
		} else
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;

		if (doesAttributeExist(ccmc::strings::variables::nelem_) == FileReader::OK)
		{
			this->nelem = (this->getGlobalAttribute(ccmc::strings::variables::nelem_)).getAttributeInt();
		} else
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;

		if (doesAttributeExist(ccmc::strings::variables::nboun_) == FileReader::OK)
		{
			this->nboun = (this->getGlobalAttribute(ccmc::strings::variables::nboun_)).getAttributeInt();
		} else
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;

		if (doesAttributeExist(ccmc::strings::variables::nconi_) == FileReader::OK)
		{
			this->nconi = (this->getGlobalAttribute(ccmc::strings::variables::nconi_)).getAttributeInt();
		} else
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;


		   /*convert original float N* to int n* if required */
		   /* bats NX was float but ucla, ctip, enlil, kpvt... are interger - no conversion needed here*/
//
//		   if ( DEBUG_FLAG)
//		   {
//		      printf("DEBUG\tINFO from get_adapt3d_cdf_info\n");
//		      printf(
//		            "DEBUG\tNPOIN=%d & npoin=%d\n",
//		            NPOIN,
//		            npoin);
//		      printf(
//		            "DEBUG\tNELEM=%d & nelem=%d\n",
//		            NELEM,
//		            nelem);
//		      printf(
//		            "DEBUG\t main_memory_flag = %d\n",
//		            main_memory_flag);
//		   }


		   /** unnecessary to preallocate memory **/

//		   if ( (coord = ( double * ) calloc(
//		         npoin*ndimn,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for coord buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   else {
//		      printf("\ncalloc succeeded for coord buffer with size %d !\n",npoin*ndimn);
//		   }
//		   if ( (unkno = ( double * ) calloc(
//		         npoin*9,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for unkno buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (rho = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for rho buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (bx = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for bx buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (by = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for by buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (bz = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for bz buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (ux = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for ux buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (uy = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for uy buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (uz = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for uz buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (p = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for p buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (temp = ( double * ) calloc(
//		         npoin,
//		         sizeof(double)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for T buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (intmat = ( int * ) calloc(
//		         nelem*4,
//		         sizeof(int)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for intmat buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (bconi = ( int * ) calloc(
//		         nconi*nboun,
//		         sizeof(int)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for bconi buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (indx = ( int * ) calloc(
//		         nelem,
//		         sizeof(int)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for indx buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (esup1 = ( int * ) calloc(
//		         nelem*4,
//		         sizeof(int)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for esup1 buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//		   if ( (esup2 = ( int * ) calloc(
//		         npoin+1,
//		         sizeof(int)) ) == NULL)
//		   {
//		      printf("\ncalloc failed for esup2 buffer!\n");
//		      exit( EXIT_FAILURE);
//		   }
//
//		   if ( DEBUG_FLAG)
//		      printf("DEBUG\tspace allocated for variables...\n");

		   /***************************** get array values **********************/

		   /* No need to pre-load variables for this application */

		   /* set the flag that records whether the setup has been done for unstructured grid use */
		    unstructured_grid_setup_done=-1;             /* not yet done */
		    printf("DEBUG\tunstructured_grid_setup_done marked undone : %d\n",unstructured_grid_setup_done);

		}

		int load_adapt3d_cdf_variable_into_main_memory(
		      char *variable_to_read)
		{

		   CDFstatus status;
		   int       istatus;

		   long recordStart = 0;
		   long recordCount = 1;
		   long recordInterval = 1;
		   long counts[1] = { 0 };
		   long indices[1];
		   long intervals[1] = { 1 };
		   long counts1[1] = { 0 };
		   long counts2[1] = { 0 };
		   long counts3[1] = { 0 };
		   long counts4[1] = { 0 };

		   long variable_cdf_number;

		   int adapt3d_reserve_mem_and_set_cdf_num(
		         char *);

		   printf("entered load_adapt3d_cdf_variable_into_main_memory \n");

		   variable_cdf_number = adapt3d_reserve_mem_and_set_cdf_num(variable_to_read);

		   if (variable_cdf_number == -1)
		   {
		      printf(
		            "ERROR:\tcould not find cdf variable number for -->%s<--\n",
		            variable_to_read);
		      return EXIT_FAILURE; /*exit( EXIT_FAILURE );*/
		   }

		   /*(printf("DEBUG\tback from reserve mem with cdf_varNum %d for variable %s \n", variable_cdf_number, variable_to_read );*/

		   counts[0] = 3*npoin;
		   counts1[0] = npoin;
		   counts2[0] = 9*npoin;
		   counts3[0] = 4*nelem;
		   counts4[0] = nconi;
		   printf("counts[0]=%d\n", counts[0]);
		   indices[0] = 0;
		   intervals[0] = 1;

		   if ( !strcmp(
		         variable_to_read,
		         coord_name) )
		   {
		      printf("calling CDFlib for variable_cdf_number %d\n",variable_cdf_number);
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, coord,
		      NULL_);
		      printf("left CDFlib \n");
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         unkno_name) )
		   {
		      printf("calling CDFlib for variable_cdf_number %d\n",variable_cdf_number);
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts2,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, unkno,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         rho_name) )
		   {
		      printf("calling CDFlib for variable_cdf_number %d\n",variable_cdf_number);
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, rho,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         bx_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, bx,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         by_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, by,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         bz_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, bz,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         ux_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, ux,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         uy_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, uy,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         uz_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, uz,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         p_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, p,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         temp_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts1,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, temp,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         intmat_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts3,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, intmat,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else if ( !strcmp(
		         variable_to_read,
		         bconi_name) )
		   {
		      status = CDFlib(
		      SELECT_, zVAR_, variable_cdf_number,
		      SELECT_, zVAR_RECNUMBER_, recordStart,
		      SELECT_, zVAR_RECCOUNT_, recordCount,
		      SELECT_, zVAR_RECINTERVAL_, recordInterval,
		      SELECT_, zVAR_DIMINDICES_, indices,
		      SELECT_, zVAR_DIMCOUNTS_, counts4,
		      SELECT_, zVAR_DIMINTERVALS_, intervals,
		      GET_, zVAR_HYPERDATA_, bconi,
		      NULL_);
		      if (status != FileReader::OK)
		         StatusHandler(status);
		   }
		   else
		   {
		      /* INSERT ERROR HANDLER HERE */
		      printf("couldn't find a variable to load...\n");
		      return EXIT_FAILURE;
		   }

		   /*printf("DEBUG\tfinished hyper read returning\n");*/

		   /* if necessary run setup for efficient searching of unstructured grids */
		   if ( !unstructured_grid_setup_done ) {


		       printf("Calling setup_search_unstructured_grid \n");
		/*       istatus=setup_search_unstructured_grid(npoin,nelem,intmat,coord,indx); */
		       istatus=setup_search_unstructured_grid();
		       printf("Exited setup_search_unstructured_grid: istatus=%d \n",istatus);


		       printf("\nCalling  smart_search_setup \n");
		/*       smart_search_setup(nelem,npoin,intmat); */
		       smart_search_setup();
		       printf("Exited smart_search_setup \n");

		       last_element_found = -1;
		       unstructured_grid_setup_done=1;
		   }


		   return EXIT_SUCCESS;

		}

		int adapt3d_reserve_mem_and_set_cdf_num(
		      char *variable_number_to_get)
		{
		   int buffer_size = npoin*3;
		   int buffer_size1 = npoin;
		   int buffer_size2 = nelem*4;
		   int buffer_size3 = nconi*nboun;
		   int buffer_size4 = npoin*9;

		   /*printf("DEBUG\tinside resrve mem\n");*/

		   /* return cdf variable Number for given variable */
/*
		   if ( !strcmp(
		         variable_number_to_get,
		         coord_name) )
		   {
		      if ( (coord = ( double * ) calloc(
		            buffer_size,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               coord_name);
		         exit( EXIT_FAILURE);
		      }
		      else {
		         printf(
		               "\ncalloc done for %s buffer with size %d !\n",
		               coord_name,buffer_size);
		      }
		      return coord_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         unkno_name) )
		   {
		      if ( (unkno = ( double * ) calloc(
		            buffer_size4,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               unkno_name);
		         exit( EXIT_FAILURE);
		      }
		      return unkno_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         rho_name) )
		   {
		      if ( (rho = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               rho_name);
		         exit( EXIT_FAILURE);
		      }
		      return rho_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         bx_name) )
		   {
		      if ( (bx = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               bx_name);
		         exit( EXIT_FAILURE);
		      }
		      return bx_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         by_name) )
		   {
		      if ( (by = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               by_name);
		         exit( EXIT_FAILURE);
		      }
		      return by_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         bz_name) )
		   {
		      if ( (bz = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               bz_name);
		         exit( EXIT_FAILURE);
		      }
		      return bz_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         ux_name) )
		   {
		      if ( (ux = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               ux_name);
		         exit( EXIT_FAILURE);
		      }
		      return ux_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         uy_name) )
		   {
		      if ( (uy = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               uy_name);
		         exit( EXIT_FAILURE);
		      }
		      return uy_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         uz_name) )
		   {
		      if ( (uz = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               uz_name);
		         exit( EXIT_FAILURE);
		      }
		      return uz_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         p_name) )
		   {
		      if ( (p = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               p_name);
		         exit( EXIT_FAILURE);
		      }
		      return p_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         temp_name) )
		   {
		      if ( (temp = ( double * ) calloc(
		            buffer_size1,
		            sizeof(double)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               temp_name);
		         exit( EXIT_FAILURE);
		      }
		      return temp_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         intmat_name) )
		   {
		      if ( (intmat = ( int * ) calloc(
		            buffer_size2,
		            sizeof(int)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               intmat_name);
		         exit( EXIT_FAILURE);
		      }
		      return intmat_cdf_varNum;
		   }
		   else if ( !strcmp(
		         variable_number_to_get,
		         bconi_name) )
		   {
		      if ( (bconi = ( int * ) calloc(
		            buffer_size3,
		            sizeof(int)) ) == NULL)
		      {
		         printf(
		               "\ncalloc failed for %s buffer!\n",
		               bconi_name);
		         exit( EXIT_FAILURE);
		      }
		      return bconi_cdf_varNum;
		   }
		   else
		   {
		      return -1;
		   }*/
		   /*else { printf("ERROR:\tcould not find cdf variable number for %s\n", variable ); return 0; }*/

		}

	/**
	 *
	 * @copydoc Model::initializeSIUnits()
	 *
	 */
	void Adapt3D::initializeSIUnits()
	{
		variableSIUnits[ccmc::strings::variables::bx_] = "T";
		variableSIUnits[ccmc::strings::variables::by_] = "T";
		variableSIUnits[ccmc::strings::variables::bz_] = "T";
		variableSIUnits[ccmc::strings::variables::ux_] = "m/s";
		variableSIUnits[ccmc::strings::variables::uy_] = "m/s";
		variableSIUnits[ccmc::strings::variables::uz_] = "m/s";
		variableSIUnits[ccmc::strings::variables::jx_] = "A/m^2";
		variableSIUnits[ccmc::strings::variables::jy_] = "A/m^2";
		variableSIUnits[ccmc::strings::variables::jz_] = "A/m^2";
		variableSIUnits[ccmc::strings::variables::b1x_] = "T";
		variableSIUnits[ccmc::strings::variables::b1y_] = "T";
		variableSIUnits[ccmc::strings::variables::b1z_] = "T";
		variableSIUnits[ccmc::strings::variables::rho_] = "m^-3";
		variableSIUnits[ccmc::strings::variables::p_] = "Pa";
		variableSIUnits[ccmc::strings::variables::e_] = "J/m^3";
	}


	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void Adapt3D::initializeConversionFactorsToSI()
	{

		/**
		 * TODO: fix these conversion factors
		 */
		conversionFactorsToSI[ccmc::strings::variables::bx_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::by_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::bz_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::ux_] = 1e3f;
		conversionFactorsToSI[ccmc::strings::variables::uy_] = 1e3f;
		conversionFactorsToSI[ccmc::strings::variables::uz_] = 1e3f;
		conversionFactorsToSI[ccmc::strings::variables::jx_] = 1e-6f;
		conversionFactorsToSI[ccmc::strings::variables::jy_] = 1e-6f;
		conversionFactorsToSI[ccmc::strings::variables::jz_] = 1e-6f;
		conversionFactorsToSI[ccmc::strings::variables::b1x_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::b1y_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::b1z_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::rho_] = 1e6;
		conversionFactorsToSI[ccmc::strings::variables::p_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::e_] = 1e-15f;

	}


	/**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator * Adapt3D::createNewInterpolator()
	{
		Interpolator * interpolator = new Adapt3DInterpolator(this);
		return interpolator;
	}

	/**
	 * Destructor
	 */
	Adapt3D::~Adapt3D()
	{
		// TODO Auto-generated destructor stub
//		std::cout << "deleting Adapt3D object" << std::endl;
	}
}
