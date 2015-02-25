//
//  AreaLight.cpp
//  testrender
//
//  Created by Paul Heffernan on 24/02/2015.
//  Copyright (c) 2015 Paul Heffernan. All rights reserved.
//


#include "AreaLight.h"
#include "Material.h"
#include "GeometricObject.h"
#include "Sphere.h"
#include "World.h"

// --------------------------------------------------------------- get_direction

void AreaLight::getDirection(const Point3D& point, Vector3D& direction) {
//    sample_point = object_ptr->sample();    // used in the G function
//    object_ptr->getNormal(sample_point, light_normal);
//    direction = sample_point - point;  		// used in the G function
//    direction.normalize();
    
    
    sample_point = object_ptr->sample();    // used in the G function
    object_ptr->getNormal(sample_point, light_normal);
    wi = point - sample_point;  		// used in the G function
    wi.normalize();
    
    direction = wi;

//    light_normal.x = 0.0;
//    light_normal.y = 1.0;
//    light_normal.z = 0.0;
//    
//    direction.x = 0.0;
//    direction.y = 1.0;
//    direction.z = 0.0;
//    
//    wi = direction;
}


// --------------------------------------------------------------- L

RGBColor
AreaLight::L(const ShadeRec& sr) {
    float ndotd = light_normal * wi;
    
    if (ndotd > 0.0)
        return (object_ptr->getMaterial().getLe());
    else
        return (RGBColor::black);
}


// ---------------------------------------------------------------- in_shadow

bool
AreaLight::in_shadow(const Ray& ray, ShadeRec& sr) const {
////    return false;
//    float tmin = (sample_point-ray.o).length()-1;
//    if (sr.w.hit_objects(ray, sr) && sr.t < tmin) {
//        return true;
//    } else {
//        return false;
//    }
    ShadeRec localSR(sr);
    float ts = (sample_point - ray.o) * ray.d;
    return (sr.w.hit_objects(ray, localSR) && localSR.t< (ts-0.1));

}


// ---------------------------------------------------------------- G
// G is part of the geometric factor


float AreaLight::G(const ShadeRec& sr) const {
    float ndotd = light_normal * wi;
    float d2 	= sample_point.d_squared(sr.local_hit_point);
    
    return (ndotd / d2);
}


// ---------------------------------------------------------------- pdf

float
AreaLight::pdf(const ShadeRec& sr) const {
    return (object_ptr->pdf(sr));
}

