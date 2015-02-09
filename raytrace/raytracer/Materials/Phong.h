#pragma once
#include "Material.h"
#include "GlossySpecular.h"
#include "Lambertian.h"

class Phong :public Material
{
public:
	Phong(void);
	virtual Material* clone(void) const;
	virtual ~Phong(void);
	virtual RGBColor shade (ShadeRec& s);
    virtual RGBColor area_light_shade (ShadeRec& sr);

	void set_ka ( float ka);
	void set_kd ( float kd);
	void set_ks ( float ks);
	void set_cd (const RGBColor& color);
	void set_cd (float r, float g, float b);
	void set_cd (float c);
	void set_exp ( float exp);
	void set_cs (const RGBColor& color);
	void set_cs ( float r,  float g,  float b);
	void set_cs (float c);

private:
	Lambertian *ambient_brdf;
	Lambertian *diffuse_brdf;
	GlossySpecular *specular_brdf;
};

inline void Phong::set_ka (float ka) {
	ambient_brdf->set_ka (ka);
}

inline void Phong::set_kd (float kd) {
	diffuse_brdf->set_kd (kd);
}

inline void Phong::set_ks (float ks) {
	specular_brdf->set_ks (ks);
}

inline void Phong::set_cd (const RGBColor& color) {
	diffuse_brdf->set_cd (color);
		ambient_brdf->set_cd (color);
}

inline void Phong::set_cd (float r, float g, float b) {
	diffuse_brdf->set_cd (r,g,b);
}

inline void Phong::set_cd (float c) {
	diffuse_brdf->set_cd (c);
}

inline void Phong::set_exp (float exp) {
	specular_brdf->set_exp (exp);
}

inline void Phong::set_cs (const RGBColor& color) {
	specular_brdf->set_cs (color);
}

inline void Phong::set_cs ( float r, float g, float b) {
	specular_brdf->set_cs (r,g,b);
}

inline void Phong::set_cs ( float c) {
	specular_brdf->set_cs (c);
}