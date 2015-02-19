#pragma once

#include "RGBColor.h"
#include "Vector3D.h"
#include "Point3D.h"

class Light {

protected:
	const RGBColor color;

	Light(const RGBColor& color) : color(color) {
	}

public:
	const RGBColor& getColor() const {
		return color;
	}
	virtual void getDirection(const Point3D& point, Vector3D& direction) const = 0;

    virtual RGBColor L(const ShadeRec& sr) = 0;
    virtual bool casts_shadows() { return true; }
    virtual bool in_shadow (const Ray& ray, ShadeRec& sr) const=0; //shadow test

};