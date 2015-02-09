// this file contains the definition of the World class
#include "wxraytracer.h"

#include "World.h"
#include "Constants.h"
// geometric objects

#include "Plane.h"
#include "Sphere.h"
#include "BBox.h"
#include "Triangle.h"
#include "Rectangle3D.h"
#include "Box.h"
#include "OpenCylinder.h"
#include "Mesh.h"
#include "SmoothMeshTriangle.h"
#include "MeshTriangle.h"
#include "FlatMeshTriangle.h"
#include "KdTree.h"

// tracers
#include "RayCast.h"

// cameras

#include "Pinhole.h"

// sampler
#include "MultiJittered.h"
#include "Sampler.h"

// lights

#include "Directional.h"
#include "AmbientOccluder.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "EnvironmentLight.h"
// materials

#include "Matte.h"
#include "Phong.h"
#include "Emissive.h"
#include "Reflective.h"

// utilities

#include "Vector3D.h"
#include "Point3D.h"
#include "Normal.h"
#include "ShadeRec.h"
#include "Maths.h"
// build functions

//#include "BuildShadedObjects.cpp"
//#include "Build_Cornell_Box.cpp"
//#include "BuildFigure18.07.cpp"
//#include "Build.cpp"  // "Build.cpp" is in 'build' folder, it contains the name of the file that
					  // implement World::build ()
// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters



World::World(void)
	:	render_ptr (new Renderer),
		ambient_ptr (new Ambient),
		camera_ptr(NULL),
		triangle_intersection_count(0),
		grid_intersection_count(0),
		grid_iteration_count(0)
{
}



//------------------------------------------------------------------ destructor

World::~World(void) {	
	
	if(render_ptr) {
		delete render_ptr;
		render_ptr = NULL;
	}
	
		
	if (ambient_ptr) {
		delete ambient_ptr;
		ambient_ptr = NULL;
	}
			
		
	if (camera_ptr) {
		delete camera_ptr;
		camera_ptr = NULL;
	}
	delete_objects();	
	delete_lights();				
}


//------------------------------------------------------------------ render_scene

// This uses orthographic viewing along the zw axis

bool World::view_frustum_culled (BBox bb) const {
	float x0=bb.x0,y0=bb.y0,z0=bb.z0,x1=bb.x1,y1=bb.y1,z1=bb.z1;
	float px,py,pz,nx,ny,nz;
	bool ret = true;
	for (int i=0;i<4;++i) {
		px = camera_ptr->frustum_planes[i].p.x;
		py = camera_ptr->frustum_planes[i].p.y;
		pz = camera_ptr->frustum_planes[i].p.z;
		nx = camera_ptr->frustum_planes[i].n.x;
		ny = camera_ptr->frustum_planes[i].n.y;
		nz = camera_ptr->frustum_planes[i].n.z;
		ret = ret && ((x0-px)*nx+(y0-py)*ny+(z0-pz)*nz >0 || 
			(x1-px)*nx+(y0-py)*ny+(z0-pz)*nz >0 ||
			(x1-px)*nx+(y0-py)*ny+(z1-pz)*nz >0 ||
			(x0-px)*nx+(y0-py)*ny+(z1-pz)*nz >0 ||
			(x0-px)*nx+(y1-py)*ny+(z0-pz)*nz >0 ||
			(x1-px)*nx+(y1-py)*ny+(z0-pz)*nz >0 ||
			(x1-px)*nx+(y1-py)*ny+(z1-pz)*nz >0 ||
			(x0-px)*nx+(y1-py)*ny+(z1-pz)*nz >0);

	}
	return !ret;
}

void World::render_scene(void) const {
	FILE* fout = fopen ("PLY_FILE/stats_intersect.txt","w");
	fprintf (fout,"Number of objects in the scene: %d\n",objects.size());
	int render_accel = render_ptr->get_ray_obj_accel ();
	if (render_accel == RAY_OBJ_GRID) {
		render_ptr->grid_ptr =new Grid();
		for (int i = 0;i<objects.size ();++i) 
			if (!view_frustum_culled(objects[i]->get_bounding_box()))
				render_ptr->grid_ptr->add_object (objects[i]);
		fprintf (fout,"Number of triangles after view frustum culling: %d\n",render_ptr->grid_ptr->objects.size());
		render_ptr->grid_ptr->setup_cells ();
	}
	else if (render_accel == RAY_OBJ_KD_TREE) {
		render_ptr->kd_tree_ptr = new KdTree ();
		render_ptr->kd_tree_ptr->build (*this);
	}
	else if (render_accel == RAY_OBJ_NESTED_GRID) {
		render_ptr->nested_grid_ptr = new NestedGrid ();
		for (int i = 0;i<objects.size ();++i) 
			if (!view_frustum_culled(objects[i]->get_bounding_box()))
				render_ptr->nested_grid_ptr->add_object (objects[i]);
		fprintf (fout,"Number of triangles after view frustum culling: %d\n",render_ptr->nested_grid_ptr->objects.size());
		render_ptr->nested_grid_ptr->setup_cells ();
	}
	long long tStart = GetTickCount();

    camera_ptr->render_scene (*this);

    long long tRender = GetTickCount() - tStart;
	int n_rays = render_ptr->vp.hres*render_ptr->vp.vres*render_ptr->vp.num_samples;
	fprintf (fout,"Total number of rays = %d \n",n_rays);
	fprintf (fout,"Total triangle intersections = %d \n",triangle_intersection_count);
	fprintf (fout,"Average intersections per ray = %.3f\n", (float) triangle_intersection_count/n_rays);
	fprintf (fout,"Total sub-grid intersections = %d \n",grid_intersection_count);
	fprintf (fout,"Average intersections per ray = %.3f\n", (float) grid_intersection_count/n_rays);
	fprintf (fout,"Total grid iterations = %d \n",grid_iteration_count);
	fprintf (fout,"Average iterations per ray = %.3f\n", (float) grid_iteration_count/n_rays);
	fprintf (fout,"Rendering time (exclude build time) = %lld ms\n",tRender);
	fclose (fout);
}  


// ------------------------------------------------------------------ clamp

RGBColor
World::max_to_one(const RGBColor& c) const  {
	float max_value = max(c.r, max(c.g, c.b));
	
	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one

RGBColor
World::clamp_to_color(const RGBColor& raw_color) const {
	RGBColor c(raw_color);
	
	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}
		
	return (c);
}


// ---------------------------------------------------------------------------display_pixel
// raw_color is the pixel color computed by the ray tracer
// its RGB floating point components can be arbitrarily large
// mapped_color has all components in the range [0, 1], but still floating point
// display color has integer components for computer display
// the Mac's components are in the range [0, 65535]
// a PC's components will probably be in the range [0, 255]
// the system-dependent code is in the function convert_to_display_color
// the function SetCPixel is a Mac OS function


void
World::display_pixel(const int row, const int column, const RGBColor& raw_color) const {
	RGBColor mapped_color;

	if (render_ptr->vp.show_out_of_gamut)
		mapped_color = clamp_to_color(raw_color);
	else
		mapped_color = max_to_one(raw_color);
	
	if (render_ptr->vp.gamma != 1.0)
		mapped_color = mapped_color.powc(render_ptr->vp.inv_gamma);
	
   //have to start from max y coordinate to convert to screen coordinates
   int x = column;
   int y = render_ptr->vp.vres - row - 1;

   paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
                             (int)(mapped_color.g * 255),
                             (int)(mapped_color.b * 255));
}

// ----------------------------------------------------------------------------- hit_objects

ShadeRec									
World::hit_objects(const Ray& ray) {
	
	ShadeRec	sr(*this); 
	float		t;
	Normal normal;
	Point3D local_hit_point;
	float		tmin 			= kHugeValue;
	int 		num_objects 	= objects.size();
	int render_accel = render_ptr->get_ray_obj_accel ();
	bool hit;
	
	if (render_accel == RAY_OBJ_GRID) {
		//Uniform Grid traversal (3D-DDA)
		hit = render_ptr->grid_ptr->hit (ray,tmin,sr);
	}
	else if (render_accel == RAY_OBJ_KD_TREE) {
		hit = render_ptr->kd_tree_ptr->hit (ray,tmin,sr);
	}
	else if (render_accel == RAY_OBJ_NESTED_GRID) {
		hit = render_ptr->nested_grid_ptr->hit (ray,tmin,sr);
	}
	else {
		for (int j = 0; j < num_objects; j++)
			if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
				sr.hit_an_object	= true;
				tmin 				= t;
				sr.material_ptr     = objects[j]->get_material();
				sr.hit_point 		= ray.o + t * ray.d;
				normal 				= sr.normal;
				local_hit_point	 	= sr.local_hit_point;
			}
	
		if (sr.hit_an_object) {
			sr.t = tmin;
			sr.normal = normal;
			sr.local_hit_point = local_hit_point;
		}
	}
	return(sr);   
}



//------------------------------------------------------------------ delete_objects

// Deletes the objects in the objects array, and erases the array.
// The objects array still exists, because it's an automatic variable, but it's empty 

void
World::delete_objects(void) {
	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		delete objects[j];
		objects[j] = NULL;
	}	
	
	objects.erase (objects.begin(), objects.end());
}


//------------------------------------------------------------------ delete_lights

void
World::delete_lights(void) {
	int num_lights = lights.size();
	
	for (int j = 0; j < num_lights; j++) {
		delete lights[j];
		lights[j] = NULL;
	}	
	
	lights.erase (lights.begin(), lights.end());
}

