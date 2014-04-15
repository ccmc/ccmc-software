#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include "Tracer.h"
#include "Point3f.h"
#include <boost/lexical_cast.hpp>
#include "StringConstants.h"
//#define DEBUG_TRACER
#define DEPRECATED_WARNING

using namespace std;

//Tracer tracer(NULL);
//Tracer static_tracer_object(NULL);

namespace ccmc
{
	/**
	 * TODO: finish documentation
	 * @param kameleon
	 */
	Tracer::Tracer(Kameleon * kameleon)
	{
		//	cout << "Tracer object created" << endl;
		this->kameleon = kameleon;
		missing = kameleon->getMissingValue();
		dn = .2f;

		initializeComponentNamesMap();

		interpolator = kameleon->createNewInterpolator();
		interpolatorAllocated = true;

		step_max = 2000;
		useMaxArcLength = false;
		useROI = false;
		//std::cout << "created Tracer object" << std::endl;
		if (kameleon->getModelName() == ccmc::strings::models::enlil_ ||
				kameleon->getModelName() == ccmc::strings::models::mas_)
		{
			r_end = 1.f;
		} else
			r_end = 3.0f;

#ifdef DEBUG_TRACER
		cerr << "After setting box dimensions" << endl;
#endif
	}

	Tracer::Tracer(Kameleon * kameleon, Interpolator * interpolator)
	{
		//	cout << "Tracer object created" << endl;
		this->kameleon = kameleon;
		missing = kameleon->getMissingValue();
		dn = .2f;

		initializeComponentNamesMap();

		this->interpolator = interpolator;
		interpolatorAllocated = false; //caller will be responsible for deallocation

		step_max = 2000;
		useMaxArcLength = false;
		useROI = false;
		//std::cout << "created Tracer object" << std::endl;
		if (kameleon->getModelName() == ccmc::strings::models::enlil_ ||
				kameleon->getModelName() == ccmc::strings::models::mas_)
		{
			r_end = 1.f;
		} else
			r_end = 3.0f;

#ifdef DEBUG_TRACER
		cerr << "After setting box dimensions" << endl;
#endif
	}

	/**
	 *
	 */
	void Tracer::initializeComponentNamesMap()
	{
		std::vector<std::string> bnames;
		std::vector<std::string> b1names;
		std::vector<std::string> b_1names;
		std::vector<std::string> unames;
		std::vector<std::string> jnames;
		std::vector<std::string> jxbnames;
		std::vector<std::string> exbnames;

		if (kameleon->doesVariableExist("r"))
		{
			bnames.push_back(ccmc::strings::variables::br_);
			bnames.push_back("btheta");
			bnames.push_back("bphi");

			b1names.push_back("b1r");
			b1names.push_back("b1theta");
			b1names.push_back("bphi");

			unames.push_back("ur");
			unames.push_back("utheta");
			unames.push_back("uphi");

			jnames.push_back("jr");
			jnames.push_back("jtheta");
			jnames.push_back("jphi");

			jxbnames.push_back("jxbr");
			jxbnames.push_back("jxbtheta");
			jxbnames.push_back("jxbphi");

			exbnames.push_back("exbr");
			exbnames.push_back("exbtheta");
			exbnames.push_back("exbphi");

			componentNamesMap["b"] = bnames;
			componentNamesMap["br"] = bnames;
			componentNamesMap["btheta"] = bnames;
			componentNamesMap["bphi"] = bnames;
			componentNamesMap["b1"] = b1names;
			componentNamesMap["b1r"] = b1names;
			componentNamesMap["b1theta"] = b1names;
			componentNamesMap["b1phi"] = b1names;

			componentNamesMap["u"] = unames;
			componentNamesMap["ur"] = unames;
			componentNamesMap["utheta"] = unames;
			componentNamesMap["uphi"] = unames;

			componentNamesMap["j"] = jnames;
			componentNamesMap["jr"] = jnames;
			componentNamesMap["jtheta"] = jnames;
			componentNamesMap["jphi"] = jnames;

			componentNamesMap["jxb"] = jxbnames;
			componentNamesMap["jxbr"] = jxbnames;
			componentNamesMap["jxbtheta"] = jxbnames;
			componentNamesMap["jxbphi"] = jxbnames;

			componentNamesMap["exb"] = exbnames;
			componentNamesMap["exbr"] = exbnames;
			componentNamesMap["exbtheta"] = exbnames;
			componentNamesMap["exbphi"] = exbnames;
		} else
		{
			bnames.push_back("bx");
			bnames.push_back("by");
			bnames.push_back("bz");

			b1names.push_back("b1x");
			b1names.push_back("b1y");
			b1names.push_back("b1z");

			b_1names.push_back("bx1");
			b_1names.push_back("by1");
			b_1names.push_back("bz1");

			unames.push_back("ux");
			unames.push_back("uy");
			unames.push_back("uz");

			jnames.push_back("jx");
			jnames.push_back("jy");
			jnames.push_back("jz");

			jxbnames.push_back("jxbx");
			jxbnames.push_back("jxby");
			jxbnames.push_back("jxbz");

			exbnames.push_back("exbx");
			exbnames.push_back("exby");
			exbnames.push_back("exbz");

			componentNamesMap["b"] = bnames;
			componentNamesMap["bx"] = bnames;
			componentNamesMap["by"] = bnames;
			componentNamesMap["bz"] = bnames;
			componentNamesMap["b1"] = b1names;
			componentNamesMap["b1x"] = b1names;
			componentNamesMap["b1y"] = b1names;
			componentNamesMap["b1z"] = b1names;

			componentNamesMap["u"] = unames;
			componentNamesMap["ux"] = unames;
			componentNamesMap["uy"] = unames;
			componentNamesMap["uz"] = unames;

			componentNamesMap["j"] = jnames;
			componentNamesMap["jx"] = jnames;
			componentNamesMap["jy"] = jnames;
			componentNamesMap["jz"] = jnames;

			componentNamesMap["jxb"] = jxbnames;
			componentNamesMap["jxbx"] = jxbnames;
			componentNamesMap["jxby"] = jxbnames;
			componentNamesMap["jxbz"] = jxbnames;

			componentNamesMap["exb"] = exbnames;
			componentNamesMap["exbx"] = exbnames;
			componentNamesMap["exby"] = exbnames;
			componentNamesMap["exbz"] = exbnames;

			componentNamesMap["b_1"] = b_1names;
			componentNamesMap["bx1"] = b_1names;
			componentNamesMap["by1"] = b_1names;
			componentNamesMap["bz1"] = b_1names;

		}






		/*componentNamesMap["br"];
		 componentNamesMap["bphi"];
		 componentNamesMap["btheta"];
		 componentNamesMap["b"];
		 componentNamesMap["ux"];
		 componentNamesMap["uy"];
		 componentNamesMap["uz"];
		 componentNamesMap["ur"];
		 componentNamesMap["uphi"];
		 componentNamesMap["utheta"];
		 componentNamesMap["exb"];
		 componentNamesMap["exbx"];
		 componentNamesMap["exby"];
		 componentNamesMap["exbz"];
		 componentNamesMap["j"];
		 componentNamesMap["jx"];
		 componentNamesMap["jy"];
		 componentNamesMap["jz"];
		 componentNamesMap["jr"];
		 componentNamesMap["jphi"];
		 componentNamesMap["jtheta"];
		 componentNamesMap["b1"];
		 componentNamesMap["b1x"];
		 componentNamesMap["b1y"];
		 componentNamesMap["b1z"];*/
	}
	/**
	 * TODO: finish documentation
	 */
	Tracer::~Tracer()
	{
		if (interpolatorAllocated)
		{
			delete interpolator;
		}
		//cout << "Tracer object destroyed" << endl;
	}

	/**
	 * Opens a CCMC generated CDF file and initializes the model name
	 * @param filename The filename of the cdf file to open.
	 */
	/*
	 void Tracer::open(char * filename)
	 {
	 derived.open(filename);
	 this->filename = filename;
	 char * cmodelname = derived.getGlobalAttributeString("model_name");
	 if (cmodelname == NULL)
	 {
	 this->model_name = "";
	 } else
	 {
	 this->model_name = cmodelname;
	 }

	 this->model_name = derived.getGlobalAttributeString("model_name");

	 FileReader fileReader;
	 fileReader.open(filename);
	 model_name = (fileReader.getGlobalAttribute("model_name")).getAttributeString();
	 fileReader.close();
	 cout << "setting filename to: '" << filename << "'" << endl;
	 cout << "setting model_name to: '" << model_name << "'" << endl;
	 }*/

	/**
	 * Closes the opened CDF file
	 */
	/*
	 void Tracer::close()
	 {
	 derived.close();
	 }*/

	/**
	 * Loads the variable, including all components of the variable, into memory.
	 * For example, if variable is "bx", three variables will be loaded: "bx",
	 * "by", and "bz"
	 * @param variable
	 */
	/*
	 void Tracer::loadVariable(char * variable)
	 {

	 derived.loadVectorVariable(variable);
	 }*/

	/**
	 * TODO: finish documentation
	 * @param variable
	 * @param startComponent1
	 * @param startComponent2
	 * @param startComponent3
	 */
	Fieldline Tracer::bidirectionalTraceWithDipole(const std::string& variable, const float& startComponent1,
			const float& startComponent2, const float& startComponent3)
	{
		std::string model_name = kameleon->getModelName();

//		Interpolator * interpolator = kameleon->createNewInterpolator();
		Fieldline f1;
		if (model_name == ccmc::strings::models::open_ggcm_ ||
				model_name == ccmc::strings::models::ucla_ggcm_ ||
				model_name == ccmc::strings::models::batsrus_ ||
				model_name == ccmc::strings::models::adapt3d_ ||
				model_name == ccmc::strings::models::lfm_)
		{
			f1 = cartesianTraceWithDipole(variable, startComponent1, startComponent2, startComponent3, interpolator,
					FOWARD);
		} else
		{
			//derived.loadVariable(variable);

			f1 = sphericalTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, FOWARD);
		}
		f1.removePoint(0);

		Fieldline f2;
		if (model_name == ccmc::strings::models::open_ggcm_ ||
				model_name == ccmc::strings::models::ucla_ggcm_ ||
				model_name == ccmc::strings::models::batsrus_ ||
				model_name == ccmc::strings::models::adapt3d_ ||
				model_name == ccmc::strings::models::lfm_)
		{

			f2 = cartesianTraceWithDipole(variable, startComponent1, startComponent2, startComponent3, interpolator,
					REVERSE);
		} else
		{
			f2 = sphericalTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, REVERSE);
		}

		Fieldline f;
		f = f2.reverseOrder();
		vector<Point3f> f1Positions = f1.getPositions();
		vector<float> f1Data = f1.getData();
		for (int i = 0; i < f1Positions.size(); i++)
		{
			f.insertPointData(f1Positions[i], f1Data[i]);
		}
		f.setStartPoint(Point3f(startComponent1, startComponent2, startComponent3));
		f.setStartIndex(f2.size()-1);
		return f;

	}

	/**
	 * TODO: finish documentation
	 * @param variable
	 * @param startComponent1
	 * @param startComponent2
	 * @param startComponent3
	 */
	Fieldline Tracer::bidirectionalTrace(const std::string& variable, const float& startComponent1,
			const float& startComponent2, const float& startComponent3)
	{
		Fieldline f1(this->step_max*2);
		std::string model_name = kameleon->getModelName();
//		Interpolator * interpolator = kameleon->createNewInterpolator(); A. Pembroke removed
		if (model_name == ccmc::strings::models::open_ggcm_ ||
				model_name == ccmc::strings::models::ucla_ggcm_ ||
				model_name == ccmc::strings::models::batsrus_ ||
				model_name == ccmc::strings::models::adapt3d_ ||
				model_name == ccmc::strings::models::lfm_)
		{
			f1 = cartesianTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, FOWARD);
		} else
		{
//			derived.loadVariable(variable);
			std::cerr << "calling spherical trace" << std::endl;
			f1 = sphericalTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, FOWARD);
		}
//		std::cerr << "after the first trace" << std::endl;
		if (f1.size() == 0)
		{
			return f1;
		}
		f1.removePoint(0);

		Fieldline f2(this->step_max *2);

		if (model_name == ccmc::strings::models::open_ggcm_ ||
				model_name == ccmc::strings::models::ucla_ggcm_ ||
				model_name == ccmc::strings::models::batsrus_ ||
				model_name == ccmc::strings::models::adapt3d_ ||
				model_name == ccmc::strings::models::lfm_ )
		{
			f2 = cartesianTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, REVERSE);
		} else
		{
			f2 = sphericalTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, REVERSE);
		}
		if (f2.size() == 0)
		{
			return f1;
		}
		int startIndex;
		startIndex = f2.size();

		//Fieldline f;
		f2.reverseOrderInPlace();
		vector<Point3f> f1Positions = f1.getPositions();
		vector<float> f1Data = f1.getData();
		for (int i = 0; i < f1Positions.size(); i++)
		{
			f2.insertPointData(f1Positions[i], f1Data[i]);
		}
		f2.setStartPoint(Point3f(startComponent1, startComponent2, startComponent3));
		f2.setStartIndex(startIndex);
		return f2;
	}

	/**
	 * TODO: finish documentation
	 * @param variable
	 * @param startComponent1
	 * @param startComponent2
	 * @param startComponent3
	 * @param dir
	 */
	Fieldline Tracer::unidirectionalTrace(const std::string& variable, const float& startComponent1,
			const float& startComponent2, const float& startComponent3, const Direction& dir)
	{
		Fieldline f;
		std::string model_name = kameleon->getModelName();
		//model_name = derived.getGlobalAttributeString("model_name");
//		Interpolator * interpolator = kameleon->createNewInterpolator(); //A. Pembroke removed
		if (model_name == ccmc::strings::models::open_ggcm_ ||
				model_name == ccmc::strings::models::ucla_ggcm_ ||
				model_name == ccmc::strings::models::batsrus_ ||
				model_name == ccmc::strings::models::adapt3d_ ||
				model_name == ccmc::strings::models::lfm_ )
		{
			f = cartesianTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, dir);
		} else
		{
			f = sphericalTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, dir);
		}
		f.setStartPoint(Point3f(startComponent1, startComponent2, startComponent3));
		f.setStartIndex(0);
		return f;
	}

	/**
	 * TODO: finish documentation
	 * @param variable
	 * @param startComponent1
	 * @param startComponent2
	 * @param startComponent3
	 * @param dir
	 */
	Fieldline Tracer::unidirectionalTraceWithDipole(const std::string& variable, const float& startComponent1,
			const float& startComponent2, const float& startComponent3, const Direction& dir)
	{
		Fieldline f;
		std::string model_name = kameleon->getModelName();
		//model_name = derived.getGlobalAttributeString("model_name");
//		Interpolator * interpolator = kameleon->createNewInterpolator(); // A. Pembroke removed
		if (model_name == ccmc::strings::models::open_ggcm_ ||
				model_name == ccmc::strings::models::ucla_ggcm_ ||
				model_name == ccmc::strings::models::batsrus_ ||
				model_name == ccmc::strings::models::adapt3d_ ||
				model_name == ccmc::strings::models::lfm_ )
		{
			f = cartesianTraceWithDipole(variable, startComponent1, startComponent2, startComponent3,
							interpolator, dir);
		} else
		{
			f = sphericalTrace(variable, startComponent1, startComponent2, startComponent3, interpolator, dir);
		}
		f.setStartPoint(Point3f(startComponent1, startComponent2, startComponent3));
		f.setStartIndex(0);
		return f;
	}

	/**
	 * TODO: finish documentation
	 * @param variable
	 * @param startComponent1
	 * @param startComponent2
	 * @param startComponent3
	 * @param interpolator
	 * @param dir
	 */
	Fieldline Tracer::cartesianTraceWithDipole(const std::string& variable, const float& startComponent1,
			const float& startComponent2, const float& startComponent3, Interpolator * interpolator,
			const Direction& dir)
	{
		Point3f min;//point representing the min values of the three dimensions;
		Point3f max;//point representing the max values of the three dimensions;
		std::string model_name = kameleon->getModelName();

		if (model_name == ccmc::strings::models::batsrus_)
		{
#ifdef DEBUG_TRACER
			cerr << "inside. model_name: \"" << model_name << "\"" << endl;
#endif

			min.component1 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_x_min_)).getAttributeFloat();
			min.component2 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_y_min_)).getAttributeFloat();
			min.component3 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_z_min_)).getAttributeFloat();

			max.component1 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_x_max_)).getAttributeFloat();
			max.component2 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_y_max_)).getAttributeFloat();
			max.component3 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_z_max_)).getAttributeFloat();

		} else
		{

			if (model_name == ccmc::strings::models::ucla_ggcm_ ||
					model_name == ccmc::strings::models::open_ggcm_)
			{
				/** the signs of x and y are flipped **/

				/*min.component1 = -1.0 * *(float *) vattribute_get("x", "actual_max");
				 min.component2 = -1.0 * *(float *) vattribute_get(ccmc::strings::variables::y_, "actual_max");
				 min.component3 = *(float *) vattribute_get(ccmc::strings::variables::z_, "actual_min");

				 max.component1 = -1.0 * *(float *) vattribute_get(ccmc::strings::variables::x_, "actual_min");
				 max.component2 = -1.0 * *(float *) vattribute_get(ccmc::strings::variables::y_, "actual_min");
				 max.component3 = *(float *) vattribute_get(ccmc::strings::variables::z_, "actual_max");*/
				min.component1 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
				min.component2 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
				min.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();

				max.component1 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
				max.component2 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
				max.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();

			} else
			{
				min.component1 = (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
				min.component2 = (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
				min.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();

				max.component1 = (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
				max.component2 = (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
				max.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
			}

		}
		Fieldline fieldline = cartesianTrace(variable, startComponent1, startComponent2, startComponent3, interpolator,
				dir);
		Point3f lastPoint;
		Point3f newPoint;
		float dpdn = dn * dn;
		if (fieldline.size() == 0)
		{
			lastPoint.component1 = startComponent1;
			lastPoint.component2 = startComponent2;
			lastPoint.component3 = startComponent3;

		} else
			lastPoint = fieldline.getPositions()[fieldline.size() - 1];
		if (lastPoint.magnitude() <= r_end && lastPoint.magnitude() > 1.0)
		{
			Point3f dpStartPoint = lastPoint;

			Point3f point;
			point = dpStartPoint;
			float sintilt = std::sin(tilt);
			float costilt = std::cos(tilt);
			float rr;
			float zz1;
			float bdp_rr5;
			float bx_dip;
			float by_dip;
			float bz_dip;
			float bdp;
			std::string bdp_string = (kameleon->getGlobalAttribute("b_dipole")).getAttributeString();
			if (bdp_string != "")
			{
				bdp = boost::lexical_cast<float>(bdp_string);
			} else
			{
				bdp = -31100.0;
			}
			tilt = (kameleon->getGlobalAttribute("dipole_tilt")).getAttributeFloat();
			rr = point.magnitude();
			//cout << "point.magnitude(): " << point.magnitude() << endl;
			bdp_rr5 = bdp / std::pow((double) rr, 5.0);
			//cout << "bdp: " << bdp << " bdp_rr5: " << bdp_rr5 << endl;
			zz1 = -1.0 * point.component1 * sintilt + point.component3 * costilt;
			bx_dip = bdp_rr5 * (3. * point.component1 * zz1 + rr * rr * sintilt);
			//cout << "point: " << point << " bx_dip: " << bx_dip << endl;
			by_dip = bdp_rr5 * 3. * point.component2 * zz1;
			bz_dip = bdp_rr5 * (3. * point.component3 * zz1 - rr * rr * costilt);
			Point3f dipolePoint(bx_dip, by_dip, bz_dip);
			dipolePoint.normalize();
			dipolePoint = dipolePoint * (dn / 2.0);

			//cout << "dipolePoint: " << dipolePoint << " dn: " << dn << endl;
			//dipolePoint = dipolePoint * dn;
			//cout << "dipolePoint: " << dipolePoint << " dn: " << dn << endl;


			//Point3f newPoint = point;// + dipolePoint;
			newPoint = point + dipolePoint;
			Point3f previousPoint;
			//cout << "Previous point: " << point << " new point: " << newPoint << endl;
			while (newPoint.magnitude() <= r_end && newPoint.magnitude() > 1.0)
			{
				//cout << "here" << endl;
				fieldline.insertPointData(newPoint, missing);
				previousPoint = newPoint;
				rr = previousPoint.magnitude();
				//cout << "previousPoint.magnitude(): " << previousPoint.magnitude() << endl;
				bdp_rr5 = bdp / std::pow((double) rr, 5.0);
				//cout << "bdp: " << bdp << " bdp_rr5: " << bdp_rr5 << endl;
				zz1 = -1.0 * previousPoint.component1 * sintilt + previousPoint.component3 * costilt;
				bx_dip = bdp_rr5 * (3. * previousPoint.component1 * zz1 + rr * rr * sintilt);
				//cout << "point: " << previousPoint << " bx_dip: " << bx_dip << endl;
				by_dip = bdp_rr5 * 3. * previousPoint.component2 * zz1;
				bz_dip = bdp_rr5 * (3. * previousPoint.component3 * zz1 - rr * rr * costilt);
				Point3f dipolePoint(bx_dip, by_dip, bz_dip);
				dipolePoint.normalize();
				dipolePoint = dipolePoint * (dn / 2.0);

				//cout << "dipolePoint: " << dipolePoint << " dn: " << dn << endl;
				//dipolePoint = dipolePoint * dn;
				//cout << "dipolePoint: " << dipolePoint << " dn: " << dn << endl;


				//Point3f newPoint = point;// + dipolePoint;
				newPoint = previousPoint + dipolePoint;
				//cout << "here" << endl;
				//cout << "Previous point: " << point << " new point: " << newPoint << endl;

			}
			cout << "here : " << newPoint << " fieldline.size(): " << fieldline.size() << " magnitude: "
					<< newPoint.magnitude() << endl;

			//if the dipole tracing stopped at the inner boundary, continue with regular tracing

			if (newPoint.magnitude() >= r_end && fieldline.size() > 1)
			{
				//if (newPoint.distance(previousPoint) > 1e5)
				//{
				Fieldline fieldline2 = cartesianTraceWithDipole(variable, newPoint.component1, newPoint.component2,
						newPoint.component3, interpolator, dir);
				for (int i = 0; i < fieldline2.size(); i++)
					fieldline.insertPointData(fieldline2.getPositions()[i], fieldline2.getData()[i]);
				//}
			}

		}

		return fieldline;

	}

	/**
	 * TODO: finish documentation
	 * @param variable
	 * @param startComponent1
	 * @param startComponent2
	 * @param startComponent3
	 * @param dir
	 */
	Fieldline Tracer::cartesianTrace(const std::string& variable, const float& startComponent1,
			const float& startComponent2, const float& startComponent3, Interpolator * interpolator,
			const Direction& dir)
	{

		float adjusted_dn = dn;
		if (dir == REVERSE)
		{
			adjusted_dn = -dn;
		}
#ifdef DEBUG_TRACER
		cerr << "entered cartesianTrace" << endl;
		cerr << "variable: " << variable << endl;
#endif
		Point3f min;//point representing the min values of the three dimensions;
		Point3f max;//point representing the max values of the three dimensions;
		std::string model_name = kameleon->getModelName();
		float arcLength = 0.f;

		if(!useROI)
		{

			if (model_name == ccmc::strings::models::batsrus_)
			{

#ifdef DEBUG_TRACER
			cerr << "inside. model_name: \"" << model_name << "\"" << endl;
#endif

				min.component1 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_x_min_)).getAttributeFloat();
				min.component2 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_y_min_)).getAttributeFloat();
				min.component3 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_z_min_)).getAttributeFloat();

				max.component1 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_x_max_)).getAttributeFloat();
				max.component2 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_y_max_)).getAttributeFloat();
				max.component3 = (kameleon->getGlobalAttribute(ccmc::strings::attributes::global_z_max_)).getAttributeFloat();
				//std::cout << "Tracer min: " << min << " max: " << max << std::endl;

			} else
			{

				if (model_name == ccmc::strings::models::ucla_ggcm_ || model_name == ccmc::strings::models::open_ggcm_)
				{
					/** the signs of x and y are flipped **/

					/*min.component1 = -1.0 * *(float *) vattribute_get(ccmc::strings::variables::x_, "actual_max");
					 min.component2 = -1.0 * *(float *) vattribute_get(ccmc::strings::variables::y_, "actual_max");
					 min.component3 = *(float *) vattribute_get(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_min_);

					 max.component1 = -1.0 * *(float *) vattribute_get(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_min_);
					 max.component2 = -1.0 * *(float *) vattribute_get(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_min_);
					 max.component3 = *(float *) vattribute_get(ccmc::strings::variables::z_, "actual_max");*/
					min.component1 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
					min.component2 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
					min.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();

					max.component1 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
					max.component2 = -1.0 * (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
					max.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();

				} else
				{
//					std::cout<<"Retrieving actual_min and actual_max from file \n";
					min.component1 = (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
					min.component2 = (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
					min.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();

					max.component1 = (kameleon->getVariableAttribute(ccmc::strings::variables::x_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
					max.component2 = (kameleon->getVariableAttribute(ccmc::strings::variables::y_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
					max.component3 = (kameleon->getVariableAttribute(ccmc::strings::variables::z_, ccmc::strings::attributes::actual_max_)).getAttributeFloat();
//					std::cout<<"min"<<min<<" max"<<max<<endl;
				}

			}
		} else
		{
			min = minROI;
			max = maxROI;
		}

		/** from Lutz's trace_fieldline_cdf.c **/
		/* calculate maximum iteration step 'dt' to limit spatial advance in
		 each direction to delta times the local grid spacing:
		 dt*bx<=delta*dComponent1, dt*by<=delta*dComponent2, , dt*bz<=delta*dComponent3 */
		Point3f oldPoint;
		float eps = 1e-5;
		Point3f point;

		float dComponent1 = -0.01;
		float dComponent2 = -0.01;
		float dComponent3 = -0.01;
		Point3f currentVectorData;
		int return_status;
		bool isB1 = false;
		float min_block_size = 1e-2;

		Fieldline fieldline;
		//vector<Point3f> streamline;
		//vector<float> mag1;
		//vector<float> mag2;
		//vector<float> magnitudes;
		//string model_name = gattribute_char_get("model_name");

		float sintilt = std::sin(tilt);
		float costilt = std::cos(tilt);
		float rr;
		float zz1;
		float bdp_rr5;
		float bx_dip;
		float by_dip;
		float bz_dip;
		float bdp;
		string bdp_string;
		string variable_string = variable;

		//vector<Point3f> fieldline1;
		//vector<Point3f> fieldline2;

		// get first half of streamline

		if (model_name == ccmc::strings::models::batsrus_)
		{
#ifdef DEBUG_TRACER
			cerr << "inside. model_name: \"" << model_name << "\"" << endl;
#endif

			if (variable_string == "b1" || variable_string == "b1x" || variable_string == "b1y" || variable_string
					== "b1z")
			{
				isB1 = true;
				std::string bdp_string = (kameleon->getGlobalAttribute("b_dipole")).getAttributeString();
				if (bdp_string != "")
				{
					bdp = boost::lexical_cast<float>(bdp_string);
				} else
				{
					bdp = -31100.0;
				}

				tilt = (kameleon->getGlobalAttribute("dipole_tilt")).getAttributeFloat();
#ifdef DEBUG_TRACER
				cerr << "bdp: " << bdp << " tilt: " << tilt << endl;
#endif

			}
		}

		Point3f startPoint(startComponent1, startComponent2, startComponent3);
#ifdef DEBUG_TRACER
		cerr << "before getVector, variable: " << variable << endl;
#endif
		Point3f vectorValue = getVector(variable, startPoint, dComponent1, dComponent2, dComponent3, interpolator);
#ifdef DEBUG_TRACER
		cerr << "After getVector for: " << variable << " = " << vectorValue<< endl;
#endif

		if (vectorValue.component1 == missing ||
			vectorValue.component2 == missing ||
			vectorValue.component3 == missing){
			std::cout<<"Fieldline returning empty\n";
			return fieldline; //empty
		}

		//start.addVariableData(variable, vectorValue.magnitude());

		Point3f start = Point3f(startComponent1, startComponent2, startComponent3);
		fieldline.insertPointData(start, vectorValue.magnitude());
		Point3f previous = startPoint;
		bool finished = false;
		int iterations = 0;
		oldPoint.component1 = 1e-20;
		oldPoint.component2 = 1e-20;
		oldPoint.component3 = 1e-20;

#ifdef DEBUG_TRACER
		cerr << "min: " << min << " max: " << max << endl;
		cerr << "finished: " << finished << endl;
#endif

		while (!finished && isValidPoint(previous, min, max))
		{
			Point3f addition;

			if (isB1 == true && bdp != 0.)
			{
				rr = previous.magnitude();
				bdp_rr5 = bdp / pow((double) rr, 5.0);
				zz1 = -1.0 * previous.component1 * sintilt + previous.component3 * costilt;
				bx_dip = bdp_rr5 * (3. * previous.component1 * zz1 + rr * rr * sintilt);
				by_dip = bdp_rr5 * 3. * previous.component2 * zz1;
				bz_dip = bdp_rr5 * (3. * previous.component3 * zz1 - rr * rr * costilt);
				vectorValue.component1 = vectorValue.component1 + bx_dip;
				vectorValue.component2 = vectorValue.component2 + by_dip;
				vectorValue.component3 = vectorValue.component3 + bz_dip;
			}

			float magValue = vectorValue.magnitude();
#ifdef DEBUG_TRACER
			cerr << "magValue: " << magValue << endl;
			cerr << "before ---- eps: " << eps << " dComponent1: " << dComponent1 << " dComponent2: " << dComponent2 << " dComponent3: " << dComponent3 << endl;

#endif
			if (isnan(dComponent1) || dComponent1 < min_block_size)
			{
				dComponent1 = min_block_size;
				dComponent2 = min_block_size;
				dComponent3 = min_block_size;
			}
			if (isnan(dComponent2) || dComponent2 < min_block_size)
			{
				dComponent1 = min_block_size;
				dComponent2 = min_block_size;
				dComponent3 = min_block_size;
			}
			if (isnan(dComponent3) || dComponent3 < min_block_size)
			{
				dComponent1 = min_block_size;
				dComponent2 = min_block_size;
				dComponent3 = min_block_size;
			}
			dComponent1 *= magValue / (fabs(vectorValue.component1) + eps * (fabs(vectorValue.component1) < eps)); // ds = [dx*B/Bx, dy*B/By, dz*B/Bz]
			dComponent2 *= magValue / (fabs(vectorValue.component2) + eps * (fabs(vectorValue.component2) < eps));
			dComponent3 *= magValue / (fabs(vectorValue.component3) + eps * (fabs(vectorValue.component3) < eps));

#ifdef DEBUG_TRACER
			cerr << "after ---- eps: " << eps << " dComponent1: " << dComponent1 << " dComponent2: " << dComponent2 << " dComponent3: " << dComponent3 << endl;
#endif

			// Use Fourth Order Runge-Kutta
			// Should make this method flexible, so user can select order
			if (magValue < eps)
				magValue = eps;

			float dt;

			//		if (dt < .01)
			//			dt = .01;



			Point3f k1 = getVector(variable, previous, dComponent1, dComponent2, dComponent3, interpolator);
			k1.normalize();
			dt = calculateDT(dComponent1, dComponent2, dComponent3, adjusted_dn);
			Point3f k2 = getVector(variable, previous + k1 * (.5 * dt), dComponent1, dComponent2, dComponent3,
					interpolator);
			k2.normalize();
			dt = calculateDT(dComponent1, dComponent2, dComponent3, adjusted_dn);
			Point3f k3 = getVector(variable, previous + k2 * (.5 * dt), dComponent1, dComponent2, dComponent3,
					interpolator);
			k3.normalize();
			dt = calculateDT(dComponent1, dComponent2, dComponent3, adjusted_dn);

#ifdef DEBUG_TRACER
			cerr << "dt: " << dt << endl;
#endif

			Point3f k4 = getVector(variable, previous + k3 * dt, dComponent1, dComponent2, dComponent3, interpolator);
			k4.normalize();
			//addition.component1 = dt/2.0*vectorValue.component1/magValue;
			//addition.component2 = dt/2.0*vectorValue.component2/magValue;
			//addition.component3 = dt/2.0*vectorValue.component3/magValue;
			addition = (k1 + k2 * 2.0f + k3 * 2.0f + k4) * (1.0f / 6.0f);
			//addition.normalize();
			Point3f newPoint = previous + addition * dt;
			float dist = previous.distance(newPoint);

			/** check if tracing went anywhere, else stop **/
			if (dist < 1e-10 || newPoint.distance(oldPoint) < 1e-4)
			{
				finished = true;
				return_status = 3;
			} else
			{
				arcLength += dist;
				if (this->useMaxArcLength && arcLength > this->maxArcLength)
				{
					finished = true;
				}
				oldPoint = previous;
				//	 newPointData;
				//	newPointData.setPosition(newPoint);
				iterations++;
				if (iterations > step_max)
					finished = true;

				previous = newPoint;
				vectorValue = getVector(variable, newPoint, dComponent1, dComponent2, dComponent3, interpolator);
				//newPointData.addVariableData(variable, vectorValue.magnitude());
				if (this->isValidPoint(newPoint, min, max))
				{
					newPoint.setCoordinates(Point3f::CARTESIAN);
					fieldline.insertPointData(newPoint, vectorValue.magnitude());
				}
			}

		}

		return fieldline;
	}

	/**
	 * TODO: finish documentation
	 * @param dComponent1
	 * @param dComponent2
	 * @param dComponent3
	 * @param dn
	 */
	float Tracer::calculateDT(float& dComponent1, float& dComponent2, float& dComponent3, float& dn)
	{
		float dt = dComponent1;
		if (dComponent2 < dt)
			dt = dComponent2;
		if (dComponent3 < dt)
			dt = dComponent3;
		dt = dt * dn;
		return dt;
	}


	/**
	 * TODO: finish documentation
	 * @param variable
	 * @param startComponent1
	 * @param startComponent2
	 * @param startComponent3
	 * @param interpolator
	 * @param dir
	 */
	Fieldline Tracer::sphericalTrace(const std::string& variable, const float& startComponent1,
			const float& startComponent2, const float& startComponent3, Interpolator * interpolator,
			const Direction& dir)
	{
		Fieldline f;
		std::string model_name = kameleon->getModelName();
		kameleon->loadVariable("r");
		kameleon->loadVariable("theta");
		kameleon->loadVariable("phi");
		const std::vector<float> * r_data = kameleon->getVariableFromMap("r");
		const std::vector<float> * lat_data = kameleon->getVariableFromMap("theta");
		const std::vector<float> * lon_data = kameleon->getVariableFromMap("phi");



		float c0,c1,c2;
		c0 = startComponent1;
		c1 = startComponent2;
		c2 = startComponent3;
		//grab the min and max for each component
		Point3f boxMin, boxMax;
		boxMin.component1 = (kameleon->getVariableAttribute(ccmc::strings::variables::r_, ccmc::strings::attributes::actual_min_)).getAttributeFloat();
		boxMin.component2 = 90. - ccmc::constants::RadiansToDegrees * (kameleon->getVariableAttribute(ccmc::strings::variables::theta_,
				ccmc::strings::attributes::actual_max_)).getAttributeFloat();
		boxMin.component3 = -360.0; /* no crap out at 0 since the boundary is periodic */
		boxMax.component1 = (kameleon->getVariableAttribute(ccmc::strings::variables::r_,
				ccmc::strings::attributes::actual_max_)).getAttributeFloat();
		if (kameleon->getModelName() == "mas")
		{
			boxMax.component1 = boxMax.component1 - ((kameleon->getVariableAttribute(ccmc::strings::variables::r1_,
					ccmc::strings::attributes::actual_max_)).getAttributeFloat() - boxMax.component1);
		}
		boxMax.component2 = 90. - ccmc::constants::RadiansToDegrees * (kameleon->getVariableAttribute(ccmc::strings::variables::theta_,
				ccmc::strings::attributes::actual_min_)).getAttributeFloat();
		boxMax.component3 = 720.; /* no crap out at 360 since the boundary is periodic */

		//since ENLIL stores the radius in meters, we need to convert to AU
		if (model_name == "enlil")
		{
			boxMin.component1 /= ccmc::constants::AU_in_meters;
			boxMax.component1 /= ccmc::constants::AU_in_meters;
		}

		//ensure the start position components are within the proper ranges
		if (c0 > boxMax.component1 || c0 < boxMin.component1)
		{
			return f;
		}
		if (c1 < -90.f)
		{
			c1 = -c1;
			c2 += 180.f;
		}
		if (c1 > 90.f)
		{
			c1 = (*lat_data)[lat_data->size()-1] - c1;
			c2 += 180.f;
		}
		if (c2 < 0.f)
		{
			c2 += 360.f;
		}
		if (c2 > 360.f)
		{
			c2 -= 360.f;
		}


		float adjusted_dn = dn;
		if (dir == REVERSE)
		{
			adjusted_dn = -dn;
		}




		float min_block_size = 1.e-10;
		float min_distance = 1.e-5;
		if (model_name == ccmc::strings::models::enlil_)
			min_block_size = 1.e-4;
		float max_latitude_change = 3.f;
//#define DEBUG_SPHTRACER

		//#define DEBUG_SPHTRACER
		//#define DEBUG_SPHTRACER

//		cerr << "entered spherical trace" << endl;
#ifdef DEBUG_SPHTRACER
		cerr << "trace_in_two_direction: File: " << filename << " Var: " << variable << endl;
		cerr << "dn:" << dn << endl;
		cerr << "I'm inside the sphtrace function. ha HA! " << c0 << " " << c1 << " " << c2 << endl;
#endif

		Point3f oldPoint;
		float eps = 1e-5, DtoR = asin(1.) / 90.f, RADEG = 90. / asin(1.);
		Point3f point(Point3f::SPHERICAL);

		float dComponent1 = -0.1;
		float dComponent2 = -0.1;
		float dComponent3 = -0.1;
		Point3f currentVectorData;
		int return_status;

		vector<Point3f> streamline;
		vector<float> mag1;
		vector<float> magnitudes;


		// ENLIL: stay in region of initial polarity (b_r > 0 or b_r <= 0)
		bool usePolarity = false;
		int polarity = 0;


		//float *latitudes, *lat_ptr; // latitude grid
		//std::vector<float>* latitudes = NULL;
		std::vector<float> latitudes;
		const std::vector<float>* lat_ptr = NULL;
		// b_r at nearest grid latitudes and estimated latitude of current sheet
		float br_up, br_down, lat_csh;

		if (model_name == "enlil")
		{

#ifdef DEBUG_SPHTRACER
			cerr << "SPHTRACE: using conversion factor for " << variable << ": " << endl;
#endif
			// set up constrained iteration in like polarity
			// get latitude grid
#ifdef DEBUG_SPHTRACER
			cerr << "calling var_get for phi (lat)" << endl;
#endif
//			std::cerr << "calling getVariable" << std::endl;
			lat_ptr = kameleon->getVariableFromMap(ccmc::strings::variables::theta_); // need to use alias "lat" later


			if (lat_ptr->size() > 0)
			{
				usePolarity = true;
				//latitudes = (float*) malloc(sizeof(float) * NLAT);
				//latitudes = new std::vector<float>(NLAT);
				for (int i = 0; i < lat_ptr->size(); i++)
				{
					latitudes.push_back(90.f - RADEG * (*lat_ptr)[i]);
				}
				//delete lat_ptr;
			} else
			{
				usePolarity = false; // we failed to get required information
			}

		}
//#define DEBUG_SPHTRACER
#ifdef DEBUG_SPHTRACER
		std::cerr << "usePolarity: " << usePolarity << std::endl;
#endif
//#undef DEBUG_SPHTRACER
		/* these are in (r,lat,lon) space */
//		std::cerr << "grabbing box dimensions" << std::endl;

//		std::cerr << "after getting box dimensions" << std::endl;
#ifdef DEBUG_SPHTRACER
		cerr << "After setting box dimensions" << endl;
		cerr << " Min: " << boxMin << " Max: " <<boxMax << endl;
#endif

		vector<Point3f> fieldline1;
		//vector<Point3f> fieldline2;

		// get first half of streamline
		Point3f startPoint(c0, c1, c2, Point3f::SPHERICAL);

#ifdef DEBUG_SPHTRACER
		cerr << "before getVector, variable: " << variable << endl;
		cerr << "Start Point : " << startPoint << endl;
#endif

		Point3f vectorValue = getVector(variable, startPoint, dComponent1, dComponent2, dComponent3, interpolator);
		if (vectorValue.component1 == missing || vectorValue.component2 == missing || vectorValue.component3 == missing)
		{
			return f;
		}

#ifdef DEBUG_SPHTRACER
		cerr << "After getVector " << variable << endl;
		cerr << "vector: " << vectorValue << endl;
		cerr << "Grid Deltas: " << dComponent1 << " " << dComponent2 << " " << dComponent3 << endl;
#endif

		if (vectorValue.component1 > 0)
			polarity = 1;
		else
			polarity = 0;

		mag1.push_back(vectorValue.magnitude());
		//	vectorValue.normalize();
		fieldline1.push_back(startPoint);
		//cout << "startPoint: " << startPoint << endl;
		Point3f previous = startPoint;
		bool finished = false;
		int iterations = 0;
		oldPoint.component1 = 1e20;
		oldPoint.component2 = 1e20;
		oldPoint.component3 = 1e20;
		//std::cerr << "Before while loop" << std::endl;
		while (!finished && isValidPoint(previous, boxMin, boxMax))
		{
			//std::cerr << "Inside while loop" << std::endl;
			Point3f addition(Point3f::SPHERICAL);
			//float bp = ((Interpolator *) (interpolator))->interpolate(ccmc::strings::variables::bp_, previous.component1, previous.component2, previous.component3);
			float magValue, dt, rsinth, rlocal;
			usePolarity = false;
			if (usePolarity && ((vectorValue.component1 > 0) != polarity))
			{
				int iz;
				for (iz = lat_ptr->size()-1; latitudes[iz] <= previous.component2; iz--);
				std::cerr << "iz: " << iz << " previous.component2: " << previous.component2 << std::endl;
				br_up = interpolator -> interpolate(ccmc::strings::variables::br_, previous.component1,
						latitudes[iz], previous.component3);
				br_down = interpolator -> interpolate(ccmc::strings::variables::br_, previous.component1,
						latitudes[iz+1], previous.component3);
#ifdef DEBUG_SPHTRACER

				if (br_up == missing || br_down == missing)
				{
					std::cerr << "previous.component2: " << previous.component2 << std::endl;
					for (int nl = 0; nl < latitudes.size(); nl++)
					{
						std::cerr << "latitudes[" << nl << "]: " << latitudes[nl] << std::endl;
					}
					std::cerr << "br_up: " << br_up << " br_down: " << br_down << " latitudes[" << iz+1 << "]: " << latitudes[iz+1];
					std::cerr << " latitudes[" << iz << "]: " << latitudes[iz] << std::endl;
					exit(1);
				}
#endif
				float updown = br_up - br_down;
				if (updown == 0.0)
					updown = 1e-10;
				lat_csh = (latitudes[iz] * br_up - latitudes[iz + 1] * br_down) / (updown);
//#ifdef DEBUG_SPHTRACER
				cerr << "Lat_CSH: " << lat_csh << endl;
				cerr << "Lat: " << previous.component2 << endl;
				cerr << "New Lat: " << 2*lat_csh-previous.component2 << endl;
//#endif

#ifdef DEBUG_SPHTRACER
				cerr << "lat_csh: " << lat_csh << " Previous.component2: " << previous.component2 << std::endl;
#endif
				previous.component2 = 2 * lat_csh - previous.component2;
				//previous.component2 = lat_csh;
#ifdef DEBUG_SPHTRACER
				cerr << " Prevoius.component2: " << previous.component2 << endl;
#endif
				vectorValue = getVector(variable, previous, dComponent1, dComponent2, dComponent3, interpolator);

				if (isnan(vectorValue.component1))
				{
					vectorValue.component1 = 1e-20;
					vectorValue.component2 = 1e-20;
					vectorValue.component3 = 1e-20;
				}
				if (isnan(vectorValue.component2))
				{
					vectorValue.component1 = 1e-20;
					vectorValue.component2 = 1e-20;
					vectorValue.component3 = 1e-20;
				}
				if (isnan(vectorValue.component3))
				{
					vectorValue.component1 = 1e-20;
					vectorValue.component2 = 1e-20;
					vectorValue.component3 = 1e-20;
				}
			}

#ifdef DEBUG_SPHTRACER
			cerr << "inside while loop" << endl;
			cerr << "deltas: " << dComponent1 << " " << dComponent2 << " " << dComponent3 << endl;
#endif
			magValue = vectorValue.magnitude();
			if (magValue < eps)
				magValue = eps;
			//vectorValue.component1 /= magValue;
			//vectorValue.component2 /= magValue;
			//vectorValue.component3 /= magValue;

			rlocal = previous.component1;
			rsinth = std::abs(rlocal * cos(DtoR * previous.component2));
			if (rsinth > 0)
			{
				float temp = rsinth*DtoR*previous.component3;
				if (temp < dt)
				{
					dt = temp;
				}
			}

			// dComponent1 *= 1./(fabs(vectorValue.component1)+eps*(fabs(vectorValue.component1) < eps));
			// dComponent2 *= DtoR*rsinth/(fabs(vectorValue.component2)+eps*(fabs(vectorValue.component2) < eps));
			// dComponent3 *= DtoR*rlocal/(fabs(vectorValue.component3)+eps*(fabs(vectorValue.component3) < eps));
			//dComponent1 /= 1.48598e11;
			dComponent1 *= 1.0; //dComponent1 should always be factored correctly
			//dComponent2 *= DtoR * rsinth;
			//dComponent3 *= DtoR * rlocal;
			dComponent3 *= rsinth;
			dComponent2 *= rlocal;

			if (isnan(dComponent1) || dComponent1 < min_block_size)
			{
				dComponent1 = min_block_size;
				dComponent2 = min_block_size;
				dComponent3 = min_block_size;
			}
			if (isnan(dComponent2) || dComponent2 < min_block_size)
			{
				dComponent1 = min_block_size;
				dComponent2 = min_block_size;
				dComponent3 = min_block_size;
			}
			if (isnan(dComponent3) || dComponent3 < min_block_size)
			{
				dComponent1 = min_block_size;
				dComponent2 = min_block_size;
				dComponent3 = min_block_size;
			}
			dt = dComponent1;
			if (dComponent3 < dt)
				dt = dComponent3;
			if (dComponent2 < dt)
				dt = dComponent2;
			dt = dt * adjusted_dn;
			addition.component1 = dt * vectorValue.component1 / magValue;
			addition.component2 = dt * vectorValue.component2 / (magValue * DtoR * rlocal);
			addition.component3 = dt * vectorValue.component3 / (magValue * DtoR * rsinth);

			if (isnan(addition.component1) || abs(addition.component1 - 0.0) < 1e-20)
			{
				addition.component1 = 1e-20;
				//addition.component2 = 1e-20;
				//addition.component3 = 1e-20;
			}
			if (isnan(addition.component2) || abs(addition.component2 - 0.0) < 1e-20)
			{
				//addition.component1 = 1e-20;
				addition.component2 = 1e-20;
				//addition.component3 = 1e-20;
			}
			if (isnan(addition.component3) || abs(addition.component3 - 0.0) < 1e-20)
			{
				//addition.component1 = 1e-20;
				//addition.component2 = 1e-20;
				addition.component3 = 1e-20;
			}
			Point3f newPoint = Point3f(previous + addition);
			newPoint.setCoordinates(Point3f::SPHERICAL);
			//float dist = previous.distance(newPoint);
			Point3f cPrevious = previous.getCartesian();
			Point3f cNewPoint = newPoint.getCartesian();
			float dist = cPrevious.distance(cNewPoint);
			//std::string temp = "blah1asdfasdfasdfasdfasdfasdfasdasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss";
			//std::string temp2 = "blah2asdfasdfasdfasdf";
			//temp = temp2;
			//boost::lexical_cast<std::string>(1.f);
			//newPoint.toString();
//#ifdef DEBUG_SPHTRACER
			//std::cerr << newPoint << std::endl;
//			std::cerr << "*****dist: " << dist << " previous: " << previous << " newPoint: " << newPoint << std::endl;
			//std::cerr << newPoint << " addition: " << addition << " dt: " << dt << " dComponent1: " << dComponent1 << " dComponent2: " << dComponent2 << " dComponent3: " << dComponent3 << " rsinth: " << rsinth << endl;
			//cerr << " DtoR: " << DtoR << " rlocal: " << rlocal << " cos(DtoR*previous.component3): " << cos(DtoR*previous.component3) << endl;
			//std::cerr << "dt / adjusted_dn: " << dt/adjusted_dn << std::endl;
//#endif
			//		float dist = previous.distance(newPoint);
			/*float dist = sqrt( (double)addition.component1 *(double)addition.component1
			 +(double)addition.component2 *(double)addition.component2*(double)rsinth
			 *(double)rsinth +(double)addition.component3*(double)addition.component3
			 *(double)rlocal*(double)rlocal );*/

#ifdef DEBUG_SPHTRACER
			cerr << "******after sqrt" << endl;
#endif
			//		float dist = previous.distance(newPoint);

			/** check if tracing went anywhere, else stop **/
			float latitude_change = newPoint.component2 - previous.component2;
			if (dist < min_distance || newPoint.distance(oldPoint) < min_distance || dist > dt / adjusted_dn || newPoint.distance(fieldline1[fieldline1.size()-1]) > 1.f)
			{
				finished = true;
				return_status = 3;

			} else if (isnan(newPoint.component1) || (isnan(newPoint.component2)) || (isnan(newPoint.component3)))
			{
				finished = true;
				return_status = 3;

			} else if (std::abs(latitude_change) > 3.f)
			{
				std::cerr << "newPoint latitude: " << newPoint.component2 << " previous latitude: " << previous.component2 << " latitude change: " << std::abs(latitude_change) << std::endl;
				finished = true;
				return_status = 3;
			} else if (std::abs(newPoint.component2) > 90.f)
			{
				std::cerr << "newPoint latitude: " << newPoint.component2 << " previous latitude: " << previous.component2 << std::endl;
				finished = true;
				return_status = 3;
			}
			else
			{


				if (newPoint.component2 > 90.)
				{
					std::cerr << "latitude is greater than 90!" << std::endl;
					newPoint.component3 = newPoint.component3 + 180;
					newPoint.component2 = 90 - newPoint.component2;
				}
				if (newPoint.component2 < -90)
				{
					std::cerr << "latitude is less than 90!" << std::endl;
					newPoint.component3 = newPoint.component3 + 180;
					newPoint.component2 = newPoint.component2 + 90;
				}
				// azimuth from 0 - 2*PI
				if (newPoint.component3 > 360)
				{
					newPoint.component3 = newPoint.component3 - 360;
				}
				if (newPoint.component3 < 0)
				{
					newPoint.component3 = newPoint.component3 + 360.;
				}

				//check if it exceeds the limits of the latitude range
				if (newPoint.component2 > boxMax.component2 || newPoint.component2  < boxMin.component2)
				{
					finished = true;
					return_status = 3;
//					std::cerr << "outside range!!" << std::endl;
//					cerr << "*****dist: " << dist << " previous: " << previous << " newPoint: ";
//					cerr << newPoint << " addition: " << addition << " dt: " << dt << " dComponent1: " << dComponent1 << " dComponent2: " << dComponent2 << " dComponent3: " << dComponent3 << " rsinth: " << rsinth << endl;

				} else
				{
					oldPoint = previous;
					fieldline1.push_back(previous);
					iterations++;
					if (iterations > step_max)
						finished = true;
					//cout << result.component1 << " ";
//					cerr << "Assigning newPoint to previous: " << newPoint << std::endl;
					previous = newPoint;
					vectorValue = getVector(variable, previous, dComponent1, dComponent2, dComponent3, interpolator);
					mag1.push_back(vectorValue.magnitude());
				}
				//vectorValue.normalize();
#ifdef DEBUG_SPHTRACER
				cerr << "ITER: After ccmc_tracer_getVector" << endl;
				cerr << "ITER: position: " << previous << endl;
				cerr << "ITER: vector: " << vectorValue << endl;
				cerr << "ITER: dist: " << dist << endl;
				cerr << "ITER: addition: " << addition << endl;
#endif
			}

		}

		for (int i = 0; i < fieldline1.size(); i++)
		{

			f.insertPointData(fieldline1[i], mag1[i]);
		}


		return f;

	}

	/**
	 * TODO: finish documentation
	 * @param p
	 * @param min
	 * @param max
	 * @return
	 */
	bool Tracer::isValidPoint(const Point3f& p, const Point3f& min, const Point3f& max)
	{
//		std::cerr << "inside isValidPoint" << std::endl;
		std::string model_name = kameleon->getModelName();
		//break this down to the individual models


		bool insideInnerBoundary = true;
		if (model_name == ccmc::strings::models::batsrus_ ||
				model_name == ccmc::strings::models::ucla_ggcm_ ||
				model_name == ccmc::strings::models::open_ggcm_ ||
				model_name == ccmc::strings::models::adapt3d_ ||
				model_name == ccmc::strings::models::lfm_)
		{
			if (sqrt(p.component1 * p.component1 + p.component2 * p.component2 + p.component3 * p.component3) >= r_end)
			{
				insideInnerBoundary = false;
			}
		} else
		{
			if (p.component1 >= r_end)
			{
				insideInnerBoundary = false;
			}
		}

		//check if the point is within the bounds of the simulation region
		bool validRegion = false;

		//TODO: fix this logic for ENLIL and MAS
		if (model_name == ccmc::strings::models::adapt3d_)
		{
			if (sqrt(p.component1 * p.component1 + p.component2 * p.component2 + p.component3 * p.component3) <= 30.f)
				validRegion = true;
		} else if (model_name == ccmc::strings::models::mas_ || model_name == ccmc::strings::models::enlil_)
		{
			if (p.component1 >= r_end && p.component1 <= max.component1 && p.component2 >= min.component2 && p.component2 <= max.component2)
				validRegion = true;
		}else if (model_name == ccmc::strings::models::lfm_)
		{
			if (!(kameleon->getMissingValue() == interpolator->interpolate("bx", p.component1, p.component2, p.component3))){
				validRegion = true;
			}


		}else if (p.component1 >= min.component1 && p.component2 >= min.component2 && p.component3 >= min.component3

				&& p.component1 <= max.component1 && p.component2 <= max.component2 && p.component3 <= max.component3)
			validRegion = true;







//		std::cerr << "r_end: " << r_end << " validRegion: " << validRegion << " insideInnerBoundary: " << insideInnerBoundary << std::endl;
//		std::cerr << "p: " << p << " min: " << min << " max: " << max << std::endl;
		if (validRegion == true && insideInnerBoundary == false)
		{
			return true;
		} else
		{
			return false;
		}
	}

	/**
	 * TODO: finish documentation
	 * @param iterations
	 * @param max
	 * @return
	 */
	bool Tracer::isValidIteration(int iterations, int max)
	{
		if (iterations >= max)
			return true;
		else
			return false;
	}

	/**
	 * TODO: finish documentation
	 * @param fieldline
	 * @return
	 */
	bool Tracer::isClosed(Fieldline& fieldline)
	{
		cout << "fieldline.size(): " << fieldline.size() << endl;
		Point3f a = fieldline.getPosition(0);
		Point3f b = fieldline.getPosition(fieldline.size() - 1);
		if (a.magnitude() < 3.0 && b.magnitude() < 3.0)
			return true;
		else
			return false;
	}

	/**
	 * TODO: finish documentation
	 * @param position
	 * @param stepMax
	 * @return
	 */
	Fieldline Tracer::findLastClosed(Point3f position, int stepMax)
	{

		Fieldline currentFieldline;
		Fieldline previousFieldline;
		//Point currentPoint = new Point();
		//Point previousPoint = new Point();

		// set both current and previous to start point
		Point3f currentPoint = position;
		Point3f previousPoint = position;
		Point3f previousClosedPoint;
		//float expansionAmount = .1f;
		Point3f direction = position;
		direction.normalize();

		//System.out.println("direction: " + start + " and after normalize: " + direction);

		//counter for number of times trace has been called

		//cout << "direction: " << Point3f(*x,*y,*z) << " and after normalize: " << direction << endl;
		int count = 0;

		currentFieldline = bidirectionalTrace(b_, currentPoint.component1, currentPoint.component2,
				currentPoint.component3);
		//cout << "step_max after btrace: " << *step_max << endl;
		count++;
		previousFieldline = currentFieldline;
		if (isClosed(currentFieldline))
		{
			previousClosedPoint = currentPoint;
		} else
		{
			cout << "uh oh... " << endl;
		}

		while (isClosed(currentFieldline))
		{
			previousPoint = currentPoint;
			previousFieldline = currentFieldline;

			currentPoint = currentPoint + direction * 3.;
			currentFieldline = bidirectionalTrace(b_, currentPoint.component1, currentPoint.component2,
					currentPoint.component3);

			count++;
		}

		//need to perform binary search now
		Point3f middlePoint;
		float dist = currentPoint.distance(previousPoint);
		while (dist > .001)
		{
			middlePoint = previousPoint + direction * dist * .5f;
			currentFieldline = bidirectionalTrace(b_, middlePoint.component1, middlePoint.component2,
					middlePoint.component3);
			count++;
			if (isClosed(currentFieldline))
			{
				previousFieldline = currentFieldline;
				previousPoint = middlePoint;

			} else
			{
				currentPoint = middlePoint;

			}

			dist = currentPoint.distance(previousPoint);

		}

		return previousFieldline;

	}

	/**
	 * TODO: finish documentation
	 * @param numberOfFieldlines
	 * @param stepMax
	 * @param numberOfPointsPerReducedFieldline
	 * @return
	 */
	vector<Fieldline> Tracer::getLastClosedFieldlines(int numberOfFieldlines, int stepMax,
			int numberOfPointsPerReducedFieldline)
	{
		vector<Fieldline> fieldlines;

		float delta = 360.0 / (float) numberOfFieldlines * 3.14159 / 180.0;
		for (int i = 0; i < numberOfFieldlines; i++)
		{

			Point3f start(3.0 * cos(delta * (float) i), 3.0 * sin(delta * (float) i), 0.0);
			Fieldline currentFieldline = findLastClosed(start, stepMax);
			cout << "currentFieldline.size(): " << currentFieldline.size() << endl;
			float totaldistance = 0.0;
			vector<Point3f> positions = currentFieldline.getPositions();
			vector<float> data = currentFieldline.getData();
			for (int j = 0; j < currentFieldline.size() - 1; j++)
			{
				float a = (positions[j + 1].component1 - positions[j].component1);
				float b = (positions[j + 1].component2 - positions[j].component2);
				float c = (positions[j + 1].component3 - positions[j].component3);
				totaldistance += sqrt(a * a + b * b + c * c);
			}

			float increments = totaldistance / (float) numberOfPointsPerReducedFieldline;
			int index = 0;
			Fieldline interpolatedFieldline;
			/** add the first point all the time **/
			interpolatedFieldline.insertPointData(positions[0], data[0]);
			float previousDistance = 0.0;

			for (int j = 1; j < (numberOfPointsPerReducedFieldline - 1); j++)
			{
				float currentTargetDistance = (float) j * increments;
				float currentDistance = previousDistance;

				while (currentDistance < currentTargetDistance)
				{
					previousDistance = currentDistance;
					float a = (positions[index + 1].component1 - positions[index].component1);
					float b = (positions[index + 1].component2 - positions[index].component2);
					float c = (positions[index + 1].component3 - positions[index].component3);
					currentDistance += sqrt(a * a + b * b + c * c);
					++index;

				}

				Point3f direction(positions[index].component1 - positions[index - 1].component1,
						positions[index].component2 - positions[index - 1].component2, positions[index].component3
								- positions[index - 1].component3);
				float length = direction.magnitude();
				direction.normalize();
				float scale = currentDistance - currentTargetDistance;
				--index;
				Point3f startPoint(positions[index].component1, positions[index].component2,
						positions[index].component3);

				Point3f targetPoint = startPoint + direction * scale;
				Point3f targetDirection(targetPoint.component1 - startPoint.component1, targetPoint.component2
						- startPoint.component2, targetPoint.component3 - startPoint.component3);
				float targetLength = targetDirection.magnitude();
				float dataScale = targetLength / length;

				float diff = data[index] - data[index - 1];
				float targetData = data[index - 1] + diff * dataScale;
				interpolatedFieldline.insertPointData(targetPoint, targetData);

			}
			/** always add the last point too, since i was reduced by one*/
			interpolatedFieldline.insertPointData(positions[positions.size() - 1], data[data.size() - 1]);
			interpolatedFieldline.setStartPoint(currentFieldline.getStartPoint());
			interpolatedFieldline.setStartIndex(currentFieldline.getStartIndex());
			fieldlines.push_back(interpolatedFieldline);
		}
		return fieldlines;
	}
	/*
	 float ccmc_tracer_parseFloat(string num)
	 {
	 istringstream iss(num);
	 float value;
	 iss >> value;
	 return value;
	 }

	 int ccmc_tracer_isClosed(vector<Point3f> streamline)
	 {
	 int value = 0;
	 if (streamline[0].magnitude() < 3.0 && streamline[streamline.size() - 1].magnitude() < 3.0)
	 value = 1;
	 else
	 value = 0;
	 return value;
	 }*/

	/** check to see if iterations are greater than or equal to max
	 I should have changed the name but it would have been too long **/
	/*bool ccmc_tracer_areIterationsGreaterThanOrEqualTo(int iterations, int max)
	 {
	 if (iterations >= max)
	 {
	 return true;
	 } else
	 return false;
	 }*/

	/**
	 * TODO: finish documentation
	 * TODO: return missing value if variable not found
	 * TODO: use a map instead of an if/else statement to check for the variable
	 * @param variable
	 * @param position
	 * @param dComponent1
	 * @param dComponent2
	 * @param dComponent3
	 * @param interpolator
	 * @return
	 *
	 */
	Point3f Tracer::getVector(const std::string& variable, const Point3f& position, float& dComponent1,
			float& dComponent2, float& dComponent3, const Interpolator * interpolator)
	{

		//std::string * vectorComponentName1;
		//std::string * vectorComponentName2;
		//std::string * vectorComponentName3;
		//Interpolator * interpolator = kameleon->createNewInterpolator();

		boost::unordered_map<std::string, std::vector<std::string> >::iterator iter = componentNamesMap.find(variable);
		if (iter == componentNamesMap.end())
			return Point3f(kameleon->getMissingValue(), kameleon->getMissingValue(), kameleon->getMissingValue());
		//std::cout << "variable: " << variable << " c0: " << ((*iter).second)[0] << std::endl;

		Point3f vectorValue;
		std::string model_name = kameleon->getModelName();
		//std::cout << "variable: " << variable << " c0: " << ((*iter).second)[0] << " position: " << position << std::endl;

		//if (mas_ == (*model_name))
		{
			/*
			 if (b_ == variable || br_ == variable || bphi_ == variable || btheta_ == variable)
			 {
			 vectorComponentName1 = &br_;
			 vectorComponentName2 = &bphi_;
			 vectorComponentName3 = &btheta_;
			 } else if (j_ == variable || jr_ == variable || jphi_ == variable || jtheta_
			 == variable)
			 {
			 vectorComponentName1 = &jr_;
			 vectorComponentName2 = &jphi_;
			 vectorComponentName3 = &jtheta_;
			 } else if ("u" == variable || "ur" == variable || "uphi" == variable || "utheta"
			 == variable)
			 {
			 vectorComponentName1 = "ur";
			 vectorComponentName2 = "uphi";
			 vectorComponentName3 = "utheta";

			 } else if ("j" == variable || "jr" == variable || "jphi" == variable || "jtheta"
			 == variable)
			 {
			 vectorComponentName1 = "jr";
			 vectorComponentName2 = "jphi";
			 vectorComponentName3 = "jtheta";
			 } else if (variable == "jpar" || variable == "jparr" || variable == "jparphi" || variable
			 == "jpartheta")
			 {
			 vectorComponentName1 = "jparr";
			 vectorComponentName2 = "jparphi";
			 vectorComponentName3 = "jpartheta";
			 } else if (variable == "jxb" || variable == "jxbr" || variable == "jxbphi" || variable
			 == "jxbtheta")
			 {
			 vectorComponentName1 = "jxbr";
			 vectorComponentName2 = "jxbphi";
			 vectorComponentName3 = "jxbtheta";
			 } else if (variable == "exb" || variable == "exbr" || variable == "exbphi" || variable
			 == "exbtheta")
			 {
			 vectorComponentName1 = "exbr";
			 vectorComponentName2 = "exbphi";
			 vectorComponentName3 = "exbtheta";
			 } else if (variable == "nv" || variable == "nvr" || variable == "nvphi" || variable
			 == "nvtheta")
			 {
			 vectorComponentName1 = "ur";
			 vectorComponentName2 = "uphi";
			 vectorComponentName3 = "utheta";
			 } else if (variable == "e" || variable == "er" || variable == "ephi" || variable
			 == "etheta")
			 {
			 vectorComponentName1 = "er";
			 vectorComponentName2 = "ephi";
			 vectorComponentName3 = "etheta";
			 }*/
		} //else if (enlil_ == (*model_name))
		{
			/*if (variable == "b" || variable == "bx" || variable == "by" || variable == "bz")
			 {
			 vectorComponentName1 = "b1x";
			 vectorComponentName3 = "b1y";
			 vectorComponentName2 = "b1z";
			 } else if (variable == "b1" || variable == "b1x" || variable == "b1y" || variable == "b1z")
			 {
			 vectorComponentName1 = "b1x";
			 vectorComponentName3 = "b1y";
			 vectorComponentName2 = "b1z";
			 } else if (variable == "j" || variable == "jx" || variable == "jy" || variable == "jz")
			 {
			 vectorComponentName1 = "jx";
			 vectorComponentName3 = "jy";
			 vectorComponentName2 = "jz";
			 } else if (variable == "v" || variable == "u" || variable == "ux" || variable == "uy"
			 || variable == "uz")
			 {
			 vectorComponentName1 = "ux";
			 vectorComponentName3 = "uy";
			 vectorComponentName2 = "uz";

			 } else if (variable == "e" || variable == "ex" || variable == "ey" || variable == "ez")
			 {
			 vectorComponentName1 = "ex";
			 vectorComponentName3 = "ey";
			 vectorComponentName2 = "ez";

			 } else if (variable == "j_par" || variable == "jpar" || variable == "jparx" || variable
			 == "jpary" || variable == "jparz")
			 {
			 vectorComponentName1 = "jparx";
			 vectorComponentName3 = "jpary";
			 vectorComponentName2 = "jparz";
			 } else if (variable == "jxb" || variable == "jxbx" || variable == "jxby" || variable
			 == "jxbz")
			 {
			 vectorComponentName1 = "jxbx";
			 vectorComponentName3 = "jxby";
			 vectorComponentName2 = "jxbz";
			 } else if (variable == exb_ || variable == exbx_ || variable == exby_ || variable
			 == exbz_)
			 {
			 vectorComponentName1 = &exbx_;
			 vectorComponentName3 = &exby_;
			 vectorComponentName2 = &exbz_;
			 } else if (variable == "nv" || variable == "nvx" || variable == "nvy" || variable == "nvz")
			 {
			 vectorComponentName1 = "ux";
			 vectorComponentName3 = "uy";
			 vectorComponentName2 = "uz";
			 }*/
		} //else
		{
			/*if (variable == b_ || variable == bx_ || variable == by_ || variable == bz_)
			 {
			 vectorComponentName1 = &bx_;
			 vectorComponentName2 = &by_;
			 vectorComponentName3 = &bz_;
			 } else if (variable == b1_ || variable == b1x_ || variable == b1y_ || variable == b1z_)
			 {
			 vectorComponentName1 = &b1x_;
			 vectorComponentName2 = &b1y_;
			 vectorComponentName3 = &b1z_;
			 } else if (variable == j_ || variable == jx_ || variable == jy_ || variable == jz_)
			 {
			 vectorComponentName1 = &jx_;
			 vectorComponentName2 = &jy_;
			 vectorComponentName3 = &jz_;
			 } /*else if (variable == "v" || variable == "u" || variable == "ux" || variable == "uy"
			 || variable == "uz")
			 {
			 vectorComponentName1 = "ux";
			 vectorComponentName2 = "uy";
			 vectorComponentName3 = "uz";

			 } else if (variable == "e" || variable == "ex" || variable == "ey" || variable == "ez")
			 {
			 vectorComponentName1 = "ex";
			 vectorComponentName2 = "ey";
			 vectorComponentName3 = "ez";

			 } else if (variable == "j_par" || variable == "jpar" || variable == "jparx" || variable
			 == "jpary" || variable == "jparz")
			 {
			 vectorComponentName1 = "jparx";
			 vectorComponentName2 = "jpary";
			 vectorComponentName3 = "jparz";
			 } else if (variable == "jxb" || variable == "jxbx" || variable == "jxby" || variable
			 == "jxbz")
			 {
			 vectorComponentName1 = "jxbx";
			 vectorComponentName2 = "jxby";
			 vectorComponentName3 = "jxbz";
			 } else if (variable == exb_ || variable == exbx_ || variable == exby_ || variable
			 == exbz_)
			 {
			 vectorComponentName1 = "exbx";
			 vectorComponentName2 = "exby";
			 vectorComponentName3 = "exbz";
			 } else if (variable == "nv" || variable == "nvx" || variable == "nvy" || variable == "nvz")
			 {
			 vectorComponentName1 = &ux_;
			 vectorComponentName2 = &uy_;
			 vectorComponentName3 = &uz_;
			 }*/
		}

		if (variable == "b1" || variable == "b1r" || variable == "b1theta" || variable == "b1phi")
		{
			vectorValue.component1 = ((Interpolator *) (interpolator))->interpolate("b1r",
											position.component1, position.component2, position.component3, dComponent1, dComponent2,
											dComponent3);
			vectorValue.component2 = ((Interpolator *) (interpolator))->interpolate("b1theta",
								position.component1, position.component2, position.component3, dComponent1, dComponent2,
								dComponent3);
			float bp = ((Interpolator *) (interpolator))->interpolate("bp",
								position.component1, position.component2, position.component3, dComponent1, dComponent2,
								dComponent3);
			vectorValue.component3 = ((Interpolator *) (interpolator))->interpolate("b1phi",
								position.component1, position.component2, position.component3, dComponent1, dComponent2,
								dComponent3) * bp;

		} else
		{
			vectorValue.component1 = ((Interpolator*) (interpolator))->interpolate(((*iter).second)[0],
					position.component1, position.component2, position.component3, dComponent1, dComponent2, dComponent3);


			if (vectorValue.component1 == missing)
			{
				vectorValue.component2 = missing;
				vectorValue.component3 = missing;
			} else
			{

				vectorValue.component2 = ((Interpolator *) (interpolator))->interpolate(((*iter).second)[1],
						position.component1, position.component2, position.component3, dComponent1, dComponent2,
						dComponent3);

				vectorValue.component3 = ((Interpolator *) (interpolator))->interpolate(((*iter).second)[2],
						position.component1, position.component2, position.component3, dComponent1, dComponent2,
						dComponent3);
			}
		}
#ifdef DEBUG_TRACER
		cerr <<"VectorValue: " << vectorValue << "positions: " << position << endl;
		cerr << "dComponent1: " << dComponent1 << " dComponent2: " << dComponent2 << " dComponent3: " << dComponent3 << endl;
#endif

		return vectorValue;

	}

	/**
	 * TODO: finish documentation
	 * @param stepMax
	 */
	void Tracer::setMaxIterations(int stepMax)
	{
		this->step_max = stepMax;
	}

	/**
	 * TODO: finish documentation
	 * @param r_end
	 */
	void Tracer::setInnerBoundary(float r_end)
	{
		this->r_end = r_end;
	}

	/**
	 * TODO: finish documentation
	 * @param dn
	 */
	void Tracer::setDn(float dn)
	{
		this->dn = dn;
	}

	/**
	 * @param missing
	 */
	void Tracer::setMissing(float missing)
	{
		this->missing = missing;
	}

	/**
	 * @param tilt
	 */
	void Tracer::setTilt(float tilt)
	{
		this->tilt = tilt;
	}



	/**
	 * @param maxArcLength
	 */
	void Tracer::setMaxArcLength(float maxArcLength)
	{
		this->maxArcLength = maxArcLength;
	}

	/**
	 * @param useMaxArcLength
	 */
	void Tracer::setUseMaxArcLength(bool useMaxArcLength)
	{
		this->useMaxArcLength = useMaxArcLength;
	}

	/**
	 * @param min
	 * @param max
	 */
	void Tracer::setRegionOfInterest(Point3f min, Point3f max)
	{
		this->minROI = min;
		this->maxROI = max;
	}

	/**
	 * @param useROI
	 */
	void Tracer::setUseRegionOfInterest(bool useROI)
	{
		this->useROI = useROI;
	}
}
