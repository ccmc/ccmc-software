/*
 * SWMFIono.h
 *
 *  Created on: May 25, 2012
 *      Author: David Berrios
 */

#ifndef _SWMFIono_h
#define _SWMFIono_h

#include "Model.h"
#include <vector>
#include <string>

namespace ccmc{
 
    
    class SWMFIono: public Model
    {
        
    public: 
        SWMFIono();
        long open(const std::string& filename);
        Interpolator* createNewInterpolator();
        const std::vector<std::string> getLoadedVariables();
        
        virtual ~SWMFIono();
        
    protected:
        void initializeConversionFactorsToSI();
        void initializeSIUnits();
    
    private://
        
        
        
    };
    
    
    
}
#endif
