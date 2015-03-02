#pragma once

#include "Light.h"

class PointLight : public Light {

    float ls;
	const Point3D origin;
public:
	PointLight(const RGBColor& colorIn, const Point3D& originIn, float lsIn) : Light(colorIn), origin(originIn), ls(lsIn) {
	}
			
	virtual void getDirection(const Point3D& point, ShadeRec& sr) {
        sr.lightDirection.x = point.x - origin.x;
        sr.lightDirection.y = point.y - origin.y;
        sr.lightDirection.z = point.z - origin.z;
		sr.lightDirection.normalize();
        sr.lightNormal.x = sr.lightDirection.x;
        sr.lightNormal.y = sr.lightDirection.y;
        sr.lightNormal.z = sr.lightDirection.z;
        sr.samplePoint = origin;    // used in the G function
	}
    
    RGBColor L(const ShadeRec& sr) {
        return (ls * color);
    }
    
    
    virtual bool
    in_shadow(const Ray& ray, ShadeRec& sr) const {
        ShadeRec localSR(sr);
        float ts = (sr.samplePoint - ray.o) * ray.d;
        return (sr.w.hit_objects(ray, localSR) && localSR.t< (ts-0.1));
    }
    
    virtual float G(const ShadeRec& sr) const;
    
    virtual float pdf(const ShadeRec& sr) const;
    

};




inline float PointLight::G(const ShadeRec& sr) const {
//    float ndotd = sr.lightNormal * sr.lightDirection;
    float d2 	= sr.samplePoint.distance(sr.local_hit_point);
    
    return 1.0 / d2; //(ndotd / d2);
}

inline float PointLight::pdf(const ShadeRec& sr) const {
    return invTWO_PI;
}
