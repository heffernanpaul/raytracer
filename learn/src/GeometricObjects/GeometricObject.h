#pragma once

#include "Material.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "Sampler.h"
#include "Point3D.h"

class GeometricObject {
	const Material& material;

protected:
	GeometricObject(const Material& materialIn) : material(materialIn) {		
	}
	GeometricObject& operator= (const GeometricObject& rhs);
    Sampler* sampler_ptr;

public :
	const Material& getMaterial() const { 
		return material;
	}
    virtual bool hit(const Ray& ray, float& t, ShadeRec& s) const = 0;
    virtual void calcUV(ShadeRec& sr) const {
        sr.u = 0.0;
        sr.v = 0.0;
    };
    virtual void getNormal(const Point3D& p, Normal& normal) {};
    virtual Point3D sample(void) {
        return Point3D(0);
    }
    virtual float pdf(const ShadeRec& sr);

};
inline float GeometricObject::pdf(const ShadeRec& sr) {
    return 0;
}
