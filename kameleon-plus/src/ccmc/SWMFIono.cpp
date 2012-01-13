//
//  SWMF.cpp
//  
//
//  Created by Nitesh Donti on 1/10/12.
//  Copyright 2012 Cornell University. All rights reserved.
//

#include "SWMFIono.h"
#include "SWMFIonoInterpolator.h"
#include "StringConstants.h"


namespace ccmc{
    
    SWMFIono::SWMFIono(){
        
    }
    
    long SWMFIono::open(const std::string& filename)
    {
        long status;
		status = openFile(filename);

		loadVariable(ccmc::strings::variables::r_);
		loadVariable(ccmc::strings::variables::theta_);
		loadVariable(ccmc::strings::variables::phi_);

        //....

        initializeSIUnits();
		initializeConversionFactorsToSI();
		return status;
    }
    
    /**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator* SWMFIono::createNewInterpolator()
	{
		Interpolator * interpolator = new SWMFInterpolator(this);
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
    
    
    
}
