/*
 * Adapt3DInterpolator.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: dberrios
 */

#include "Adapt3DInterpolator.h"
#include "Adapt3D.h" //include the smart values structure
#include "StringConstants.h"
#include "MathHelper.h"
#include <stdio.h>
#include <iostream>
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9
#define NNODE_ADAPT3D 4
#define LINEAR_INTERPOL


namespace ccmc
{
	/**
	 * @param modelReader Pointer to the Model object containing the appropriate variable maps.  Adapt3DInterpolator
	 * should be returned by a Adapt3D::createNewInterpolator() call.
	 */
	Adapt3DInterpolator::Adapt3DInterpolator(Model * modelReader)
	{
		// TODO Auto-generated constructor stub
		this->modelReader = modelReader;
		/***  Open should have failed previosly, so they should exist! ***/

		this->grid_reg_no = (modelReader->getGlobalAttribute(ccmc::strings::variables::grid_reg_no_)).getAttributeInt();
		this->ndimn = (modelReader->getGlobalAttribute(ccmc::strings::variables::ndimn_)).getAttributeInt();
		this->npoin = (modelReader->getGlobalAttribute(ccmc::strings::variables::npoin_)).getAttributeInt();
		this->nelem = (modelReader->getGlobalAttribute(ccmc::strings::variables::nelem_)).getAttributeInt();
		//->nboun = (modelReader->getGlobalAttribute(ccmc::strings::variables::nboun_)).getAttributeInt();
		//this->nconi = (modelReader->getGlobalAttribute(ccmc::strings::variables::nconi_)).getAttributeInt();
		this->coord = (modelReader->getVariableFromMap(ccmc::strings::variables::coord_));
		this->intmat = (modelReader->getIntVariableFromMap(ccmc::strings::variables::intmat_));
		//this->unkno = (modelReader->getVariableFromMap(ccmc::strings::variables::unkno_));

		this->smartSearchValues = ((Adapt3D*)(modelReader))->getSmartGridSearchValues();
		//indx = new int[nelem];
		//esup1 = new int[nelem*4];
		//esup2 = new int[npoin+1];
		this->nnode = NNODE_ADAPT3D;
		//this->unstructured_grid_setup_done = this->setupUnstructuredGridSearch();
		//this->smartSearchSetup();
		this->last_element_found = -1;
	}

	/**
	 * @param variable
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @return
	 */
	float Adapt3DInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{

		float dc0, dc1, dc2;
		//long variable_id = modelReader->getVariableID(variable);
		return interpolate(variable, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable_id
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @return
	 */
	float Adapt3DInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2)
	{

		float dc0, dc1, dc2;
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * Interpolation method. Note that using the variable ID is significantly faster than using the variable string.
	 * @param variable_id A long representing the variable ID.
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float Adapt3DInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{

		std::string variable = this->modelReader->getVariableName(variable_id);
		return interpolate(variable, c0,c1,c2,dc0,dc1,dc2);
	}

	/**
	 * Interpolation method. Note that using the variable ID is significantly faster than using the variable string.
	 * @param variable The input variable.
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float Adapt3DInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{

		//this->last_element_found = -1;
		if (!point_within_grid(c0,c1,c2))
			return this->missingValue;
		float rsun_in_meters = 7.0e8;

		float coord1[3];
		float unkno_local[9];

		long counts[1] = { 0 };
		long intervals[1] = { 1 };

		float interpolated_value;
		float X = c0, Y= c1, Z= c2;






		int     array_size;
		int     istatus;
		int     ielem;

		//char variable_name0[] = "intmat";
		//char variable_name1[] = "coord";

		/**
		* TODO: figure out what to do about the dc0,dc1,dc2 values
		*/
		dc0=dc1=dc2=.02f;





		/** lets see if required variables are in memory **/


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


		//char *filename="adapt3d_kameleon_soln.cdf";




		if ( variable == ccmc::strings::variables::bx_ || variable == "b1")
		{
		  //unkno_index=5;
		}
		else if ( variable == "by" || variable == "b2")
		{
		 // unkno_index=6;
		}
		else if ( variable == "bz" || variable == "b3")
		{
		 // unkno_index=7;
		}
		else if ( variable == "ux" || variable == "u1")
		{
		  //unkno_index=1;
		}
		else if ( variable == "uy" || variable == "u2")
		{
		  //unkno_index=2;
		}
		else if ( variable == "uz" || variable == "u3")
		{
		  //unkno_index=3;
		}
		else if (variable == ccmc::strings::variables::rho_)
		{
		  //unkno_index=0;
		}
		else if (variable == ccmc::strings::variables::p_ || variable == ccmc::strings::variables::e_)
		{
		  //unkno_index=4;
		}
		else if ( variable == ccmc::strings::variables::temp_ )
		{
		  //unkno_index=8;
		}
		else
		{
		  printf(
				"ERROR:\tcould not find cdf variable number for %s\n",
				variable.c_str());
		  printf(
				"Valid Variable Names for ADAPT3D :\n bx OR b1 \n by OR b2 \n bz OR b3\n ux OR u1 \n uy OR u2 \n uz OR u3 \n p OR e \n OR temp\n----------------------------------------------\n"
				);
		  return 0;
		}


			/* locate the grid element that contains the point coord1 */
		ielem = smartSearch(coord1);
#ifdef DEBUG
	   std::cerr << "ielem: " << ielem << " for position " << X << "," << Y << "," << Z << std::endl;
#endif
		interpolated_value=this->missingValue;     /* test value */

		if(ielem > -1)
		{
		   interpolated_value = interpolate_adapt3d_solution(coord1, ielem, variable);
		   last_element_found = ielem;
		} else {
			//printf("Failed to find point in grid\n");
			last_element_found = -1;
		}


		/*  return interpolated_value  */

		if (interpolated_value >= MIN_RANGE && interpolated_value <= MAX_RANGE && interpolated_value != this->missingValue)
		{

		  //std::cerr << "position: " << c0 << "," << c1 << "," << c2 << ": " << (float)interpolated_value << std::endl;
		  return (float)interpolated_value;

		}
		else
		{
		  //std::cerr << "position: " << c0 << "," << c1 << "," << c2 << ": " << this->missingValue << std::endl;
		  return this->missingValue;
		}
	}







	int Adapt3DInterpolator::smartSearch(float * search_point_coords)
	{
//#define DEBUGS
		int lfound, mask[NNODE_ADAPT3D], try_grid_search;

		int  i,j,k,ielem,inode,jnode ;
		int  ifound, jelem, kelem;
		int  node_order[NNODE_ADAPT3D];
		int  i_node, j_node, k_node, k_node_hi;
		int  next_node, i_order;

		int  nelems_checked;
		int  clear_cache;

		float  shapex[NNODE_ADAPT3D];
		float  radius;

		float  distance[NNODE_ADAPT3D];


		/*----------------------------------------------------------------
		!
		! Step A
		!
		! First check the last_element_found to see if the new point is still
		! inside it. If yes, then set ifound=.true'
		*/
		ifound = -1;
		if( last_element_found >= 0 )
		{
			#ifdef DEBUGS
			printf("Checkin if still in last element \n");
			#endif

			ifound = chkineln( search_point_coords, last_element_found ,shapex);
			nelems_checked = 1;
		}

		/*--------*/
		if( ifound == 0 )
		{
			/*--------*/

			#ifdef DEBUGS
			printf("Point is still in starting element! \n");
			std::cerr << "last_element_found: " << last_element_found << std::endl;
			#endif
			kelem = last_element_found;


			/*--------*/
		} else
		{
			/*--------*/


			/* If we have a starting_element number set to begin the search  */
			if( last_element_found >= 0 )
			{


				#ifdef DEBUGS
				printf("Point is not still in starting element! \n");
				#endif

				/*
				!
				! Step B
				!
				! Compute the distances of the new point from each of the nodes of the
				! starting_element.
				*/
				float a,b,c;
				for (jnode=0; jnode<nnode; jnode++)
				{
					mask[jnode]=1;
					inode = (*intmat)[ index_2d_to_1d(last_element_found,jnode,nelem,4) ] -1 ;
					a = ((*coord)[ index_2d_to_1d(0,inode,0,npoin) ] -search_point_coords[0]);
					b = ((*coord)[ index_2d_to_1d(1,inode,0,npoin) ] -search_point_coords[1]);
					c = ((*coord)[ index_2d_to_1d(2,inode,0,npoin) ] -search_point_coords[2]);

					distance[jnode] = a*a + b*b + c*c;
				}

				/*
				!
				! Step C
				!
				! Sort the starting element nodes based on distance from the new search point
				*/
				node_order[0]       = ccmc::Math::fminloc1d(distance,nnode,mask);
				node_order[nnode-1] = ccmc::Math::fmaxloc1d(distance,nnode,mask);
				mask[node_order[0]] = 0;                        /* false */
				mask[node_order[nnode-1]] = 0;
				//std::cerr << "-----nnode: " << nnode << std::endl;
				if(nnode == 3)
				{
					for (j=0; j<nnode; j++)
					{
						if(mask[j]) node_order[1] = j;
					}
				}
				if(nnode == 4)
				{
					node_order[1] = ccmc::Math::fminloc1d(distance,nnode,mask);
					node_order[2] = ccmc::Math::fmaxloc1d(distance,nnode,mask);
				}
				if(nnode > 4)
				{
					printf("Error : Code only works for nnode=3 or 4 ! \n");
					exit(EXIT_FAILURE);
				}


				/*
				! Step D
				!
				! Begin search through the element lists for these nodes
				*/

				i_order = -1;
				/*++++*/
				while( (ifound != 0) && (i_order < nnode ) )
				{
					/*++++*/

					i_order += 1;
					next_node = node_order[i_order];


					/* Now we search the list of elements that contain this node */
#ifdef DEBUGS
					std::cerr << "index_2d_to_1d(last_element_found,next_node,nelem,4): " << index_2d_to_1d(last_element_found,next_node,nelem,4) << std::endl;
					std::cerr << "last_element_found: " << last_element_found << " next_node: " << next_node << std::endl;
					std::cerr << "intmat.size(): " << intmat->size() << std::endl;
#endif
					inode = (*intmat)[ index_2d_to_1d(last_element_found,next_node,nelem,4) ] -1 ;

					#ifdef DEBUGS
					printf("node list for this element is %i %i %i %i \n",
					(*intmat)[ index_2d_to_1d(last_element_found,0,nelem,4) ]-1,
					(*intmat)[ index_2d_to_1d(last_element_found,1,nelem,4) ]-1,
					(*intmat)[ index_2d_to_1d(last_element_found,2,nelem,4) ]-1,
					(*intmat)[ index_2d_to_1d(last_element_found,3,nelem,4) ]-1);
					printf("First node in list is %i \n",inode);
					#endif



					k_node    = this->smartSearchValues->esup2[inode]   +1 ;
					k_node_hi = this->smartSearchValues->esup2[inode+1] +1 ;
#ifdef DEBUGS
					std::cerr << "inode: " << inode << " k_node: " << k_node << " sizeof(esup1) " << (nelem*4) << std::endl;
#endif
					jelem =  this->smartSearchValues->esup1[k_node];
					while( (ifound != 0) && (k_node < k_node_hi) )
					{

						ifound = chkineln( search_point_coords, jelem ,shapex);

						nelems_checked = nelems_checked + 1;
						if(ifound != 0)
						{
							#ifdef DEBUGS
							printf("Not found in elem %i \n",jelem);
							#endif
							k_node += 1;
							jelem =  this->smartSearchValues->esup1[k_node];
							#ifdef DEBUGS
							printf("Next element to check is %i %i %i \n",jelem,i_node,i_order);
							#endif
						}
						if(ifound == 0)
						{
							kelem = jelem;
						}
						#ifdef DEBUGS
						if(ifound == 0)
						{
							printf("Found in elem %i \n",jelem);
							printf("Found after checking %i elements \n",nelems_checked);
						}
						#endif

					}    /* while */


					/*++++*/
				}     /* while */
				/*++++*/

			}      /*   if( last_element_found .ge. 0 )  */

			/*--------*/
		}      /*   if( ifound .eq. 0)  */
		/*--------*/
		if( ifound != 0)
		{
			#ifdef DEBUGS
			printf("Smart search failed! \n");
			printf("search_point_coords %e %e %e \n",search_point_coords[0]
				  ,search_point_coords[1] ,search_point_coords[2]);
			#endif

			/* Check to see if the point is still within the grid bounds */
			try_grid_search = point_within_grid(search_point_coords);
			#ifdef DEBUGS
			std::cerr << "is point inside grid: " << try_grid_search << std::endl;
			#endif
			kelem=-1;
			if(try_grid_search)
			{
				#ifdef DEBUGS
				radius=sqrt( search_point_coords[0]*search_point_coords[0]+
					 search_point_coords[1]*search_point_coords[1]+
					 search_point_coords[2]*search_point_coords[2] );
				printf("Using grid based search \n");
				printf("search_point_coords %e %e %e \n",search_point_coords[0]
					  ,search_point_coords[1] ,search_point_coords[2]);
				printf("radius %e \n",radius);
				#endif
				clear_cache=1;
				kelem=findElement(search_point_coords,clear_cache);
			}
			#ifdef DEBUGS
			if(kelem > 0)
			{
				printf("Found in element %i \n",kelem);
			} else
			{
				printf("Failed to locate element in grid \n");
			}
			#endif
		}

		return kelem;

	}

	int Adapt3DInterpolator::findElement(float * cintp, int clear_cache)
	{

	       int                 ielem,kelem;
	       int                 i_s,j_s,k_s,i,j,k,indx_start,indx_end;
	       int                 indx1,ifound,just_found,jelem;
	       float              x,y,z,shapex[NNODE_ADAPT3D];



	         kelem=-1;
	         ielem=-1;

	         if(clear_cache == 1) last_element_found=-1;
	#ifdef DEBUG
	       printf("0find_element: coord[0][0-2] : %e %e %e \n",coord[ index_2d_to_1d(0,0,npoin) ],coord[ index_2d_to_1d(0,1,npoin) ],coord[ index_2d_to_1d(0,2,npoin) ]);
	#endif

	/* If available, use the last element found to begin the search */
	         if(last_element_found != -1) {
	#ifdef DEBUG
	       printf("find_element: coord[0][0-2] : %e %e %e \n",coord[ index_2d_to_1d(0,0,npoin) ],coord[ index_2d_to_1d(0,1,npoin) ],coord[ index_2d_to_1d(0,2,npoin) ]);
	#endif
	           kelem = smartSearch(cintp);
	         }

	/* If there is no starting guess for the element number in last_element_found
	   or the smart search failed, use the grid based search
	*/
	 //kelem = -1;
	        if(kelem == -1) {


	         x = cintp[0];
	         y = cintp[1];
	         z = cintp[2];
	#ifdef DEBUG
	         printf("Searching for point x y z = %e %e %e\n",x,y,z);
	#endif
	         i_s = (int)( (x-this->smartSearchValues->xl_sg)/this->smartSearchValues->dx_sg );
	         j_s = (int)( (y-this->smartSearchValues->yl_sg)/this->smartSearchValues->dy_sg );
	         k_s = (int)( (z-this->smartSearchValues->zl_sg)/this->smartSearchValues->dz_sg );

	#ifdef DEBUG
	         printf("Located in structured cell %d %d %d\n",i_s,j_s,k_s);
	#endif
	         indx_start = this->smartSearchValues->start_index[k_s][j_s][i_s];
	         indx_end   = this->smartSearchValues->end_index[k_s][j_s][i_s];

	/* test each element between indx_start and indx_end to find the cell
	   containing coord1 = (x,y,z)         */
	#ifdef DEBUG
	         printf("Searching index list %d to %d \n",indx_start,indx_end);
	#endif

	         indx1 = indx_start;
	         ifound = 1;
	         while ( (ifound == 1) && (indx1 <= indx_end) && (indx1 > -1) )
	         {
	           jelem=this->smartSearchValues->indx[indx1];
	           ifound = chkineln(cintp ,jelem ,shapex);
	           if (ifound == 0) ielem=indx1;
	           indx1=indx1+1;
	         }
	#ifdef DEBUG
	         if (ifound == 0) {
	           printf("Found point in first cell\n");
	         } else {
	           printf("Did not find point in first cell\n");
	         }
	#endif


	/* If element has still not been found then search in list for
	    neighboring structured grid cells */
	         if (ifound == 1) {
	         for ( k=std::max(0,k_s-1); k<=std::min(k_s+1,nz_sg-1); k++) {
	         for ( j=std::max(0,j_s-1); j<=std::min(j_s+1,ny_sg-1); j++) {
	         for ( i=std::max(0,i_s-1); i<=std::min(i_s+1,nx_sg-1); i++) {
	           if(ifound == 1) {
	             just_found=1;
	             if( ( (i != i_s) || (j != j_s) || (k != k_s) ) ) {
	               indx_start = this->smartSearchValues->start_index[k][j][i];
	               indx_end   = this->smartSearchValues->end_index[k][j][i];
	               indx1 = indx_start;
	               ifound = 1;
	               while ( (ifound == 1) && (indx1 <= indx_end) && (indx1 > -1) ) {
	                 jelem=this->smartSearchValues->indx[indx1];
	                 ifound = chkineln(cintp ,jelem ,shapex);
	                 if (ifound == 0 ) {
	                   ielem=indx1;
	                   just_found=0;
	                 }
	                 indx1=indx1+1;
	               }
	             }
	#ifdef DEBUG
	             if ( (ifound == 0) && (just_found == 0) ) {
	               printf("Found point in neighbor cell %d %d %d \n",i,j,k);
	             } else {
	               printf("Did not find point in neighbor cell %d %d %d \n",i,j,k);
	             }
	#endif
	           }
	         }}}
	         }

	#ifdef DEBUG
	           printf("ielem is now ielem=%d\n",ielem);
	#endif

	         if( ielem == -1) {
	           //printf("Failed to find element using smart search\n");
	           //printf("Using Brute force now!\n");
	         }
	         kelem=-1;
	         if( ielem != -1) kelem=this->smartSearchValues->indx[ielem];

	         }
	         last_element_found=kelem;

	         return kelem;

	/*       end subroutine find_element  */
	}

	int Adapt3DInterpolator::index_2d_to_1d( int i1, int i2, int n1, int n2)
	{
	/* converts a 2D array index into a flat 1D index */
	      int idx = n2*i1 + i2;

	      return idx;
	}

	int Adapt3DInterpolator::point_within_grid( const float& c0, const float& c1, const float& c2)
	{
		/*
		!
		! This function test to see if the given point (coord) is inside
		! the grid bounds. This function requires specific knowledge of the
		! grid type and range. It will need a customized function for each model
		! used with this search.
		*/


	      float  radius;
	      int within_bounds = 1;

	      radius=std::sqrt(c0*c0+c1*c1+c2*c2);
	      if(c0 < this->smartSearchValues->xl_sg)
	      {
	    	  within_bounds = 0;

	    	  //std::cerr << "scoord[0]: " << scoord[0] << " < " << this->smartSearchValues->xl_sg << std::endl;
	      } else if(c0 > this->smartSearchValues->xr_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[0]: " << scoord[0] << " > " << this->smartSearchValues->xr_sg << std::endl;
	      } else if (c1 < this->smartSearchValues->yl_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[1]: " << scoord[1] << " < " << this->smartSearchValues->yl_sg << std::endl;
	      } else if (c1 > this->smartSearchValues->yr_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[1]: " << scoord[1] << " > " << this->smartSearchValues->yr_sg << std::endl;

	      } else if(c2 < this->smartSearchValues->zl_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[2]: " << scoord[2] << " < " << this->smartSearchValues->zl_sg << std::endl;

	      } else if(c2 > this->smartSearchValues->zr_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[2]: " << scoord[2] << " > " << this->smartSearchValues->zr_sg << std::endl;

	      }
	      if(radius < INNER_RADIUS) within_bounds = 0;
	      if(radius > OUTER_RADIUS) within_bounds = 0;


	      return within_bounds;


	}

	int Adapt3DInterpolator::point_within_grid( float * scoord )
	{
		/*
		!
		! This function test to see if the given point (coord) is inside
		! the grid bounds. This function requires specific knowledge of the
		! grid type and range. It will need a customized function for each model
		! used with this search.
		*/


	      float  radius;
	      int within_bounds = 1;

	      radius=std::sqrt(scoord[0]*scoord[0]+scoord[1]*scoord[1]+scoord[2]*scoord[2]);
	      if(scoord[0] < this->smartSearchValues->xl_sg)
	      {
	    	  within_bounds = 0;

	    	  //std::cerr << "scoord[0]: " << scoord[0] << " < " << this->smartSearchValues->xl_sg << std::endl;
	      } else if(scoord[0] > this->smartSearchValues->xr_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[0]: " << scoord[0] << " > " << this->smartSearchValues->xr_sg << std::endl;
	      } else if (scoord[1] < this->smartSearchValues->yl_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[1]: " << scoord[1] << " < " << this->smartSearchValues->yl_sg << std::endl;
	      } else if (scoord[1] > this->smartSearchValues->yr_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[1]: " << scoord[1] << " > " << this->smartSearchValues->yr_sg << std::endl;

	      } else if(scoord[2] < this->smartSearchValues->zl_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[2]: " << scoord[2] << " < " << this->smartSearchValues->zl_sg << std::endl;

	      } else if(scoord[2] > this->smartSearchValues->zr_sg)
	      {
	    	  within_bounds = 0;
	    	  //std::cerr << "scoord[2]: " << scoord[2] << " > " << this->smartSearchValues->zr_sg << std::endl;

	      }
	      if(radius < INNER_RADIUS) within_bounds = 0;
	      if(radius > OUTER_RADIUS) within_bounds = 0;


	      return within_bounds;

	}

    int Adapt3DInterpolator::chkineln( float * cintp ,int ielem , float * shapex)
	{
//#define DEBUG
	/*
	!...  mesh arrays
	*/
	/*
		integer,intent(in) ::  ndimn,npoin,nnode,nelem
		integer,intent(in) ::  intmat(nnode,nelem)
		integer,intent(in) ::  ielem
		real*8,intent(in)  ::  coord(ndimn,npoin),cintp(ndimn)

		real*8,intent(out) ::  shapex(nnode)
	*/

		int   ipa,ipb,ipc,ipd;
		int   ierro;
		float xa,ya,za,xba,yba,zba,xca,yca,zca,xda,yda,zda;
		float xpa,ypa,zpa;
		float deter,detin,shmin,shmax;
		float rin11,rin12,rin13;
		float rin21,rin22,rin23;
		float rin31,rin32,rin33;


	/*
	!       data tolow/ -0.005 /
	!       data tolhi/  1.005 /
	!       data   c00/  0.0   /
	!       data   c10/  1.0   /
	!
	!...  this sub sees if element ielem contains point cintp,
	!     writing the shape-function values into ==> shape
	!
	!...  find the local coordinates
	!
	*/

		ipa = (*intmat)[ index_2d_to_1d(ielem,0,nelem,4) ]-1;
		ipb = (*intmat)[ index_2d_to_1d(ielem,1,nelem,4) ]-1;
		ipc = (*intmat)[ index_2d_to_1d(ielem,2,nelem,4) ]-1;
		ipd = (*intmat)[ index_2d_to_1d(ielem,3,nelem,4) ]-1;

		//std::cerr << "npoin: " << npoin << " ndimn: " << ndimn << " ipa: " << ipa << " ipb: " << ipb << " ipc: " << ipc << " ipd: " << ipd << std::endl;
		xa  = (*coord)[ index_2d_to_1d(0,ipa,0,npoin) ];
		ya  = (*coord)[ index_2d_to_1d(1,ipa,0,npoin) ];
		za  = (*coord)[ index_2d_to_1d(2,ipa,0,npoin) ];
		xba = (*coord)[ index_2d_to_1d(0,ipb,0,npoin) ] - xa;
		yba = (*coord)[ index_2d_to_1d(1,ipb,0,npoin) ] - ya;
		zba = (*coord)[ index_2d_to_1d(2,ipb,0,npoin) ] - za;
		xca = (*coord)[ index_2d_to_1d(0,ipc,0,npoin) ] - xa;
		yca = (*coord)[ index_2d_to_1d(1,ipc,0,npoin) ] - ya;
		zca = (*coord)[ index_2d_to_1d(2,ipc,0,npoin) ] - za;
		xda = (*coord)[ index_2d_to_1d(0,ipd,0,npoin) ] - xa;
		yda = (*coord)[ index_2d_to_1d(1,ipd,0,npoin) ] - ya;
		zda = (*coord)[ index_2d_to_1d(2,ipd,0,npoin) ] - za;

		deter = xba*(yca*zda-zca*yda) - yba*(xca*zda-zca*xda) + zba*(xca*yda-yca*xda);

	#ifdef DEBUG
//		std::cerr << "xa: " << xa << " ya: " << ya << " za: " << za << " xba: " << xba;
//		std::cerr << " yba: " << yba << " zba: " << zba << " xca: " << xca << " yca: " << yca;
//		std::cerr << " zca: " << zca << " xda: " << xda << " yda: " << yda << " zda: " << zda << std::endl;
		  printf("coord[ipa]= %d %e %e %e \n",ipa,(*coord)[ index_2d_to_1d(0,ipa,0,npoin) ],(*coord)[ index_2d_to_1d(1,ipa,0,npoin) ],(*coord)[ index_2d_to_1d(2,ipa,0,npoin) ]);
		  std::cerr << "deter= " << deter << std::endl;
	#endif
	/*       detin = c10/deter */
		detin = 1.0/deter;

		rin11 = detin*(yca*zda-zca*yda);
		rin12 =-detin*(xca*zda-zca*xda);
		rin13 = detin*(xca*yda-yca*xda);
		rin21 =-detin*(yba*zda-zba*yda);
		rin22 = detin*(xba*zda-zba*xda);
		rin23 =-detin*(xba*yda-yba*xda);
		rin31 = detin*(yba*zca-zba*yca);
		rin32 =-detin*(xba*zca-zba*xca);
		rin33 = detin*(xba*yca-yba*xca);

		xpa = cintp[0]-xa;
		ypa = cintp[1]-ya;
		zpa = cintp[2]-za;
	/*
	!...  local coordinates & shape-function values
	*/
		shapex[1] = rin11*xpa + rin12*ypa + rin13*zpa;
		shapex[2] = rin21*xpa + rin22*ypa + rin23*zpa;
		shapex[3] = rin31*xpa + rin32*ypa + rin33*zpa;
		shapex[0] = 1.0 - shapex[1] - shapex[2] - shapex[3];
	#ifdef DEBUG
//		  printf("cintp= %e %e %e \n",cintp[0],cintp[1],cintp[2]);
//		  printf("xa-za= %e %e %e \n",xa,ya,za);
		  //std::cerr << "rin11: " << rin11 << " xpa: " << xpa << " rin12: " << rin12 << " ypa: " << ypa << " rin13: " << rin13 << " zpa: " << zpa << std::endl;
		std::cerr << "cintp: " << cintp[0] << "," << cintp[1] << "," << cintp[2] << std::endl;
		std::cerr << "ielem: " << ielem << " shapex = " << shapex[0] << " " << shapex[1] << " " << shapex[2] << " " << shapex[3] << std::endl;
	#endif
	/*       shape(1) = c10 - shape(2) - shape(3) - shape(4)
	!
	!...  max/min of these shape-functions
	!
	*/
		shmin = ccmc::Math::ffindmin(shapex,4);
		shmax = ccmc::Math::ffindmax(shapex,4);
	/*
	!...  see if in the element
	!
	!       if(shmin .ge. tolow .and. shmax .le. tolhi) then
	*/
		if ( (shmin > 0.) && (shmax <= 1.0) && !std::isnan(shapex[0])) {
			   ierro = 0;
		} else {
			   ierro = 1;
		}

	/*
	!...  control output
	!     write(*,*)' ielem,shmin,shmax,ierro=',ielem,shmin,shmax,ierro
	*/

	#ifdef DEBUG
		if( ierro == 0) {
		  printf("shmin= %e \n",shmin);
		  printf("shmax= %e \n",shmax);
		  printf("ierro= %d \n",ierro);
		  printf("cintp= %e %e %e \n",cintp[0],cintp[1],cintp[2]);
		  printf("node 1 = %e %e %e %d \n",(*coord)[ index_2d_to_1d(0,ipa,0,npoin) ],(*coord)[ index_2d_to_1d(1,ipa,0,npoin) ],(*coord)[ index_2d_to_1d(2,ipa,0,npoin) ],ipa);
		  printf("node 2 = %e %e %e %d \n",(*coord)[ index_2d_to_1d(0,ipb,0,npoin) ],(*coord)[ index_2d_to_1d(1,ipb,0,npoin) ],(*coord)[ index_2d_to_1d(2,ipb,0,npoin) ],ipb);
		  printf("node 3 = %e %e %e %d \n",(*coord)[ index_2d_to_1d(0,ipc,0,npoin) ],(*coord)[ index_2d_to_1d(1,ipc,0,npoin) ],(*coord)[ index_2d_to_1d(2,ipc,0,npoin) ],ipc);
		  printf("node 4 = %e %e %e %d \n",(*coord)[ index_2d_to_1d(0,ipd,0,npoin) ],(*coord)[ index_2d_to_1d(1,ipd,0,npoin) ],(*coord)[ index_2d_to_1d(2,ipd,0,npoin) ],ipd);
		 }
	#endif

		 return ierro;

	/*       end subroutine chkineln */
	}

    float Adapt3DInterpolator::interpolate_adapt3d_solution(float *coord1,int ielem, const std::string& variable)
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


           ipa = (*intmat)[ index_2d_to_1d(ielem,0,nelem,4) ]-1;
           ipb = (*intmat)[ index_2d_to_1d(ielem,1,nelem,4) ]-1;
           ipc = (*intmat)[ index_2d_to_1d(ielem,2,nelem,4) ]-1;
           ipd = (*intmat)[ index_2d_to_1d(ielem,3,nelem,4) ]-1;
           x1 = (*coord)[ index_2d_to_1d(0,ipa,0,npoin) ];
           y1 = (*coord)[ index_2d_to_1d(1,ipa,0,npoin) ];
           z1 = (*coord)[ index_2d_to_1d(2,ipa,0,npoin) ];
           x2 = (*coord)[ index_2d_to_1d(0,ipb,0,npoin) ];
           y2 = (*coord)[ index_2d_to_1d(1,ipb,0,npoin) ];
           z2 = (*coord)[ index_2d_to_1d(2,ipb,0,npoin) ];
           x3 = (*coord)[ index_2d_to_1d(0,ipc,0,npoin) ];
           y3 = (*coord)[ index_2d_to_1d(1,ipc,0,npoin) ];
           z3 = (*coord)[ index_2d_to_1d(2,ipc,0,npoin) ];
           x4 = (*coord)[ index_2d_to_1d(0,ipd,0,npoin) ];
           y4 = (*coord)[ index_2d_to_1d(1,ipd,0,npoin) ];
           z4 = (*coord)[ index_2d_to_1d(2,ipd,0,npoin) ];

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

           const std::vector<float> * vData = modelReader->getVariableFromMap(variable);
           if (vData == NULL || vData->size() == 0)
           {
        	   return this->missingValue;
           }
          /* for ( iv=0; iv<9; iv++) {

             unkno_local[iv] = 0.;
             f1 =  (a1 + b1*x + c1*y + d1*z)/vol6;
             f2 = -(a2 + b2*x + c2*y + d2*z)/vol6;
             f3 =  (a3 + b3*x + c3*y + d3*z)/vol6;
             f4 = -(a4 + b4*x + c4*y + d4*z)/vol6;

             unkno_local[iv] = f1*(*unkno)[ index_2d_to_1d(ipa,iv,npoin,NVARS_ADAPT3D) ]+f2*(*unkno)[ index_2d_to_1d(ipb,iv,npoin,NVARS_ADAPT3D) ]
                              +f3*(*unkno)[ index_2d_to_1d(ipc,iv,npoin,NVARS_ADAPT3D) ]+f4*(*unkno)[ index_2d_to_1d(ipd,iv,npoin,NVARS_ADAPT3D) ] ;

           }*/

           /*
            * int Adapt3DInterpolator::index_2d_to_1d( int i1, int i2, int n1, int n2)
			{

				  int idx = n2*i1 + i2;

				  return idx;
			}
    */
           f1 =  (a1 + b1*x + c1*y + d1*z)/vol6;
           f2 = -(a2 + b2*x + c2*y + d2*z)/vol6;
           f3 =  (a3 + b3*x + c3*y + d3*z)/vol6;
           f4 = -(a4 + b4*x + c4*y + d4*z)/vol6;

/*           std::cout << "f1: " << f1 << " f2: " << f2 << " f3: " << f3 << " f4: " << f4 << std::endl;
           std::cout << "(*vData)[ index_2d_to_1d(" << ipa << ",0," << npoin << ",1) ] " << (*vData)[ index_2d_to_1d(ipa,0,npoin,1) ] << std::endl;
           std::cout << "(*vData)[ index_2d_to_1d(" << ipb << ",0," << npoin << ",1) ] " << (*vData)[ index_2d_to_1d(ipb,0,npoin,1) ] << std::endl;
           std::cout << "(*vData)[ index_2d_to_1d(" << ipc << ",0," << npoin << ",1) ] " << (*vData)[ index_2d_to_1d(ipc,0,npoin,1) ] << std::endl;
           std::cout << "(*vData)[ index_2d_to_1d(" << ipd << ",0," << npoin << ",1) ] " << (*vData)[ index_2d_to_1d(ipd,0,npoin,1) ] << std::endl;
*/

           return f1*(*vData)[ index_2d_to_1d(ipa,0,npoin,1) ]+f2*(*vData)[ index_2d_to_1d(ipb,0,npoin,1) ]
				 +f3*(*vData)[ index_2d_to_1d(ipc,0,npoin,1) ]+f4*(*vData)[ index_2d_to_1d(ipd,0,npoin,1) ] ;

    #endif


    /*       end subroutine interpolate_solution  */
    }

	/**
	 * Destructor
	 */
	Adapt3DInterpolator::~Adapt3DInterpolator()
	{
		// TODO Auto-generated destructor stub

	}
}
