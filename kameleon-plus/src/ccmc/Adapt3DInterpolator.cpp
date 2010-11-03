/*
 * Adapt3DInterpolator.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: dberrios
 */

#include "Adapt3DInterpolator.h"
#include "StringConstants.h"
#include "MathHelper.h"
#include <stdio.h>

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
		this->nboun = (modelReader->getGlobalAttribute(ccmc::strings::variables::nboun_)).getAttributeInt();
		this->nconi = (modelReader->getGlobalAttribute(ccmc::strings::variables::nconi_)).getAttributeInt();
		this->coord = (modelReader->getVariableData(ccmc::strings::variables::coord_));
		this->setupUnstructuredGridSearch();
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
		long variable_id = modelReader->getVariableID(variable);
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable_id
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @return
	 */
	float Adapt3DInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2)
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
	float Adapt3DInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{

		return 0.0f;
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
		return 0.0f;
	}



	bool Adapt3DInterpolator::setupUnstructuredGridSearch()
	{

		/*-----------------------------------------------------------------
		!
		!
		! This routine sets up a look-up style index for use in searching
		! for the element containing any arbitrary location in an unstructured
		! grid.
		!
		!
		!-----------------------------------------------------------------*/



		int ipa,ipb,ipc,ipd;
		int i_s,j_s,k_s,ielem;
		int i,j,k;
		int ii;
		int nelems_in_cell[nz_sg][ny_sg][nx_sg];
		int countup, countdown;


		double xlo,xhi;
		double ylo,yhi;
		double zlo,zhi;
		double xmean,ymean,zmean;
		double side_l_1,side_l_2,side_l_3;
		double side_l_4,side_l_5,side_l_6;
		double max_element_length,max_length_sqrd;
		double max_length_sqrd_old;
		double dxyz[3];
		double dxyz_min;
		double arr2[2];
		double arr4[4];
		double arr7[7];
		long   len;


		/*-----------------------------------------------------------------*/
		printf("Entered Structured Search Grid\n");


		/* allocate for ELEM_INDEX_STRUCT */
		printf("Begin allocation of elem_index_struct \n");
		int (* elem_index_struct)[3] = new int[nelem][3];
		//int (*elem_index_struct)[3] = (int (*)[3])malloc(sizeof(int)*(int)nelem*3);
		if(elem_index_struct == NULL)
		{
			/*printf("Error: allocation of elem_index_struct failed\n");
			return 1;*/

			//figure out the correct way of checking the success of the allocation;
		}
		printf("Allocation of elem_index_struct complete \n");

		last_element_found = -1;


		xl_sg=1.e30;
		xr_sg=-1.e30;
		yl_sg=1.e30;
		yr_sg=-1.e30;
		zl_sg=1.e30;
		zr_sg=-1.e30;

		printf("npoin,ndimn %d %d \n",npoin, ndimn);

		/* coord is a 1D vector where the first ndimn words are x,y,z of node 0, the next ndimn words
		* for node 1, etc
		*/
		for ( i=0; i<(int)npoin; i++) {
			xl_sg=min(xl_sg,(*coord)[ index_2d_to_1d(i,0,npoin,ndimn) ]);
			xr_sg=max(xr_sg,(*coord)[ index_2d_to_1d(i,0,npoin,ndimn) ]);
			yl_sg=min(yl_sg,(*coord)[ index_2d_to_1d(i,1,npoin,ndimn) ]);
			yr_sg=max(yr_sg,(*coord)[ index_2d_to_1d(i,1,npoin,ndimn) ]);
			zl_sg=min(zl_sg,(*coord)[ index_2d_to_1d(i,2,npoin,ndimn) ]);
			zr_sg=max(zr_sg,(*coord)[ index_2d_to_1d(i,2,npoin,ndimn) ]);
		}

		printf("-------------------------------\n");
		printf("Range of Structured Search Grid\n");
		printf("xl_sg= %e \n",xl_sg);
		printf("xr_sg= %e \n",xr_sg);
		printf("yl_sg= %e \n",yl_sg);
		printf("yr_sg= %e \n",yr_sg);
		printf("zl_sg= %e \n",zl_sg);
		printf("zr_sg= %e \n",zr_sg);
		printf("-------------------------------\n");


		/* Step 1 - Define structured grid */

		dx_sg = (xr_sg - xl_sg)/( (double)nx_sg );
		dy_sg = (yr_sg - yl_sg)/( (double)ny_sg );
		dz_sg = (zr_sg - zl_sg)/( (double)nz_sg );
		dxyz[0] = dx_sg;
		dxyz[1] = dy_sg;
		dxyz[2] = dz_sg;
		len=3;
		dxyz_min = ccmc::Math::dfindmin(dxyz,len);


		/* Initialize the counters for the number of elements in each grid cell */
		for ( k=0; k<nz_sg; k++)
		{
			for ( j=0; j<ny_sg; j++)
			{
				for ( i=0; i<nx_sg; i++)
				{
					nelems_in_cell[k][j][i] = 0;
				}
			}
		}


		/* Step 2 - Compute index relative to structured grid for each element of
			the unstructured grid, using the element mid-point coord.
		*/

		max_length_sqrd_old=0.;
		max_length_sqrd=0.;


		/* intmat is a 1D vector where the first 4 words are the node numbers of element 0, the next 4 words
		* for element 1, etc
		*/
		for ( ielem=0; ielem<(int)nelem; ielem++)
		{

			ipa = (*intmat)[ index_2d_to_1d(ielem,0,nelem,4) ] -1 ;
			ipb = (*intmat)[ index_2d_to_1d(ielem,1,nelem,4) ] -1 ;
			ipc = (*intmat)[ index_2d_to_1d(ielem,2,nelem,4) ] -1 ;
			ipd = (*intmat)[ index_2d_to_1d(ielem,3,nelem,4) ] -1 ;
			/*
			 *  coord is a 1D vector where the first ndimn words are x,y,z of node 0, the next ndimn words
			 * for node 1, etc
			 */
			side_l_1= pow((*coord)[ index_2d_to_1d(ipa,0,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipb,0,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipa,1,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipb,1,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipa,2,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipb,2,npoin,ndimn) ],2) ;
			side_l_2= pow((*coord)[ index_2d_to_1d(ipa,0,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipc,0,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipa,1,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipc,1,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipa,2,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipc,2,npoin,ndimn) ],2) ;
			side_l_3= pow((*coord)[ index_2d_to_1d(ipa,0,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,0,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipa,1,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,1,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipa,2,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,2,npoin,ndimn) ],2) ;
			side_l_4= pow((*coord)[ index_2d_to_1d(ipb,0,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipc,0,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipb,1,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipc,1,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipb,2,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipc,2,npoin,ndimn) ],2) ;
			side_l_5= pow((*coord)[ index_2d_to_1d(ipb,0,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,0,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipb,1,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,1,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipb,2,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,2,npoin,ndimn) ],2) ;
			side_l_6= pow((*coord)[ index_2d_to_1d(ipc,0,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,0,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipc,1,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,1,npoin,ndimn) ],2) +
				   pow((*coord)[ index_2d_to_1d(ipc,2,npoin,ndimn) ]-(*coord)[ index_2d_to_1d(ipd,2,npoin,ndimn) ],2) ;

			arr7[0] = max_length_sqrd;
			arr7[1] = side_l_1;
			arr7[2] = side_l_2;
			arr7[3] = side_l_3;
			arr7[4] = side_l_4;
			arr7[5] = side_l_5;
			arr7[6] = side_l_6;

			max_length_sqrd=ccmc::Math::dfindmax(arr7,7);

			arr4[0] = (*coord)[ index_2d_to_1d(ipa,0,npoin,ndimn) ];
			arr4[1] = (*coord)[ index_2d_to_1d(ipb,0,npoin,ndimn) ];
			arr4[2] = (*coord)[ index_2d_to_1d(ipc,0,npoin,ndimn) ];
			arr4[3] = (*coord)[ index_2d_to_1d(ipd,0,npoin,ndimn) ];
			xlo = ccmc::Math::dfindmin(arr4,4);
			xhi = ccmc::Math::dfindmax(arr4,4);
			arr4[0] = (*coord)[ index_2d_to_1d(ipa,1,npoin,ndimn) ];
			arr4[1] = (*coord)[ index_2d_to_1d(ipb,1,npoin,ndimn) ];
			arr4[2] = (*coord)[ index_2d_to_1d(ipc,1,npoin,ndimn) ];
			arr4[3] = (*coord)[ index_2d_to_1d(ipd,1,npoin,ndimn) ];
			ylo = ccmc::Math::dfindmin(arr4,4);
			yhi = ccmc::Math::dfindmax(arr4,4);
			arr4[0] = (*coord)[ index_2d_to_1d(ipa,2,npoin,ndimn) ];
			arr4[1] = (*coord)[ index_2d_to_1d(ipb,2,npoin,ndimn) ];
			arr4[2] = (*coord)[ index_2d_to_1d(ipc,2,npoin,ndimn) ];
			arr4[3] = (*coord)[ index_2d_to_1d(ipd,2,npoin,ndimn) ];
			zlo = ccmc::Math::dfindmin(arr4,4);
			zhi = ccmc::Math::dfindmax(arr4,4);

			xmean = 0.5*(xlo+xhi);
			ymean = 0.5*(ylo+yhi);
			zmean = 0.5*(zlo+zhi);


			i_s = (int)( (xmean-xl_sg)/dx_sg ) ;
			j_s = (int)( (ymean-yl_sg)/dy_sg ) ;
			k_s = (int)( (zmean-zl_sg)/dz_sg ) ;
			elem_index_struct[ielem][0] = i_s;
			elem_index_struct[ielem][1] = j_s;
			elem_index_struct[ielem][2] = k_s;
			nelems_in_cell[k_s][j_s][i_s] = nelems_in_cell[k_s][j_s][i_s] + 1;



			#ifdef DEBUGX
			if(ielem < 10)
			{
				printf("indexes %d %d %d %d \n",
				index_2d_to_1d(ielem,0,nelem,4),
				index_2d_to_1d(ielem,1,nelem,4),
				index_2d_to_1d(ielem,2,nelem,4),
				index_2d_to_1d(ielem,3,nelem,4));
				printf("npoin ndimn %d %d\n",npoin,ndimn);
				printf("ielem %d xmean ymean zmean %e %e %e \n",ielem,xmean,ymean,zmean);
				printf("ielem %d xlo ylo zlo %e %e %e \n",ielem,xlo,ylo,zlo);
				printf("ielem %d xhi yhi zhi %e %e %e \n",ielem,xhi,yhi,zhi);
				printf("ielem %d ipa ipb ipc ipd %d %d %d %d \n",ielem,ipa,ipb,ipc,ipd);
				printf("ielem %d dx_sg dy_sg dz_sg %e %e %e \n",ielem,dx_sg, dy_sg, dz_sg);
				printf("ielem %d i_s j_s k_s %d %d %d \n",ielem,i_s,j_s,k_s);
				printf("ielem %d i_s j_s k_s %d %d %d \n",ielem,i_s,j_s,k_s);
				printf("ielem %d nelems_in_cell[k_s][j_s][i_s]=%d \n",ielem,nelems_in_cell[k_s][j_s][i_s]);
			}
			if(elem_index_struct[ielem][0] > nx_sg)
			{
				printf("ielem %d elem_index_struct[ielem][0] too big %d %d \n",ielem,elem_index_struct[ielem][0]);
			}
			if(elem_index_struct[ielem][1] > ny_sg)
			{
				printf("ielem %d elem_index_struct[ielem][1] too big %d %d \n",ielem,elem_index_struct[ielem][1]);
			}
			if(elem_index_struct[ielem][2] > nz_sg)
			{
				printf("ielem %d elem_index_struct[ielem][2] too big %d %d \n",ielem,elem_index_struct[ielem][2]);
			}
			#endif

		}
		max_element_length = sqrt(max_length_sqrd);

		printf("Maximum element length = %e \n",max_element_length);
		printf("Grid cell spacing = %e \n",dxyz_min);

		printf("Ratio of search grid spacing to max element length (Must be greater than 1) = %e \n",
								   dxyz_min/max_element_length);
		if(max_element_length > dxyz_min) {
			printf("ERROR: UNSTRUCTURED SEARCH GRID SPACING IS TOO FINE \n");
			printf("ERROR: SOLUTION - INCREASE NO OF GRID POINTS BY FACTOR %e \n", max_element_length/dxyz_min);
			exit;
		}


		/* Step 3 - place starting and ending indeces of element list into each structured cell */

		/* Create start and end pointers for each grid cell's section of the
		index to the element list */
		countup   = 0;
		countdown = nelem-1;
		for ( k=0; k<nz_sg; k++ )
		{
			for ( j=0; j<ny_sg; j++ )
			{
				for ( i=0; i<nx_sg; i++ )
				{
					start_index[k][j][i] = countup;
					countup = countup + nelems_in_cell[k][j][i];
					end_index[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i] = countdown;
					countdown = countdown - nelems_in_cell[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i];
				}
			}
		}
		for ( k=0; k<nz_sg; k++ )
		{
			for ( j=0; j<ny_sg; j++ )
				{
					for ( i=0; i<nx_sg; i++ )
					{
						end_index[k][j][i] = max(start_index[k][j][i],end_index[k][j][i]);
					}
			}
		}


		/* Step 4 - Create the index */
		for ( k=0; k<nz_sg; k++)
		{
			for ( j=0; j<ny_sg; j++)
			{
				for ( i=0; i<nx_sg; i++)
				{
					nelems_in_cell[k][j][i] = 0;
				}
			}
		}
		for ( ielem=0; ielem<nelem; ielem++)
		{
			i = elem_index_struct[ielem][0];
			j = elem_index_struct[ielem][1];
			k = elem_index_struct[ielem][2];
			ii = start_index[k][j][i] + nelems_in_cell[k][j][i];
			indx[ii] = ielem;
			nelems_in_cell[k][j][i] = nelems_in_cell[k][j][i] + 1;
		}

		delete(elem_index_struct);

		/*     end subroutine setup_search_unstructured_grid */
		return true;

	}

	void Adapt3DInterpolator::smartSearchSetup()
	{

		/*
		!
		!
		! Written:    Original by Hong Luo
		!             Modified by Peter MacNeice - Aug 2010
		!
		*/


	   int ip,ie,inode,nstor,ielem,istor;
	   int nnodes, ipoin;

		/*----------------------------------------------------------------
		! Start of smart search setup
		!----------------------------------------------------------------*/

		nnodes = nelem*nnode;     /* number of element/node couples */




		/*
		!
		!...  this sub finds the elements that surround each point
		!
		!...  loop over the points, seeing how many elements surround each point
		!
		!...  set esup2=0
		!
		*/

		for( ip=0; ip<npoin+1; ip++)
		{
			esup2[ip] = 0;
		}

		/*
		!
		!...  loop over the elements, storing 'ahead'
		!
		*/
		for( ie=0; ie<nelem; ie++)
		{
			for( inode=0; inode<nnode; inode++)
			{
				ip         = (*intmat)[ index_2d_to_1d(ie,inode,nelem,4) ];
				esup2[ip]  = esup2[ip] + 1;
			}
		}
		/*
		!
		!...  reshuffle esup2
		!
		*/
		for( ip=1; ip<npoin+1; ip++)
		{
			esup2[ip] = esup2[ip] + esup2[ip-1];
		}

		nstor = esup2[npoin];

		/*
		!
		!...  check
		!
		!      if(nstor .gt. mesup) print 1,nstor
		!      if(nstor .gt. mesup) stop
		!    1 format(' please increase mesup in sub fielsup1',
		!     &     /,' needed:',i8)

		!
		!...  now store the surrounding elements in esup1
		!
		*/
		for( ielem=0; ielem<nelem; ielem++)
		{
			for( inode=0; inode<nnode; inode++)
			{
				ipoin        = (*intmat)[ index_2d_to_1d(ielem,inode,nelem,4) ] -1;
				istor        = esup2[ipoin] + 1;
				esup2[ipoin] = istor;
				esup1[istor-1] = ielem;
			}
		}
		/*
		!
		!...  finally, reorder esup2
		!
		*/
		for( ip=npoin; ip>0; ip--) {
			esup2[ip] = esup2[ip-1];
		}

		esup2[0] = 0;


		/*    int nesup    = esup2[npoin];
		printf(" nesup = %i in fielsup1 \n",nesup); */


		/* The routine locate_facing_elements takes too long to execute when
		used to optimize support fieldline tracing */
		/*      locate_facing_elements; */

		/*----------------------------------------------------------------
		! End of smart search setup
		!----------------------------------------------------------------*/


	}

	int Adapt3DInterpolator::smartSearch(double * search_point_coords)
	{
	       int lfound, mask[nnode], try_grid_search;

	       int  i,j,k,ielem,inode,jnode ;
	       int  ifound, jelem, kelem;
	       int  node_order[nnode];
	       int  i_node, j_node, k_node, k_node_hi;
	       int  next_node, i_order;

	       int  nelems_checked;
	       int  clear_cache;

	       double  shapex[nnode];
	       double  radius;

	       double  distance[nnode];


	/*----------------------------------------------------------------
	!
	! Step A
	!
	! First check the last_element_found to see if the new point is still
	! inside it. If yes, then set ifound=.true'
	*/
	       ifound = -1;
	       if( last_element_found >= 0 ) {
	#ifdef DEBUGS
	       printf("Checkin if still in last element \n");
	#endif

	         ifound = chkineln( search_point_coords, last_element_found ,shapex);
	         nelems_checked = 1;


	       }

	/*--------*/
	       if( ifound == 0 ) {
	/*--------*/

	#ifdef DEBUGS
	          printf("Point is still in starting element! \n");
	#endif
	          kelem = last_element_found;

	/*--------*/
	       } else {
	/*--------*/


	/* If we have a starting_element number set to begin the search  */
	       if( last_element_found >= 0 ) {


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
	       for (jnode=0; jnode<nnode; jnode++) {
	         mask[jnode]=1;
	         inode = (*intmat)[ index_2d_to_1d(last_element_found,jnode,nelem,4) ] -1 ;
	         distance[jnode] =
	     ((*coord)[ index_2d_to_1d(inode,0,npoin,ndimn) ]-search_point_coords[0])*((*coord)[ index_2d_to_1d(inode,0,npoin,ndimn) ]-search_point_coords[0])
	   + ((*coord)[ index_2d_to_1d(inode,1,npoin,ndimn) ]-search_point_coords[1])*((*coord)[ index_2d_to_1d(inode,1,npoin,ndimn) ]-search_point_coords[1])
	   + ((*coord)[ index_2d_to_1d(inode,2,npoin,ndimn) ]-search_point_coords[2])*((*coord)[ index_2d_to_1d(inode,2,npoin,ndimn) ]-search_point_coords[2]) ;
	       }

	/*
	!
	! Step C
	!
	! Sort the starting element nodes based on distance from the new search point
	*/
	       node_order[0]       = ccmc::Math::dminloc1d(distance,nnode,mask);
	       node_order[nnode-1] = ccmc::Math::dmaxloc1d(distance,nnode,mask);
	       mask[node_order[0]] = 0;                        /* false */
	       mask[node_order[nnode-1]] = 0;
	       if(nnode == 3) {
	         for (j=0; j<nnode; j++) {
	           if(mask[j]) node_order[1] = j;
	         }
	       }
	       if(nnode == 4) {
	         node_order[1] = ccmc::Math::dminloc1d(distance,nnode,mask);
	         node_order[2] = ccmc::Math::dmaxloc1d(distance,nnode,mask);
	       }
	       if(nnode > 4) {
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
	       while( (ifound != 0) && (i_order < nnode ) ) {
	/*++++*/

	        i_order += 1;
	        next_node = node_order[i_order];


	/* Now we search the list of elements that contain this node */
	        inode = (*intmat)[ index_2d_to_1d(last_element_found,next_node,nelem,4) ] -1 ;

	#ifdef DEBUGS
	        printf("node list for this element is %i %i %i %i \n",
	                   intmat[ index_2d_to_1d(last_element_found,0,nelem,4) ]-1,
	                   intmat[ index_2d_to_1d(last_element_found,1,nelem,4) ]-1,
	                   intmat[ index_2d_to_1d(last_element_found,2,nelem,4) ]-1,
	                   intmat[ index_2d_to_1d(last_element_found,3,nelem,4) ]-1);
	        printf("First node in list is %i ",inode);
	#endif



	        k_node    = esup2[inode]   +1 ;
	        k_node_hi = esup2[inode+1] +1 ;

	        jelem =  esup1[k_node];
	        while( (ifound != 0) && (k_node < k_node_hi) ) {

	          ifound = chkineln( search_point_coords, jelem ,shapex);

	          nelems_checked = nelems_checked + 1;
	          if(ifound != 0) {
	#ifdef DEBUGS
	             printf("Not found in elem %i \n",jelem);
	#endif
	             k_node += 1;
	             jelem =  esup1[k_node];
	#ifdef DEBUGS
	             printf("Next element to check is %i %i %i \n",jelem,i_node,i_order);
	#endif
	          }
	          if(ifound == 0) {
	            kelem = jelem;
	          }
	#ifdef DEBUGS
	          if(ifound == 0) {
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

	       if( ifound != 0) {
	#ifdef DEBUGS
	          printf("Smart search failed! \n");
	          printf("search_point_coords %e %e %e \n",search_point_coords[0]
	                          ,search_point_coords[1] ,search_point_coords[2]);
	#endif

	/* Check to see if the point is still within the grid bounds */
	          try_grid_search = point_within_grid(search_point_coords);

	          kelem=-1;
	          if(try_grid_search) {
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
	          if(kelem > 0) {
	              printf("Found in element %i \n",kelem);
	          } else {
	              printf("Failed to locate element in grid \n");
	          }
	#endif
	       }

	       return kelem;

	}

	int Adapt3DInterpolator::findElement(double * cintp, int clear_cache)
	{

	       int                 ielem,kelem;
	       int                 i_s,j_s,k_s,i,j,k,indx_start,indx_end;
	       int                 indx1,ifound,just_found,jelem;
	       double              x,y,z,shapex[nnode];



	         kelem=-1;
	         ielem=-1;

	         if(clear_cache == 1) last_element_found=-1;
	#ifdef DEBUG
	       printf("0find_element: coord[0][0-2] : %e %e %e \n",coord[ index_2d_to_1d(0,0,npoin,ndimn) ],coord[ index_2d_to_1d(0,1,npoin,ndimn) ],coord[ index_2d_to_1d(0,2,npoin,ndimn) ]);
	#endif

	/* If available, use the last element found to begin the search */
	         if(last_element_found != -1) {
	#ifdef DEBUG
	       printf("find_element: coord[0][0-2] : %e %e %e \n",coord[ index_2d_to_1d(0,0,npoin,ndimn) ],coord[ index_2d_to_1d(0,1,npoin,ndimn) ],coord[ index_2d_to_1d(0,2,npoin,ndimn) ]);
	#endif
	           kelem = smartSearch(cintp);
	         }

	/* If there is no starting guess for the element number in last_element_found
	   or the smart search failed, use the grid based search
	*/
	        if(kelem == -1) {


	         x = cintp[0];
	         y = cintp[1];
	         z = cintp[2];
	#ifdef DEBUG
	         printf("Searching for point x y z = %e %e %e\n",x,y,z);
	#endif
	         i_s = (int)( (x-xl_sg)/dx_sg );
	         j_s = (int)( (y-yl_sg)/dy_sg );
	         k_s = (int)( (z-zl_sg)/dz_sg );

	#ifdef DEBUG
	         printf("Located in structured cell %d %d %d\n",i_s,j_s,k_s);
	#endif
	         indx_start = start_index[k_s][j_s][i_s];
	         indx_end   = end_index[k_s][j_s][i_s];

	/* test each element between indx_start and indx_end to find the cell
	   containing coord1 = (x,y,z)         */
	#ifdef DEBUG
	         printf("Searching index list %d to %d \n",indx_start,indx_end);
	#endif

	         indx1 = indx_start;
	         ifound = 1;
	         while ( (ifound == 1) && (indx1 <= indx_end) && (indx1 > -1) )
	         {
	           jelem=indx[indx1];
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
	         for ( k=max(0,k_s-1); k<min(k_s+2,nz_sg-1); k++) {
	         for ( j=max(0,j_s-1); j<min(j_s+2,ny_sg-1); j++) {
	         for ( i=max(0,i_s-1); i<min(i_s+2,nx_sg-1); i++) {
	           if(ifound == 1) {
	             just_found=1;
	             if( ( (i != i_s) || (j != j_s) || (k != k_s) ) ) {
	               indx_start = start_index[k][j][i];
	               indx_end   = end_index[k][j][i];
	               indx1 = indx_start;
	               ifound = 1;
	               while ( (ifound == 1) && (indx1 <= indx_end) && (indx1 > -1) ) {
	                 jelem=indx[indx1];
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
	           printf("Failed to find element using smart search\n");
	           printf("Using Brute force now!\n");
	         }
	         kelem=-1;
	         if( ielem != -1) kelem=indx[ielem];

	         }
	         last_element_found=kelem;

	         return kelem;

	/*       end subroutine find_element  */
	}

	/**
	 * Destructor
	 */
	Adapt3DInterpolator::~Adapt3DInterpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
