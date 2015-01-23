/*
 * LFMInterpolator.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: Asher Pembroke
 */
#include "LFMInterpolator.h"
#include "LFM.h"
#include "Utils.h"
#include "StringConstants.h"
#include "time.h"
#include "math.h"
#include <fstream>
#include "Point3f.h"
#include "Vector.h"
#include "Point.h"
#include "Polyhedron.h"
#include "boost/ptr_container/ptr_vector.hpp"


namespace ccmc
{
	bool LFMInterpolator::initializeKDTreePolyhedra = true;
	NanoKdTree<float> LFMInterpolator::lfmtree; //how do I initialize??
	boost::ptr_vector<Polyhedron<float> > LFMInterpolator::polyhedra;

	/**
	 * Constructor
	 * @param modelReader pointer to LFM model object
	 */
	LFMInterpolator::LFMInterpolator(Model * modelReader)
	{
		// TODO Auto-generated constructor stub
		this->modelReader = modelReader;
		conversionFactors["x"] = 1.0f;
		conversionFactors["y"] = 1.0f;
		conversionFactors["bx"] = 1.0e-4; // from [gauss] to [T]
		conversionFactors["by"] = 1.0e-4;
		conversionFactors["bz"] = 1.0e-4;
		conversionFactors["bx1"] = 1.0f;
		conversionFactors["by1"] = 1.0f;
		conversionFactors["ux"] = 1.0f/(1.0e2);// from [cm/s] to [m/s]
		conversionFactors["uy"] = 1.0f/(1.0e2);
		conversionFactors["uz"] = 1.0f/(1.0e2);
		conversionFactors["jx"] = -1.0f;
		conversionFactors["jy"] = -1.0f;
		conversionFactors["rho"] = 1.0f; //[grams/cc] assuming protons (mass of proton is 1.672622e-24grams)
		conversionFactors["p"] = 1.0f; //already in [Pa]
		conversionFactors["ex"] = -1.0f; // [T*m/s] or [v/m]
		conversionFactors["ey"] = -1.0f; //should this be +1 or -1? If E=-VxB, it should be -1
		conversionFactors["ez"] = -1.0f;

		conversionFactorsByID[modelReader->getVariableID("x")] = 1.0f;
		conversionFactorsByID[modelReader->getVariableID("y")] = 1.0f;
		conversionFactorsByID[modelReader->getVariableID("bx")] = 1.0e-4; // from [gauss] to [T]
		conversionFactorsByID[modelReader->getVariableID("by")] = 1.0e-4;
		conversionFactorsByID[modelReader->getVariableID("bz")] = 1.0e-4;
		conversionFactorsByID[modelReader->getVariableID("bx1")] = 1.0f;
		conversionFactorsByID[modelReader->getVariableID("by1")] = 1.0f;
		conversionFactorsByID[modelReader->getVariableID("ux")] = 1.0f/(1.0e2); //from [cm/s] to [m/s]
		conversionFactorsByID[modelReader->getVariableID("uy")] = 1.0f/(1.0e2);
		conversionFactorsByID[modelReader->getVariableID("uz")] = 1.0f/(1.0e2);
		conversionFactorsByID[modelReader->getVariableID("jx")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("jy")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("rho")] = 1.0f; // [grams/cc]
		conversionFactorsByID[modelReader->getVariableID("p")] = 1.0f;
		conversionFactorsByID[modelReader->getVariableID("ex")] = -1.0f; // [T*m/s] or [v/m]
		conversionFactorsByID[modelReader->getVariableID("ey")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("ez")] = -1.0f;

		float missingValue = this->modelReader->getMissingValue();

		previousVariable = "NULL";
		previousVariableID = -1L;
		previousValue = missingValue;
		previousConversionFactor = 1.0f;
		previous_x = missingValue;
		previous_y = missingValue;
		previous_z = missingValue;

		((LFM*)this->modelReader)->getResolution(nip1,njp1,nkp1); //put this inside initialize?
		ni = nip1-1;
		nj = njp1-1;
		nk = nkp1-1;

		if (LFMInterpolator::initializeKDTreePolyhedra){
			x_array = NULL;
			y_array = NULL;
			z_array = NULL;

			//These are pointers to grid corners (need to convert to cell centered to match up with data)
			x_array = ((LFM*) this->modelReader)->getLFMVariable("x");
			y_array = ((LFM*) this->modelReader)->getLFMVariable("y");
			z_array = ((LFM*) this->modelReader)->getLFMVariable("z");
			setCellCenters(x_array,y_array,z_array);
	

			clock_t telapsed;
			// std::cout<<"Setting polyhedral cells\n";
			telapsed = clock();
			setPolyhedralCells(); // populates polyhedra container
			telapsed = clock() - telapsed;
			// printf ("It took %f seconds to initialize search cells.\n",((float)telapsed)/CLOCKS_PER_SEC);

			telapsed = clock();
			LFMInterpolator::lfmtree.build(); // initialization
			telapsed = clock() - telapsed;
			// printf ("It took %f seconds to build kd-tree index.\n",((float)telapsed)/CLOCKS_PER_SEC);
			LFMInterpolator::initializeKDTreePolyhedra = false; 
		};

		searchPoly = NULL;
		errorsPoly = NULL;

		interpolationTime = 0;
		creationTime = 0;
		destructionTime = 0;
		kdtreeTime = 0;
		getCellTime = 0;
		interpolationNumber = 0;

	};


	/**
	 * @param variable_id 
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	float LFMInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2)
	{
//		std::cout<<"Calling LFM interpolator with variable id";
		float dc0, dc1, dc2;
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	float LFMInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
//		std::cout<<"LFM interpolator called with variable string "<< variable <<endl;
		float dc0, dc1, dc2;
		long variable_id = modelReader->getVariableID(variable);
//		std::cout<<"Calling LFM interpolate with variable_id "<<variable_id<<endl;
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float LFMInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//Point<float> point;
//		std:: cout <<"LFM interpolator called with variable "<< variable<<", id=";
		long variable_id = modelReader->getVariableID(variable);
//		std:: cout<<variable_id<<endl;
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable_id
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float LFMInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{

		/*
		 * If the point is the same as the last point, use previous searchPoly
		 * Else, see if the point is in the previous searchCell
		 * If searchPoly==NULL, this is the first time through, so getCell
		 */

//		float Re_cm = 6.378e8; //Radius of earth in cm
		Vector<float> point(c0,c1,c2); //point = point*Re_cm;

//		clock_t startDestruction, endDestruction;

		if (searchPoly==NULL){ //this is the first time through, find cell
//			std::cout<<"search poly was null, performing initial search"<<endl;
			searchPoly = getCell(point);
//			std::cout<<"after initial search, assigning first interpolation poly"<<endl;
			interpolationPolysMap[searchPoly->currentPolyhedron] = searchPoly;
//			std::cout<<"first poly assigned. Isinside?"<<searchPoly->isInside<<endl;
		}
		else if (previous_x==c0 && previous_y==c1 && previous_z==c2){
			//do nothing - lambdas for previous search poly will be used
		}
		else {
			searchPoly->setBarycentricCoordinates(point); //check the current cell
			interpolationNumber ++;
			if (!searchPoly->isInside){
				searchPoly = getCell(point);
				if (interpolationPolysMap.find(searchPoly->currentPolyhedron) == interpolationPolysMap.end()){
					interpolationPolysMap[searchPoly->currentPolyhedron] = searchPoly;
				}
			}
		}


		Vector<float> dc = searchPoly->getMinDistanceToCentroid();

		int polyType = searchPoly->getType();
		if (polyType==0){
			dc0 = dc.length(); //return to caller
		}
		else if (polyType==1){ //axis cell 2*pi*radius/nk where pi=atan(1)*4
			dc0 = 16*16*atan(1.f)*dc.length()/searchPoly->positions.size(); // 8*atan(1)*radius/#positions
		}
		else{ // inner boundary sqrt(4pi*r^2/#positions)
			dc0 = 4*pow(16*atan(1.f)*(pow(dc.length(),2.f)/searchPoly->positions.size()),.5f);
		}

		dc1 = dc0;
		dc2 = dc0;

		if (!searchPoly->isInside){ //reached outer bounday
			return this->modelReader->getMissingValue();
		}

		float xc=0;
		int globalIndex;
		float lambda;
		int polyI, polyJ, polyK;


		const std::vector<float> * vData = modelReader->getVariableFromMap(variable_id);
		/* Variables data is stored in vectors size nip1,njp1,nkp1, but only
		 * the first ni*nj*nk values are meaningful (https://wiki.ucar.edu/display/LTR/Output)
		 */
		for (int i = 0; i < searchPoly->lambda.size(); i++){
			globalIndex = searchPoly->globalVertex[i]; //index into LFM cell centers
			getIndices(globalIndex, polyI,polyJ,polyK,ni,nj); //indices of LFM cell center
			globalIndex = getIndex(polyI,polyJ,polyK,ni+1,nj+1); //index into LFM cell corners (LFM data arrays are all size ni+1,nj+1,nk+1)
			lambda = searchPoly->lambda[i];
			if (vData == NULL){
				xc += (modelReader->getVariableAtIndex(variable_id,globalIndex)) * lambda;
			}
			else{
				xc += ((*vData)[globalIndex]) * lambda;
			}
		}

		if (previousVariableID != variable_id)
		{
			previousVariableID = variable_id;
			previousVariable = modelReader->getVariableName(variable_id);
			previousConversionFactor = getConversionFactor(variable_id);
		}

		previous_x = c0;
		previous_y = c1;
		previous_z = c2;

		previousValue = xc;

		return xc * previousConversionFactor ;


	}

	/**
	 * @param variable
	 * @return
	 */
	float LFMInterpolator::getConversionFactor(const std::string& variable)
	{
		float conversionFactor = 1.0f;
		boost::unordered_map<std::string, float>::iterator cf = conversionFactors.find(variable);

		if (cf != conversionFactors.end())
			conversionFactor = (*cf).second;
		return conversionFactor;
	}

	/**
	 * @param variable_id
	 * @return
	 */
	float LFMInterpolator::getConversionFactor(const long& variable_id)
	{
		float conversionFactor = 1.0f;
		boost::unordered_map<long, float>::iterator cf = conversionFactorsByID.find(variable_id);

		if (cf != conversionFactorsByID.end())
			conversionFactor = (*cf).second;
		return conversionFactor;
	}

	void LFMInterpolator::setCellCenters(const std::vector<float> * x_array, const std::vector<float>* y_array,
						const std::vector<float>* z_array)
	{
		int i,j,k, l = 0;
		size_t N = ni*nj*nk; //number of cells. No k-wrapping needed.
		cell_centers.pts.resize(N);

		float Re_cm = 6.3675e8; //radius of earth in cm
		//calculate cell centers
		float norm = 1.f/Re_cm; //normalization constant for cell positions
		for (k=0;k<nk;k++){
			for (j=0;j<nj;j++){
				for (i=0;i<ni;i++){
					cell_centers.pts[l].x = ((*x_array)[ijk1(i,j,k)]+(*x_array)[ijk1(i+1,j,k)] +
					          (*x_array)[ijk1(i,j+1,k)]+(*x_array)[ijk1(i+1,j+1,k)]+(*x_array)[ijk1(i,j,k+1)] +
					          (*x_array)[ijk1(i+1,j,k+1)]+(*x_array)[ijk1(i,j+1,k+1)]+(*x_array)[ijk1(i+1,j+1,k+1)]) *
					          0.125*norm;
					cell_centers.pts[l].y = ((*y_array)[ijk1(i,j,k)]+(*y_array)[ijk1(i+1,j,k)] +
					          (*y_array)[ijk1(i,j+1,k)]+(*y_array)[ijk1(i+1,j+1,k)]+(*y_array)[ijk1(i,j,k+1)] +
					          (*y_array)[ijk1(i+1,j,k+1)]+(*y_array)[ijk1(i,j+1,k+1)]+(*y_array)[ijk1(i+1,j+1,k+1)]) *
					          0.125*norm;
					cell_centers.pts[l].z = ((*z_array)[ijk1(i,j,k)]+(*z_array)[ijk1(i+1,j,k)] +
					          (*z_array)[ijk1(i,j+1,k)]+(*z_array)[ijk1(i+1,j+1,k)]+(*z_array)[ijk1(i,j,k+1)] +
					          (*z_array)[ijk1(i+1,j,k+1)]+(*z_array)[ijk1(i,j+1,k+1)]+(*z_array)[ijk1(i+1,j+1,k+1)]) *
					          0.125*norm;
					l++;
				}
			}
		}
		return;

	}

	/**
	 * This function creates and stores the polyhedra used for interpolation.
	 * Each polyhedron has vertices set by LFM cell centers.
	 * In total, there are (ni-1)*(nj-1)*nk hexahedra (6 faces),
	 * 				2*(ni-1) axis cells having nk+2 faces,
	 * 				1 inner boundary cell having (nj-1)*nk + 2 faces.
	 * This function also calculates polyhedral cell centers, which are put
	 * in a point cloud and used for kd-tree searches
	 *
	 */
	void LFMInterpolator::setPolyhedralCells()
	{
		//the first nk*(nj-1)*(ni-1) hexahedral cells
		for (int k = 0; k < nk; k++){
			for (int j = 0; j < nj-1; j++){
				for (int i = 0; i < ni-1; i++){
                    polyhedra.push_back(new GridPolyhedron<float>(i, j, k, this));
				}
			}
		}

		//day side axis polyhedra
		for (int i = 0; i < ni-1; i++){
            polyhedra.push_back(new AxisPolyhedron<float>(i, true, this));
		}
		//night side axis polyhedra
		for (int i = 0; i < ni-1; i++){
            polyhedra.push_back(new AxisPolyhedron<float>(i, false, this));
		}

		//inner boundary polyhedron
        polyhedra.push_back(new IPoly<float>(0, this));

		innerBoundaryRadius = polyhedra[polyhedra.size()-1].maxDistanceToCentroid().length();

		/*
		 * Create helper nodes - a point cloud representing polyhedral centers.
		 * These are used for kd-tree searches
		 */

		helper_nodes.pts.resize(polyhedra.size());
		Vector<float> centroid;
		for (int i = 0; i<polyhedra.size();i++){
			centroid = polyhedra[i].centroid();
			helper_nodes.pts[i].x = centroid.c0();
			helper_nodes.pts[i].y = centroid.c1();
			helper_nodes.pts[i].z = centroid.c2();
		}

		LFMInterpolator::lfmtree.cloud = helper_nodes; //initialization: copy helper nodes created by polyhedra

		return;
	}

	void LFMInterpolator::getIndices(const int index, int& i, int& j, int& k, const int ii, const int jj)
	{
		int temp = 0;
		i = index%ii;
		temp = (index-i)/ii;
		j = temp%jj;
		k = (temp-j)/jj;
		return;
	}
	int LFMInterpolator::getIndex(const int i,const int j, const int k, const int ii, const int jj)
	{
		return i+ii*(j+jj*k);
	}


	/**
	 * Destructor
	 */
	LFMInterpolator::~LFMInterpolator()
	{
        float elapsed_time;
//		float total = 0; float elapsed_time = 0;
		// std::cout<<"destructing LFMInterpolator"<<endl;
		// TODO Auto-generated destructor stub
//		elapsed_time = ((float) getCellTime)/CLOCKS_PER_SEC; total += elapsed_time;
//		std::cout<<"getCell time ="<<getCellTime<<" or "<< elapsed_time<<" seconds"<<endl;
//		elapsed_time = ((float) creationTime)/CLOCKS_PER_SEC; total += elapsed_time;
//		std::cout<<"creation time ="<<creationTime<<" or "<< elapsed_time<<" seconds"<<endl;
//		elapsed_time = ((float) Polyhedron<float>::stage1);
//		std::cout<<"setBarycentricCordinates,stage1 time:"<<elapsed_time/CLOCKS_PER_SEC<<endl;
//		elapsed_time = ((float) Polyhedron<float>::stage2);
//		std::cout<<"setBarycentricCordinates,stage2 time:"<<elapsed_time/CLOCKS_PER_SEC<<endl;
//		elapsed_time = ((float) Polyhedron<float>::stage3);
//		std::cout<<"setBarycentricCordinates,stage3 time:"<<elapsed_time/CLOCKS_PER_SEC<<endl;

//		elapsed_time = ((float) destructionTime)/CLOCKS_PER_SEC; total += elapsed_time;
//		std::cout<<"destruction Time="<<elapsed_time<<endl;

		elapsed_time = ((float) Polyhedron<float>::interpolationTime)/CLOCKS_PER_SEC;
		// std::cout<<"interpolation Time="<<elapsed_time<<endl;
		// std::cout<<"Interpolations performed:"<<Polyhedron<float>::interpolations<<endl;

//		if (searchPoly != NULL){
//			delete searchPoly;
//		}
//
//		if (errorsPoly != NULL){
//			delete errorsPoly;
//		}
//		interpolationPolys.saveAsDXObject("/tmp/interpolationPolys.dx");

	}

	Polyhedron<float>* LFMInterpolator::createNextPolyhedron(Polyhedron<float>* poly)
	{

		int face = poly->closestFace;
		int cellIndex = poly->currentPolyhedron;
		int polyI, polyJ, polyK;
		bool day;

		if (poly->thetaMax <= float(atan(1)*2) ){
//			std::cout<<"still inside polyhedron\n";
			return poly;
		}

		Polyhedron<float>* pNewPoly = poly;

		if (poly->getType() == 0){	// hexahedron: create adjacent polyhedron
			getIndices(cellIndex, polyI, polyJ, polyK, ni-1,nj-1);


			/*Check which face it is closest to
			  and return the index of the polyhedron on the opposite side
			  faces in order { i=0,i=1, j=0, j=1, k = 0, k=1}*/



			if (face==0){
				if (polyI==0){
//					std::cout<<"entering inner boundary\n";
                    pNewPoly = new IPoly<float>(polyI, this);
				}
				else {
					polyI --;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
			}
			else if (face==1){
				if (polyI==ni-2){
//					std::cout<<"exiting outer boundary\n";
				}
				else {
					polyI++;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
			}
			else if (face==2){
				if (polyJ==0){ //day side axis
					day = true;
//					std::cout<<"entering day side axis\n";
                    pNewPoly = new AxisPolyhedron<float>(polyI, day, this);
				}
				else{
					polyJ--;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
			}
			else if (face==3){
				if (polyJ==nj-2){ //night side axis
					day = false;
//					std::cout<<"entering night side axis\n";
                    pNewPoly = new AxisPolyhedron<float>(polyI, day, this);
				}
				else{
					polyJ++;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
			}
			else if (face==4){
				if (polyK == 0){ //wrap backward
					polyK = nk-1;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
				else{
					polyK--;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
			}
			else if (face==5){
				if (polyK == nk-1){ //wrap forward
					polyK = 0;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
				else{
					polyK++;
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
				}
			}
			else{
				return poly;
			}
		}


		else if (poly->getType() == 1){ //axis cell: create adjacent polyhedron
			// find polyI and determine if day or night side
			polyI = cellIndex - (ni-1)*(nj-1)*nk;
			polyJ = (nj-1)*(polyI/(ni-1)); //sets polyJ = nj-1 for night side, polyJ = 0 for day
			day = polyJ==0;
			polyI = polyI%(ni-1);


			if (face < nk){ //azimuthal face
				if (day){
//					std::cout<<"exiting day side axis\n";
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, face, this);
					}
				else{
//					std::cout<<"exiting night side axis\n";
					polyJ--; //create poly at (i,nj-2,face)
                    pNewPoly = new GridPolyhedron<float>(polyI, polyJ, face, this);
				}
			}
			else if (face == nk){ //earth-facing face
				if (polyI == 0){ //inner boundary
//					std::cout<<"entering inner boundary\n";
                    pNewPoly = new IPoly<float>(polyI, this);
				}
				else{
					polyI--;
                    pNewPoly = new AxisPolyhedron<float>(polyI, day, this);
				}
			}
			else if (face == nk+1){
				if (polyI == ni-2){//outer boundary
//					std::cout<<"exiting outer boundary\n";
				}
				else{
					polyI++;
                    pNewPoly = new AxisPolyhedron<float>(polyI, day, this);
				}
			}
		}
		else if (poly->getType() == 2){ //boundary polygon
			polyI = 0;
			if (face < nk*(nj-1)){ //next cell is hexahedron
//				std::cout<<"exiting inner boundary through hexahedron"<<endl;
				polyJ = face%(nj-1);
				polyK = face/(nj-1);
                pNewPoly = new GridPolyhedron<float>(polyI, polyJ, polyK, this);
			}
			else { //day or night side axis
//				std::cout<<"exiting inner boundary through axis"<<endl;
				day = face==nk*(nj-1);
                pNewPoly = new AxisPolyhedron<float>(polyI, day, this);
			}
		}
		else { //type not supported
			return poly;
		}

		return pNewPoly;

	}


	/**
	 * Return a pointer to the cell containing the query point.
	 */
	Polyhedron<float>* LFMInterpolator::getCell(Vector<float> point){
		/*
		 * A "search cell" is one whose corners are the lfm cell centers and whose center is
		 * the centroid of the search cell corners.
		 * The idea is to use a kd-tree to find the nearest cell center,
		 * then use Spherical Barycentric Coordinates to tell whether we are inside the cell or, if not,
		 * move to the cell oposite the closest face.
		 */

		clock_t tgetCellStart,tgetCellEnd;
//		clock_t startCreation,endCreation;
//		clock_t startDestruction,endDestruction;
		tgetCellStart = clock();


//		float Re_cm = 6.378e8; //unused
		typedef float num_t;
		num_t query_pt[3] = {point.c0(), point.c1(), point.c2()}; // c0,c1 do not need to be flipped...
		const int num_results = 1; //changed from const size_t num_results = 1;
		std::vector<size_t> ret_index(num_results); //changed from size_t ret_index;
		std::vector<num_t> out_dist_sqr(num_results); //changed from num_t out_dist_sqr;
		nanoflann::KNNResultSet<num_t> resultSet(num_results);
		resultSet.init(&ret_index[0], &out_dist_sqr[0]);


		typedef std::pair<size_t, num_t> KDResultPair;
		typedef std::vector< KDResultPair  > KDResults;

		KDResults ret_matches;

		LFMInterpolator::lfmtree.nearest(query_pt, resultSet);

//		int polyI,polyJ,polyK;
		int closestHelper = ret_index[0];
		typedef float num_p;
//		Polyhedron<num_p>* pStartPoly;
		Polyhedron<num_p>* pLastPoly;
		Polyhedron<num_p>* pCurrentPoly;
		Polyhedron<num_p>* pNextPoly;
		Polyhedron<num_p>* pTestPoly;

		pTestPoly = &(polyhedra[closestHelper]);
//		startCreation = clock();

		/*
		 * Construct cell around the closest helper node
		 */
//		if (closestHelper < (ni-1)*(nj-1)*nk){ //hexahedral cell
//			getIndices(ret_index[0], polyI,polyJ,polyK,ni-1,nj-1); //i,j,k is the lowest corner of the surrounding cell
//			pStartPoly = new GridPolyhedron<num_p>::GridPolyhedron(polyI,polyJ,polyK, this);
//		}
//		else if(closestHelper < (ni-1)*(nj-1)*nk + 2*(ni-1)) //Axis cell
//		{
//			polyI = closestHelper - (ni-1)*(nj-1)*nk;
//			polyJ = (nj-1)*(polyI/(ni-1));
//			bool day = polyJ==0;
//			polyI = polyI%(ni-1);
//			pStartPoly = new AxisPolyhedron<num_p>::AxisPolyhedron(polyI,day,this);
//		}
//		else{ //closest helper is at x=y=z=0
//			pStartPoly = new IPoly<num_p>::IPoly(0, this);
//		}
//		endCreation = clock();
//		creationTime += endCreation-startCreation;

		Polyhedron<float> startPoly = *pTestPoly;

		pCurrentPoly = pTestPoly;
		bool inside = false;
		int loopIndex = 0;
		int maxTries = 20;
		while (!inside){
			pCurrentPoly->setBarycentricCoordinates(point);
			inside = pCurrentPoly->isInside;
			if(inside){
				break;
			}
			if (loopIndex > maxTries-3){
				int face = pCurrentPoly->closestFace;
//				float eps = std::numeric_limits<float>::epsilon();
//				float piOver2 = Polyhedron<float>::piOver2;
				std::cerr<<"loop index: "<<loopIndex
						<< " goal point: "
						<< point.toString()
						<< " Polyhedron#:"<< pCurrentPoly->currentPolyhedron
						<<"\nclosest face#: "<< pCurrentPoly->closestFace
						<<"\ncentroid of startPoly:"
						<<(startPoly.centroid()).toString()
						<<"\ncentroid of nearest poly: "<< (pCurrentPoly->centroid()).toString()
						<<"\nface centroid:"<<(pCurrentPoly->faceCentroid(pCurrentPoly->closestFace)).toString()<<"\n"
						<<"nearest poly tan(thetaMax):"
						<<tan(pCurrentPoly->thetaMax);
				std::cerr<<"tan(theta):\n";
				for (int j = pCurrentPoly->faces[face]; j < pCurrentPoly->faces[face+1]; j++){
					std::cerr<< tan(pCurrentPoly->theta[j]) <<"\n";
				}
			}
			if (loopIndex == maxTries){
				break;
			}

			pLastPoly = pCurrentPoly;
			pNextPoly = &polyhedra[pCurrentPoly->neighbors[pCurrentPoly->closestFace]];
			if (pNextPoly == pLastPoly){ //exited boundary
//				std::cerr<<"exited boundary"<<endl;
				break;
			}
			pCurrentPoly = pNextPoly;
			loopIndex++;
		}

		tgetCellEnd = clock();
		getCellTime += tgetCellEnd-tgetCellStart;

		return pCurrentPoly;

	}

	/**
	* Constructs a single polyhedron out of all the polys used in interpolation
	*/
	Polyhedron<float> LFMInterpolator::getInterpolationPolys(){
		Polyhedron<float> mergedPoly;
		for(boost::unordered_map<int, Polyhedron<float>* >::iterator iter = interpolationPolysMap.begin(); iter != interpolationPolysMap.end(); iter++){
			mergedPoly.merge(iter->second);
		}

		return mergedPoly;
	}


}
