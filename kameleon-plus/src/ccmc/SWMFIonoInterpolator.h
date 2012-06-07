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
        std::string psi_string;
        int theta_size;
        int psi_size;

        const std::vector<float> * th_data;
    	const std::vector<float> * psi_data;
       	//const std::vector<float> * phi_data;

		float previous_th;
		float previous_psi;

		int index_2D_to_1D(const int& ilat, const int& ilon);

    };
    
}



#endif
