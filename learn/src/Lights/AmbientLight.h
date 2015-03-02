#pragma once

class AmbientLight : public Light {


public:
	AmbientLight(const RGBColor& color) : Light(color) {
	}

	virtual void getDirection(const Point3D& point, ShadeRec& sr) {
        sr.lightDirection.x = -point.x;
        sr.lightDirection.y = -point.y;
        sr.lightDirection.z = -point.z;
        sr.lightDirection.normalize();
        sr.samplePoint=point;
	}

    RGBColor L(const ShadeRec& sr) {
        return (color);
    }
    
    virtual bool in_shadow (const Ray& ray, ShadeRec& sr) const {
        return false;
    }; //shadow test


};