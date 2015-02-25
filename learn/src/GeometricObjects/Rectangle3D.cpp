// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Rectangle3D.h"

const float Rectangle3D::kEpsilon = 0.001f;


// ----------------------------------------------------------------  constructor
// this has the normal as an argument

Rectangle3D::Rectangle3D(const Material& material, const Point3D& _p0, const Vector3D& _a, const Vector3D& _b, const Normal& n, Sampler* sampler_ptr)
	:	GeometricObject(material),
		p0(_p0),
		a(_a),
		b(_b),
		a_len_squared(a.len_squared()), 
		b_len_squared(b.len_squared()),
		area(a.length() * b.length()),	
		inv_area(1.0f / area),
		normal(n),
		sampler_ptr(sampler_ptr)
{
	normal.normalize();
}




//------------------------------------------------------------------ get_bounding_box 
/*
BBox
Rectangle3D::get_bounding_box(void) {
	float delta = 0.0001; 

	return(BBox(min(p0.x, p0.x + a.x + b.x) - delta, max(p0.x, p0.x + a.x + b.x) + delta,
				min(p0.y, p0.y + a.y + b.y) - delta, max(p0.y, p0.y + a.y + b.y) + delta, 
				min(p0.z, p0.z + a.z + b.z) - delta, max(p0.z, p0.z + a.z + b.z) + delta));
}
*/																	

//------------------------------------------------------------------ hit 

bool 												 
Rectangle3D::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {	
	
	float t = (p0 - ray.o) * normal / (ray.d * normal); 
	
	if (t <= kEpsilon)
		return (false);
			
	Point3D p = ray.o + t * ray.d;
	Vector3D d = p - p0;
	
	float ddota = d * a;
	
	if (ddota < 0.0 || ddota > a_len_squared)
		return (false);
		
	float ddotb = d * b;
	
	if (ddotb < 0.0 || ddotb > b_len_squared)
		return (false);
		
	tmin 				= t;
	sr.normal 			= normal;
	sr.local_hit_point 	= p;
	
	return (true);
}


// ---------------------------------------------------------------- setSampler

void 								
Rectangle3D::set_sampler(Sampler* sampler) {
	sampler_ptr = sampler;
}


// ---------------------------------------------------------------- sample
// returns a sample point on the Rectangle3D

Point3D 											
Rectangle3D::sample(void) {
	Point2D sample_point = sampler_ptr->sample_unit_square();
	return (p0 + sample_point.x * a + sample_point.y * b);
}


//------------------------------------------------------------------ get_normal 


// ---------------------------------------------------------------- pdf

float
Rectangle3D::pdf(const ShadeRec& sr) {	
	return (inv_area);
} 





