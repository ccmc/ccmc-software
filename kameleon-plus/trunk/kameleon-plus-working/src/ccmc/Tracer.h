#ifndef CCMC_TRACER_H
#define CCMC_TRACER_H

//#ifdef __cplusplus
//#include <map>
#include <vector>
#include <string>
#include "Kameleon.h"
#include "Interpolator.h"
#include "Point3f.h"
#include "Fieldline.h"
#include <boost/unordered_map.hpp>
//using namespace std;
/**
 * @namespace ccmc
 * @brief Namespace for the Community Coordinated Modeling Center code
 *
 * Full documentation for the ccmc namespace
 */
namespace ccmc
{
	class Kameleon;

	/**
	 * @class Tracer
	 * @brief TODO: Brief description
	 *
	 * Full Description of Tracer class
	 */
	class Tracer
	{
		public:
			enum Direction {FOWARD, REVERSE};
			Tracer(Kameleon * kameleon);
			Tracer(Kameleon * kameleon, Interpolator * interpolator);
			~Tracer();
			//void open(char * filename);
			//void loadVariable(char * variable);
			void setInnerBoundary(float r_end);
			void setMaxIterations(int stepMax);
			void setDn(float dn);
			void setTilt(float tilt);
			void setMissing(float missing);
			void setMaxArcLength(float maxArcLength);
			Fieldline
					bidirectionalTrace(const std::string& variable, const float& startComponent1, const float& startComponent2,
							const float& startComponent3);
			Fieldline bidirectionalTraceWithDipole(const std::string& variable, const float& startComponent1, const float& startComponent2,
							const float& startComponent3);
			Fieldline unidirectionalTrace(const std::string& variable, const float& startComponent1, const float& startComponent2,
					const float& startComponent3, const Direction& dir = FOWARD);
			Fieldline unidirectionalTraceWithDipole(const std::string& variable, const float& startComponent1, const float& startComponent2,
					const float& startComponent3, const Direction& dir = FOWARD);
			Fieldline findLastClosed(Point3f position, int stepMax);
			std::vector<Fieldline> getLastClosedFieldlines(int numberOfFieldlines, int stepMax, int numberOfPointsPerReducedFieldline);
			bool isClosed(Fieldline& fieldline);
			void setUseMaxArcLength(bool useMaxArcLength);
			Point3f getVector(const std::string& variable, const Point3f& position, float& dComponent1,
					float& dComponent2, float& dComponent3, const Interpolator * interpolator);
			void setRegionOfInterest(Point3f min, Point3f max);
			void setUseRegionOfInterest(bool useROI);
			Interpolator * interpolator;
			//void close();

		private:
			std::string filename;

			float r_end;
			int step_max;
			float dn;
			float tilt;
			float missing;
			float maxArcLength;
			std::string b_,bx_,by_,bz_;
			std::string br_,bphi_,btheta_;
			std::string ux_,uy_,uz_;
			std::string ur_,uphi_,utheta_;
			std::string exb_, exbx_,exby_,exbz_;
			std::string j_, jx_, jy_, jz_;
			std::string jr_, jphi_, jtheta_;
			std::string b1_, b1x_, b1y_, b1z_;

			bool useMaxArcLength;
			bool useROI;
			Point3f minROI;
			Point3f maxROI;

			Fieldline cartesianTrace(const std::string& variable, const float& startComponent1, const float& startComponent2,
							const float& startComponent3, Interpolator * interpolator, const Direction& dir = FOWARD);
			Fieldline cartesianTraceWithDipole(const std::string& variable, const float& startComponent1, const float& startComponent2,
					const float& startComponent3, Interpolator * interpolator, const Direction& dir = FOWARD);
			Fieldline sphericalTrace(const std::string& variable, const float& startComponent1, const float& startComponent2,
					const float& startComponent3, Interpolator * interpolator, const Direction& dir = FOWARD);

			bool isValidPoint(const Point3f& position, const Point3f& min, const Point3f& max);
			bool isValidIteration(int iterations, int max);
			float calculateDT(float& dComponent1, float& dComponent2, float& dComponent3, float& dn);
			Kameleon * kameleon;
			boost::unordered_map<std::string, std::vector<std::string> > componentNamesMap;
			void initializeComponentNamesMap();
			bool interpolatorAllocated;



	};
}

//#endif//__cplusplus
#endif//CCMC_TRACER_H
