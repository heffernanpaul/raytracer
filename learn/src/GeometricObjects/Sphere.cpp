#include "Sphere.h"
#include "math.h"

const float Sphere::kEpsilon = 0.001f;

bool Sphere::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {
	float 		t;
    Vector3D	temp 	= ray.o - center;       // Once Sphere per rendering
	float 		a 		= ray.d * ray.d;        // Once per pixel
	float 		b 		= 2.0 * (temp * ray.d); // Once per pixel per Sphere per rendering
	float 		c 		= temp * temp - radius * radius;
    float 		disc	= b * b - 4.0f * a * c;// Once Sphere per rendering

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

void Sphere::calcUV(ShadeRec& sr) const {
    sr.u = sr.normal * Vector3D(-1,0,0);
    sr.v = sr.normal * Vector3D(0,1,0);
}

