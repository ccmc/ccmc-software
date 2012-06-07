/*
 * Tracer_c.cpp
 *
 *  Created on: Jun 9, 2010
 *      Author: dberrios
 */

#include "Tracer_c.h"
#include <ccmc/Kameleon.h>
#include <ccmc/Kameleon-Tracer.h>
#include "Kameleon_c.h"
#include <ccmc/Point3f.h>

using namespace ccmc;

typedef boost::unordered_map<int, ccmc::Kameleon*> map_i_K;

//for some reason, the code has to exist in the same .cpp file, or the static
//maps can't be accessed consistently
