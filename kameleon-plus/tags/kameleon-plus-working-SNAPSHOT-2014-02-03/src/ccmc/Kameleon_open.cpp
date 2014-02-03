#include "Kameleon.h"
#include "FileReader.h"
#include "OpenGGCM.h"
#include "BATSRUS.h"
#include "ENLIL.h"
#include "MAS.h"
#include "Adapt3D.h"
#include "SWMFIono.h"
#include "LFM.h"
#include "CDFFileReader.h"
#include <string>
#include <boost/algorithm/string.hpp>

namespace ccmc
{
	/**
	 * @param filename
	 * @return
	 */
	long Kameleon::open(const std::string& filename)
	{
		//need to store filename
		//need to intialize relevant information

//std::cout << "Kameleon::open(" << filename << ")" << std::endl;
		GeneralFileReader generalFileReader;
		long status = generalFileReader.open(filename);

		if (status == FileReader::OK)
		{
//			std::cout<<"File reader was succesful. Does model_name attribute exist?"<<std::endl;
			if (generalFileReader.doesAttributeExist("model_name"))
			{
//				std::cout << "model_name attribute does exist... loading model_name"<<std::endl;
				this->modelName = (generalFileReader.getGlobalAttribute("model_name")).getAttributeString();
				std::cout << "Kameleon::open() Model name: '" << modelName << "'" << std::endl;

				generalFileReader.close();
				if (modelName == "open_ggcm" || modelName == "ucla_ggcm")
				{
					//std::cout << "created OpenGGCM object" << std::endl;
					model = new OpenGGCM();
				} else if (modelName == "batsrus")
				{
					//std::cout << "created BATSRUS object" << std::endl;
					model = new BATSRUS();
				} else if (modelName == "enlil")
				{
					//std::cout << "created ENLIL object" << std::endl;
					model = new ENLIL();
				} else if (modelName == "mas")
				{
					//std::cout << "created MAS object" << std::endl;
					model = new MAS();
				} else if (modelName == "ADAPT3D")
				{
					//std::cout << "created Adapt3D object" << std::endl;
					model = new Adapt3D();
				} else if (modelName == "swmf")
				{
					model = new SWMFIono();
				} else if (modelName == "LFM")
				{
					//sdt::cout <<"created LFM object" << std::endl;
					model = new LFM();
				} else //unknown model
				{

					if (model != NULL)
						delete model;
					model = NULL;
					status = FileReader::MODEL_NOT_SUPPORTED;

				}
			} else
			{
				if (model != NULL)
					delete model;
				model = NULL;
				std::cerr << "not a valid kameleon file" << std::endl;
				status = FileReader::NOT_A_VALID_KAMELEON_FILE;
			}

			if (model != NULL)
			{

				model->setModelName(modelName);
				status = model->open(filename);

//				std::cout << "initializing extra information" << std::endl;
				//current_cdf_id = get_current_CDFid();
				if (status == FileReader::OK)
				{
					initializeExtraInformation();
				} else
				{
					std::cerr << "Failed to read model file!" << std::endl;
				}

			}
		}
		return status;

	}
}
