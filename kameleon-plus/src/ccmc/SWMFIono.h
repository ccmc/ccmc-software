//
//  SWMF.h
//  
//
//  Created by Nitesh Donti on 1/10/12.
//  Copyright 2012 Cornell University. All rights reserved.
//

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
        
        virtual ~SWMFIono();
        
    protected:
        void initializeConversionFactorsToSI();
        void initializeSIUnits();
    
    private://
        
        
        
    };
    
    
    
}
#endif
