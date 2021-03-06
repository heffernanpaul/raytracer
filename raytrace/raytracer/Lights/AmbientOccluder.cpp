#include "AmbientOccluder.h"


AmbientOccluder::AmbientOccluder(void):
Light (), min_amount (0),ls(1.0), color (1), sampler_ptr (NULL) {}

AmbientOccluder::~AmbientOccluder(void)
{
}

Light* AmbientOccluder::clone(void) const {
	return (new AmbientOccluder(*this));
}


Vector3D AmbientOccluder::get_direction (const ShadeRec& sr) {
	Point3D sp = sampler_ptr->sample_hemisphere ();
	return (sp.x*u + sp.y* v + sp.z*w);
}

void AmbientOccluder::set_sampler (Sampler *s_ptr) {
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
	sampler_ptr = s_ptr;
	sampler_ptr->map_samples_to_hemisphere (1);
}

bool AmbientOccluder::in_shadow (const Ray& ray, const ShadeRec& sr) const {
	float t;
	// Native shadow ray hit
	
	for (int i = 0;i < sr.w.objects.size (); ++i) 
		if (sr.w.objects[i]->cast_shadow () && sr.w.objects[i]->shadow_hit (ray,t)) return true;
	return false;
	
	//return (sr.w.render_ptr->grid_ptr->shadow_hit (ray,t,sr.obj_ptr));
}

RGBColor AmbientOccluder::L (const ShadeRec& sr) {
	w = sr.normal;
	// jitter up vector in case normal is vertical
	v = w^Vector3D(.0072f,1.0f,.0034f);
	v.normalize ();
	u = v^w;
	/*
	Ray shadow_ray;
	shadow_ray.o = sr.hit_point;
	shadow_ray.d = get_direction (sr);
		
	if (!in_shadow (shadow_ray, sr)) 
		return (ls*color)*min_amount;
	else return (black);
	*/
	
	
	Ray shadow_ray;
	int num_unoccluded = 0;
	for (int i = 0;i<num_shadow_rays;++i) {
		shadow_ray.o = sr.hit_point;
		shadow_ray.d = get_direction (sr);
		
		if (!in_shadow (shadow_ray, sr)) num_unoccluded++;
	}
	float accessibility = 1.0f*num_unoccluded / num_shadow_rays;
	return (ls*color)*min_amount*accessibility;
	
	
}