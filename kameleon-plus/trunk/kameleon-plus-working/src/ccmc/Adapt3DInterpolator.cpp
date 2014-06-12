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
//#include <boost/thread.hpp>
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9
#define NNODE_ADAPT3D 4
#define LINEAR_INTERPOL

//#define DEBUG
//#define DEBUGS
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
		this->coord = modelReader->getVariableFromMap(ccmc::strings::variables::coord_);
		this->intmat = modelReader->getIntVariableFromMap(ccmc::strings::variables::intmat_);
		//this->unkno = (modelReader->getVariableFromMap(ccmc::strings::variables::unkno_));

		this->smartSearchValues = ((Adapt3D*)(modelReader))->getSmartGridSearchValues();
		//indx = new int[nelem];
		//esup1 = new int[nelem*4];
		//esup2 = new int[npoin+1];
		this->nnode = NNODE_ADAPT3D;
		//this->unstructured_grid_setup_done = this->setupUnstructuredGridSearch();
		//this->smartSearchSetup();
		this->smartSearchValues->last_element_found = -1;
//		std::cout << "created Adapt3DInterpolator object" << std::endl;

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
		float missingValue = this->modelReader->getMissingValue();
		//this->last_element_found = -1;
		if (!point_within_grid(c0,c1,c2))
			return missingValue;
		//float rsun_in_meters = 7.0e8;
		//float unkno_local[9];

		long counts[1] = { 0 };
		long intervals[1] = { 1 };

		float interpolated_value;

		int clear_cache;





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




		counts[0] = 0; /*reset values after once through */
		intervals[0] = 1;


		/* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */
		/* also select appropriate position arrays x, y , z for bx1, by1,bz1 */

		/* set default grid arrays and change if neccessary */


		//char *filename="adapt3d_kameleon_soln.cdf";




		if ( variable == ccmc::strings::variables::bx_ || variable == ccmc::strings::variables::b1_)
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
		else if ( variable == ccmc::strings::variables::ux_ || variable == "u1")
		{
		  //unkno_index=1;
		}
		else if ( variable == ccmc::strings::variables::uy_ || variable == "u2")
		{
		  //unkno_index=2;
		}
		else if ( variable == ccmc::strings::variables::uz_ || variable == "u3")
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


		clear_cache = 0;
			/* locate the grid element that contains the point coord1 */
		//ielem = smartSearch(c0,c1,c2);
		ielem = findElement(c0,c1,c2, clear_cache);
		//Element * element = this->smartSearchValues->parent->findElement(c0, c1, c2);
		//if (element != NULL)
		//	ielem = this->smartSearchValues->parent->findElement(c0, c1, c2)->getIndex();
		//else ielem = -1;
#ifdef DEBUG
	   std::cerr << "ielem: " << ielem << " for position " << c0 << "," << c1 << "," << c2 << std::endl;
#endif
		interpolated_value = missingValue;     /* test value */

		if(ielem > -1)
		{
		   interpolated_value = interpolate_adapt3d_solution(c0, c1, c2, ielem, variable);
#ifdef DEBUG
		   std::cout << "MIN_RANGE: " << MIN_RANGE << " this->missingValue: " << this->missingValue << " interpolated_value: " << interpolated_value << std::endl;
#endif
		   this->smartSearchValues->last_element_found = ielem;
		} else {
			//printf("Failed to find point in grid\n");
			this->smartSearchValues->last_element_found = -1;
		}


		/*  return interpolated_value  */
		if (interpolated_value >= MIN_RANGE && interpolated_value <= MAX_RANGE && interpolated_value != missingValue)
		{

		  //std::cerr << "position: " << c0 << "," << c1 << "," << c2 << ": " << (float)interpolated_value << std::endl;
		  return (float)interpolated_value;

		}
		else
		{
		  //std::cerr << "position: " << c0 << "," << c1 << "," << c2 << ": " << this->missingValue << std::endl;
		  return missingValue;
		}
	}







	int Adapt3DInterpolator::smartSearch(const float& c0, const float& c1, const float& c2)
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

		float size_of_last_element;

#ifdef DELAUNEY_SEARCH
       int   iteration, iteration_max;
       int   next_element, next_element0;
       int   in0,in1,in2,in3,iselect;
       int   jnext,jk,jrand;
       int   i_s,j_s,k_s;
       float x_last_element,y_last_element,z_last_element,distance0;
       float x,y,z,r,t,p;
#endif

//std::cout << "smart search" << std::endl;
/*----------------------------------------------------------------
!
! Step A
!
! First check the last_element_found to see if the new point is still
! inside it. If yes, then set ifound=.true'
*/
		ifound = -1;
		if( this->smartSearchValues->last_element_found >= 0 )
		{
#ifdef DEBUGS
			printf("Checkin if still in last element \n");
#endif

			ifound = chkineln(c0,c1,c2, this->smartSearchValues->last_element_found ,shapex);
			nelems_checked = 1;
		}
//std::cout << "ifound: " << ifound << std::endl;
/*--------*/
		if( ifound == 0 )
		{
/*--------*/

			#ifdef DEBUGS
			printf("Point is still in starting element! \n");
			std::cerr << "this->smartSearchValues->last_element_found: " << this->smartSearchValues->last_element_found << std::endl;
			#endif
			kelem = this->smartSearchValues->last_element_found;
			//this->smartSearchValues->still_in_same_element++;


/*--------*/
		} else
		{
/*--------*/


/* If we have a starting_element number set to begin the search  */
			if( this->smartSearchValues->last_element_found >= 0 )
			{

//std::cout << "starting search" << std::endl;
#ifdef DELAUNEY_SEARCH
/* Delauney algorithm */
           ifound=1;
           iteration=0;
           next_element=this->smartSearchValues->last_element_found;
           iteration_max=DELAUNEY_ITER_MAX;
           while ((ifound != 0) && (iteration < iteration_max)) {
             kelem = chkineln(c0,c1,c2,next_element ,shapex);

             in0=(*intmat)[ index_2d_to_1d(next_element,0,4) ];
             in1=(*intmat)[ index_2d_to_1d(next_element,1,4) ];
             in2=(*intmat)[ index_2d_to_1d(next_element,2,4) ];
             in3=(*intmat)[ index_2d_to_1d(next_element,3,4) ];
             x_last_element = 0.25*(
                    (*coord)[ index_2d_to_1d(in0,0,3) ]
                   +(*coord)[ index_2d_to_1d(in1,0,3) ]
                   +(*coord)[ index_2d_to_1d(in2,0,3) ]
                   +(*coord)[ index_2d_to_1d(in3,0,3) ] );
             y_last_element = 0.25*(
                    (*coord)[ index_2d_to_1d(in0,1,3) ]
                   +(*coord)[ index_2d_to_1d(in1,1,3) ]
                   +(*coord)[ index_2d_to_1d(in2,1,3) ]
                   +(*coord)[ index_2d_to_1d(in3,1,3) ] );
             z_last_element = 0.25*(
                    (*coord)[ index_2d_to_1d(in0,2,3) ]
                   +(*coord)[ index_2d_to_1d(in1,2,3) ]
                   +(*coord)[ index_2d_to_1d(in2,2,3) ]
                   +(*coord)[ index_2d_to_1d(in3,2,3) ] );
             distance0=std::sqrt( (x_last_element-c0)*(x_last_element-c0)
                           + (y_last_element-c1)*(y_last_element-c1)
                           + (z_last_element-c2)*(z_last_element-c2) );

/* If the distance  from last element found is too great compared with the element size then force use of the structured grid */

#ifdef DEBUGS
             printf("Delauney iteration no %d\n",iteration);
             radius=sqrt(x_last_element*x_last_element+y_last_element*y_last_element+z_last_element*z_last_element);
             printf("Center of last element %d in search : radius %e\n",next_element,radius);
             printf("Center of last element %d in search %e %e %e\n",next_element,x_last_element,y_last_element,z_last_element);
        x=x_last_element;
        y=y_last_element;
        z=z_last_element;
        Math::convert_xyz_to_rthetaphi(x,y,z,&r,&t,&p);
             printf("Center of last element (rtp) %d in search %e %e %e\n",next_element,r,t,p);

           i_s = (int)( (r-this->smartSearchValues->xl_sg)/this->smartSearchValues->dx_sg );
           j_s = (int)( (t-this->smartSearchValues->yl_sg)/this->smartSearchValues->dy_sg );
           k_s = (int)( (p-this->smartSearchValues->zl_sg)/this->smartSearchValues->dz_sg );
           printf("Located in structured cell %d %d %d\n",i_s,j_s,k_s);


             printf("Distance from last element to search point is %e\n",distance0);
             printf("Search pt coords %e %e %e \n",c0,c1,c2);
             printf("Search pt radius %e\n",std::sqrt(c0*c0+c1*c1+c2*c2));
             fflush(stdout);
#endif
             next_element0=next_element;
             jrand=(int)(3.0001 *(float)rand() / (float)RAND_MAX );
             jk=0;
             iselect=-1;
             while ((iselect == -1) && (jk < 4)) {
               jnext=(jk+jrand)%4;
               if(shapex[ jnext ] < 0.) {
                 next_element=(*this->smartSearchValues->facing_elements)[ index_2d_to_1d(next_element0,jnext,4) ];
                 if(next_element > -1) {
                   iselect=jnext;
#ifdef DEBUGS
                 } else {
                  printf("Loop %d: Face %d is a boundary with next_element=%d. Skip to next face\n",jk,jnext,next_element);
                  printf("shapex %e %e %e %e\n",shapex[0],shapex[1],shapex[2],shapex[3]);
                  fflush(stdout);
#endif
                 }
#ifdef DEBUGS
                 printf("jrand=%d\n",jrand);
                 printf("iselect=%d\n",iselect);
                 fflush(stdout);
#endif
               }
               jk=jk+1;
             }


             iteration++;

/* If only face in direction of search point is a boundary then end search and reset last-element_found */
             if(next_element == -999) {
                iteration=iteration_max;
                this->smartSearchValues->last_element_found=-1;
                kelem=-1;
               // this->smartSearchValues->outside_grid += 1;
             }

             if(kelem == 0) {
               ifound=0;
#ifdef DEBUGS
                 printf("Delauney search successful : found in element %d \n",next_element);
    //             fflush(stdout);
#endif
                 if(iteration-1 < DELAUNEY_ITER_MAX) (*this->smartSearchValues->delauney_search_iteration_profile)[iteration-1] += 1;
             }
           }
           if(ifound == 0)  kelem=next_element;

/* End of Delauney algorithm */
#endif /* ifdef DELAUNEY_SEARCH */


       }      /*   if( last_element_found .ge. 0 )  */

/*--------*/
       }      /*   if( ifound .eq. 0)  */
/*--------*/


#ifdef DEBUGS
       if( ifound != 0) {
          printf("Smart search failed! \n");
          printf("search_point_coords %e %e %e \n",c0,c1,c2);
                 fflush(stdout);

          if(kelem > 0) {
              printf("Found in element %i \n",kelem);
              fflush(stdout);
          } else {
              printf("Failed to locate element in grid \n");
              fflush(stdout);
          }
       }
#endif


		return kelem;

	}

	int Adapt3DInterpolator::findElement(const float& c0, const float& c1, const float& c2, int clear_cache)
	{

//#define DEBUGS

		//std::cout << "entered findElement" << std::endl;

		int			ielem,kelem, inode;
		int         i_s,j_s,k_s,i,j,k,indx_start,indx_end;
		int         indx1,ifound,just_found,jelem;
		float       x,y,z;
        //float* shapex = new float[nnode];
		int			next_element, next_element0, iselect;

		int        in0,in1,in2,in3,iteration;
		int        delta_i;
		float      x_last_element,y_last_element,z_last_element;
		float      distance;
		float      ddx,ddy,ddz,ss,xx,yy,zz,radius;
		int        new_del,i_new,j_new,k_new;
		float      r, t, p;
		float      distance0,size_of_last_element;

		int      i_min,i_max;
		int      j_min,j_max;
		int      k_min,k_max;
		int      j0,k0;

		kelem=-1;
		ielem=-1;
		ifound=-1;
		if ( point_within_grid(c0, c1, c2) == 1)
		{

	         if(clear_cache == 1) this->smartSearchValues->last_element_found=-1;
	#ifdef DEBUG
	       printf("0find_element: (*coord)[0][0-2] : %e %e %e \n",(*coord)[ index_2d_to_1d(0,0,npoin) ],(*coord)[ index_2d_to_1d(0,1,npoin) ],(*coord)[ index_2d_to_1d(0,2,npoin) ]);
	#endif

	       /* If available, use the last element found to begin the search */
	       if(this->smartSearchValues->last_element_found != -1) {
	    	   in0=(*intmat)[ index_2d_to_1d(this->smartSearchValues->last_element_found,0,4) ];
	    	   in1=(*intmat)[ index_2d_to_1d(this->smartSearchValues->last_element_found,1,4) ];
	    	   x_last_element=(*coord)[ index_2d_to_1d(in0,0,3) ];
	    	   y_last_element=(*coord)[ index_2d_to_1d(in0,1,3) ];
	    	   z_last_element=(*coord)[ index_2d_to_1d(in0,2,3) ];
	    	   distance0=std::sqrt( (x_last_element-c0)*(x_last_element-c0)
	    			   	   	      + (y_last_element-c1)*(y_last_element-c1)
	    			   	   	      + (z_last_element-c2)*(z_last_element-c2));
	    	   size_of_last_element=std::sqrt(
	    			   ( (*coord)[ index_2d_to_1d(in0,0,3) ] -(*coord)[ index_2d_to_1d(in1,0,3) ] )*
	    			   ( (*coord)[ index_2d_to_1d(in0,0,3) ] -(*coord)[ index_2d_to_1d(in1,0,3) ] )
	    			   +( (*coord)[ index_2d_to_1d(in0,1,3) ] -(*coord)[ index_2d_to_1d(in1,1,3) ] )*
	    			   ( (*coord)[ index_2d_to_1d(in0,1,3) ] -(*coord)[ index_2d_to_1d(in1,1,3) ] )
	    			   +( (*coord)[ index_2d_to_1d(in0,2,3) ] -(*coord)[ index_2d_to_1d(in1,2,3) ] )*
	    			   ( (*coord)[ index_2d_to_1d(in0,2,3) ] -(*coord)[ index_2d_to_1d(in1,2,3) ] ) );

#ifdef DEBUGS
	    	   printf("coords %e %e %e %e %e %e\n", (*coord)[ index_2d_to_1d(in0,0,3) ],(*coord)[ index_2d_to_1d(in1,0,3) ],
	    			   (*coord)[ index_2d_to_1d(in0,1,3) ] ,(*coord)[ index_2d_to_1d(in1,1,3) ],
	    			   (*coord)[ index_2d_to_1d(in0,2,3) ] ,(*coord)[ index_2d_to_1d(in1,2,3) ] );
	    	   printf("distance to element  : %e\n",distance0);
	    	   printf("element size  : %e\n",size_of_last_element);
	    	   printf("Ratio of distance to element size  : %e\n",distance0/size_of_last_element);
#endif


	    	   /* If the distance  from last element found is too great compared with the element size then force use of the structured grid */
	    	   if (distance0/size_of_last_element > 50.) {
	    		   this->smartSearchValues->last_element_found = -1;
#ifdef DEBUGS
	    		   printf("Force new structured grid search\n");
#endif
	    	   }

	       }
	       /* If necessary generate a first guess element to begin search */
	       if(this->smartSearchValues->last_element_found == -1) {


#ifdef CARTESIAN_S_GRID
	    	   x = cintp[0];
	    	   y = cintp[1];
	    	   z = cintp[2];
#endif /* CARTESIAN_S_GRID */
#ifdef SPHERICAL_S_GRID
	    	   Math::convert_xyz_to_rthetaphi(c0,c1,c2,&r,&t,&p);
	    	   x=r;
	    	   y=t;
	    	   z=p;
#endif /* SPHERICAL_S_GRID */

#ifdef DEBUGS
	    	   printf("find_element: Searching for point x y z = %e %e %e\n",x,y,z);
#endif
	    	   i_s = (int)( (x-this->smartSearchValues->xl_sg)/this->smartSearchValues->dx_sg );
	    	   j_s = (int)( (y-this->smartSearchValues->yl_sg)/this->smartSearchValues->dy_sg );
	    	   k_s = (int)( (z-this->smartSearchValues->zl_sg)/this->smartSearchValues->dz_sg );
	    	   /* pmn fix section */
	    	   i_s = std::min(i_s,nx_sg-1);
	    	   i_s = std::max(i_s,0);
	    	   j_s = std::min(j_s,ny_sg-1);
	    	   j_s = std::max(j_s,0);
	    	   k_s = std::min(k_s,nz_sg-1);
	    	   k_s = std::max(k_s,0);
	    	   /* end pmn fix section */
#ifdef DEBUGS
	    	   printf("Located in structured cell %d %d %d\n",i_s,j_s,k_s);
#endif
	    	   if(this->smartSearchValues->nelems_in_cell[k_s][j_s][i_s] > 0) {
	    		   indx_start = this->smartSearchValues->start_index[k_s][j_s][i_s];
	    		   indx_end   = this->smartSearchValues->end_index[k_s][j_s][i_s];
	    		   delta_i=(int)( (float)(indx_end-indx_start) * ( (float)rand() / (float)RAND_MAX ) );
	    		   this->smartSearchValues->last_element_found = (*this->smartSearchValues->indx)[indx_start + delta_i];
#ifdef DEBUGS
	    		   printf("Reseting last_element_found from structured grid to %d delta_i %d\n",this->smartSearchValues->last_element_found,indx_end-indx_start);
#endif
	    	   } else {
	    		   this->smartSearchValues->last_element_found = -1;
#ifdef DEBUGS
	    		   printf("Structured cell has no elements - Trying the node list now\n");
	    		   printf("nnodes_in_cell %d %d %d is %d \n",i_s,j_s,k_s,this->smartSearchValues->nnodes_in_cell[k_s][j_s][i_s]);
	    		   fflush(stdout);
#endif
	    		   if(this->smartSearchValues->nnodes_in_cell[k_s][j_s][i_s] > 0) {
	    			   indx_start = this->smartSearchValues->start_index_nodes[k_s][j_s][i_s];
	    			   indx_end = this->smartSearchValues->end_index_nodes[k_s][j_s][i_s];
#ifdef DEBUGS
	    			   printf("nnodes_in_cell %d %d %d is %d \n",i_s,j_s,k_s,this->smartSearchValues->nnodes_in_cell[k_s][j_s][i_s]);
	    			   printf("indx_start indx_end %d %d\n",indx_start,indx_end);
	    			   fflush(stdout);
#endif
	    			   delta_i=(int)( (float)(indx_end-indx_start) * ( (float)rand() / (float)RAND_MAX ) );
	    			   last_node_found = (*this->smartSearchValues->indx_nodes)[indx_start + delta_i];
#ifdef DEBUGS
	    			   printf("last_node_found=%d\n",last_node_found);
	    			   printf("coords of last node are %e %e %e\n",(*coord)[last_node_found*3],
	    					   (*coord)[last_node_found*3+1],(*coord)[last_node_found*3+2]);
	    			   fflush(stdout);
#endif
	    			   this->smartSearchValues->last_element_found = this->smartSearchValues->esup1->at( this->smartSearchValues->esup2->at(last_node_found) );
#ifdef DEBUGS
	    			   printf("Node search found element %d as starting point for search\n", this->smartSearchValues->last_element_found);
	    			   //fflush(stdout);
#endif
	    		   } else {

#ifdef DEBUGS
	    			   printf("Structured cell is empty - Node search also failed\n");
	    			   printf("Start scanning neighbor cells using elements\n");
#endif
	    			   /* search immediate neighbor cells for search starting point */

	    			   /* Locate a neighboring cell of the structured grid that contains a node */
	    			   iselect = 1 ;

	    			   i_min = std::max(0,i_s-1);
	    			   i_max = std::min(nx_sg-1,i_s+1);
#ifdef CARTESIAN_S_GRID
	    			   j_min = max(0,j_s-1);
	    			   j_max = min(ny_sg-1,j_s+1);
	    			   k_min = max(0,k_s-1);
	    			   k_max = min(nz_sg-1,k_s+1);
#endif  /* CARTESIAN_S_GRID */
#ifdef SPHERICAL_S_GRID
	    			   j_min = std::max(j_s-1, 0);			/* pmn fix */
	    			   j_max = std::min(j_s+1, ny_sg-1); 	/* pmn fix */
	    			   k_min = k_s-1;
	    			   k_max = k_s+1;
#endif  /* SPHERICAL_S_GRID */

	    			   /* if using elements for search */
	    			   for(k=k_min;k<k_max+1;k++) {
	    				   k0=k;
#ifdef SPHERICAL_S_GRID
	    				   if(k==-1) k0=nz_sg-1;
	    				   if(k==nz_sg) k0=0;
#endif  /* SPHERICAL_S_GRID */
	    				   for(j=j_min;j<j_max+1;j++) {
	    					   j0=j;
#ifdef SPHERICAL_S_GRID
	    					   if(j==ny_sg) {
	    						   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    						   j0 = ny_sg-1;
	    					   }
	    					   if(j==-1) {
	    						   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    						   j0 = 0;
	    					   }
#endif  /* SPHERICAL_S_GRID */
	    					   for(i=i_min;i<i_max+1;i++) {
#ifdef DEBUGS
	    						   printf("cell %d %d %d nelems_in_cell %d\n",i,j0,k0,this->smartSearchValues->nelems_in_cell[k0][j0][i]);
#endif
	    						   if(this->smartSearchValues->nelems_in_cell[k0][j0][i]>0) {
	    							   iselect=0;
	    							   i_s=i;
	    							   j_s=j0;
	    							   k_s=k0;
	    						   }
	    					   }}}
	    			   if(iselect==0)  {
#ifdef DEBUGS
	    				   printf("Found a starting element in neighbor %d %d %d\n",i_s,j_s,k_s);
#endif
	    				   indx_start = this->smartSearchValues->start_index[k_s][j_s][i_s];
	    				   indx_end   = this->smartSearchValues->end_index[k_s][j_s][i_s];
	    				   delta_i=(int)( (float)(indx_end-indx_start) * ( (float)rand() / (float)RAND_MAX ) );
	    				   this->smartSearchValues->last_element_found = (*this->smartSearchValues->indx)[indx_start + delta_i];
#ifdef DEBUGS
	    				   printf("Element is %d\n",this->smartSearchValues->last_element_found);
#endif
	    			   }

	    			   /* if using nodes for search */
	    			   if(iselect == 1) {
#ifdef DEBUGS
	    				   printf("Neighbor search using elements failed also\n");
	    				   printf("Start scanning neighbor cells using nodes\n");
#endif
	    				   for(k=k_min;k<k_max+1;k++) {
	    					   k0=k;
#ifdef SPHERICAL_S_GRID
	    					   if(k==-1) k0=nz_sg-1;
	    					   if(k==nz_sg) k0=0;
#endif  /* SPHERICAL_S_GRID */
	    					   for(j=j_min;j<j_max+1;j++) {
	    						   j0=j;
#ifdef SPHERICAL_S_GRID
	    						   if(j==ny_sg) {
	    							   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    							   j0 = ny_sg-1;
	    						   }
	    						   if(j==-1) {
	    							   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    							   j0 = 0;
	    						   }
#endif  /* SPHERICAL_S_GRID */
	    						   for(i=i_min;i<i_max+1;i++) {
#ifdef DEBUGS
	    							   printf("cell %d %d %d nnodes_in_cell %d \n",i,j0,k0,this->smartSearchValues->nnodes_in_cell[k0][j0][i]);
#endif
	    							   if(this->smartSearchValues->nnodes_in_cell[k0][j0][i]>0) {
	    								   iselect=0;
	    								   i_s=i;
	    								   j_s=j0;
	    								   k_s=k0;
	    							   }
	    						   }}}
	    				   if(iselect==0)  {
#ifdef DEBUGS
	    					   printf("Found a starting node in neighbor %d %d %d\n",i_s,j_s,k_s);
#endif
	    					   indx_start = this->smartSearchValues->start_index_nodes[k_s][j_s][i_s];
	    					   indx_end = this->smartSearchValues->end_index_nodes[k_s][j_s][i_s];
	    					   delta_i=(int)( (float)(indx_end-indx_start) * ( (float)rand() / (float)RAND_MAX ) );
	    					   last_node_found = (*this->smartSearchValues->indx_nodes)[indx_start + delta_i];
#ifdef DEBUGS
	    					   printf("Node is %d\n",last_node_found);
#endif
	    					   this->smartSearchValues->last_element_found = this->smartSearchValues->esup1->at( this->smartSearchValues->esup2->at(last_node_found) );
#ifdef DEBUGS
	    					   printf("Element is %d\n",this->smartSearchValues->last_element_found);
	    				   } else {
	    					   printf("\n\n Final part of search failed\n\n\n");
#endif
	    				   }

	    				   /* end of immediate neighbor search */

	    				   /* If still not found search up to 2 neighbors away */
	    				   if(iselect == 1) {

	    					   i_min = std::max(0,i_s-2);
	    					   i_max = std::min(nx_sg-1,i_s+2);
#ifdef CARTESIAN_S_GRID
	    					   j_min = max(0,j_s-2);
	    					   j_max = min(ny_sg-1,j_s+2);
	    					   k_min = max(0,k_s-2);
	    					   k_max = min(nz_sg-1,k_s+2);
#endif  /* CARTESIAN_S_GRID */
#ifdef SPHERICAL_S_GRID
	    					   j_min = std::max(j_s-2, 0);
	    					   j_max = std::min(j_s+2, ny_sg-1);
	    					   k_min = k_s-2;
	    					   k_max = k_s+2;
#endif  /* SPHERICAL_S_GRID */

	    					   /* if using elements for search */
	    					   for(k=k_min;k<k_max+1;k++) {
	    						   k0=k;
#ifdef SPHERICAL_S_GRID
	    						   if(k==-1) k0=nz_sg-1;
	    						   if(k==nz_sg) k0=0;
	    						   if(k==-2) k0=nz_sg-2;
	    						   if(k==nz_sg+1) k0=1;
#endif  /* SPHERICAL_S_GRID */
	    						   for(j=j_min;j<j_max+1;j++) {
	    							   j0=j;
#ifdef SPHERICAL_S_GRID
	    							   if(j==ny_sg) {
	    								   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    								   j0 = ny_sg-1;
	    							   }
	    							   if(j==ny_sg+1) {
	    								   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    								   j0 = ny_sg-2;
	    							   }
	    							   if(j==-1) {
	    								   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    								   j0 = 0;
	    							   }
	    							   if(j==-2) {
	    								   k0 = (k0+nz_sg/2)%(nz_sg-1);
	    								   j0 = 1;
	    							   }
#endif  /* SPHERICAL_S_GRID */
	    							   for(i=i_min;i<i_max+1;i++) {
#ifdef DEBUGS
	    								   printf("cell %d %d %d nelems_in_cell %d\n",i,j0,k0,this->smartSearchValues->nelems_in_cell[k0][j0][i]);
#endif
	    								   if(this->smartSearchValues->nelems_in_cell[k0][j0][i]>0) {
	    									   iselect=0;
	    									   i_s=i;
	    									   j_s=j0;
	    									   k_s=k0;
	    								   }
	    							   }}}
	    					   if(iselect==0)  {
#ifdef DEBUGS
	    						   printf("Found a starting element in neighbor %d %d %d\n",i_s,j_s,k_s);
#endif
	    						   indx_start = this->smartSearchValues->start_index[k_s][j_s][i_s];
	    						   indx_end   = this->smartSearchValues->end_index[k_s][j_s][i_s];
	    						   delta_i=(int)( (float)(indx_end-indx_start) * ( (float)rand() / (float)RAND_MAX ) );
	    						   this->smartSearchValues->last_element_found = (*this->smartSearchValues->indx)[indx_start + delta_i];
#ifdef DEBUGS
	    						   printf("Element is %d\n",this->smartSearchValues->last_element_found);
#endif
	    					   }

	    				   }
	    				   /* end of 2 neighbor search */


	    			   }
	    			   /* next cell to pick from should be i_s,j_s,k_s */


	    		   }
	    	   }


	       }
//	       std::cout << "last_element_found = " << this->smartSearchValues->last_element_found << std::endl;
	       /* If available, use the last element found to begin the search */
	       if(this->smartSearchValues->last_element_found != -1) kelem = smartSearch(c0,c1,c2);


	       this->smartSearchValues->last_element_found=kelem;
	       previous_c0=c0;
	       previous_c1=c1;
	       previous_c2=c2;

#ifdef DEBUGS
	       printf("Exiting find element with last_element_found=%d\n",this->smartSearchValues->last_element_found);
#endif



		} else
		{
#ifdef DEBUGS
			//this->smartSearchValues->outside_grid += 1;
			this->smartSearchValues->last_element_found = -1;
			previous_c0 = 0.f;
			previous_c1 = 0.f;
			previous_c2 = 0.f;
#endif
		}
	         return kelem;

	/*       end subroutine find_element  */
	}

	int Adapt3DInterpolator::index_2d_to_1d( int i1, int i2, int n2)
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
	      if(c0 < this->smartSearchValues->xl_gr)
	      {
	    	  return 0;

	      } else if(c0 > this->smartSearchValues->xr_gr)
	      {
	    	  return 0;

	      } else if (c1 < this->smartSearchValues->yl_gr)
	      {
	    	  return 0;

	      } else if (c1 > this->smartSearchValues->yr_gr)
	      {
	    	  return 0;


	      } else if(c2 < this->smartSearchValues->zl_gr)
	      {
	    	  return 0;


	      } else if(c2 > this->smartSearchValues->zr_gr)
	      {
	    	  return 0;


	      }
	      if(radius < INNER_RADIUS) return 0;
	      if(radius > OUTER_RADIUS) return 0;


	      return within_bounds;


	}

	int Adapt3DInterpolator::point_within_grid( const float * scoord )
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
	      if(scoord[0] < this->smartSearchValues->xl_gr)
	      {
	    	  within_bounds = 0;

	    	  std::cerr << "scoord[0]: " << scoord[0] << " < " << this->smartSearchValues->xl_gr << std::endl;
	      } else if(scoord[0] > this->smartSearchValues->xr_gr)
	      {
	    	  within_bounds = 0;
	    	  std::cerr << "scoord[0]: " << scoord[0] << " > " << this->smartSearchValues->xr_gr << std::endl;
	      } else if (scoord[1] < this->smartSearchValues->yl_gr)
	      {
	    	  within_bounds = 0;
	    	  std::cerr << "scoord[1]: " << scoord[1] << " < " << this->smartSearchValues->yl_gr << std::endl;
	      } else if (scoord[1] > this->smartSearchValues->yr_gr)
	      {
	    	  within_bounds = 0;
	    	  std::cerr << "scoord[1]: " << scoord[1] << " > " << this->smartSearchValues->yr_gr << std::endl;

	      } else if(scoord[2] < this->smartSearchValues->zl_gr)
	      {
	    	  within_bounds = 0;
	    	  std::cerr << "scoord[2]: " << scoord[2] << " < " << this->smartSearchValues->zl_gr << std::endl;

	      } else if(scoord[2] > this->smartSearchValues->zr_gr)
	      {
	    	  within_bounds = 0;
	    	  std::cerr << "scoord[2]: " << scoord[2] << " > " << this->smartSearchValues->zr_sg << std::endl;

	      }
	      if(radius < INNER_RADIUS) within_bounds = 0;
	      if(radius > OUTER_RADIUS) within_bounds = 0;


	      return within_bounds;

	}

	void Adapt3DInterpolator::calculation1(const float& a, const float& b, const float& c, const float& d, const float& e, float& result)
	{
		result = a*(b*c-d*e);

	}



    int Adapt3DInterpolator::chkineln( const float& c0, const float& c1, const float& c2, int ielem , float * shapex)
	{

    	//
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
		ipa = intmat->at(index_2d_to_1d(ielem,0,4));
		ipb = intmat->at(index_2d_to_1d(ielem,1,4));
		ipc = intmat->at(index_2d_to_1d(ielem,2,4));
		ipd = intmat->at(index_2d_to_1d(ielem,3,4));

		//std::cerr << "npoin: " << npoin << " ndimn: " << ndimn << " ipa: " << ipa << " ipb: " << ipb << " ipc: " << ipc << " ipd: " << ipd << std::endl;
		xa  = (*coord)[index_2d_to_1d(ipa,0,3)];
		ya  = (*coord)[index_2d_to_1d(ipa,1,3)];
		za  = (*coord)[index_2d_to_1d(ipa,2,3)];
		xba = (*coord)[index_2d_to_1d(ipb,0,3)] - xa;
		yba = (*coord)[index_2d_to_1d(ipb,1,3)] - ya;
		zba = (*coord)[index_2d_to_1d(ipb,2,3)] - za;
		xca = (*coord)[index_2d_to_1d(ipc,0,3)] - xa;
		yca = (*coord)[index_2d_to_1d(ipc,1,3)] - ya;
		zca = (*coord)[index_2d_to_1d(ipc,2,3)] - za;
		xda = (*coord)[index_2d_to_1d(ipd,0,3)] - xa;
		yda = (*coord)[index_2d_to_1d(ipd,1,3)] - ya;
		zda = (*coord)[index_2d_to_1d(ipd,2,3)] - za;

		float t1 = 0.f;
		float t2 = 0.f;
		float t3 = 0.f;
		t1 = xba*(yca*zda - zca*yda);
		t2 = yba*(xca*zda - zca*xda);
		t3 = zba*(xca*yda - yca*xda);
		//a*(b*c-d*e);
//		boost::thread thread1(&Adapt3DInterpolator::calculation1,boost::ref(xba),boost::ref(yca),boost::ref(zda),boost::ref(zca),boost::ref(yda), boost::ref(t1));
//		boost::thread thread2(&Adapt3DInterpolator::calculation1,boost::ref(yba),boost::ref(xca),boost::ref(zda),boost::ref(zca),boost::ref(xda), boost::ref(t2));
//		boost::thread thread3(&Adapt3DInterpolator::calculation1,boost::ref(zba),boost::ref(xca),boost::ref(yda),boost::ref(yca),boost::ref(xda), boost::ref(t3));
//		thread1.join();
//		thread2.join();
//		thread3.join();
		//calculation1(xba,yca,zda,zca,yda, t1);
		//calculation1(yba,xca,zda,zca,xda, t2);
		//calculation1(zba,xca,yda,yca,xda, t3);
		//float deter = xba*(yca*zda-zca*yda) - yba*(xca*zda-zca*xda) + zba*(xca*yda-yca*xda);
		deter = t1 - t2 + t3;


	#ifdef DEBUG
//		std::cerr << "xa: " << xa << " ya: " << ya << " za: " << za << " xba: " << xba;
//		std::cerr << " yba: " << yba << " zba: " << zba << " xca: " << xca << " yca: " << yca;
//		std::cerr << " zca: " << zca << " xda: " << xda << " yda: " << yda << " zda: " << zda << std::endl;
		  printf("(*coord)[ipa]= %d %e %e %e \n",ipa,(*coord)[index_2d_to_1d(ipa,0, 3)],(*coord)[index_2d_to_1d(ipa,1,3)],(*coord)[index_2d_to_1d(ipa,2,3)]);
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

		xpa = c0-xa;
		ypa = c1-ya;
		zpa = c2-za;
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
		std::cerr << "cintp: " << c0 << "," << c1 << "," << c2 << std::endl;
		std::cerr << "ielem: " << ielem << " shapex = " << shapex[0] << " " << shapex[1] << " " << shapex[2] << " " << shapex[3] << std::endl;
	#endif
	/*       shape(1) = c10 - shape(2) - shape(3) - shape(4)
	!
	!...  max/min of these shape-functions
	!
	*/
		//float t = shapex[0] + shapex[1] + shapex[2] + shapex[3];
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
		  printf("cintp= %e %e %e \n",c0,c1,c2);
		  printf("node 1 = %e %e %e %d \n",(*coord)[index_2d_to_1d(ipa,0,3)],(*coord)[index_2d_to_1d(ipa,1,3)],(*coord)[index_2d_to_1d(ipa,2,3)],ipa);
		  printf("node 2 = %e %e %e %d \n",(*coord)[index_2d_to_1d(ipb,0,3)],(*coord)[index_2d_to_1d(ipb,1,3)],(*coord)[index_2d_to_1d(ipb,2,3)],ipb);
		  printf("node 3 = %e %e %e %d \n",(*coord)[index_2d_to_1d(ipc,0,3)],(*coord)[index_2d_to_1d(ipc,1,3)],(*coord)[index_2d_to_1d(ipc,2,3)],ipc);
		  printf("node 4 = %e %e %e %d \n",(*coord)[index_2d_to_1d(ipd,0,3)],(*coord)[index_2d_to_1d(ipd,1,3)],(*coord)[index_2d_to_1d(ipd,2,3)],ipd);
		 }
	#endif

		 return ierro;

	/*       end subroutine chkineln */
	}

    float Adapt3DInterpolator::interpolate_adapt3d_solution(const float& x, const float& y, const float& z, int ielem, const std::string& variable)
    {
    	float missingValue = this->modelReader->getMissingValue();
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


           ipa = intmat->at(index_2d_to_1d(ielem,0,4));
           ipb = intmat->at(index_2d_to_1d(ielem,1,4));
           ipc = intmat->at(index_2d_to_1d(ielem,2,4));
           ipd = intmat->at(index_2d_to_1d(ielem,3,4));
           x1 = (*coord)[index_2d_to_1d(ipa,0,3)];
           y1 = (*coord)[index_2d_to_1d(ipa,1,3)];
           z1 = (*coord)[index_2d_to_1d(ipa,2,3)];
           x2 = (*coord)[index_2d_to_1d(ipb,0,3)];
           y2 = (*coord)[index_2d_to_1d(ipb,1,3)];
           z2 = (*coord)[index_2d_to_1d(ipb,2,3)];
           x3 = (*coord)[index_2d_to_1d(ipc,0,3)];
           y3 = (*coord)[index_2d_to_1d(ipc,1,3)];
           z3 = (*coord)[index_2d_to_1d(ipc,2,3)];
           x4 = (*coord)[index_2d_to_1d(ipd,0,3)];
           y4 = (*coord)[index_2d_to_1d(ipd,1,3)];
           z4 = (*coord)[index_2d_to_1d(ipd,2,3)];



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

//    #ifdef LINEAR_INTERPOL
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
        	   std::cerr << "missing value" << std::endl;
        	   return missingValue;
           }
          /* for ( iv=0; iv<9; iv++) {

             unkno_local[iv] = 0.;
             f1 =  (a1 + b1*x + c1*y + d1*z)/vol6;
             f2 = -(a2 + b2*x + c2*y + d2*z)/vol6;
             f3 =  (a3 + b3*x + c3*y + d3*z)/vol6;
             f4 = -(a4 + b4*x + c4*y + d4*z)/vol6;

             unkno_local[iv] = f1*(*unkno)[ index_2d_to_1d(ipa,iv,npoin,NVARS_ADAPT3D))+f2*(*unkno)[ index_2d_to_1d(ipb,iv,npoin,NVARS_ADAPT3D) ]
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
//           std::cout << "a1: " << a1 << " a2: " << a2 << " a3: " << a3 << " a4: " << a4 << std::endl;
//           std::cout << "b1: " << b1 << " b2: " << b2 << " b3: " << b3 << " b4: " << b4 << std::endl;
//           std::cout << "c1: " << c1 << " c2: " << c2 << " c3: " << c3 << " c4: " << c4 << std::endl;
//           std::cout << "d1: " << d1 << " d2: " << d2 << " d3: " << d3 << " d4: " << d4 << std::endl;
#ifdef DEBUG
           std::cout << "vol6: " << vol6 << " vol: " << vol << std::endl;
           std::cout << "f1: " << f1 << " f2: " << f2 << " f3: " << f3 << " f4: " << f4 << std::endl;
           std::cout << "ipa: " << ipa << " ipb: " << ipb << " ipc: " << ipc << " ipd: " << ipd << std::endl;
           std::cout << "(*vData)[" << ipa << "] " << (*vData)[ ipa ] << std::endl;
           std::cout << "(*vData)[" << ipb << "] " << (*vData)[ ipb ] << std::endl;
           std::cout << "(*vData)[" << ipc << "] " << (*vData)[ ipc ] << std::endl;
           std::cout << "(*vData)[" << ipd << "] " << (*vData)[ ipd ] << std::endl;
#endif

           return f1*(*vData)[ ipa ]+f2*(*vData)[ ipb ]
				 +f3*(*vData)[ ipc ]+f4*(*vData)[ ipd ] ;




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
