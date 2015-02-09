// This file contains the definition of the class sphere

#include "Sphere.h"
#include "math.h"

const float Sphere::kEpsilon = 0.001f;
					
// ---------------------------------------------------------------- default constructor

Sphere::Sphere(void)	
	: 	GeometricObject(),
		center(0.0),
		radius(1.0)
{
	const float SQRT2 = 1.4142135623731f;
	bbox.x0 = center.x-SQRT2*radius;
	bbox.y0 = center.y-SQRT2*radius;
	bbox.z0 = center.z-SQRT2*radius;
	bbox.x1 = center.x+SQRT2*radius;
	bbox.y1 = center.y+SQRT2*radius;
	bbox.z1 = center.z+SQRT2*radius;
}


// ---------------------------------------------------------------- constructor

Sphere::Sphere(Point3D c, float r)
	: 	GeometricObject(),
		center(c),
		radius(r)
{
	const float SQRT2 = 1.4142135623731f;
	bbox.x0 = center.x-SQRT2*radius;
	bbox.y0 = center.y-SQRT2*radius;
	bbox.z0 = center.z-SQRT2*radius;
	bbox.x1 = center.x+SQRT2*radius;
	bbox.y1 = center.y+SQRT2*radius;
	bbox.z1 = center.z+SQRT2*radius;
}


// ---------------------------------------------------------------- clone

Sphere* 
Sphere::clone(void) const {
	return (new Sphere(*this));
}


// ---------------------------------------------------------------- copy constructor

Sphere::Sphere (const Sphere& sphere)
	: 	GeometricObject(sphere),
		center(sphere.center),
		radius(sphere.radius)
{
	const float SQRT2 = 1.4142135623731f;
	bbox.x0 = center.x-SQRT2*radius;
	bbox.y0 = center.y-SQRT2*radius;
	bbox.z0 = center.z-SQRT2*radius;
	bbox.x1 = center.x+SQRT2*radius;
	bbox.y1 = center.y+SQRT2*radius;
	bbox.z1 = center.z+SQRT2*radius;
}



// ---------------------------------------------------------------- assignment operator

Sphere& 
Sphere::operator= (const Sphere& rhs)		
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center 	= rhs.center;
	radius	= rhs.radius;
	bbox = rhs.bbox;
	return (*this);
}


// ---------------------------------------------------------------- destructor

Sphere::~Sphere(void) {}


//---------------------------------------------------------------- hit


BBox Sphere::get_bounding_box () {
	float rSqrt2 = radius*1.41421357f;
	return BBox (center.x-rSqrt2,center.x+rSqrt2,center.y-rSqrt2,center.y+rSqrt2,center.z-rSqrt2,center.z+rSqrt2);
}

bool Sphere::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {
	float 		t;
	Vector3D	temp 	= ray.o - center;
	float 		a 		= ray.d * ray.d;
	float 		b 		= 2.0 * temp * ray.d;
	float 		c 		= temp * temp - radius * radius;
	float 		disc	= b * b - 4.0f * a * c;
	
	if (disc < 0.0)
		return(false);
	else {	
		float e = sqrt(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
	
		if (t > kEpsilon) {
			tmin = t;
			sr.normal 	 = (temp + t * ray.d) / radius;
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		} 
	
		t = (-b + e) / denom;    // larger root
	
		if (t > kEpsilon) {
			tmin = t;	
			sr.normal   = (temp + t * ray.d) / radius;
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		} 
	}
	
	return (false);
}


// shadow hit
bool Sphere::shadow_hit (const Ray& ray) const { // for directional light only
	float 		t;
	Vector3D	temp 	= ray.o - center;
	float 		a 		= ray.d * ray.d;
	float 		b 		= 2.0 * temp * ray.d;
	float 		c 		= temp * temp - radius * radius;
	float 		disc	= b * b - 4.0f * a * c;
	
	if (disc < 0.0)
		return(false);
	else {	
		float e = sqrt(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
	
		if (t > kEpsilon) return true;
	
		t = (-b + e) / denom;    // larger root
	
		if (t > kEpsilon) return true;
	}
	
	return (false);
}

bool Sphere::shadow_hit(const Ray& ray, float& tmin) const {	
	float 		t;
	Vector3D	temp 	= ray.o - center;
	float 		a 		= ray.d * ray.d;
	float 		b 		= 2.0 * temp * ray.d;
	float 		c 		= temp * temp - radius * radius;
	float 		disc	= b * b - 4.0f * a * c;
	
	if (disc < 0.0)
		return(false);
	else {	
		float e = sqrt(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
	
		if (t > kEpsilon) {
			tmin = t;
			return (true);
		} 
	
		t = (-b + e) / denom;    // larger root
	
		if (t > kEpsilon) {
			tmin = t;
			return (true);
		} 
	}
	
	return (false);
}






