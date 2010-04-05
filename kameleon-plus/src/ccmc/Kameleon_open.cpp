#include "Kameleon.h"
#include "FileReader.h"
#include "OpenGGCM.h"
#include "BATSRUS.h"
#include "ENLIL.h"
#include "MAS.h"
#include <string>

namespace ccmc
{
	long Kameleon::open(const std::string& filename)
	{
		//need to store filename
		//need to intialize relevant information


		FileReader fileReader;
		long status = fileReader.open(filename);

		if (status > -1)
		{
			if (fileReader.doesAttributeExist("model_name"))
			{

				this->modelName = (fileReader.getGlobalAttribute("model_name")).getAttributeString();
				fileReader.close();
				//std::cerr << "modelName: '" << modelName << "'" << std::endl;
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
				} else //unknown model
				{
					if (model != NULL)
						delete model;
					model = NULL;
					status = -1;

				}
			} else
			{
				if (model != NULL)
					delete model;
				model = NULL;
				status = -1;
			}

			if (model != NULL)
			{

				model->setModelName(modelName);
				status = model->open(filename);

	//std::cout << "initializing extra information" << std::endl;
				//current_cdf_id = get_current_CDFid();
				initializeExtraInformation();

			}
		}
		return status;

	}
}
