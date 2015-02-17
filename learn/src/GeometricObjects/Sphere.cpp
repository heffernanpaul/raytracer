#include "Sphere.h"
#include "math.h"

const float Sphere::kEpsilon = 0.001f;

bool Sphere::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {
	float 		t;
	Vector3D	temp 	= ray.o - center;
	float 		a 		= ray.d * ray.d;
	float 		b 		= 2.0 * (temp * ray.d);
	float 		c 		= temp * temp - radius * radius;
	float 		disc	= b * b - 4.0f * a * c;

	if (disc < 0.0)
		return(false);
	else {	
		float e = sqrt(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
	
		if (t > kEpsilon && t < tmin) {
			tmin = t;
			sr.normal 	 = (temp + t * ray.d) / radius;
			sr.local_hit_point = ray.o + t * ray.d;			
			return (true);
		} 
	
		t = (-b + e) / denom;    // larger root
	
		if (t > kEpsilon && t < tmin) {
			tmin = t;	
			sr.normal   = (temp + t * ray.d) / radius;
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		} 
	}
	
	return (false);
}
