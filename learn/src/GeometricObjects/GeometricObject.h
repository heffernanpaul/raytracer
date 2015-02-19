#pragma once

#include "Material.h"
#include "Ray.h"
#include "ShadeRec.h"

class GeometricObject {
	const Material& material;

protected:
	GeometricObject(const Material& materialIn) : material(materialIn) {		
	}
	GeometricObject& operator= (const GeometricObject& rhs);

public :
	const Material& getMaterial() const { 
		return material;
	}
    virtual bool hit(const Ray& ray, float& t, ShadeRec& s) const = 0;
    virtual void calcUV(ShadeRec& sr) const {
        sr.u = 0.0;
        sr.v = 0.0;
    };

};
