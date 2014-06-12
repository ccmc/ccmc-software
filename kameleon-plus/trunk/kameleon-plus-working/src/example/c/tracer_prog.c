//#include <ccmc/Kameleon_compatibility.h>

#include <stdio.h>
#include <stdlib.h>
#include <ccmc/wrappers/c/Kameleon_c.h>
#include <ccmc/wrappers/c/Tracer_c.h>


#define LENGTH 500;


int main (int argc, char * argv[])
{

	float * flx;
	float * fly;
	float * flz;
	float * v_mag;
	float r_end = 3.0f;
	int step_max = 5000;
	float dn = .2f;
	float x = -10.f;
	float y = 0.f;
	float z = 0.f;

	//char * filename = "/Users/dberrios/data/3df_CDF_BETA/Alexa_Halford_062105_2.3df.04932.cdf";
	char * variable = "b";

	if (argc < 2)
	{
		printf("Must provide the filename\n");
		return 1;
	}
	flx = (float *) calloc(5000, sizeof(float));
	fly = (float *) calloc(5000, sizeof(float));
	flz = (float *) calloc(5000, sizeof(float));
	v_mag = (float *) calloc(5000, sizeof(float));

	int kid = Kameleon_create();
	int status = Kameleon_open(kid, argv[1]);
	//load the vector variable (all components)
	status = Kameleon_load_vector_variable(kid, variable);

	//create a tracer object using the kameleon object previously created. store the id of the tracer object
	int tid = Tracer_create(kid);
	int actual_steps = 0;
	Tracer_unidirectionalTrace(tid, variable, &x, &y, &z, &step_max, &dn, &actual_steps, flx, fly, flz);

//	ccmc_tracer_otrace(argv[1], variable, x,y,z, flx, fly, flz, v_mag, r_end, &step_max, dn, 0.0, 0.0, -1234.f);
//	ccmc_derived_close_cdf();
	Tracer_delete(tid);

	Kameleon_close(kid);
	Kameleon_delete(kid);


	free(flx);
	free(fly);
	free(flz);
	free (v_mag);



	return 0;
}
