#include "EnvironmentLight.h"
#include "cstdlib"
#include "Constants.h"

EnvironmentLight::~EnvironmentLight(void)
{
}

EnvironmentLight::EnvironmentLight(void)
	: 	Light(),
		sampler_ptr(NULL),
		material_ptr(NULL)

{isArea = true;}	


// ---------------------------------------------------------------- copy constructor 

EnvironmentLight::EnvironmentLight(const EnvironmentLight& el)
	: 	Light(el) {
	if(el.sampler_ptr)
		sampler_ptr = el.sampler_ptr->clone(); 
	else  sampler_ptr = NULL;
	
	if(el.material_ptr)
		material_ptr = el.material_ptr->clone(); 
	else  material_ptr = NULL;
}


// ---------------------------------------------------------------- clone

Light* EnvironmentLight::clone(void) const {
	return (new EnvironmentLight(*this));
}

Vector3D EnvironmentLight::get_direction (const ShadeRec& sr) {
	w = sr.normal;
	v = Vector3D(0.0034f,1.0f,0.0071f)^w;
	v.normalize ();
	u = v^w;
	Point3D sp = sampler_ptr->sample_hemisphere ();
	wi = -(sp.x*u + sp.y*v + sp.z*w); // wi is pointing into the surface

	return (wi);
}

RGBColor EnvironmentLight::L (const ShadeRec& sr) {
	return material_ptr->get_Le ();
}

float EnvironmentLight::G(const ShadeRec& sr) const {
	return 1.0;
}

float EnvironmentLight::pdf(const ShadeRec& sr) const {
	float ndotwi = sr.normal*(-wi);
	return ndotwi*invPI;
}

void EnvironmentLight::set_material (Material* m_ptr) {
	material_ptr = m_ptr;
}

void EnvironmentLight::set_sampler (Sampler* s_ptr) {
	sampler_ptr = s_ptr;
}

bool EnvironmentLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {
	float t;
	int num_objects = sr.w.objects.size();
	/*
	float ts = (sample_point - ray.o) * ray.d;
	for (int j = 0; j < num_objects; j++)
		if (sr.w.objects[j]->shadow_hit(ray, t) && t < ts)
			return (true); 
	*/
	// my code
	for (int j = 0; j < num_objects; j++)
		if (sr.w.objects[j]->cast_shadow () && sr.w.objects[j]->shadow_hit(ray, t))
			return (true); 
		
	return (false);     
}
