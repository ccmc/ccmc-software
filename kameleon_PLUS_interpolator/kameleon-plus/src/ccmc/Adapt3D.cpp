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
#include "MathHelper.h"


namespace ccmc
{
	/**
	 * Default constructor
	 */
	Adapt3D::Adapt3D()
	{
		this->ndimn=NDIMN_ADAPT3D;
		this->nnode=NNODE_ADAPT3D;
		this->nvars=NVARS_ADAPT3D;

		this->smartSearchValues.indx = NULL;
		this->smartSearchValues.esup1 = NULL;
		this->smartSearchValues.esup2 = NULL;

	}

	/**
	 * @copydoc Model::open()
	 */
	long Adapt3D::open(const std::string& filename)
	{

		long status;
		status = openFile(filename);

		if (status != FileReader::OK)
		   return status;

		//counts[0] = 1; 									/* set values */
		//intervals[0] = 1;

		   /* get all of the cdf attribute/variable numbers that will be needed *******/
		long loadStatus = this->loadVariable(ccmc::strings::variables::coord_);
		if (loadStatus != FileReader::OK)
				return loadStatus;

		loadStatus = this->loadVariable(ccmc::strings::variables::unkno_);
		if (status != FileReader::OK)
			return loadStatus;

		loadStatus = this->loadVariableInt(ccmc::strings::variables::intmat_);
		if (status != FileReader::OK)
			return loadStatus;

		/***  Check the existence of necessary attributes.  Fail if any don't exist ***/
		if (doesAttributeExist(ccmc::strings::variables::grid_reg_no_))
		{
			this->grid_reg_no = (this->getGlobalAttribute(ccmc::strings::variables::grid_reg_no_)).getAttributeInt();
		} else
		{
			std::cerr << "grid_reg_no not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}
		if (doesAttributeExist(ccmc::strings::variables::ndimn_))
		{
			this->ndimn = (this->getGlobalAttribute(ccmc::strings::variables::ndimn_)).getAttributeInt();
		} else
		{
			std::cerr << "ndimn not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}

		if (doesAttributeExist(ccmc::strings::variables::npoin_))
		{
			this->npoin = (this->getGlobalAttribute(ccmc::strings::variables::npoin_)).getAttributeInt();
		} else
		{
			std::cerr << "npoin not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}

		if (doesAttributeExist(ccmc::strings::variables::nelem_))
		{
			this->nelem = (this->getGlobalAttribute(ccmc::strings::variables::nelem_)).getAttributeInt();
		} else
		{
			std::cerr << "nelem not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}
		/*if (doesAttributeExist(ccmc::strings::variables::nboun_))
		{
			this->nboun = (this->getGlobalAttribute(ccmc::strings::variables::nboun_)).getAttributeInt();
		} else
		{
			std::cerr << "nboun not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}
		if (doesAttributeExist(ccmc::strings::variables::nconi_))
		{
			this->nconi = (this->getGlobalAttribute(ccmc::strings::variables::nconi_)).getAttributeInt();
		} else
		{
			std::cerr << "nconi not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}*/
		std::cerr << "Adapt3D openFile status: " << status << std::endl;

		//create and setup the necessary smart search stuff
		this->nelem = (this->getGlobalAttribute(ccmc::strings::variables::nelem_)).getAttributeInt();
		this->npoin = (this->getGlobalAttribute(ccmc::strings::variables::npoin_)).getAttributeInt();
		this->smartSearchValues.indx = new int[nelem];
		this->smartSearchValues.esup1 = new int[nelem*4];
		this->smartSearchValues.esup2 = new int[npoin+1];
		this->setupUnstructuredGridSearch();
		this->smartSearchSetup();

		return status;

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

	bool Adapt3D::setupUnstructuredGridSearch()
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
		this->coord = (this->getVariableData(ccmc::strings::variables::coord_));
		this->intmat = (this->getVariableDataInt(ccmc::strings::variables::intmat_));




		/*-----------------------------------------------------------------*/
		printf("Entered Structured Search Grid\n");


		/* allocate for ELEM_INDEX_STRUCT */
		printf("Begin allocation of elem_index_struct \n");
		int (* elem_index_struct)[3] = NULL;
		try
		{
			elem_index_struct = new int[nelem][3];

		} catch (std::bad_alloc& ba)
		{
			std::cerr << "Unable to allocate memory: " << ba.what() << std::endl;
			return false;
		}
		printf("Allocation of elem_index_struct complete \n");
		int last_element_found = -1;


		this->smartSearchValues.xl_sg=1.e30;
		this->smartSearchValues.xr_sg=-1.e30;
		this->smartSearchValues.yl_sg=1.e30;
		this->smartSearchValues.yr_sg=-1.e30;
		this->smartSearchValues.zl_sg=1.e30;
		this->smartSearchValues.zr_sg=-1.e30;

		printf("npoin,ndimn %d %d \n",npoin, ndimn);

		/* coord is a 1D vector where the first ndimn words are x,y,z of node 0, the next ndimn words
		* for node 1, etc
		*/
		for ( i=0; i<(int)npoin; i++) {
			this->smartSearchValues.xl_sg=min(this->smartSearchValues.xl_sg,(double)(*coord)[ index_2d_to_1d(i,0,npoin,ndimn) ]);
			this->smartSearchValues.xr_sg=max(this->smartSearchValues.xr_sg,(double)(*coord)[ index_2d_to_1d(i,0,npoin,ndimn) ]);
			this->smartSearchValues.yl_sg=min(this->smartSearchValues.yl_sg,(double)(*coord)[ index_2d_to_1d(i,1,npoin,ndimn) ]);
			this->smartSearchValues.yr_sg=max(this->smartSearchValues.yr_sg,(double)(*coord)[ index_2d_to_1d(i,1,npoin,ndimn) ]);
			this->smartSearchValues.zl_sg=min(this->smartSearchValues.zl_sg,(double)(*coord)[ index_2d_to_1d(i,2,npoin,ndimn) ]);
			this->smartSearchValues.zr_sg=max(this->smartSearchValues.zr_sg,(double)(*coord)[ index_2d_to_1d(i,2,npoin,ndimn) ]);
		}

		printf("-------------------------------\n");
		printf("Range of Structured Search Grid\n");
		printf("xl_sg= %e \n",this->smartSearchValues.xl_sg);
		printf("xr_sg= %e \n",this->smartSearchValues.xr_sg);
		printf("yl_sg= %e \n",this->smartSearchValues.yl_sg);
		printf("yr_sg= %e \n",this->smartSearchValues.yr_sg);
		printf("zl_sg= %e \n",this->smartSearchValues.zl_sg);
		printf("zr_sg= %e \n",this->smartSearchValues.zr_sg);
		printf("-------------------------------\n");


		/* Step 1 - Define structured grid */

		this->smartSearchValues.dx_sg = (this->smartSearchValues.xr_sg - this->smartSearchValues.xl_sg)/( (float)nx_sg );
		this->smartSearchValues.dy_sg = (this->smartSearchValues.yr_sg - this->smartSearchValues.yl_sg)/( (float)ny_sg );
		this->smartSearchValues.dz_sg = (this->smartSearchValues.zr_sg - this->smartSearchValues.zl_sg)/( (float)nz_sg );
		dxyz[0] = this->smartSearchValues.dx_sg;
		dxyz[1] = this->smartSearchValues.dy_sg;
		dxyz[2] = this->smartSearchValues.dz_sg;
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
			//std::cerr << "index_2d_to_1d(" << ielem << ",0," << nelem << ",4): " << index_2d_to_1d(ielem,0,nelem,4)  << std::endl;
			//std::cerr << "sizeof intmat: " << (*intmat).size() << std::endl;
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


			i_s = (int)( (xmean-this->smartSearchValues.xl_sg)/this->smartSearchValues.dx_sg ) ;
			j_s = (int)( (ymean-this->smartSearchValues.yl_sg)/this->smartSearchValues.dy_sg ) ;
			k_s = (int)( (zmean-this->smartSearchValues.zl_sg)/this->smartSearchValues.dz_sg ) ;
			elem_index_struct[ielem][0] = i_s;
			elem_index_struct[ielem][1] = j_s;
			elem_index_struct[ielem][2] = k_s;
			nelems_in_cell[k_s][j_s][i_s] = nelems_in_cell[k_s][j_s][i_s] + 1;


//#define DEBUGX
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
			//return false;
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
					this->smartSearchValues.start_index[k][j][i] = countup;
					countup = countup + nelems_in_cell[k][j][i];
					this->smartSearchValues.end_index[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i] = countdown;
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
						this->smartSearchValues.end_index[k][j][i] = max(this->smartSearchValues.start_index[k][j][i],this->smartSearchValues.end_index[k][j][i]);
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
			ii = this->smartSearchValues.start_index[k][j][i] + nelems_in_cell[k][j][i];
			this->smartSearchValues.indx[ii] = ielem;
			nelems_in_cell[k][j][i] = nelems_in_cell[k][j][i] + 1;
		}

		delete(elem_index_struct);

		/*     end subroutine setup_search_unstructured_grid */
		return true;

	}

	void Adapt3D::smartSearchSetup()
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
			this->smartSearchValues.esup2[ip] = 0;
		}

		/*
		!
		!...  loop over the elements, storing 'ahead'
		!
		*/
		printf(" nnode: %d\n",nnode);
		for( ie=0; ie<nelem; ie++)
		{
			for( inode=0; inode<nnode; inode++)
			{
				ip         = (*intmat)[ index_2d_to_1d(ie,inode,nelem,4) ];
				//std::cerr << "ip: " << ip << std::endl;
				this->smartSearchValues.esup2[ip]  = this->smartSearchValues.esup2[ip] + 1;
			}
		}
		/*
		!
		!...  reshuffle esup2
		!
		*/
		for( ip=1; ip<npoin+1; ip++)
		{
			this->smartSearchValues.esup2[ip] = this->smartSearchValues.esup2[ip] + this->smartSearchValues.esup2[ip-1];
		}

		nstor = this->smartSearchValues.esup2[npoin];

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
				istor        = this->smartSearchValues.esup2[ipoin] + 1;
				this->smartSearchValues.esup2[ipoin] = istor;
				this->smartSearchValues.esup1[istor-1] = ielem;
			}
		}
		/*
		!
		!...  finally, reorder esup2
		!
		*/
		for( ip=npoin; ip>0; ip--) {
			this->smartSearchValues.esup2[ip] = this->smartSearchValues.esup2[ip-1];
		}

		this->smartSearchValues.esup2[0] = 0;


		/*    int nesup    = esup2[npoin];
		printf(" nesup = %i in fielsup1 \n",nesup); */


		/* The routine locate_facing_elements takes too long to execute when
		used to optimize support fieldline tracing */
		/*      locate_facing_elements; */

		/*----------------------------------------------------------------
		! End of smart search setup
		!----------------------------------------------------------------*/


	}

	const SmartGridSearchValues& Adapt3D::getSmartGridSearchValues()
	{
		return this->smartSearchValues;
	}

	int Adapt3D::index_2d_to_1d( int i1, int i2, int n1, int n2)
	{
	/* converts a 2D array index into a flat 1D index */
	      int idx = n2*i1 + i2;

	      return idx;
	}


	/**
	 * Destructor
	 */
	Adapt3D::~Adapt3D()
	{
		// TODO Auto-generated destructor stub
//		std::cout << "deleting Adapt3D object" << std::endl;
		if (this->smartSearchValues.indx != NULL)
			delete this->smartSearchValues.indx;
		if (this->smartSearchValues.esup1 != NULL)
			delete this->smartSearchValues.esup1;
		if (this->smartSearchValues.esup2 != NULL)
			delete this->smartSearchValues.esup2;
	}
}
