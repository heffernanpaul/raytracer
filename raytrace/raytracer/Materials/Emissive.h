#include "Material.h"

class Emissive: public Material {
public:
	Emissive(void);
	~Emissive(void);
	virtual Material* clone(void) const;	

	void scale_radiance (const float f);
	void set_ce (const RGBColor& c);
	virtual RGBColor get_Le () const;
	virtual RGBColor shade (ShadeRec& sr);
	RGBColor get_ce () const;
	virtual RGBColor area_light_shade (ShadeRec& sr);

private:
	float ls;    // radiance scale
	RGBColor ce; // color
};

inline void Emissive::scale_radiance (const float f) {
	ls = f;
}

inline void Emissive::set_ce (const RGBColor& c) {
	ce = c;
}

inline RGBColor Emissive::get_ce () const {
	return RGBColor (ce);
}

