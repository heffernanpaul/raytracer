#pragma once

#include "Light.h"

class PointLight : public Light {

    float ls;
	const Point3D origin;

public:
	PointLight(const RGBColor& colorIn, const Point3D& originIn, float lsIn) : Light(colorIn), origin(originIn), ls(lsIn) {
	}
			
	virtual void getDirection(const Point3D& point, Vector3D& direction) const {
        direction.x = origin.x - point.x;
        direction.y = origin.y - point.y;
        direction.z = origin.z - point.z;
		direction.normalize();
	}
    
    RGBColor L(const ShadeRec& sr) {
        return (ls * color);
    }
    
    
    virtual bool
    in_shadow(const Ray& ray, ShadeRec& sr) const {
        float tmin = (origin-ray.o).length();
        return sr.w.hit_objects(ray, sr) && sr.t < tmin;
    }
};