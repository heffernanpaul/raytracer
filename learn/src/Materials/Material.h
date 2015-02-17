#pragma once

#include "RGBColor.h"
#include "ShadeRec.h"

class Material {


public:
    Material() {

	}
    
    virtual RGBColor
    shade(ShadeRec& sr) const=0;
};
