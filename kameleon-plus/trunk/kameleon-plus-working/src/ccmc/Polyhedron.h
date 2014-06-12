/*
 * Polyhedron.h
 *
 *  Created on: May 20, 2013
 *      Author: Asher Pembroke
 */

#ifndef POLYHEDRON_H_
#define POLYHEDRON_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Point.h"
#include "Vector.h"
#include "time.h"
namespace ccmc

{
	using namespace std;

	/*
	 * Polyhedron is a class that can represent any arbitary polyhedron where
	 * the faces are planar. Each face can have any number of vertices.
	 * This class also implements Spherical Barycentric Coordinates, which may be used
	 * for linear interpolation for points inside or on the polyhedron's boundary.
	 * Points outside the polyhedron are given zero weight.
	 */
	template<class T>
	class Polyhedron
	{
		public:
			Polyhedron();
			void setPositions(std::vector<Vector<T> > const &pos);
			void setLoopsFaces(std::vector<int> const &loop, std::vector<int> const &face);
			void setNeighbors(std::vector<int> const &neighbor);
			Vector<T> centroid();
			Vector<T> faceCentroid(int face);
			void setBarycentricCoordinates(Vector<T> const &point);
			void setFloaterCoordinates(int face,Vector<T> const &point);
			void setLinearInterpolants(int v1, int v2, Vector<T> const &point);
			void saveAsDXObject(string filename);
			virtual ~Polyhedron();
			Vector<T> getFaceVectorError();
			void setCurrentPolyhedron(int a);
			virtual Polyhedron<T>* getNextPolyhedron();
			Vector<T> maxDistanceToCentroid();
			Vector<T> minDistanceToCentroid();
			int getClosestFace(Vector<T> const &point);
			Vector<T> getMinDistanceToCentroid();
			Vector<T> testLinearity(Vector<T> const &point);
			bool isPointInside(Vector<T> const &point);
			virtual int getType();
			void merge(Polyhedron<T> const * const p);

			T thetaMax, thetaMin;
			std::vector<T> lambda;
			std::vector<T> theta;
			Vector<T> minimumDistanceToCentroid;

			std::vector<Vector<T> > positions;
			std::vector<int> globalVertex;
			int closestFace;
			int currentPolyhedron;
			std::vector<int> loops;
			std::vector<int> faces;
			bool isInside;
			std::vector<int> neighbors; //must match faces
			static clock_t interpolationTime, stage1, stage2, stage3;
			static int interpolations;
			static T piOver2;

		private:
			std::vector<T> lambdatemp;
			std::vector<int> plus, minus;
			std::vector<Vector<T> > facevectors;
			int nextVertex(int loop, int face);
			int previousVertex(int loop, int face);
			std::vector<Vector<T> > distances, distanceNorms;
			static Vector<T> vZero;


			//The following variables are used by setBarycentricCoordinates
			int vertex, vertexMinus, vertexPlus;
			Vector<T> u1xu2;
			T u1u2Angle;
			Vector<T> ufxu0, ufxu1, ufxu2, uf;


	};

	template<class T>
	Vector<T> Polyhedron<T>::vZero = Vector<T>(0,0,0); //used for zeroing vectors

	template<class T>
	T Polyhedron<T>::piOver2 = T(atan(1)*2);

	template<class T>
	clock_t Polyhedron<T>::interpolationTime = 0;

	template<class T>
	int Polyhedron<T>::interpolations = 0;

	template<class T>
	clock_t Polyhedron<T>::stage1 = 0;

	template<class T>
	clock_t Polyhedron<T>::stage2 = 0;

	template<class T>
	clock_t Polyhedron<T>::stage3 = 0;

	template<class T>
	Polyhedron<T>::Polyhedron()
	{
		currentPolyhedron = 0;
		thetaMax = T(0);
		thetaMin = T(7);
		closestFace = 0;
		isInside = false;
		minimumDistanceToCentroid = Vector<T>(0,0,0);
	}

	template<class T>
	Polyhedron<T>::~Polyhedron()
	{

	}

	template<class T>
	void Polyhedron<T>::setPositions(std::vector<Vector<T> > const &pos)
	{
		this->positions = pos;

		lambda.resize(positions.size());
		lambdatemp.resize(positions.size());
		distances.resize(positions.size());
		distanceNorms.resize(positions.size());

	}

	template<class T>
	void Polyhedron<T>::setLoopsFaces(std::vector<int> const &loop, std::vector<int> const &face)
	{
		/*
		 * Loops must index into positions, following the right-hand rule
		 * Faces must index into the start of each loop in loops
		 */
		this->loops = loop;
		this->faces = face;
		this->faces.push_back(loops.size());

		plus.resize(loops.size());
		minus.resize(loops.size());
		facevectors.resize(faces.size()-1);
		theta.resize(loops.size());

		for (int i=0;i<faces.size()-1;i++){ //for each of the 6 faces of a cube
			for (int j = faces[i];j<faces[i+1];j++){ //cycle through indices of face vertices
				plus[j] = this->nextVertex(j,i);
				minus[j] = this->previousVertex(j,i);
			}
		}

		neighbors.resize(faces.size()-1);
		for (int i=0;i<neighbors.size();i++){
			neighbors[i]=NULL;
		}
		return;
	}

	/*
	 * Sets indices into neighboring cells
	 * Ideally, the number of neighbors should match the number of faces
	 * If a face has no neighbor, set it's neighbor value to currentPolyhedron;
	 */
	template<class T>
	void Polyhedron<T>::setNeighbors(std::vector<int> const &neighbor)
	{
		this->neighbors = neighbor;
		return;
	}


	template<class T>
	Vector<T> Polyhedron<T>::centroid()
	{
		Vector<T> average(0,0,0);
		for (int i = 0;i<positions.size(); i++)
		{
			average = average+positions[i];
		}
		average = average/T(positions.size());
//		std::cout<<"Centroid: "<< average.toString()<<"\n";
		return average;
	}

	template<class T>
	Vector<T> Polyhedron<T>::faceCentroid(int face)
	{
		int vertex;
		Vector<T> average(0,0,0);
		for (int i = faces[face]; i < faces[face+1]; i++){
			vertex = loops[i];
			average = average + positions[vertex];
		}
		average = average/T(faces[face+1]-faces[face]);
		return average;
	}


	template<class T>
	void Polyhedron<T>::setBarycentricCoordinates(Vector<T> const &point)
	{

		clock_t start,end, t0,t1,t2;
		start = clock(); t0 = start; interpolations++;
		/* Given point, calculates Spherical Barycentric Coordinates, such that
		 * every position is given a weight in [0,1]. The weights (lambdas) will be linearly precise:
		 * Sum_i lambda_i position[i] = point
		 *
		 * See Langer et. al, Spherical Barycentric Coordinates, 2006 Eurographics Symposium on Geometry
		 */


		// reset lambda, lambdatemp in case this method was previously called
		for (int i = 0; i < lambda.size(); i++){
			lambda[i]=0;
			lambdatemp[i]= 0;
		}


		for (int i = 0; i < facevectors.size(); i++){
			facevectors[i] = vZero;
		}


		/*
		 * Set distance vectors to point
		 *  if |vi-x|==0, return lambda_i=1, lambda_(j!=i)=0
		 *  else, store normalized distance vectors U_i
		 */

		for (int i = 0; i< positions.size(); i++){
			distances[i] = vZero;
			distances[i] += positions[i];
			distances[i] -= point;
			if (distances[i].length() == 0){
				lambda[i] = T(1.0);
				thetaMax = piOver2;
				end = clock(); interpolationTime += end-start;
				return;
			}
			distanceNorms[i] = distances[i];
			distanceNorms[i].norm();
//			distanceNorms[i] = Vector<T>::norm(distances[i]);
		}

		closestFace = 0;

		//Assign face vectors F_face such that sum_faces F_face = 0
		int vertex, vertexMinus, vertexPlus;
		for (int i = 0; i < faces.size()-1; i++){
			for (int j = faces[i]; j < faces[i+1]; j++){
				vertex = loops[j]; vertexPlus = loops[plus[j]];
				Vector<T>::cross(u1xu2,distanceNorms[vertex],distanceNorms[vertexPlus]);
				if (u1xu2.length() == 0){
					setLinearInterpolants(vertex, vertexPlus, point);
					closestFace = i;
					end = clock(); interpolationTime += end-start;
					return;
				}
				else{
					Vector<T>::angle(u1u2Angle, distanceNorms[vertex],distanceNorms[vertexPlus]);
					u1xu2.norm();
					facevectors[i] += u1xu2*(u1u2Angle/2);
				}
			}
		}

		t1 = clock();
		stage1 += t1-t0;


		thetaMax = T(0);
		thetaMin = T(7);
		T thetaFace; //average of face angles
		//set angles theta_i between F_face and U_i
		for (int i = 0; i < facevectors.size();i++){
			thetaFace = 0;
			for (int j = faces[i]; j < faces[i+1]; j++){
				vertex = loops[j];
				Vector<T>::angle(theta[j], facevectors[i],distanceNorms[vertex]);
				thetaFace += theta[j];
			}
			thetaFace /= faces[i+1] - faces[i];
			if (thetaFace < thetaMin){
				thetaMin = thetaFace;
			}
			if (thetaFace > thetaMax){ //want thetaMax to be the largest angle
				thetaMax = thetaFace;
				closestFace = i;
			}
		}

		// Use theta_i to check if point is inside/outside polyhedron
		const T infinity = numeric_limits<T>::infinity();

		// true if theta > pi/2 or theta < pi/2
		if (!(-infinity < tan(thetaMax) && infinity > tan(thetaMax))){ //point is in same plane as the cell face
			setFloaterCoordinates(closestFace,point);
			this->isInside = true;
			end = clock(); interpolationTime += end-start;
			return;
		}

		if (tan(thetaMax) < T(0)){ // theta > pi/2, point is outside polyhedron
			this->isInside = false;
			end = clock(); interpolationTime += end-start;
			return; //lambdas stay at 0
		}

		t2 = clock();
		stage2 += t2-t1;

		//Else point is inside the polyhedron, update lambdas
		this->isInside = true;

		T alpha, alphaMinus, tangentSum;
		T sum=T(0);
//		Vector<T> ufxu0, ufxu1, ufxu2, uf;

		for (int i = 0; i < facevectors.size();i++){
			uf = facevectors[i];
			uf.norm();
			for (int j = faces[i]; j< faces[i+1]; j++){
				vertexMinus = loops[minus[j]];
				vertex = loops[j];
				vertexPlus = loops[plus[j]];

				Vector<T>::cross(ufxu0,uf,distanceNorms[vertexMinus]);
				Vector<T>::cross(ufxu1,uf,distanceNorms[vertex]);
				Vector<T>::cross(ufxu2,uf,distanceNorms[vertexPlus]);

				Vector<T>::angle(alpha, ufxu1,ufxu2);
				Vector<T>::angle(alphaMinus, ufxu0,ufxu1);

				tangentSum = tan(alphaMinus/2) + tan(alpha/2);

				lambdatemp[vertex] = (facevectors[i].length()/distances[vertex].length())*tangentSum/T(sin(theta[j]));
				sum = sum + tangentSum/T(tan(theta[j]));
			}

			for (int j = faces[i]; j< faces[i+1];j++){
				vertex = loops[j];
				lambda[vertex] += lambdatemp[vertex]/sum;
			}
			sum = T(0);

		}

		/*
		 * Normalize weights
		 */
		sum = T(0);
		for (int i = 0; i < lambda.size(); i++){
			sum += lambda[i];
		}
		for (int i = 0; i < lambda.size(); i++){
			lambda[i] = lambda[i]/sum;
		}


		end = clock();
		stage3 += end - t2;
		interpolationTime += end-start; //interpolationTime = stage1+stage2+stage3
		return;

	}

	template<class T>
	void Polyhedron<T>::setFloaterCoordinates(int face, Vector<T> const &point){

		/*
		 * distances should have nonzero length, else this method shouldn't be called
		 */
//		std::cout<<point.toString()<<" point on plane of face"<<std::endl;

		int vertex, vertexMinus, vertexPlus;
		std::vector<T> alpha;
		alpha.resize(loops.size());
		int maxEdge = 0;
		T maxAlpha = 0;
		for (int i = faces[face]; i<faces[face+1]; i++){
			vertex = loops[i];
			vertexPlus = loops[plus[i]];
			alpha[i] = Vector<T>::angle(distances[vertex],distances[vertexPlus]);
			if (alpha[i]>maxAlpha){
				maxAlpha = alpha[i];
				maxEdge = i;
			}
		}

		if (maxAlpha == T(atan(1)*4)){
			setLinearInterpolants(loops[maxEdge], loops[plus[maxEdge]],point);
			return;
		}

		T alphaMinus;
		T weight, weightSum = 0;

		for (int i = faces[face]; i<faces[face+1]; i++){
			vertexMinus = loops[minus[i]];
			vertex = loops[i];
			vertexPlus = loops[plus[i]];

			weight = tan(alpha[minus[i]]/T(2))+tan(alpha[i]/T(2));
			weight = weight/distances[vertex].length();
			lambda[vertex] = weight;
			weightSum += weight;
		}

		for (int i = faces[face]; i<faces[face+1]; i++){ //all lambdas except for those incident on face should be zero
			vertex = loops[i];
			lambda[vertex] = lambda[vertex]/weightSum;
		}
		return;
	}

	template<class T>
	void Polyhedron<T>::setLinearInterpolants(int v1, int v2, Vector<T> const &point){
		/*
		 * Linearly interpolate between vertex 1 and vertex 2
		 */
//		std::cout<<"point on edge"<<endl;
		for (int i = 0; i<lambda.size(); i++){
			lambda[i]=0;
		}
		T v1v2Distance = (positions[v1]-positions[v2]).length();
		T v1PointDistance = distances[v1].length();
		lambda[v2] = v1PointDistance/v1v2Distance;
		lambda[v1] = T(1) - lambda[v2];

		thetaMax = T(atan(1)*2);
		return;
	}

	template<class T>
	int Polyhedron<T>::getClosestFace(Vector<T> const &point)
	{
		/*
		 * Find closest face by comparing dot products of (point - centroid) and (face centers - centroid)
		 */
		int face=0;
		T maxFaceLength = T(0);
		T faceLength;

		Vector<T> dPointFace;

		T minLength = (point-faceCentroid(0)).length();
		for (int i = 1; i< faces.size()-1;i++){
			dPointFace = point - faceCentroid(i);
			if (dPointFace.length() < minLength){
				minLength = dPointFace.length();
				face = i;
			}
		}

		return face;
	}

	template<class T>
	int Polyhedron<T>::nextVertex(int i, int face)
	{
		int length = faces[face+1]-faces[face];
		return faces[face] + (i - faces[face] + 1)%length;
	}

	template<class T>
	int Polyhedron<T>::previousVertex(int i, int face)
	{
		int length = faces[face+1]-faces[face];
		return faces[face] + (i-faces[face] + length - 1)%length;
	}

	template<class T>
	Vector<T> Polyhedron<T>::getFaceVectorError()
	{
		//Check that face vectors sum to zero from Stoke's theorem
		Vector<T> facesum(0,0,0);
		for (int i = 0; i < facevectors.size();i++){
			facesum = facesum + facevectors[i];
		}

		return facesum;
	}

	template<class T>
	Vector<T> Polyhedron<T>::maxDistanceToCentroid()
	{
		Vector<T> centroid = this->centroid();
		Vector<T> result(0,0,0);
		Vector<T> distance;
		T d, maxd = 0;
		for (int i = 0; i< positions.size();i++){
			distance = positions[i] - centroid;
			d = distance.length();
			if (d > maxd){
				maxd = d;
				result = distance;
			}
		}
		return result;
	}

	template<class T>
	Vector<T> Polyhedron<T>::minDistanceToCentroid()
	{
		Vector<T> centroid = this->centroid();
		Vector<T> result = positions[0] - centroid;
		Vector<T> distance;
		T d, mind;
		mind = result.length();
		for (int i = 1; i< positions.size();i++){
			distance = positions[i] - centroid;
			d = distance.length();
			if (d < mind){
				mind = d;
				result = distance;
			}
		}
		return result;
	}

	template<class T>
	Vector<T> Polyhedron<T>::getMinDistanceToCentroid()
	{
		if (minimumDistanceToCentroid.length() == T(0)){
			minimumDistanceToCentroid = minDistanceToCentroid();
		}
		return minimumDistanceToCentroid;

	}

	template<class T>
	Vector<T> Polyhedron<T>::testLinearity( Vector<T> const &point)
	{
		this->setBarycentricCoordinates(point);
		Vector<T> result(0,0,0);
		for (int i = 0; i< positions.size(); i++){
			result = result + (positions[i]*lambda[i]);
		}
		return result;

	}

	template<class T>
	void Polyhedron<T>::saveAsDXObject(string filename)
	{
		/*
		 * Save as human-readable open-dx object
		 */

		cout<<"Saving polyhedron in "<< filename<<"\n";
		Vector<T> x(1,0,0);
		Vector<T> y(0,1,0);
		Vector<T> z(0,0,1);
		ofstream file(filename.c_str());

		file << "object \"position list\" class array type float rank 1 shape 3 items "<< positions.size() <<" data follows\n";
		for (int i=0;i<positions.size();i++){
			file << float(Vector<T>::dot(x,positions[i])) <<" "<< float(Vector<T>::dot(y,positions[i]))
					<<" "<<float(Vector<T>::dot(z,positions[i]))<<"\n";
		}


		file << "\nobject \"edge list\" class array type int rank 0 items "<< loops.size() <<" data follows\n";
		for (int j = 0; j<faces.size()-1;j++){
			for (int i=faces[j];i<faces[j+1];i++){
				file << loops[i] << " ";
			}
			file << "# face index "<< faces[j]<<"\n";
		}
		file <<"attribute \"ref\" string \"positions\"\n\n";


		file <<"object \"loop list\" class array type int rank 0 items "<< faces.size()-1<<" data follows\n";
		for (int i = 0; i<faces.size()-1;i++){
			file << faces[i] << "\n";
		}
		file << "attribute \"ref\" string \"edges\"\n\n";

		file <<" object \"face list\" class array type int rank 0 items "<<faces.size()-1<< " data follows\n";
		for (int i = 0; i<faces.size()-1;i++){
			file << i << "\n";
		}

		file << " attribute \"ref\" string \"loops\"\n\n";

		file << "#  Field definition to put the arrays together.#\n"
				<< "object \"Polyhedron\" class field \n"
				<< "component \"positions\" \"position list\" \n"
				<< "component \"edges\" \"edge list\" \n"
				<< "component \"loops\" \"loop list\"\n"
				<< "component \"faces\" \"face list\"\n"
/*				<< "component \"data\"  \"data\" \n" */
				<< "end\n";

		file.close();
	}

	template <class T>
	void Polyhedron<T>::setCurrentPolyhedron(int a)
	{
		currentPolyhedron = a;
	}

	template<class T>
	Polyhedron<T>* Polyhedron<T>::getNextPolyhedron()
	{
		return this;
	}

	template<class T>
	bool Polyhedron<T>::isPointInside( Vector<T> const &point)
	{
		this->setBarycentricCoordinates(point);

		return this->isInside;
	}

	template <class T>
	int Polyhedron<T>::getType()
	{
		return 0;
	}

	template<class T>
	void Polyhedron<T>::merge(Polyhedron<T> const * const p)
	{
		/*
		 * Merge this polyhedron with input p
		 */

		int np1 = positions.size();
		int nl1 = loops.size();
		for (int i=0;i< p->positions.size();i++){
			positions.push_back(p->positions[i]);
		}
		// positions now size p1+p2, add size of p1 to p2's loop values
		for (int i=0;i< p->loops.size();i++)
		{
			loops.push_back(np1+p->loops[i]);
		}

		if (nl1 != 0){
			faces.pop_back(); //remove last element (size of loops in p1)
		}

		//last face element must be the index of the last loop element
		for (int i=0;i< p->faces.size()-1;i++)
		{
			faces.push_back(nl1 + p->faces[i]);
		}
		faces.push_back(loops.size());
	}
}


#endif /* POLYHEDRON_H_ */
