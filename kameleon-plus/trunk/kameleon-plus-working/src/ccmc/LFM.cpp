/*
 * LFM.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: Brian Curtis
 */

#include "LFM.h"
#include "Point.h"
#include "Cell3D.h"
#include "Vector.h"
#include "LFMInterpolator.h"
#include "StringConstants.h"
#include "GeneralFileReader.h"
#include <iostream>
#include "nanoflann.hpp"
#include "math.h"

namespace ccmc
{
	/**
	 * Default constructor
	 */
	LFM::LFM()
	{
		// TODO Auto-generated constructor stub

		previous_x = missingValue;
		previous_y = missingValue;
		previous_z = missingValue;
		previous_ix = missingValue;
		previous_iy = missingValue;
		previous_iz = missingValue;
		previousConversionFactor = missingValue;
		previousVariableID = missingValue;
		x_array = NULL;
		y_array = NULL;
		z_array = NULL;

		this->busyStatus = Model::OK;

	}

	/**
	 * @copydoc Model::open()
	 */
	long LFM::open(const std::string& filename)
	{
		this->setBusyStatus(Model::BUSY);
		long status;
		std::cout << "calling GeneralFileReader open function"<<endl;
		status = GeneralFileReader::open(filename);
		std::cout <<"File opened, loading x,y,z,V_th,rho"<<endl;
		this->progress = 20;
		loadVariable(ccmc::strings::variables::x_);
		x_array = this->variableData["x"];
		this->progress = 40;
		loadVariable(ccmc::strings::variables::y_);
		y_array = this->variableData["y"];
		this->progress = 60;
		loadVariable(ccmc::strings::variables::z_);
		z_array = this->variableData["z"];

		setResolution();

		this->progress = 80;
		loadVariable("V_th"); //needed for calculating pressure
		this->progress = 85;
		loadVariable("rho");
		this->progress = 90;

		/*
		 * Is it more accurate to compuate pressure now or use KameleonInterpolator_compute_p?
		 * If the user doesn't use a kameleon object, they still need to access pressure,
		 * so weight as well compute it now.
		 */
		loadPressure();
		loadEfield();


		initializeSIUnits();
		initializeConversionFactorsToSI();
		this->progress = 100;
		this->setBusyStatus(Model::OK);
		return status;
	}

	/*
	 * assumes requested variable is already loaded
	 */
	std::vector<float>* LFM::getLFMVariable(const std::string& variable)
	{
		return this->variableData[variable];
	}

	/*
	 * Compute electric field from edge components stored in hdf file
	 * HDF file stores edge components of electric field (ei,ej,ek) but labels them incorrectly as (ex,ey,ez)
	 *
	 * Note: these results might be off by a negative sign: check that E = -v x B
	 */
	void LFM::loadEfield(){
		std::cout<<"calculating electric field from edge components"<<endl;

		loadVariable("ei");
		loadVariable("ej");
		loadVariable("ek");

		std::vector<float> * pDensity = getLFMVariable("rho");//this->variableData["rho"]; //already loaded
		std::vector<float> * pEx = new std::vector<float>(pDensity->size()); //ni*nj*nk
		std::vector<float> * pEy = new std::vector<float>(pDensity->size());
		std::vector<float> * pEz = new std::vector<float>(pDensity->size());

		std::vector<float> *pEi = getLFMVariable("ei");// [cm/s * gauss]*[cm] E dot dl
		std::vector<float> *pEj = getLFMVariable("ej");
		std::vector<float> *pEk = getLFMVariable("ek");

		std::vector<float> *px = getLFMVariable("x"); // [cm]
		std::vector<float> *py = getLFMVariable("y");
		std::vector<float> *pz = getLFMVariable("z");

		float ei_average, ej_average, ek_average;
		float di_x, dj_x, dk_x;
		float di_y, dj_y, dk_y;
		float di_z, dj_z, dk_z;

		Vector<float> cx, cy, cz, et;
		float determinant;
		float conversion = 1.0e-6; // from [gauss * cm/s] to [T*m/s] or [v/m]

		for (int k = 0; k<nk; k++){
			for (int j = 0; j<nj; j++){
				for (int i = 0; i < ni; i++){
					ei_average = averageFace(pEi, 0, i,j,k, nip1,njp1); //fixed i (nip1,njp1) from ni,njp1
					ej_average = averageFace(pEj, 1, i,j,k, nip1,njp1); //fixed j (nip1,njp1) from nip1,nj
					ek_average = averageFace(pEk, 2, i,j,k, nip1,njp1); //fixed k (nip1,njp1) from nip1,njp1

					//components of vector between i, i+1 faces
					di_x = averageFace(px, 0, i+1,j,k, nip1,njp1) - averageFace(px, 0, i,j,k, nip1,njp1);// units: [cm]
					di_y = averageFace(py, 0, i+1,j,k, nip1,njp1) - averageFace(py, 0, i,j,k, nip1,njp1);
					di_z = averageFace(pz, 0, i+1,j,k, nip1,njp1) - averageFace(pz, 0, i,j,k, nip1,njp1);

					//components of vector between j, j+1 faces
					dj_x = averageFace(px, 1, i,j+1,k, nip1,njp1) - averageFace(px, 1, i,j,k, nip1,njp1);
					dj_y = averageFace(py, 1, i,j+1,k, nip1,njp1) - averageFace(py, 1, i,j,k, nip1,njp1);
					dj_z = averageFace(pz, 1, i,j+1,k, nip1,njp1) - averageFace(pz, 1, i,j,k, nip1,njp1);

					//components of vector between k, k+1 faces
					dk_x = averageFace(px, 2, i,j,k+1, nip1,njp1) - averageFace(px, 2, i,j,k, nip1,njp1);
					dk_y = averageFace(py, 2, i,j,k+1, nip1,njp1) - averageFace(py, 2, i,j,k, nip1,njp1);
					dk_z = averageFace(pz, 2, i,j,k+1, nip1,njp1) - averageFace(pz, 2, i,j,k, nip1,njp1);

					cx.setComponents(di_x,dj_x,dk_x); // [cm]
					cy.setComponents(di_y,dj_y,dk_y);
					cz.setComponents(di_z,dj_z,dk_z);

					et.setComponents(ei_average,ej_average,ek_average); // [cm/s * gauss * cm]

					determinant = 1.0f/(Vector<float>::triple(cx,cy,cz)); //[cm^-3]

					/* triple(et,cy,cz) * determinant = [(cm/s)*gauss*cm * cm^2 * cm^-3] = [(cm/s)*gauss]
					 * we multiply by conversion factor to put result in mks: [T*m/s] or [v/m]
					 */
					(*pEx)[getIndex(i,j,k,nip1,njp1)] = (Vector<float>::triple(et,cy,cz))*determinant*conversion;
					(*pEy)[getIndex(i,j,k,nip1,njp1)] = (Vector<float>::triple(cx,et,cz))*determinant*conversion;
					(*pEz)[getIndex(i,j,k,nip1,njp1)] = (Vector<float>::triple(cx,cy,et))*determinant*conversion;
					// (i,j,k,nip1,njp1) data arrays indexed by lfm corners
					// volume = Vector<float>::triple(cx,cy,cz) //[cm^3]
				}
			}
		}

		addFloatVariableToMap("ex",pEx);
		addFloatVariableToMap("ey",pEy);
		addFloatVariableToMap("ez",pEz);

		return;
	}

	/*
	 * Average requested variable for a face, holding face_index constant (holding i,j,or k constant)
	 */
	float LFM::averageFace(const std::vector<float> * variable, int face_index, const int i, const int j, const int k, const int ii, const int jj){
		/*
		 * face_index = 0: hold i constant
		 * face_index = 1: hold j constant
		 * face_index = 2: hold k constant
		 */

		float average = 0;
		if (face_index == 0){
			for (int m = 0; m < 2; m++){
				for (int n = 0; n < 2; n++){
					average += (*variable)[getIndex(i,j+m,k+n,ii,jj)];
				}
			}
		}
		else if (face_index == 1){
			for (int m = 0; m < 2; m++){
				for (int n = 0; n < 2; n++){
					average += (*variable)[getIndex(i+m,j,k+n,ii,jj)];
				}
			}
		}
		else{
			for (int m = 0; m < 2; m++){
				for (int n = 0; n < 2; n++){
					average += (*variable)[getIndex(i+m,j+n,k,ii,jj)];
				}
			}
		}

		average /= 4;

		return average;
	}

	void LFM::loadPressure(){
		/*populate pressure array
		 * (no need to store an extra array and add variable to map)
		 */
		std::vector<float> * pSoundSpeed = this->variableData["V_th"];
		std::vector<float> * pDensity = this->variableData["rho"];
		std::vector<float> * pPressure = new std::vector<float>(pSoundSpeed->size());
		float soundSpeed, density, pressure;
		float amu = 1.66053892e-24; // grams per amu

		for (int i = 0; i < pPressure->size(); i++){
			soundSpeed = (*pSoundSpeed)[i]; //[cm/s]
			density = (*pDensity)[i]; //[gm/cc]
			pressure = .1*(3.0/5)*density*pow(soundSpeed,2); // [nPa]
			(*pPressure)[i] = pressure;
		}
		addFloatVariableToMap("p",pPressure);

	}

	Interpolator * LFM::createNewInterpolator()
	{
		std::cout <<"creating new LFM interpolator\n";
		Interpolator * interpolator = new LFMInterpolator(this);

		return interpolator;
	}

	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void LFM::initializeConversionFactorsToSI()
	{
//		std::cout << "LFM::initializeConversionFactorsToSI()" << std::endl;
		conversionFactorsToSI["bx1"] = 1e-9f;
		conversionFactorsToSI["by1"] = 1e-9f;
		conversionFactorsToSI["bz1"] = 1e-9f;
		conversionFactorsToSI["bx"] = 1e-9f;
		conversionFactorsToSI["by"] = 1e-9f;
		conversionFactorsToSI["bz"] = 1e-9f;
		conversionFactorsToSI["ux"] = 1e3f;
		conversionFactorsToSI["uy"] = 1e3f;
		conversionFactorsToSI["uz"] = 1e3f;
		conversionFactorsToSI["jx"] = 1e3f;
		conversionFactorsToSI["jy"] = 1e3f;
		conversionFactorsToSI["jz"] = 1e3f;
		conversionFactorsToSI["rho"] = 1e6f;
		conversionFactorsToSI["p"] = 1e-12f;

	}

	/**
	 * @copydoc Model::initializeSIUnits()
	 */
	void LFM::initializeSIUnits()
	{

		variableSIUnits["bx1"] = "T";
		variableSIUnits["by1"] = "T";
		variableSIUnits["bz1"] = "T";
		variableSIUnits["bx"] = "T";
		variableSIUnits["by"] = "T";
		variableSIUnits["bz"] = "T";
		variableSIUnits["ux"] = "m/s";
		variableSIUnits["uy"] = "m/s";
		variableSIUnits["uz"] = "m/s";
		variableSIUnits["jx"] = "A/m^2";
		variableSIUnits["jy"] = "A/m^2";
		variableSIUnits["jz"] = "A/m^2";
		variableSIUnits["rho"] = "m^-3";
		variableSIUnits["p"] = "Pa";
		variableSIUnits["ex"] = "v/m";
		variableSIUnits["ey"] = "v/m";
		variableSIUnits["ez"] = "v/m";
	}

	void LFM::setResolution()
	{
		std::vector<float>* x_array = this->variableData["x"];
		std::vector<float>* y_array = this->variableData["y"];
		//The hdf file stores positions of LFM grid corners (size ni+1 x nj+1 x nk+1)
		float maxx = 0;
		int ncorners = (*x_array).size();
		for (size_t i = 0; i < ncorners; i++){
			if ((*x_array)[i] > maxx){
				maxx = (*x_array)[i];
			}
			else {
				nip1 = i;
//				std::cout<< "nip1 scan: nip1="<< nip1 << "\n";
				break;
			}
		}
		float maxphi = -1;
		float phi = 0;
		for (size_t j = 0; j< ncorners; j=j+nip1){
			phi = atan2((*y_array)[j],(*x_array)[j]);
			if	(phi > maxphi) {
				maxphi = phi;
			}
			else{
				njp1 = j/nip1;
//				std::cout<< "njp1 scan: njp1 ="<< njp1 << "\n";
				break;
			}
		}
		nkp1 = ncorners/(nip1*njp1);

		ni = nip1-1;
		nj = njp1-1;
		nk = nkp1-1;

	}

	void LFM::getResolution(int & nip1, int & njp1, int & nkp1)
		{
		nip1 = this->nip1;
		njp1 = this->njp1;
		nkp1 = this->nkp1;
		return;
		}

	int LFM::getIndex(const int i,const int j, const int k, const int ii, const int jj)
	{
		return i+ii*(j+jj*k);
	}
	/**
	 *
	 */
	LFM::~LFM()
	{
		// TODO Auto-generated destructor stub
//		std::cout << "deleting OpenGGCM object" << std::endl;
	}

}
