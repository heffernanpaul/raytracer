#pragma once
#include "Light.h"
#include "Sampler.h"
#include "ShadeRec.h"
#include "World.h"

class AmbientOccluder :public Light {
public:
	AmbientOccluder(void);
	~AmbientOccluder(void);

	virtual Light* clone(void) const;	

	void set_sampler (Sampler* s_ptr);

	virtual Vector3D get_direction (const ShadeRec& sr);

	virtual bool in_shadow (const Ray& ray, const ShadeRec& sr) const;

	virtual RGBColor L (const ShadeRec& r);

	void scale_radiance (float f);
	void set_color (const RGBColor& color);
	void set_min_amount (float mm);
	
private:
	Vector3D u,v,w;
	Sampler* sampler_ptr;
	RGBColor min_amount;
	float ls;
	RGBColor color; // ambient color
};


void inline AmbientOccluder::scale_radiance (float f) {
	ls = f;
}

void inline AmbientOccluder::set_color (const RGBColor& c) {
	color = c;
}

void inline AmbientOccluder::set_min_amount (float mm) {
	min_amount = mm;
}
