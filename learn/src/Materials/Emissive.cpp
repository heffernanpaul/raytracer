//
//  Emissive.cpp
//  testrender
//
//  Created by Paul Heffernan on 21/02/2015.
//  Copyright (c) 2015 Paul Heffernan. All rights reserved.
//

#include "Emissive.h"

RGBColor Emissive::shade (ShadeRec& sr) const {
    if (-sr.normal * sr.ray.d > 0)
        return (ce*ls);
    else
        return (RGBColor::black);
}

RGBColor Emissive::getLe () const {
    return ls*ce;
}

RGBColor Emissive::area_light_shade(ShadeRec& sr) const {
    
//    if (-sr.normal * sr.ray.d > 0.0) {
        return (ls * ce);
//    }
//    else return RGBColor::black;
}
