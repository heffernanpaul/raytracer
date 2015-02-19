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
#include "math.h"

RGBColor
Matte::shade(ShadeRec& sr) const {
    Vector3D 	wo 			= -sr.ray.d;
    RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);

    int 		num_lights	= sr.w.getLights().size();
    Vector3D wi;
    for (int j = 0; j < num_lights; j++) {
        Light* light = sr.w.getLights()[j];

        light->getDirection(sr.local_hit_point, wi);
        float ndotwi = sr.normal * wi;
        
        if (ndotwi > 0.0)
            L += diffuse_brdf->f(sr, wo, wi) * light->L(sr) * ndotwi;
    }
    
    return (L);
}

RGBColor
Matte::area_light_shade(ShadeRec& sr) const {
    Vector3D 	wo 			= -sr.ray.d;
    RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int 		num_lights	= sr.w.getLights().size();
    Vector3D 	wi;
    
    ShadeRec srTemp(sr);
    
    for (int j = 0; j < num_lights; j++) {
        Light* light = sr.w.getLights()[j];
        light->getDirection(sr.local_hit_point, wi);
        float 		ndotwi 	= sr.normal * wi;
        
        if (ndotwi > 0.0) {
            bool in_shadow = false;
            
            if (light->casts_shadows()) {
                Ray shadow_ray(sr.local_hit_point, wi);
                in_shadow = light->in_shadow(shadow_ray, srTemp);
            }
            
            if (!in_shadow) {
                L += diffuse_brdf->f(sr, wo, wi) * light->L(sr) * ndotwi;
            }

//                L += diffuse_brdf->f(sr, wo, wi) * light->L(sr) * light->G(sr) * ndotwi / light->pdf(sr);
        }
    }
    
    return (L);
}