/*
 * SWMFIonoInterpolator.h
 *
 *  Created on: May 25, 2012
 *      Author: David Berrios
 */

#ifndef _SWMFIonoInterpolator_h
#define _SWMFIonoInterpolator_h

#include "Interpolator.h"
#include "Model.h"

namespace ccmc{ 
    
    class SWMFIonoInterpolator: public Interpolator
    {
     
    public:
        SWMFIonoInterpolator(Model * model);
        float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon);
        float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
        float interpolate(const long& variable_id, const float& r, const float& lat, const float& lon);
        float interpolate(const long& variable_id, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
        virtual ~SWMFIonoInterpolator();
    
    private:
        
        std::string th_string;
        std::string phi_string;
        int theta_size;
        int phi_size;

        const std::vector<float> * th_data;
    	const std::vector<float> * phi_data;
       	//const std::vector<float> * phi_data;

		float previous_th;
		float previous_phi;
		int previous_th_index;
		int previous_phi_index;

		int index_2D_to_1D(const int& ilat, const int& ilon);

    };
    
}



#endif
