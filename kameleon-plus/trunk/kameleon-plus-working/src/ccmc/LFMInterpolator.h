/*
 * LFMInterpolator.h
 *
 *  Created on: Dec 9, 2012
 *      Author: Brian Curtis
 */

#ifndef LFMINTERPOLATOR_H_
#define LFMINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"
#include "pointcloud.h"
#include "time.h"
#define ijk1(i,j,k) ((i)+nip1*((j)+njp1*(k))) // cell corner index
#define ijkc(i,j,k) ((i)+ni*((j)+nj*(k))) //cell center index
#include "Polyhedron.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/unordered_map.hpp"

namespace ccmc
{
	/**
	 * @class LFMInterpolator LFMInterpolator.h ccmc/LFMInterpolator.h
	 * @brief TODO: Brief description of LFMInterpolator class
	 *
	 * TODO: Full description of LFMInterpolator class
	 */

	class LFMInterpolator: public Interpolator
	{
		public:
			LFMInterpolator(Model * modelReader);
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& dc0, float& dc1, float& dc2);
			float interpolate(const long& variable_id, const float & c0, const float& c1, const float& c2);
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2);
			float getConversionFactor(const std::string&);
			float getConversionFactor(const long& variable_id);
			virtual ~LFMInterpolator();

			void setCellCenters(const std::vector<float> * x_array, const std::vector<float>* y_array,
					const std::vector<float>* z_array);
			void setPolyhedralCells();
			static void getIndices(const int index, int& i, int& j, int& k, const int ii, const int jj);
			static int getIndex(const int i,const int j, const int k, const int ii, const int jj);
			Polyhedron<float>* createNextPolyhedron(Polyhedron<float>* poly);
			Polyhedron<float>* getCell(Vector<float> point);
			Polyhedron<float> getInterpolationPolys();

			int ni,nj,nk,nip1,njp1,nkp1;
			PointCloud<float> cell_centers; //size ni,nj,nk - nonstatic: only used once for polyhedra
			PointCloud<float> helper_nodes; //size ni-1,nj-1,nk (+axis later) - nonstatic: copied to lfmtree.cloud

			clock_t interpolationTime;
			clock_t creationTime;
			clock_t destructionTime;
			clock_t kdtreeTime;
			clock_t getCellTime;
			int interpolationNumber;

			float innerBoundaryRadius;


		private:
			Model * modelReader;
			boost::unordered_map<std::string, float> conversionFactors;
			boost::unordered_map<long, float> conversionFactorsByID;


			const std::vector<float> * x_array; //lfm cell corners - nonstatic: used for setting cell centers
			const std::vector<float> * y_array;
			const std::vector<float> * z_array;

			const std::vector<float> * cx; // unused cell centers
			const std::vector<float> * cy;
			const std::vector<float> * cz;
			std::string previousVariable;
			long previousVariableID;
			float previousConversionFactor;
			float previous_x;
			float previous_y;
			float previous_z;
			float previousValue;

			// Create a custom data set class;
			typedef float num_t; //set data type here (float,int, etc)
			static bool initializeKDTreePolyhedra;
			static NanoKdTree<num_t> lfmtree; //made static. only need one tree
			Polyhedron<float> interpolationPolys;
			Polyhedron<float>* searchPoly;
			Polyhedron<float>* errorsPoly;
			static boost::ptr_vector<Polyhedron<float> > polyhedra; //made static. only need one set loaded
			typedef boost::ptr_vector<Polyhedron<float> > ptr_vec;
			ptr_vec::iterator poly_iter; //unused
			boost::unordered_map<int, Polyhedron<float>* > interpolationPolysMap;
	};



	/*
	 * Classes for LFM polyhedra.
	 * 	The LFM domain has nixnjxnk cells. For interpolation, the domain  is decomposed into
	 * 	ni-1 x nj-1 x nk hexahedra (type=0) and 2 x ni-1 axis polyhedra (type=1), where each
	 * 	vertex is an lfm cell center and each axis polyhedron is composed of nk + 2 faces.
	 * 	Todo: Add a single polyhedron representing the inner boundary of lfm.
	 */

	template <class T>
	class GridPolyhedron: public Polyhedron<T>
	{
		public:
			GridPolyhedron(int a, int b, int c, LFMInterpolator * lfm);
			int polyI, polyJ, polyK;
			int ni,nj,nk;
			int getType();
	};

	/*
	 * Constructs a hexahedral cell with LFM index (a,b,c)
	 */
	template <class T>
	GridPolyhedron<T>::GridPolyhedron(int a, int b, int c, LFMInterpolator * lfm)
	{
		ni = lfm->ni;
		nj = lfm->nj;
		nk = lfm->nk;

//		std::cout<<"generating hexahedron at "<<Vector<int>(a,b,c).toString()<<"\n";
		polyI = a;
		polyJ = b;
		polyK = c;

		this->currentPolyhedron = lfm->getIndex(polyI,polyJ,polyK,ni-1,nj-1); //index of current helper cell

		std::vector<Vector<T> > vertices(8);
		int vertex, p=0;

		for (int k = polyK; k<polyK+2;k++){
			for (int j = polyJ; j < polyJ+2;j++){
				for (int i = polyI; i<polyI+2; i++){
					vertex=lfm->getIndex(i,j,k%nk,ni,nj);
					this->globalVertex.push_back(vertex);
					vertices[p] = Vector<T> (lfm->cell_centers.pts[vertex].x,
							lfm->cell_centers.pts[vertex].y, lfm->cell_centers.pts[vertex].z);
					p++;
				}
			}
		}
		this->setPositions(vertices);

		int fv[] = { 0,4,6,2, 1,3,7,5, 0,1,5,4, 2,6,7,3, 0,2,3,1, 4,5,7,6,};
		std::vector<int> loops(fv, fv + sizeof(fv) / sizeof(int));

		// faces in order { i=0,i=1, j=0, j=1, k = 0, k=1}
		int cf[] = {0, 4, 8, 12, 16, 20}; // index into face vertices
		std::vector<int> faces(cf,cf+sizeof(cf)/sizeof(int));

		this->setLoopsFaces(loops, faces);

		// neighbors must match faces
		std::vector<int> neighbors(6);
		// i- face
		if (polyI > 0){
			neighbors[0] = lfm->getIndex(polyI-1,polyJ,polyK,ni-1,nj-1);
		}
		else{ //inner boundary
			neighbors[0] = nk*(nj-1)*(ni-1) + 2*(ni-1);
		}

		// i+ 1 face
		if (polyI < ni-2){
			neighbors[1] = lfm->getIndex(polyI+1,polyJ,polyK,ni-1,nj-1);
		}
		else{ //outer boundary
			neighbors[1] = this->currentPolyhedron;
		}

		// j- face
		if (polyJ > 0){
			neighbors[2] = lfm->getIndex(polyI,polyJ-1,polyK,ni-1,nj-1);
		}
		else{ //day side axis
			neighbors[2] = (ni-1)*(nj-1)*nk + polyI;
		}

		// j+ face
		if (polyJ < nj-2){
			neighbors[3] = lfm->getIndex(polyI,polyJ+1,polyK,ni-1,nj-1);
		}
		else{ //night side axis
			neighbors[3] = (ni-1)*(nj-1)*nk + (ni-1) + polyI;
		}

		// k- face
		if (polyK > 0){
			neighbors[4] = lfm->getIndex(polyI,polyJ,polyK-1,ni-1,nj-1);
		}
		else{ //wrap backward
			neighbors[4] = lfm->getIndex(polyI,polyJ,nk-1,ni-1,nj-1);
		}

		// k+ face
		if (polyK < nk-1){
			neighbors[5] = lfm->getIndex(polyI,polyJ,polyK+1,ni-1,nj-1);
		}
		else {// wrap forward
			neighbors[5] = lfm->getIndex(polyI,polyJ,0,ni-1,nj-1);
		}

		this->setNeighbors(neighbors);
		return;
	}

	template <class T>
	int GridPolyhedron<T>::getType(){
		return 0;
	}

	template <class T>
	class AxisPolyhedron: public Polyhedron<T>
	{
		public:
			AxisPolyhedron(int a,bool dday, ccmc::LFMInterpolator * lfm);
			int polyI;
			bool day; //true if day side, false if night side
			int ni,nj,nk;
			int getType();
	};

	template <class T>
	AxisPolyhedron<T>::AxisPolyhedron(int a, bool dday, ccmc::LFMInterpolator *lfm)
	{
		ni = lfm->ni;
		nj = lfm->nj;
		nk = lfm->nk;

		polyI = a;
		this->day = dday;

		int polyJ;
		if (day){
			polyJ = 0;
		}
		else{
			polyJ = nj-1;
		}

//		std::cout<<"Generating AXIS polyhedron at i = "<< polyI<<" day = "<< day <<"\n";
		this->currentPolyhedron = (ni-1)*(nj-1)*nk + (polyJ/(nj-1)) * (ni-1) + polyI; //index of current helper cell

		std::vector<Vector<T> > vertices(2*nk);
		int vertex, p=0;

		for (int i = polyI; i<polyI+2; i++){
			for (int k = 0; k<nk;k++){
				vertex=lfm->getIndex(i,polyJ,k,ni,nj);
				this->globalVertex.push_back(vertex);
				vertices[p] = Vector<T> (lfm->cell_centers.pts[vertex].x,
						lfm->cell_centers.pts[vertex].y, lfm->cell_centers.pts[vertex].z);
				p++;
			}
		}


		this->setPositions(vertices);

		std::vector<int> faces;
		std::vector<int> neighbors;
		std::vector<int> loops;

		int faceNumber = 0;
		// setup faces around axis, listed counter-clockwise with your thumb pointed at the sun
		for (int k = 0; k < nk; k++){
			if(day){
				loops.push_back(k);
				loops.push_back((k+1)%nk);
				loops.push_back(nk+(k+1)%nk);
				loops.push_back(k+nk);
				neighbors.push_back(lfm->getIndex(polyI,0,k,ni-1,nj-1));
			}
			else{
				loops.push_back(k);
				loops.push_back(k+nk);
				loops.push_back(nk+(k+1)%nk);
				loops.push_back((k+1)%nk);
				neighbors.push_back(lfm->getIndex(polyI,nj-2,k,ni-1,nj-1));
			}
			faces.push_back(faceNumber);
			faceNumber += 4;
		}

		faces.push_back(faceNumber); //start of polyI (earth-facing) face
		if(day){
			// set i- neighbor
			if (polyI > 0){
				neighbors.push_back(nk*(nj-1)*(ni-1) + polyI-1);
			}
			else { //inner boundary
				neighbors.push_back(nk*(nj-1)*(ni-1) + 2*(ni-1));
			}
			// set i+ neighbor
			if (polyI < ni-2){
				neighbors.push_back(nk*(nj-1)*(ni-1) + polyI+1);
			}
			else { //outer boundary
				neighbors.push_back(this->currentPolyhedron);
			}


			for (int k = nk-1; k > -1; k--){
				loops.push_back(k);
				faceNumber ++;
			}
			for (int k = 0; k<nk; k++){
				loops.push_back(k+nk);
			}
		}
		else{ //night side
			// set i- neighbor
			if (polyI > 0){
				neighbors.push_back(nk*(nj-1)*(ni-1) + (ni-1) + polyI-1);
			}
			else { //inner boundary
				neighbors.push_back(nk*(nj-1)*(ni-1) + 2*(ni-1));
			}
			// set i+ neighbor
			if (polyI < ni-2){
				neighbors.push_back(nk*(nj-1)*(ni-1) + (ni-1) + polyI+1);
			}
			else { //outer boundary
				neighbors.push_back(this->currentPolyhedron);
			}

			for (int k = 0; k < nk; k++){
				loops.push_back(k);
				faceNumber ++;
			}

			for (int k = nk-1; k > -1; k--){
				loops.push_back(k+nk);
			}
		}
		faces.push_back(faceNumber); //start of polyI+1 face


		this->setLoopsFaces(loops,faces);

		this->setNeighbors(neighbors);
		return;

	}

	template <class T>
	int AxisPolyhedron<T>::getType(){
		return 1;
	};

	template <class T>
	class JPoly: public Polyhedron<T>
	{
		public:
			JPoly(int a,int b, ccmc::LFMInterpolator * lfm);
			int polyI, polyJ;
			int ni,nj,nk;
			int getType();
	};

	template <class T>
	JPoly<T>::JPoly(int a, int b, ccmc::LFMInterpolator *lfm)
	{
		ni = lfm->ni;
		nj = lfm->nj;
		nk = lfm->nk;

		polyI = a%ni;
		polyJ = b%nj;
		bool day = polyJ <= nj/2;

//		std::cout<<"Generating AXIS polyhedron at i = "<< polyI<<" day = "<< day <<"\n";
		this->currentPolyhedron = (ni-1)*(nj-1)*nk + (polyJ/(nj-1)) * (ni-1) + polyI; //index of current helper cell

		std::vector<Vector<T> > vertices(2*nk);
		int vertex, p=0;

		for (int i = polyI; i<polyI+2; i++){
			for (int k = 0; k<nk;k++){
				vertex=lfm->getIndex(i,polyJ,k,ni,nj);
				this->globalVertex.push_back(vertex);
				vertices[p] = Vector<T> (lfm->cell_centers.pts[vertex].x,
						lfm->cell_centers.pts[vertex].y, lfm->cell_centers.pts[vertex].z);
				p++;
			}
		}


		this->setPositions(vertices);

		std::vector<int> faces;
		std::vector<int> loops;

		int faceNumber = 0;
		// setup faces around axis, listed counter-clockwise with your thumb pointed at the sun
		for (int k = 0; k < nk; k++){
			if(day){
				loops.push_back(k);
				loops.push_back((k+1)%nk);
				loops.push_back(nk+(k+1)%nk);
				loops.push_back(k+nk);
			}
			else{
				loops.push_back(k);
				loops.push_back(k+nk);
				loops.push_back(nk+(k+1)%nk);
				loops.push_back((k+1)%nk);
			}
			faces.push_back(faceNumber);
			faceNumber += 4;
		}

		faces.push_back(faceNumber); //start of polyI (earth-facing) face
		if(day){
			for (int k = nk-1; k > -1; k--){
				loops.push_back(k);
				faceNumber ++;
			}

			for (int k = 0; k<nk; k++){
				loops.push_back(k+nk);
			}
		}
		else{
			for (int k = 0; k < nk; k++){
				loops.push_back(k);
				faceNumber ++;
			}

			for (int k = nk-1; k > -1; k--){
				loops.push_back(k+nk);
			}
		}
		faces.push_back(faceNumber); //start of polyI+1 face


		this->setLoopsFaces(loops,faces);


		return;

	}

	template <class T>
	int JPoly<T>::getType(){
		return 3;
	};


	template <class T>
	class IPoly: public Polyhedron<T>
	{
		public:
			IPoly(int a, LFMInterpolator * lfm);
			int ni, nj,nk;
			int polyI;
			int getType();
	};

	template <class T>
	IPoly<T>::IPoly(int a, LFMInterpolator *lfm){
		ni = lfm->ni;
		nj = lfm->nj;
		nk = lfm->nk;
		polyI = a;

//		std::cout<<"Generating polyhedron at I = "<<polyI<<endl;
		this->currentPolyhedron = (ni-1)*(nj-1)*nk + 2 * (ni-1); //index of current helper cell

		std::vector<Vector<T> > vertices(nk*nj);
		int vertex, p=0;

		for (int k = 0; k<nk; k++){
			for(int j = 0; j<nj; j++){
				vertex=lfm->getIndex(polyI,j,k,ni,nj);
				this->globalVertex.push_back(vertex);
				vertices[p] = Vector<T> (lfm->cell_centers.pts[vertex].x,
						lfm->cell_centers.pts[vertex].y, lfm->cell_centers.pts[vertex].z);
				p++;
			}
		}
		this->setPositions(vertices);

		std::vector<int> faces;
		std::vector<int> neighbors;
		std::vector<int> loops;

		int faceNumber = 0;

		for (int k = 0; k < nk; k++){
			for (int j = 0; j < nj-1; j++){
				loops.push_back(j   + nj*k);
				loops.push_back(j+1 + nj*k);
				loops.push_back(j+1 + nj*((k+1)%nk));
				loops.push_back(j   + nj*((k+1)%nk));

				faces.push_back(faceNumber);
				neighbors.push_back(lfm->getIndex(0,j,k,ni-1,nj-1));
				faceNumber+=4;
			}
		}


		//day side cap at j=0
		faces.push_back(faceNumber);
		neighbors.push_back(nk*(nj-1)*(ni-1));
		for (int k = 0; k < nk; k++){
			loops.push_back(nj*k);
		}

		//night side cap at j = nj-1
		faceNumber+=nk;
		faces.push_back(faceNumber);
		neighbors.push_back(nk*(nj-1)*(ni-1)+(ni-1));
		for (int k = nk-1; k>-1; k--){
			loops.push_back(nj-1 + nj*k);
		}

		this->setLoopsFaces(loops,faces);

		this->setNeighbors(neighbors);

		return;
	}

	template<class T>
	int IPoly<T>::getType(){
		return 2;
	}
}


#endif /* LFMINTERPOLATOR_H_ */
