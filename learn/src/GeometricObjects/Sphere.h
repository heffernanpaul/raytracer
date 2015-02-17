#pragma once

#include "Point3D.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "Material.h"
#include "GeometricObject.h"

class Sphere : public GeometricObject {

		Point3D 	center;   			// center coordinates as a point  
		float 		radius;				// the radius 
		static const float kEpsilon;   // for shadows and secondary rays

public:
	Sphere(const Point3D& centerIn, const float radiusIn, const Material& materialIn) : GeometricObject(materialIn),
	 center(centerIn), radius(radiusIn) {};

	virtual bool hit(const Ray& ray, float& t, ShadeRec& s) const;	

};