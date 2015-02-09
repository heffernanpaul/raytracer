#include "Plane.h"

const float Plane::kEpsilon = 0.001f;

// ----------------------------------------------------------------------  default constructor

Plane::Plane(void)	
	: 	GeometricObject(),
		p(0.0),
		n(0, 1, 0)						
{}


// ----------------------------------------------------------------------  constructor

Plane::Plane(const Point3D& point, const Normal& normal)
	:	GeometricObject(),
		p(point),
		n(normal)
{
		n.normalize();
}


// ---------------------------------------------------------------- copy constructor

Plane::Plane(const Plane& plane) 
	:	GeometricObject(plane),
		p(plane.p),
		n(plane.n) 				
{}


// ---------------------------------------------------------------- clone

Plane* 
Plane::clone(void) const {
	return (new Plane(*this));
}


// ---------------------------------------------------------------- assignment operator

Plane& 
Plane::operator= (const Plane& rhs)	{
	
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	p = rhs.p;
	n = rhs.n;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Plane::~Plane(void)				
{}


// ----------------------------------------------------------------- hit

bool 															 
Plane::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {	
	float t = (p - ray.o) * n / (ray.d * n); 
														
	if (t > kEpsilon) {
		tmin = t;
		sr.normal = n;
		sr.local_hit_point = ray.o + t * ray.d;
		
		return (true);	
	}

	return(false);
}

// shadow hit
bool Plane::shadow_hit (const Ray& ray) const { // for directional light only
	float t = (p - ray.o) * n / (ray.d * n); 
														
	if (t > kEpsilon) return (true);	

	return(false);
}

bool Plane::shadow_hit(const Ray& ray, float& tmin) const {	
	float t = (p - ray.o) * n / (ray.d * n); 
														
	if (t > kEpsilon) {
		tmin = t;
		return (true);	
	}

	return(false);
}
