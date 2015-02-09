#ifndef __ENVIRONMENT_LIGHT__
#define __ENVIRONMENT_LIGHT__

#include "Light.h"
#include "Sampler.h"
#include "Material.h"

class EnvironmentLight :public Light {
public:
	EnvironmentLight(void);
	EnvironmentLight(const EnvironmentLight& el);				
	virtual Light* clone(void) const;
	~EnvironmentLight(void);
	//EnvironmentLight& operator= (const EnvironmentLight& rhs); 
	void set_material (Material* material_ptr);
	void set_sampler (Sampler* material_ptr);
	virtual Vector3D get_direction(const ShadeRec& s);
	virtual RGBColor L(const ShadeRec& sr);
	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;
	virtual float G(const ShadeRec& sr) const;
	virtual float pdf(const ShadeRec& sr) const;

private:
	Sampler* sampler_ptr;
	Material* material_ptr;
	Vector3D u,v,w;
	Vector3D wi;
};

#endif