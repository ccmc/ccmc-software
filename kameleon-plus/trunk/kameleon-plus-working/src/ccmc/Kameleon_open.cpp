#include "Kameleon.h"
#include "FileReader.h"
#include "OpenGGCM.h"
#include "BATSRUS.h"
#include "ENLIL.h"
#include "MAS.h"
#include "Adapt3D.h"
#include "SWMFIono.h"
#include "LFM.h"
#include "PythonModel.h"
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
#ifdef DEBUG
		std::cout << "\tKameleon::open(" << filename << ")" << std::endl;
#endif

		GeneralFileReader generalFileReader;
		long status = generalFileReader.open(filename);

		if (status == FileReader::OK)
		{
#ifdef DEBUG
			std::cout<<"\tKameleon::open File reader was succesful. Does model_name attribute exist?"<<std::endl;
#endif
			this->pythonModel = (generalFileReader.getGlobalAttribute("python_model")).getAttributeInt();


			if (generalFileReader.doesAttributeExist("model_name"))
			{
#ifdef DEBUG
				std::cout << "\tKameleon::open model_name attribute exists... loading model_name"<<std::endl;
#endif
				this->modelName = (generalFileReader.getGlobalAttribute("model_name")).getAttributeString();
#ifdef DEBUG
				std::cout << "\tKameleon::open Model name: '" << modelName << "' closing" << std::endl;
#endif
				generalFileReader.close();

				if (pythonModel != 0) { 
				//can override existing models with python readers
#ifdef DEBUG
					std::cout <<"\tKameleon::open creating PythonModel object for " << modelName << std::endl;
#endif
					model = new PythonModel();
					
				} else if (modelName == "open_ggcm" || modelName == "ucla_ggcm")
				{
#ifdef DEBUG
					std::cout << "\tKameleon::open creating OpenGGCM object" << std::endl;
#endif
					model = new OpenGGCM();				
				} else if (modelName == "batsrus")
				{
#ifdef DEBUG
					std::cout << "\tKameleon::open creating BATSRUS object" << std::endl;
#endif
					model = new BATSRUS();				
				} else if (modelName == "enlil")
				{
#ifdef DEBUG
					std::cout << "\tKameleon::open creating ENLIL object" << std::endl;
#endif
					model = new ENLIL();				
				} else if (modelName == "mas")
				{
#ifdef DEBUG
					std::cout << "\tKameleon::open creating MAS object" << std::endl;
#endif
					model = new MAS();				
				} else if (modelName == "ADAPT3D")
				{
#ifdef DEBUG
					std::cout << "\tKameleon::open creating Adapt3D object" << std::endl;
#endif
					model = new Adapt3D();				
				} else if (modelName == "swmf")
				{
#ifdef DEBUG
					std::cout << "\tKameleon::open creating swmf ionosphere object" << std::endl;
#endif
					model = new SWMFIono();				
				} else if (modelName == "LFM")
				{
#ifdef DEBUG
					std::cout <<"\tKameleon::open creating LFM object" << std::endl;
#endif
					model = new LFM();

				} else
				{	//unknown model
				
					if (model != NULL)
						delete model;
					model = NULL;
					std::cerr << "not a MODEL_NOT_SUPPORTED " << std::endl;
					status = FileReader::MODEL_NOT_SUPPORTED;

				}
			} else
			{
				std::cout << "model_name attribute does not exist!" << std::endl;
				if (model != NULL)
					delete model;
				model = NULL;
				std::cout <<  filename << " was not a valid kameleon file" << std::endl;
				status = FileReader::NOT_A_VALID_KAMELEON_FILE;
			}

			if (model != NULL)
			{

				model->setModelName(modelName);
				status = model->open(filename);

				if (status == FileReader::OK)
				{
					initializeExtraInformation();
				} else
				{
					std::cerr << "Failed to read model file!" << std::endl;
				}

			}
		}
		// std::cout<< "kameleon open returning with status:" << status << std::endl;
		return status;

	}
}
