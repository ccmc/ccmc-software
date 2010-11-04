#include "Kameleon.h"
#include "FileReader.h"
#include "OpenGGCM.h"
#include "BATSRUS.h"
#include "ENLIL.h"
#include "MAS.h"
#include "Adapt3D.h"
#include <string>

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

std::cout << "inside Kameleon::open" << std::endl;
		FileReader fileReader;
		long status = fileReader.open(filename);

		if (status == FileReader::OK)
		{
			if (fileReader.doesAttributeExist("model_name"))
			{

				this->modelName = (fileReader.getGlobalAttribute("model_name")).getAttributeString();
				fileReader.close();
				//std::cout << "modelName: '" << modelName << "'" << std::endl;
				if (modelName == "open_ggcm" || modelName == "ucla_ggcm")
				{
		//			std::cout << "created OpenGGCM object" << std::endl;
					model = new OpenGGCM();
				} else if (modelName == "batsrus")
				{
		//			std::cout << "created BATSRUS object" << std::endl;
					model = new BATSRUS();
				} else if (modelName == "enlil")
				{
		//			std::cout << "created ENLIL object" << std::endl;
					model = new ENLIL();
				} else if (modelName == "mas")
				{
		//			std::cout << "created MAS object" << std::endl;
					model = new MAS();
				} else if (modelName == "ADAPT3D")
				{
					std::cout << "created Adapt3D object" << std::endl;
					model = new Adapt3D();
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
				status = FileReader::NOT_A_VALID_KAMELEON_FILE;
			}

			if (model != NULL)
			{

				model->setModelName(modelName);
				status = model->open(filename);

	//std::cout << "initializing extra information" << std::endl;
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
