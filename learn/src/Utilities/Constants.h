#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <stdlib.h>
#include "RGBColor.h"

const float 	PI 			= 3.1415926535897932384f;
const float 	TWO_PI 		= 6.2831853071795864769f;
const float 	PI_ON_180 	= 0.0174532925199432957f;
const float 	invPI 		= 0.3183098861837906715f;
const float 	invTWO_PI 	= 0.1591549430918953358f;

const float 	kEpsilon 	= 0.000001f; 
const float	kHugeValue	= 1.0E10f;

const RGBColor	black(0.0);
const RGBColor	white(1.0);
const RGBColor	grey(0.5);
const RGBColor	red(1.0, 0.0, 0.0);

const float 	invRAND_MAX = 1.0f / (float)RAND_MAX;

const int DIRECT_DIFFUSE = 0;
const int PATH_TRACING = 1;

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;
const int START = 0;
const int PLANAR = 1;
const int END = 2;
const int LEFT = 0;
const int RIGHT = 1;
const int MAX_KD_TREE_DEPTH = 16;
const int KD_TREE_MAX_NUM_OBJ_LEAF = 1;
const int RAY_OBJ_EXHAUSTIVE = 0;
const int RAY_OBJ_GRID = 1;
const int RAY_OBJ_KD_TREE = 2;
const int RAY_OBJ_NESTED_GRID = 3;

#endif
