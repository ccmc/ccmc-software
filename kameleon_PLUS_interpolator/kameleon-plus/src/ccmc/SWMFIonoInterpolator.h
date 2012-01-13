//
//  SWMFInterpolator.h
//  
//
//  Created by Nitesh Donti on 1/11/12.
//  Copyright 2012 Cornell University. All rights reserved.
//

#ifndef _SWMFInterpolator_h
#define _SWMFInterpolator_h

#include "Interpolator.h"
#include "Model.h"

namespace ccmc{ 
    
    class SWMFInterpolator: public Interpolator
    {
     
    public:
        SWMFInterpolator(Model * model);
        float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon);
        float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
        float interpolate(const long& variable_id, const float& r, const float& lat, const float& lon);
        float interpolate(const long& variable_id, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
        virtual ~SWMFInterpolator();
    
    private:
        
        std::string r_string;
        std::string th_string;
        std::string phi_string;

        const std::vector<float> * r_data;
    	const std::vector<float> * th_data;
       	const std::vector<float> * phi_data;

		float previous_r;
		float previous_th;
		float previous_phi;

    };
    
}



#endif
