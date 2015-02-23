//
//  Emissive.h
//  testrender
//
//  Created by Paul Heffernan on 21/02/2015.
//  Copyright (c) 2015 Paul Heffernan. All rights reserved.
//

#pragma once

#include "RGBColor.h"
#include "ShadeRec.h"
#include "Material.h"

class Emissive : public Material {
    
public:
    Emissive(const float f, const RGBColor& c) : ls(f), ce(c){
        
    };
    void scale_radiance (const float f);
    void set_ce (const RGBColor& c);
    virtual RGBColor get_Le () const;
    virtual RGBColor shade (ShadeRec& sr) const;
    RGBColor get_ce () const;
    virtual RGBColor area_light_shade (ShadeRec& sr) const;

private:
    float ls;    // radiance scale
    RGBColor ce; // color
};

inline void Emissive::scale_radiance (const float f) {
    ls = f;
}

inline void Emissive::set_ce (const RGBColor& c) {
    ce = c;
}

inline RGBColor Emissive::get_ce () const {
    return RGBColor (ce);
}

