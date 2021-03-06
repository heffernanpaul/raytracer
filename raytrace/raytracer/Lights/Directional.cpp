#include "Directional.h"
#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

Directional::Directional(void)
	: 	Light(),
		color(1.0),
		dir(0, 1, 0)			
{}


// ---------------------------------------------------------------------- dopy constructor

Directional::Directional(const Directional& dl)
	: 	Light(dl.intensity),
		color(dl.color),
		dir(dl.dir)  		
{}


// ---------------------------------------------------------------------- clone

Light* 
Directional::clone(void) const {
	return (new Directional(*this));
}


// ---------------------------------------------------------------------- assignment operator

Directional& 
Directional::operator= (const Directional& rhs) 	
{
	if (this == &rhs)
		return (*this);
			
	Light::operator= (rhs);
	
	intensity = rhs.intensity;
	color 	= rhs.color;
	dir 	= rhs.dir;

	return (*this);
}


// ---------------------------------------------------------------------- destructor																			

Directional::~Directional(void) {}


// ---------------------------------------------------------------------- get_direction
// as this function is virtual, it shouldn't be inlined 

Vector3D								
Directional::get_direction(const ShadeRec& sr) {
	return (dir);
}	

// ------------------------------------------------------------------------------  L

RGBColor
Directional::L(const ShadeRec& s) {	
	return (intensity * color);
}
bool Directional::in_shadow (const Ray& ray, const ShadeRec& sr) const {
	float t;
	int render_accel = sr.w.render_ptr->get_ray_obj_accel();
	if (render_accel == RAY_OBJ_NESTED_GRID) 
		return  (sr.w.render_ptr->nested_grid_ptr->shadow_hit (ray,t,sr.obj_ptr));
	else if (render_accel == RAY_OBJ_GRID) 
		return  (sr.w.render_ptr->grid_ptr->shadow_hit (ray,t,sr.obj_ptr));

}

