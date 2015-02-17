//
//  Matte.cpp
//  testrender
//
//  Created by Paul Heffernan on 15/02/2015.
//  Copyright (c) 2015 Paul Heffernan. All rights reserved.
//

#include "Matte.h"
#include "World.h"
#include "../Utilities/Vector3D.h"

RGBColor
Matte::shade(ShadeRec& sr) const {
    Vector3D 	wo 			= -sr.ray.d;
    RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);

    int 		num_lights	= sr.w.getLights().size();
    Vector3D wi;
    for (int j = 0; j < num_lights; j++) {
        sr.w.getLights()[j]->getDirection(sr.local_hit_point, wi);
        float ndotwi = sr.normal * wi;
        
        if (ndotwi > 0.0)
            L += diffuse_brdf->f(sr, wo, wi) * sr.w.getLights()[j]->L(sr) * ndotwi;
    }
    
    return (L);
}