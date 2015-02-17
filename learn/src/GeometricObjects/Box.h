#pragma once

#include "GeometricObject.h"

class Box :	public GeometricObject {
public:
    Box (const Material& materialIn, const float _x0, const float _y0,const float _z0, const float _x1,const float _y1, const float _z1) : GeometricObject(materialIn),
    x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1)
    {}

	Normal get_normal (const int face_hit) const;
	virtual bool hit(const Ray& ray, float& t, ShadeRec& sr) const;
	virtual bool shadow_hit(const Ray& ray, float& t) const;
protected:
	float x0, x1, y0, y1, z0, z1;
};

