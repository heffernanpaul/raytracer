// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "OpenCylinder.h"
#include "Constants.h"
#include "math.h"


// This uses the value of kEpsilon defined in Constants.h

// ----------------------------------------------------------------------------- hit
// The code reverses the normal when the ray hits the inside surface, allows both
// sides to be shaded, but completely messes up transparency.

bool 															 
OpenCylinder::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {
															
	float t;
	float ox = ray.o.x;
	float oy = ray.o.y;
	float oz = ray.o.z;
	float dx = ray.d.x;
	float dy = ray.d.y;
	float dz = ray.d.z;
	
	float a = dx * dx + dz * dz;  	
	float b = 2.0f * (ox * dx + oz * dz);					
	float c = ox * ox + oz * oz - radius * radius;
	float disc = b * b - 4.0f * a * c ;

			
	if (disc < 0.0f)
		return(false);
	else {	
		float e = sqrt(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
		
		if (t > kEpsilon) {
			float yhit = oy + t * dy;
			
			if (yhit > y0 && yhit < y1) {
				tmin = t;
				sr.normal = Normal((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius);
				
				// test for hitting from inside
			
				if (-ray.d * sr.normal < 0.0f)
					sr.normal = -sr.normal;
					
				sr.local_hit_point = ray.o + tmin * ray.d;
				
				return (true);
			}
		} 
		
		t = (-b + e) / denom;    // larger root
		
		if (t > kEpsilon) {
			float yhit = oy + t * dy;
			
			if (yhit > y0 && yhit < y1) {
				tmin = t;
				sr.normal = Normal((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius);
				
				// test for hitting inside surface
			
				if (-ray.d * sr.normal < 0.0)
					sr.normal = -sr.normal;
					
				sr.local_hit_point = ray.o + tmin * ray.d;
	
				return (true);
			}
		} 
	}
	
	return (false);			
}


bool OpenCylinder::shadow_hit (const Ray& ray, float &tmin) const {															
	float t;
	float ox = ray.o.x;
	float oy = ray.o.y;
	float oz = ray.o.z;
	float dx = ray.d.x;
	float dy = ray.d.y;
	float dz = ray.d.z;
	
	float a = dx * dx + dz * dz;  	
	float b = 2.0f * (ox * dx + oz * dz);					
	float c = ox * ox + oz * oz - radius * radius;
	float disc = b * b - 4.0f * a * c ;

			
	if (disc < 0.0f)
		return(false);
	else {	
		float e = sqrt(disc);
		float denom = 2.0f * a;
		t = (-b - e) / denom;    // smaller root
		
		if (t > kEpsilon) {
			float yhit = oy + t * dy;
			
			if (yhit > y0 && yhit < y1) {
				tmin = t;
				return (true);
			}
		} 
		
		t = (-b + e) / denom;    // larger root
		
		if (t > kEpsilon) {
			float yhit = oy + t * dy;
			
			if (yhit > y0 && yhit < y1) {
				tmin = t;
				return (true);
			}
		} 
	}
	
	return (false);			
}