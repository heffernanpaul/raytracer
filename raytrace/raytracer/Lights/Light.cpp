#include "Light.h"

#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

Light::Light(void):intensity(1.0f),num_shadow_rays (1),isArea(false) {}

Light::Light(float _intensity):intensity(_intensity),num_shadow_rays (1),isArea(false) {}

// ---------------------------------------------------------------------- copy constructor

Light::Light(const Light& ls) {}


// ---------------------------------------------------------------------- assignment operator

Light& 
Light::operator= (const Light& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------------- destructor

Light::~Light(void) {} 



// ---------------------------------------------------------------------- L
// returns the radiance

RGBColor								
Light::L(const ShadeRec& s) {
	return (black);
}


// shadow test
bool Light::in_shadow (const Ray& ray, const ShadeRec& sr) const {
	return false;
}

