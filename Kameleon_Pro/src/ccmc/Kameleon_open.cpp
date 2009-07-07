#include "Kameleon.h"
#include "FileReader.h"
#include <cdf.h>
#include "OpenGGCM.h"
#include "BATSRUS.h"
#include <string>

long Kameleon::open(const std::string& filename)
{
	//need to store filename
	//need to intialize relevant information


	openFile(filename);
	this->modelName =
			(getGlobalAttribute("model_name")).getAttributeString();
	if (modelName == "open_ggcm" || modelName == "ucla_ggcm")
	{
		std::cout << "created OpenGGCM object" << std::endl;
		model = new OpenGGCM();
	} else if (modelName == "batsrus")
	{
		std::cout << "created BATSRUS object" << std::endl;
		model = new BATSRUS();
	}

	long status;
	if (model != NULL)
		status = model->open(filename);

	//current_cdf_id = get_current_CDFid();
	initializeExtraInformation();
	return status;

}
