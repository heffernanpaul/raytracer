#include "RayCast.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// -------------------------------------------------------------------- default constructor

RayCast::RayCast(void)
	: Tracer()
{
}


// -------------------------------------------------------------------- constructor
		
RayCast::RayCast(World* _worldPtr)
	: Tracer(_worldPtr)
{}

RayCast::RayCast(const RayCast& rc)
	: Tracer(rc)
{}



// -------------------------------------------------------------------- destructor

RayCast::~RayCast(void) {}


// -------------------------------------------------------------------- trace_ray

RGBColor	
RayCast::trace_ray(const Ray& ray) const {
	ShadeRec sr(world_ptr->hit_objects(ray));
		
	if (sr.hit_an_object) {
		sr.ray = ray;			// used for specular shading
		return (sr.material_ptr->shade(sr));
	}   
	else
		return (world_ptr->render_ptr->background_color);
}


// -------------------------------------------------------------------- trace_ray
// this ignores the depth argument

RGBColor RayCast::trace_ray(const Ray ray, const int depth) const {
	if (depth > world_ptr->render_ptr->max_ray_tracing_depth) 
		return (world_ptr->render_ptr->background_color);

	ShadeRec sr(world_ptr->hit_objects(ray));
	if (sr.hit_an_object) {
		sr.depth = depth;
		sr.ray = ray;			// used for specular shading
		return (sr.material_ptr->shade(sr));
	}   
	else
		return (world_ptr->render_ptr->background_color);
}

