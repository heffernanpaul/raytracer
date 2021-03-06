#include "Lambertian.h"
#include "Constants.h"
#include "World.h"

// ---------------------------------------------------------------------- default constructor

Lambertian::Lambertian(void) 
	:   BRDF(),
		kd(0.0), 
		cd(0.0)
{}


// ---------------------------------------------------------------------- copy constructor

Lambertian::Lambertian(const Lambertian& lamb) 
	:   BRDF(lamb),
		kd(lamb.kd), 
		cd(lamb.cd)
{}


// ---------------------------------------------------------------------- clone

Lambertian* 
Lambertian::clone(void) const {
	return (new Lambertian(*this));
}	


// ---------------------------------------------------------------------- destructor

Lambertian::~Lambertian(void) {}


// ---------------------------------------------------------------------- assignment operator

Lambertian& 
Lambertian::operator= (const Lambertian& rhs) {
	if (this == &rhs)
		return (*this);
		
	BRDF::operator= (rhs);
	
	kd = rhs.kd; 
	cd = rhs.cd;
	
	return (*this);
}


// ---------------------------------------------------------------------- f

RGBColor
Lambertian::f(ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	return (kd * cd * invPI);
}


// ---------------------------------------------------------------------- rho

RGBColor
Lambertian::rho(ShadeRec& sr, const Vector3D& wo) const {
	return (kd * cd);
}


RGBColor Lambertian::sample_f(ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.0034f,1.0f,0.0071f) ^ w;
	v.normalize ();
	Vector3D u = v ^ w;
	
    Point3D sp = sr.w.render_ptr->sampler_ptr->sample_hemisphere ();
//    Point3D sp = sr.w.render_ptr->sampler_ptr->sample_sphere ();
	wi = -(sp.x*u + sp.y*v + sp.z*w); // wi is pointing into the surface
	wi.normalize (); 
	pdf = sr.normal*(-wi)*invPI;

	return (kd*cd*invPI);
}
