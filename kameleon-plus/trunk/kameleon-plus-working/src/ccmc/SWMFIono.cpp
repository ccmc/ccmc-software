/*
 * SWMFIono.cpp
 *
 *  Created on: May 25, 2012
 *      Author: David Berrios
 */

#include "SWMFIono.h"
#include "SWMFIonoInterpolator.h"
#include "StringConstants.h"
#include "GeneralFileReader.h"


namespace ccmc{
    
    SWMFIono::SWMFIono(){
        
    }
    
    long SWMFIono::open(const std::string& filename)
    {
    	this->setBusyStatus(Model::BUSY);
    
        long status =  GeneralFileReader::open(filename);

		//loadVariable(ccmc::strings::variables::r_);
		loadVariable(ccmc::strings::variables::theta_);
		loadVariable(ccmc::strings::variables::phi_);

        //....

        initializeSIUnits();
		initializeConversionFactorsToSI();
		this->setBusyStatus(Model::OK);
		return status;
    }
    
    /**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator* SWMFIono::createNewInterpolator()
	{
		Interpolator * interpolator = new SWMFIonoInterpolator(this);
		return interpolator;
	}
    
	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void SWMFIono::initializeConversionFactorsToSI()
	{
        
	}
    
	/**
	 * @copydoc Model::initializeSIUnits()
	 */
	void SWMFIono::initializeSIUnits()
	{
        
	}

	const std::vector<std::string> SWMFIono::getLoadedVariables()
	{
		std::vector<std::string> requiredVariables;
		requiredVariables.push_back(ccmc::strings::variables::theta_);
		requiredVariables.push_back(ccmc::strings::variables::phi_);


		std::vector<std::string> variablesLoaded = Model::getLoadedVariables();

		for (int i = 0; i < requiredVariables.size(); i++)
		{
			int size = variablesLoaded.size();
			for (int j = 0; j < size; j++)
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

	SWMFIono::~SWMFIono()
	{
		// TODO Auto-generated destructor stub
	}
    
    
    
}
