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
#include "GeneralFileReader.h"
#include "Point3f.h"
#include <stdio.h>
#include <algorithm>
#include <limits>
#include <cmath>
#include <vector>
#include <string>

//#define DEBUG
//#define DEBUGS
namespace ccmc
{
	/**
	 * Default constructor
	 */
	Adapt3D::Adapt3D()
	{
		std::cout << "entered constructor" << std::endl;
		this->ndimn=NDIMN_ADAPT3D;
		this->nnode=NNODE_ADAPT3D;
		this->nvars=NVARS_ADAPT3D;
		this->smartSearchValues.nelems_in_cell.resize(boost::extents[nz_sg][ny_sg][nx_sg]);
		this->smartSearchValues.nnodes_in_cell.resize(boost::extents[nz_sg][ny_sg][nx_sg]);
		this->smartSearchValues.start_index.resize(boost::extents[nz_sg][ny_sg][nx_sg]);
		this->smartSearchValues.start_index_nodes.resize(boost::extents[nz_sg][ny_sg][nx_sg]);
		this->smartSearchValues.end_index.resize(boost::extents[nz_sg][ny_sg][nx_sg]);
		this->smartSearchValues.end_index_nodes.resize(boost::extents[nz_sg][ny_sg][nx_sg]);

		this->smartSearchValues.indx = NULL;
		this->smartSearchValues.esup1 = NULL;
		this->smartSearchValues.esup2 = NULL;
		this->smartSearchValues.indx_nodes = NULL;
		this->smartSearchValues.delauney_search_iteration_profile = NULL;
		this->smartSearchValues.facing_elements = NULL;
		this->coord_modified = NULL;
		this->busyStatus = Model::OK;

	}

	/**
	 * @copydoc Model::open()
	 */
	long Adapt3D::open(const std::string& filename)
	{

		this->setBusyStatus(Model::BUSY);
		long status;
		status = GeneralFileReader::open(filename);
		this->progress = 10;
		if (status != FileReader::OK)
		   return status;

		//counts[0] = 1; 									/* set values */
		//intervals[0] = 1;

		   /* get all of the cdf attribute/variable numbers that will be needed *******/
		//if (!this->doesVariableExist(ccmc::strings::variables::coord_))
		{
		//	return FileReader::VARIABLE_DOES_NOT_EXIST;
		}
		long loadStatus = this->loadVariable(ccmc::strings::variables::coord_);
		if (loadStatus != FileReader::OK)
				return loadStatus;
		this->progress = 15;
		loadStatus = this->loadVariableInt(ccmc::strings::variables::intmat_);
		if (status != FileReader::OK)
			return loadStatus;
		this->progress = 20;
		/***  Check the existence of necessary attributes.  Fail if any don't exist ***/
		if (doesAttributeExist(ccmc::strings::variables::grid_reg_no_))
		{
			this->grid_reg_no = (this->getGlobalAttribute(ccmc::strings::variables::grid_reg_no_)).getAttributeInt();
		} else
		{
			std::cerr << "grid_reg_no not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}
		this->progress = 25;
		if (doesAttributeExist(ccmc::strings::variables::ndimn_))
		{
			this->ndimn = (this->getGlobalAttribute(ccmc::strings::variables::ndimn_)).getAttributeInt();
		} else
		{
			std::cerr << "ndimn not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}
		//this->progress = 40;
		if (doesAttributeExist(ccmc::strings::variables::npoin_))
		{
			this->npoin = (this->getGlobalAttribute(ccmc::strings::variables::npoin_)).getAttributeInt();
		} else
		{
			std::cerr << "npoin not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}
		this->progress = 30;
		if (doesAttributeExist(ccmc::strings::variables::nelem_))
		{
			this->nelem = (this->getGlobalAttribute(ccmc::strings::variables::nelem_)).getAttributeInt();
		} else
		{
			std::cerr << "nelem not found" << std::endl;
			return FileReader::ATTRIBUTE_DOES_NOT_EXIST;
		}
		this->progress = 35;
		//create and setup the necessary smart search stuff
		this->nelem = (this->getGlobalAttribute(ccmc::strings::variables::nelem_)).getAttributeInt();

		this->npoin = (this->getGlobalAttribute(ccmc::strings::variables::npoin_)).getAttributeInt();
		this->smartSearchValues.indx = new std::vector<int>(nelem);
		this->smartSearchValues.delauney_search_iteration_profile = new std::vector<int>(DELAUNEY_ITER_MAX);
		if (this->smartSearchValues.facing_elements != NULL)
			delete this->smartSearchValues.facing_elements;
		this->smartSearchValues.facing_elements = new std::vector<int>(this->nelem*4);
		this->smartSearchValues.esup1 = new std::vector<int>(nelem*4);
		for (int i = 0; i < nelem*4; i++)
		{
			this->smartSearchValues.esup1->push_back(0);
		}
		this->smartSearchValues.esup2 = new std::vector<int>(npoin+1);
		for (int i = 0; i < npoin+1; i++)
		{
			this->smartSearchValues.esup2->push_back(0);
		}
		this->progress = 45;
		//this->coord = (this->getVariableFromMap(ccmc::strings::variables::coord_));
		coord_modified = this->getVariableFromMapRW(ccmc::strings::variables::coord_);
#ifdef DEBUG
		std::cout << "this->coord->size(): " << this->coord_modified->size() << std::endl;
#endif
		std::vector<float> * buffer = new std::vector<float>(coord_modified->size());
		for (int i = 0; i < coord_modified->size(); i++)
		{
			(*buffer)[i] = coord_modified->at(i);
		}
		this->progress = 50;
		float tempc = 0;
		for ( int i=0; i<npoin; i++) {
//			(*coord_modified)[i]=(*buffer)[i*3];
//			(*coord_modified)[i+npoin]=(*buffer)[i*3+1];
//			(*coord_modified)[i+npoin*2]=(*buffer)[i*3+2];
			(*coord_modified)[i*3]=(*buffer)[i];
			(*coord_modified)[i*3+1]=(*buffer)[i+npoin];
			(*coord_modified)[i*3+2]=(*buffer)[i+npoin*2];
			//std::cout << "this->coord_modifed->size(): " << this->coord_modified->size() << " tempc: " << tempc << " [i+npoin*2]: " << (i+npoin*2) <<  " i+npoin " << (i+npoin) << std::endl;
		}

		delete buffer;
		this->progress = 55;
		this->intmat_modified = (this->getIntVariableFromMapRW(ccmc::strings::variables::intmat_));

		//intmat_modified = new std::vector<int>(intmat->size());
		for (int i = 0; i < this->intmat_modified->size(); i++)
		{
			(*intmat_modified)[i] = (*intmat_modified)[i] - 1;
		}
		//this->unloadVariable(ccmc::strings::variables::intmat_);

		this->smartSearchValues.indx_nodes = new std::vector<int>(npoin);
		this->progress = 65;
		this->setupSearchUnstructuredGrid();
		this->progress = 75;
		this->smartSearchSetup();

		//this->setupOctreeGrid();
		this->setBusyStatus(Model::OK);

		return status;

	}



	const std::vector<std::string> Adapt3D::getLoadedVariables()
	{
		std::vector<std::string> requiredVariables;
		requiredVariables.push_back(ccmc::strings::variables::coord_);
		requiredVariables.push_back(ccmc::strings::variables::intmat_);


		std::vector<std::string> variablesLoaded = Model::getLoadedVariables();

		for (int i = 0; i < requiredVariables.size(); i++)
		{
			size_t size = variablesLoaded.size();
			for (size_t j = 0; j < size; j++)
			{
				if (variablesLoaded[j] == requiredVariables[i])
				{
					variablesLoaded.erase(variablesLoaded.begin() + j);
					j = size;
				}


			}
		}
		return variablesLoaded;
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

	bool Adapt3D::setupSearchUnstructuredGrid()
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
		//int i,j,k;
		int ii;
		//int nelems_in_cell[nz_sg][ny_sg][nx_sg];
		//int nnodes_in_cell[nz_sg][ny_sg][nx_sg];
		//int start_index_nodes[nz_sg][ny_sg][nx_sg];
		//int end_index_nodes[nz_sg][ny_sg][nx_sg];

		int countup, countdown;

		float x,y,z;
		float r = 0.f,t = 0.f,p = 0.f;
		float xlo,xhi;
		float ylo,yhi;
		float zlo,zhi;
		float xmean,ymean,zmean;
		//float side_l_1,side_l_2,side_l_3;
		//float side_l_4,side_l_5,side_l_6;
		float /*max_element_length,*/max_length_sqrd;
		float max_length_sqrd_old;
		float dxyz[3];
		float dxyz_min;
		//float arr2[2];
		float arr4[4];
		//float arr7[7];
		long   len;
		double const PI=4.*atan(1.);





		/*-----------------------------------------------------------------*/
//		std::cout << "Entered Structured Search Grid" << std::endl;


		/* allocate for ELEM_INDEX_STRUCT */
//		printf("Begin allocation of elem_index_struct \n");
		int (* elem_index_struct)[3] = NULL;
		try
		{
			elem_index_struct = new int[nelem][3];

		} catch (std::bad_alloc& ba)
		{
			std::cerr << "Unable to allocate memory: " << ba.what() << std::endl;
			return false;
		}

		int (* node_index_struct)[3] = NULL;
		try
		{
			node_index_struct = new int[npoin][3];

		} catch (std::bad_alloc& ba)
		{
			std::cerr << "Unable to allocate memory: " << ba.what() << std::endl;
			return false;
		}

		//this->smartSearchValues.indx_nodes = new int[npoin];
		this->smartSearchValues.last_element_found = -1;
//		printf("Allocation of elem_index_struct complete \n");


		this->smartSearchValues.xl_gr=1.e30f;
		this->smartSearchValues.xr_gr=-1.e30f;
		this->smartSearchValues.yl_gr=1.e30f;
		this->smartSearchValues.yr_gr=-1.e30f;
		this->smartSearchValues.zl_gr=1.e30f;
		this->smartSearchValues.zr_gr=-1.e30f;

	#ifdef CARTESIAN_S_GRID
	      xl_sg=1.e30;
	      xr_sg=-1.e30;
	      yl_sg=1.e30;
	      yr_sg=-1.e30;
	      zl_sg=1.e30;
	      zr_sg=-1.e30;
	#endif /* CARTESIAN_S_GRID */

	#ifdef SPHERICAL_S_GRID
	      this->smartSearchValues.xl_sg=1.e30f;
	      this->smartSearchValues.xr_sg=0.;
	      this->smartSearchValues.yl_sg=float(PI);
	      this->smartSearchValues.yr_sg=0.;
	      this->smartSearchValues.zl_sg=float(2.*PI);
	      this->smartSearchValues.zr_sg=0.;
	#endif /* SPHERICAL_S_GRID */

#ifdef DEBUG
	      printf("npoin,ndimn %d %d \n",npoin, ndimn);
#endif

	/* coord is a 1D vector where the first ndimn words are x,y,z of node 0, the next ndimn words
	 * for node 1, etc
	*/
	      for ( int i=0; i<(int)npoin; i++) {
//#ifdef CARTESIAN_S_GRID
	    	this->smartSearchValues.xl_gr=std::min(this->smartSearchValues.xl_gr,(*coord_modified)[ index_2d_to_1d(i,0,3) ]);
	    	this->smartSearchValues.xr_gr=std::max(this->smartSearchValues.xr_gr,(*coord_modified)[ index_2d_to_1d(i,0,3) ]);
	    	this->smartSearchValues.yl_gr=std::min(this->smartSearchValues.yl_gr,(*coord_modified)[ index_2d_to_1d(i,1,3) ]);
	    	this->smartSearchValues.yr_gr=std::max(this->smartSearchValues.yr_gr,(*coord_modified)[ index_2d_to_1d(i,1,3) ]);
	    	this->smartSearchValues.zl_gr=std::min(this->smartSearchValues.zl_gr,(*coord_modified)[ index_2d_to_1d(i,2,3) ]);
	    	this->smartSearchValues.zr_gr=std::max(this->smartSearchValues.zr_gr,(*coord_modified)[ index_2d_to_1d(i,2,3) ]);
//#endif

#ifdef SPHERICAL_S_GRID
	        int a = 0, b = 0, c = 0;
	        a = index_2d_to_1d(i,0,3);
	        b = index_2d_to_1d(i,1,3);
	        c = index_2d_to_1d(i,2,3);
	        //std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;

	        x=(*coord_modified)[ a ];
	        y=(*coord_modified)[ b ];
	        z=(*coord_modified)[ c ];
	        Math::convert_xyz_to_rthetaphi(x,y,z,&r,&t,&p);
	        if (r < .001)
	        	std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
	        this->smartSearchValues.xl_sg=std::min(this->smartSearchValues.xl_sg,r);
	        this->smartSearchValues.xr_sg=std::max(this->smartSearchValues.xr_sg,r);
	        this->smartSearchValues.yl_sg=std::min(this->smartSearchValues.yl_sg,t);
	        this->smartSearchValues.yr_sg=std::max(this->smartSearchValues.yr_sg,t);
	        this->smartSearchValues.zl_sg=std::min(this->smartSearchValues.zl_sg,p);
	        this->smartSearchValues.zr_sg=std::max(this->smartSearchValues.zr_sg,p);

#endif /* SPHERICAL_S_GRID */
	      }
	#ifdef CARTESIAN_S_GRID
	      this->smartSearchValues.xl_sg=xl_gr;
	      this->smartSearchValues.xr_sg=xr_gr;
	      this->smartSearchValues.yl_sg=yl_gr;
	      this->smartSearchValues.yr_sg=yr_gr;
	      this->smartSearchValues.zl_sg=zl_gr;
	      this->smartSearchValues.zr_sg=zr_gr;
	#endif /* CARTESIAN_S_GRID */








//		printf("-------------------------------\n");
//		printf("Range of Structured Search Grid\n");
//		printf("xl_sg= %e \n",this->smartSearchValues.xl_sg);
//		printf("xr_sg= %e \n",this->smartSearchValues.xr_sg);
//		printf("yl_sg= %e \n",this->smartSearchValues.yl_sg);
//		printf("yr_sg= %e \n",this->smartSearchValues.yr_sg);
//		printf("zl_sg= %e \n",this->smartSearchValues.zl_sg);
//		printf("zr_sg= %e \n",this->smartSearchValues.zr_sg);
//		printf("-------------------------------\n");


		/* Step 1 - Define structured grid */

		this->smartSearchValues.dx_sg = (this->smartSearchValues.xr_sg - this->smartSearchValues.xl_sg)/(float)(nx_sg );
		this->smartSearchValues.dy_sg = (this->smartSearchValues.yr_sg - this->smartSearchValues.yl_sg)/(float)(ny_sg );
		this->smartSearchValues.dz_sg = (this->smartSearchValues.zr_sg - this->smartSearchValues.zl_sg)/(float)(nz_sg );
		dxyz[0] = this->smartSearchValues.dx_sg;
		dxyz[1] = this->smartSearchValues.dy_sg;
		dxyz[2] = this->smartSearchValues.dz_sg;
		len=3;
		dxyz_min = ccmc::Math::ffindmin(dxyz,len);


		/* Initialize the counters for the number of elements in each grid cell */
		for (int k=0; k<nz_sg; k++) {
		for (int j=0; j<ny_sg; j++) {
		for (int i=0; i<nx_sg; i++) {

					this->smartSearchValues.nelems_in_cell[k][j][i] = 0;
					this->smartSearchValues.nnodes_in_cell[k][j][i] = 0;
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
			ipa = (*intmat_modified)[ index_2d_to_1d(ielem,0,4) ] ;
			ipb = (*intmat_modified)[ index_2d_to_1d(ielem,1,4) ] ;
			ipc = (*intmat_modified)[ index_2d_to_1d(ielem,2,4) ] ;
			ipd = (*intmat_modified)[ index_2d_to_1d(ielem,3,4) ] ;
#ifdef CARTESIAN_S_GRID
			/*
			 *  coord is a 1D vector where the first ndimn words are x,y,z of node 0, the next ndimn words
			 * for node 1, etc
			 */
			side_l_1= pow((*coord_modified)[ index_2d_to_1d(ipa,0,3) ]-(*coord_modified)[ index_2d_to_1d(ipb,0,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipa,1,3) ]-(*coord_modified)[ index_2d_to_1d(ipb,1,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipa,2,3) ]-(*coord_modified)[ index_2d_to_1d(ipb,2,3) ],2) ;
			side_l_2= pow((*coord_modified)[ index_2d_to_1d(ipa,0,3) ]-(*coord_modified)[ index_2d_to_1d(ipc,0,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipa,1,3) ]-(*coord_modified)[ index_2d_to_1d(ipc,1,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipa,2,3) ]-(*coord_modified)[ index_2d_to_1d(ipc,2,3) ],2) ;
			side_l_3= pow((*coord_modified)[ index_2d_to_1d(ipa,0,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,0,3) ],2) +
					  pow((*coord_modified)[ index_2d_to_1d(ipa,1,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,1,3) ],2) +
					  pow((*coord_modified)[ index_2d_to_1d(ipa,2,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,2,3) ],2) ;
			side_l_4= pow((*coord_modified)[ index_2d_to_1d(ipb,0,3) ]-(*coord_modified)[ index_2d_to_1d(ipc,0,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipb,1,3) ]-(*coord_modified)[ index_2d_to_1d(ipc,1,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipb,2,3) ]-(*coord_modified)[ index_2d_to_1d(ipc,2,3) ],2) ;
			side_l_5= pow((*coord_modified)[ index_2d_to_1d(ipb,0,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,0,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipb,1,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,1,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipb,2,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,2,3) ],2) ;
			side_l_6= pow((*coord_modified)[ index_2d_to_1d(ipc,0,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,0,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipc,1,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,1,3) ],2) +
				      pow((*coord_modified)[ index_2d_to_1d(ipc,2,3) ]-(*coord_modified)[ index_2d_to_1d(ipd,2,3) ],2) ;

			arr7[0] = max_length_sqrd;
			arr7[1] = side_l_1;
			arr7[2] = side_l_2;
			arr7[3] = side_l_3;
			arr7[4] = side_l_4;
			arr7[5] = side_l_5;
			arr7[6] = side_l_6;

			max_length_sqrd=ccmc::Math::ffindmax(arr7,7);
#endif

			arr4[0] = (*coord_modified)[ index_2d_to_1d(ipa,0,3) ];
			arr4[1] = (*coord_modified)[ index_2d_to_1d(ipb,0,3) ];
			arr4[2] = (*coord_modified)[ index_2d_to_1d(ipc,0,3) ];
			arr4[3] = (*coord_modified)[ index_2d_to_1d(ipd,0,3) ];
			xlo = ccmc::Math::ffindmin(arr4,4);
			xhi = ccmc::Math::ffindmax(arr4,4);
			arr4[0] = (*coord_modified)[ index_2d_to_1d(ipa,1,3) ];
			arr4[1] = (*coord_modified)[ index_2d_to_1d(ipb,1,3) ];
			arr4[2] = (*coord_modified)[ index_2d_to_1d(ipc,1,3) ];
			arr4[3] = (*coord_modified)[ index_2d_to_1d(ipd,1,3) ];
			ylo = ccmc::Math::ffindmin(arr4,4);
			yhi = ccmc::Math::ffindmax(arr4,4);
			arr4[0] = (*coord_modified)[ index_2d_to_1d(ipa,2,3) ];
			arr4[1] = (*coord_modified)[ index_2d_to_1d(ipb,2,3) ];
			arr4[2] = (*coord_modified)[ index_2d_to_1d(ipc,2,3) ];
			arr4[3] = (*coord_modified)[ index_2d_to_1d(ipd,2,3) ];
			zlo = ccmc::Math::ffindmin(arr4,4);
			zhi = ccmc::Math::ffindmax(arr4,4);

			xmean = 0.5f*(xlo+xhi);
			ymean = 0.5f*(ylo+yhi);
			zmean = 0.5f*(zlo+zhi);

#ifdef SPHERICAL_S_GRID
			x = xmean;
			y = ymean;
			z = zmean;
			Math::convert_xyz_to_rthetaphi(x,y,z, &r, &t, &p);
			xmean = r;
			ymean = t;
			zmean = p;
#endif


			i_s = (int)( (xmean-this->smartSearchValues.xl_sg)/this->smartSearchValues.dx_sg ) ;
			j_s = (int)( (ymean-this->smartSearchValues.yl_sg)/this->smartSearchValues.dy_sg ) ;
			k_s = (int)( (zmean-this->smartSearchValues.zl_sg)/this->smartSearchValues.dz_sg ) ;
			i_s = std::max(0, std::min(i_s, nx_sg-1));
			j_s = std::max(0, std::min(j_s, ny_sg-1));
			k_s = std::max(0, std::min(k_s, nz_sg-1));
			elem_index_struct[ielem][0] = i_s;
			elem_index_struct[ielem][1] = j_s;
			elem_index_struct[ielem][2] = k_s;
#ifdef DEBUG
	//		std::cout << "elem_index_struct[" << ielem << "] = [" << i_s << " " << j_s << " " << k_s << "]" << std::endl;
#endif
			this->smartSearchValues.nelems_in_cell[k_s][j_s][i_s] = this->smartSearchValues.nelems_in_cell[k_s][j_s][i_s] + 1;

		}

#ifdef CARTESIAN_S_GRID
		max_element_length = sqrt(max_length_sqrd);

//		printf("Maximum element length = %e \n",max_element_length);
//		printf("Grid cell spacing = %e \n",dxyz_min);

///		printf("Ratio of search grid spacing to max element length (Must be greater than 1) = %e \n",
//								   dxyz_min/max_element_length);
		if(max_element_length > dxyz_min) {
			printf("ERROR: UNSTRUCTURED SEARCH GRID SPACING IS TOO FINE \n");
			printf("ERROR: SOLUTION - INCREASE NO OF GRID POINTS BY FACTOR %e \n", max_element_length/dxyz_min);
			//return false;
		}
#endif

		/* Step 3 - place starting and ending indices of element list into each structured cell */

		/* Create start and end pointers for each grid cell's section of the
		index to the element list */
		countup   = 0;
		countdown = nelem-1;
		for (int k=0; k<nz_sg; k++ ) {
		for (int j=0; j<ny_sg; j++ ) {
		for (int i=0; i<nx_sg; i++ ) {
			this->smartSearchValues.start_index[k][j][i] = countup;
			countup = countup + this->smartSearchValues.nelems_in_cell[k][j][i];
			this->smartSearchValues.end_index[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i] = countdown;
			countdown = countdown - this->smartSearchValues.nelems_in_cell[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i];
		}}}

		for (int k=0; k<nz_sg; k++ ) {
		for (int j=0; j<ny_sg; j++ ) {
		for (int i=0; i<nx_sg; i++ ) {
			this->smartSearchValues.end_index[k][j][i] = std::max(/*(float)*/(this->smartSearchValues.start_index[k][j][i]),/*(float)*/(this->smartSearchValues.end_index[k][j][i]));
		}}}


		/* Step 4 - Create the index */
		for (int k=0; k<nz_sg; k++) {
		for (int j=0; j<ny_sg; j++) {
		for (int i=0; i<nx_sg; i++) {
					this->smartSearchValues.nelems_in_cell[k][j][i] = 0;
		}}}

		for (int ielem=0; ielem<nelem; ielem++)
		{
			int i = elem_index_struct[ielem][0];
			int j = elem_index_struct[ielem][1];
			int k = elem_index_struct[ielem][2];
			ii = this->smartSearchValues.start_index[k][j][i] + this->smartSearchValues.nelems_in_cell[k][j][i];
			(*this->smartSearchValues.indx)[ii] = ielem;
			this->smartSearchValues.nelems_in_cell[k][j][i] = this->smartSearchValues.nelems_in_cell[k][j][i] + 1;
		}
#ifdef DEBUG
		for (int k=0; k<nz_sg; k++) {
		for (int j=0; j<ny_sg; j++) {
		for (int i=0; i<nx_sg; i++) {
			if (this->smartSearchValues.nelems_in_cell[k][j][i] > 0)
				std::cout << "nelems_in_cell[" << k << "][" << j << "][" << i << "]: " << this->smartSearchValues.nelems_in_cell[k][j][i] << "\n";
		}}}
		std::cout << std::endl;
#endif

/* Step 5 - Count number of nodes that lie in each cell of the structured grid */
		for (int inode=0; inode<npoin; inode++) {
		         x=(*coord_modified)[inode*3];
		         y=(*coord_modified)[inode*3+1];
		         z=(*coord_modified)[inode*3+2];
#ifdef SPHERICAL_S_GRID
		         Math::convert_xyz_to_rthetaphi(x,y,z,&r,&t,&p);
		         x=r;
		         y=t;
		         z=p;
#endif /* SPHERICAL_S_GRID */

		         i_s = (int)( (x-this->smartSearchValues.xl_sg)/this->smartSearchValues.dx_sg ) ;
		         j_s = (int)( (y-this->smartSearchValues.yl_sg)/this->smartSearchValues.dy_sg ) ;
		         k_s = (int)( (z-this->smartSearchValues.zl_sg)/this->smartSearchValues.dz_sg ) ;
		         i_s = std::max( 0, std::min(i_s,nx_sg-1) );  /* prevent roundoff messing up index calc */
		         j_s = std::max( 0, std::min(j_s,ny_sg-1) );  /* prevent roundoff messing up index calc */
		         k_s = std::max( 0, std::min(k_s,nz_sg-1) );  /* prevent roundoff messing up index calc */
		         node_index_struct[inode][0] = i_s;
		         node_index_struct[inode][1] = j_s;
		         node_index_struct[inode][2] = k_s;
		         this->smartSearchValues.nnodes_in_cell[k_s][j_s][i_s] = this->smartSearchValues.nnodes_in_cell[k_s][j_s][i_s] + 1;
		      }
		#ifdef DEBUG
		         printf("END node count\n");
		         fflush(stdout);
		#endif

		/* Step 6 - Create start and end pointers for each grid cell's section of the
		 index to the node list */
		      countup   = 0;
		      countdown = npoin-1;
		      for (int k=0; k<nz_sg; k++ ) {
		      for (int j=0; j<ny_sg; j++ ) {
		      for (int i=0; i<nx_sg; i++ ) {
		    	  this->smartSearchValues.start_index_nodes[k][j][i] = countup;
		         countup = countup + this->smartSearchValues.nnodes_in_cell[k][j][i];
		         this->smartSearchValues.end_index_nodes[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i] = countdown;
		         countdown = countdown - this->smartSearchValues.nnodes_in_cell[nz_sg-1-k][ny_sg-1-j][nx_sg-1-i];
		      }}}
		#ifdef DEBUG
		         printf("END start and end indx node count\n");
		         fflush(stdout);
		#endif

		      for (int k=0; k<nz_sg; k++ ) {
		      for (int j=0; j<ny_sg; j++ ) {
		      for (int i=0; i<nx_sg; i++ ) {
		    	  this->smartSearchValues.end_index_nodes[k][j][i] = std::max(/*(float)*/this->smartSearchValues.start_index_nodes[k][j][i],
		    			  /*(float)*/this->smartSearchValues.end_index_nodes[k][j][i]);
		      }}}

		#ifdef DEBUG
		         printf("END start and end indx node count 2\n");

		#endif

		/* Step 7 - Create the index */
		      for (int k=0; k<nz_sg; k++) {
		      for (int j=0; j<ny_sg; j++) {
		      for (int i=0; i<nx_sg; i++) {
		    	  this->smartSearchValues.nnodes_in_cell[k][j][i] = 0;
		      }}}

		#ifdef DEBUG
		         printf("END init nnodes_in_cell\n");
		         fflush(stdout);
		#endif
		      for (int inode=0; inode<npoin; inode++) {
		         int i = node_index_struct[inode][0];
		         int j = node_index_struct[inode][1];
		         int k = node_index_struct[inode][2];
		         ii = this->smartSearchValues.start_index_nodes[k][j][i] + this->smartSearchValues.nnodes_in_cell[k][j][i];
		         (*this->smartSearchValues.indx_nodes)[ii] = inode;
		         this->smartSearchValues.nnodes_in_cell[k][j][i] = this->smartSearchValues.nnodes_in_cell[k][j][i] + 1;
		      }


		delete[] elem_index_struct;
		delete[] node_index_struct;

		/*     end subroutine setup_search_unstructured_grid */
		return false;

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


	   int ip,/*ie,*/inode,nstor,ielem,istor;
	   int nnodes, ipoin;

		/*----------------------------------------------------------------
		! Start of smart search setup
		!----------------------------------------------------------------*/

		nnodes = nelem*nnode;     /* number of element/node couples */

		/* Settings for monitoring the usefulness of parts of the smart search algorithm */
		                                                                        /* new 11-16-11 - next 6 lines */
		for(int i=0;i<DELAUNEY_ITER_MAX;i++) {
			(*this->smartSearchValues.delauney_search_iteration_profile)[i]=0;
		}
		//this->smartSearchValues.still_in_same_element=0;
		//this->smartSearchValues.outside_grid=0;
#ifdef DEBUG
		printf("Delauney counter allocated and initialized\n");
#endif


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
			(*this->smartSearchValues.esup2)[ip] = 0;
		}

		/* Count the number of times each node number appears in intmat. This gives the number of elements
		 * which include that node number
		 */
		for( inode=0; inode<nnodes; inode++)
		{
			ip         = (*intmat_modified)[ inode]+1;
			//std::cerr << "ip: " << ip << std::endl;
			(*this->smartSearchValues.esup2)[ip]  = this->smartSearchValues.esup2->at(ip) + 1;
		}

		/*
		!
		!...  reshuffle esup2
		! Convert esup2 into an accumulating total
		*/
		for( ip=1; ip<npoin+1; ip++)
		{
			(*this->smartSearchValues.esup2)[ip] = this->smartSearchValues.esup2->at(ip) + this->smartSearchValues.esup2->at(ip-1);
		}

		nstor = this->smartSearchValues.esup2->at(npoin);

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
				ipoin        = (*intmat_modified)[ index_2d_to_1d(ielem,inode,4) ];
				istor        = this->smartSearchValues.esup2->at(ipoin) + 1;
				(*this->smartSearchValues.esup2)[ipoin] = istor;
				(*this->smartSearchValues.esup1)[istor-1] = ielem;
			}
		}
		/*
		!
		!...  finally, reorder esup2
		!
		*/
		for( ip=npoin; ip>0; ip--) {
			(*this->smartSearchValues.esup2)[ip] = this->smartSearchValues.esup2->at(ip-1);
		}

		(*this->smartSearchValues.esup2)[0] = 0;
		this->progress = 80;

		/*    int nesup    = esup2[npoin];
		printf(" nesup = %i in fielsup1 \n",nesup); */


		/* The routine locate_facing_elements takes too long to execute when
		used to optimize support fieldline tracing */
		locate_facing_elements();
		//this->smartSearchValues.last_element_found = -1;
		/*----------------------------------------------------------------
		! End of smart search setup
		!----------------------------------------------------------------*/


	}

	void Adapt3D::locate_facing_elements()
	/*----------------------------------------------------------------
	  This sub finds the elements that are adjacent to
	  each element ==> facing_elements

	  Written:    Original by Hong Luo
	              Modified by Peter MacNeice - Nov 2011
	 *----------------------------------------------------------------*/

	{
	      int  lhelp[4][3];
	      //int  ielem,iface,ip,ieadj,je,i,nnodes;
	      //int  in1,in2,in3,ip1,ip2,ip3;
	      //int  j1,j2,j3,j4;
	      //int  iesu0 ,iesu1,iesup,icoun;
	      std::vector<int> *  lpoin;

	      lhelp[0][0] = 2;
	      lhelp[0][1] = 3;
	      lhelp[0][2] = 4;
	      lhelp[1][0] = 3;
	      lhelp[1][1] = 1;
	      lhelp[1][2] = 4;
	      lhelp[2][0] = 4;
	      lhelp[2][1] = 1;
	      lhelp[2][2] = 2;
	      lhelp[3][0] = 1;
	      lhelp[3][1] = 3;
	      lhelp[3][2] = 2;


	/* initialize facing_elements=0 */
	      int nnodes = nelem*nnode;     /* number of element/node couples */
	      size_t size = this->smartSearchValues.facing_elements->size();
	      for (int i=0;i<nnodes;i++) {
	        (*this->smartSearchValues.facing_elements)[i]=0;
	      }

	/* initialize lpoin */
	      lpoin = new std::vector<int>(npoin);
	      for (int i=0;i<npoin;i++) {
	        (*lpoin)[i]=0;
	      }

	/* loop over the elements */
	      for (int ielem=0;ielem<nelem;ielem++) {
	    	  this->progress = 80 + (int)((float)ielem/(float)nelem*20);

	/* loop over the faces */
	        for (int iface=0;iface<nnode;iface++) {

	/* store the three local node numbers (ie between 0 and 3) of the current face */
	          int in1 = lhelp[iface][0]-1;
	          int in2 = lhelp[iface][1]-1;
	          int in3 = lhelp[iface][2]-1;

	/* For each of these three nodes, store the global index */
	          int ip1 = (*intmat_modified)[ index_2d_to_1d(ielem,in1,4) ];
	          int ip2 = (*intmat_modified)[ index_2d_to_1d(ielem,in2,4) ];
	          int ip3 = (*intmat_modified)[ index_2d_to_1d(ielem,in3,4) ];
//#define DEBUG
	#ifdef DEBUG
	          if(ielem == 1) {
	            printf("locate_face: ielem=%d  face=%d nodes are %d %d %d\n",ielem,iface,ip1,ip2,ip3);
	          }
	#endif

	/* mark lpoin identifying these nodes as the current active nodes for this search*/
	          (*lpoin)[ip1] = 1;
	          (*lpoin)[ip2] = 1;
	          (*lpoin)[ip3] = 1;

	/* store the range of element indeces of elements which contain the first of these nodes */
	          int iesu0 = this->smartSearchValues.esup2->at(ip1);
	          int iesu1 = this->smartSearchValues.esup2->at(ip1+1);

	#ifdef DEBUG
	          if(ielem == 1) {
	            printf("locate_face: range of elements containing node ip1=%d is %d to %d\n",ip1,iesu0,iesu1);
	          }
	#endif

	          int ieadj = -999;

	/* then loop over the surrounding elements than contain the first of these nodes, testing
	 * each of these elements to see if it has 3 nodes in common with our original element. If it
	 * does, then record this element as the neighboring element to face iface */
	          int iesup = iesu0;
	          while ((iesup < iesu1) && (ieadj == -999))
	          {
	        	  //std::cout << "inside" << std::endl;
	        	  int je = this->smartSearchValues.esup1->at(iesup);
	        	  if(je != ielem) {

	        		  /* For the current test element store its node numbers */
	        		  int j1=(*intmat_modified)[ index_2d_to_1d(je,0,4) ] ;
	        		  int j2=(*intmat_modified)[ index_2d_to_1d(je,1,4) ] ;
	        		  int j3=(*intmat_modified)[ index_2d_to_1d(je,2,4) ] ;
	        		  int j4=(*intmat_modified)[ index_2d_to_1d(je,3,4) ] ;
#ifdef DEBUG
	        		  if(ielem == 1) {
	        			  printf("locate_face: testing neighbor element no %d\n",je);
	        			  printf("locate_face: neighbor element nodes are %d %d %d %d\n",j1,j2,j3,j4);
	        		  }
#endif

	        		  /* For each of the test elements nodes, add the corresponding entry from lpoin, which is 1 if
	        		   * the node belongs to the original element, but is otherwise 0. */
	        		  int icoun = (*lpoin)[j1]+(*lpoin)[j2]+(*lpoin)[j3]+(*lpoin)[j4];
#ifdef DEBUG
	        		  if(ielem == 1) {
	        			  printf("locate_face: testing neighbor element no %d  icoun=%d\n",je,icoun);
	        		  }
#endif
	        		  if(icoun == 3) ieadj=je;
	        	  }
	        	  iesup = iesup+1;
	          }


//	          std::cout << "ielem = " << ielem << " iface = " << iface << std::endl;

	/* this is the element ! */
	          int je = index_2d_to_1d(ielem,iface,4);
	          (*(this->smartSearchValues.facing_elements))[je] = ieadj;
//std::cout << "facing element: " << ieadj << std::endl;
	/* re-mark lpoin */
	          (*lpoin)[ip1] = 0;
	          (*lpoin)[ip2] = 0;
	          (*lpoin)[ip3] = 0;

	        }

	      }

	      delete lpoin;
	}

	SmartGridSearchValues * Adapt3D::getSmartGridSearchValues()
	{
		return &(this->smartSearchValues);
	}

	int Adapt3D::index_2d_to_1d( int i1, int i2, int n)
	{
	/* converts a 2D array index into a flat 1D index */
	      int idx = n*i1 + i2;

	      return idx;
	}

/*	void Adapt3D::setupOctreeGrid()
	{
		//smartSearchValues.parent = new BoundingBox();
		int numLevels = 2;
		vertices.clear();
		int numVertices = this->coord->size()/3;
		for (int i = 0; i < numVertices; i++)
		{

			this->vertices.push_back(Point3f(this->coord->at((npoin * 0) + i),
											 this->coord->at((npoin * 1) + i),
											 this->coord->at((npoin * 2) + i)));
		}
		std::cout << "finished adding vertices." << std::endl;
		std::cout << "Need to add " << this->nelem << " elements." << std::endl;
		int count = -1;
		for (int i = 0; i < this->nelem; i++)
		{

			Element element;
//			element.addVertex(&this->vertices[i*4]);
//			element.addVertex(&this->vertices[i*4+1]);
//			element.addVertex(&this->vertices[i*4+2]);
//			element.addVertex(&this->vertices[i*4+3]);
			element.addVertex(&this->vertices[intmat->at(++count)]);
			element.addVertex(&this->vertices[intmat->at(++count)]);
			element.addVertex(&this->vertices[intmat->at(++count)]);
			element.addVertex(&this->vertices[intmat->at(++count)]);
			//element.updateBoundingBox();
			element.setIndex(i);
			this->elements.push_back(element);
			if (i % 200000 == 0)
			{
				std::cout << "i: " << i << " out of " << nelem << std::endl;
			}
		}

		std::cout << "finished adding elements." << std::endl;
		//exit(0);

		Point3f deltas;
		smartSearchValues.parent->min = Point3f(smartSearchValues.xl_sg, smartSearchValues.yl_sg, smartSearchValues.zl_sg);
		smartSearchValues.parent->max = Point3f(smartSearchValues.xr_sg, smartSearchValues.yr_sg, smartSearchValues.zr_sg);
		//smartSearchValues.parent->createVertices();



		std::cout << "finished setting up initial structured grid of bounding boxes." << std::endl;
		smartSearchValues.parent->getElements()->reserve(this->nelem);
		for (int i = 0; i < elements.size(); i++)
		{
			smartSearchValues.parent->addElement(&elements[i]);
		}

		//exit(0);
		std::cout << "subdividing!!!" << std::endl;
		smartSearchValues.parent->subdivide(3);
exit(0);
//		for (int i = 0; i < nx_b; i++)
//		{
//			for (int j = 0; j < ny_b; j++)
//			{
//				for (int k = 0; k < nz_b; k++)
//				{
//					int count = 0;
//					for (int l = 0; l < elements.size(); l++)
//					{
//
//						if (!elements[l].enclosed)
//						{
//							bool entirely = true;
//							bool partly = false;
//							structuredGrid[k][j][i].contains(&elements[l], &entirely, &partly);
//							if (entirely)
//							{
//								elements[l].enclosed = true;
//								structuredGrid[k][j][i].addElement(&elements[l]);
//							} else if (partly ||
//									elements[l].getBoundingBox()->containsb(&structuredGrid[k][j][i]))
//							{
//								structuredGrid[k][j][i].addElement(&elements[l]);
//							} else
//							{
//								//std::cout << "did not contain" << std::endl;
//							}
//							count++;
//						} else
//						{
//							//std::cout << "saved time!" << std::endl;
//						}
//
//					}
//					//std::cout << "number elements scanned: " << count << std::endl;
//				}
//			}
//			std::cout << " i: " << i << std::endl;
//		}

		std::cout << "finished adding elements to structured grid cells." << std::endl;

//		for (int i = 0; i < nx_b; i++)
//		{
//			for (int j = 0; j < ny_b; j++)
//			{
//				for (int k = 0; k < nz_b; k++)
//				{
//					std::cout << "structuredGrid[" << k << "][" << j << "][" << i << "]: " << structuredGrid[k][j][i].getElements()->size() << std::endl;
//				}
//			}
//		}

		//smartSearchValues.parent->print();

		//exit(0);

	}
*/


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
		if (this->smartSearchValues.indx_nodes != NULL)
			delete this->smartSearchValues.indx_nodes;
		if (this->smartSearchValues.delauney_search_iteration_profile != NULL)
			delete this->smartSearchValues.delauney_search_iteration_profile;
		if (this->smartSearchValues.facing_elements != NULL)
			delete this->smartSearchValues.facing_elements;

	}

}
