//
//  Matte.h
//  testrender
//
//  Created by Paul Heffernan on 15/02/2015.
//  Copyright (c) 2015 Paul Heffernan. All rights reserved.
//

#pragma once

#include "Material.h"
#include "../BRDFs/Lambertian.h"

class Matte : public Material {

    Lambertian*		ambient_brdf;
    Lambertian*		diffuse_brdf;

    
public:
    
    Matte(float kaIn, float kdIn, const RGBColor& rgbIn)
    :	Material(),
    ambient_brdf(new Lambertian),
    diffuse_brdf(new Lambertian)
    {
        set_ka(kaIn);
        set_kd(kdIn);
        set_cd(rgbIn);
    }
    
    void
    set_ka(const float k);
    
    void
    set_kd(const float k);
    
    void
    set_cd(const RGBColor c);
    
    
    virtual RGBColor shade(ShadeRec& sr) const;
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection
// is diffuse reflection

inline void
Matte::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void
Matte::set_kd (const float kd) {
    diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void
Matte::set_cd(const RGBColor c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
}
