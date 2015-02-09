#include "RGBColor.h"
#include "ViewPlane.h"
#include "Tracer.h"
#include "Sampler.h"
#include "Grid.h"
#include "NestedGrid.h"
#include "KdTree.h"

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);

	void set_max_ray_tracing_depth (int depth);
	void enable_shadow (bool option);
	void set_render_option (bool option);  // direct diffuse, path tracing, etc...
	void set_sampler (Sampler* s_ptr);
	void set_ray_obj_accel (int option);
	int get_ray_obj_accel ();
public:
	RGBColor background_color;
	ViewPlane vp;
	int max_ray_tracing_depth;
	int ray_obj_accel;
	bool render_shadow;
	Tracer* tracer_ptr;
	int render_option;
	Sampler* sampler_ptr;
	Grid*						grid_ptr; // 3D-DDA traversal
	KdTree*						kd_tree_ptr; // SAH K-d tree
	NestedGrid*					nested_grid_ptr;
};

inline void Renderer::set_ray_obj_accel (int option) {
	ray_obj_accel = option;
}

inline int Renderer::get_ray_obj_accel () {
	return ray_obj_accel;
}

inline void Renderer::set_max_ray_tracing_depth (int depth) {
	max_ray_tracing_depth = depth;
}

inline void Renderer::enable_shadow (bool option) {
	render_shadow = option;
}

inline void Renderer::set_render_option (bool option) {
	render_option = option;
}

inline void Renderer::set_sampler (Sampler* s_ptr) {
	sampler_ptr = s_ptr;
}

