//
//  AreaLight.h
//  testrender
//
//  Created by Paul Heffernan on 24/02/2015.
//  Copyright (c) 2015 Paul Heffernan. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "ShadeRec.h"
#include "Light.h"

class AreaLight : public Light {
public:
    
    AreaLight(GeometricObject* obj_ptr) : Light(RGBColor::black), object_ptr(obj_ptr) {
        
    }
    
    
    virtual
    ~AreaLight(void) {}
    
    
    virtual void getDirection(const Point3D& point, ShadeRec& sr);
    
    virtual RGBColor
    L(const ShadeRec& sr);
    
    virtual bool
    in_shadow(const Ray& ray, ShadeRec& sr) const;
    
    virtual float G(const ShadeRec& sr) const;
    virtual float pdf(const ShadeRec& sr) const;
    
    
private:
    GeometricObject* 	object_ptr;
    Material* 			material_ptr;	 // will be an emissive material
//    Point3D				sample_point;
//    Normal				light_normal;    // assigned in get_direction - which therefore can't be const for any light
//    Vector3D			wi;			     // unit direction from hit point being shaded to sample point on light surface
    
};
