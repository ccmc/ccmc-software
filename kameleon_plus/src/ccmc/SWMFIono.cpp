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

	SWMFIono::~SWMFIono()
	{
		// TODO Auto-generated destructor stub
	}
    
    
    
}
