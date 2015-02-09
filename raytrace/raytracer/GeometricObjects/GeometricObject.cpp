// this file contains the definition of the class GeometricObject 

#include "Constants.h"
#include "Material.h"
#include "GeometricObject.h"


// ---------------------------------------------------------------------- default constructor


GeometricObject::GeometricObject(void)
	: shadow(true),
	material_ptr(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

GeometricObject::GeometricObject (const GeometricObject& object) {
	shadow = object.cast_shadow ();
	if(object.material_ptr)
		material_ptr = object.material_ptr->clone(); 
	else  material_ptr = NULL;
}	


// ---------------------------------------------------------------------- assignment operator

GeometricObject&														
GeometricObject::operator= (const GeometricObject& rhs) {
	if (this == &rhs)
		return (*this);
			
	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}

	if (rhs.material_ptr)
		material_ptr = rhs.material_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------------- destructor

GeometricObject::~GeometricObject (void) {	
	// uncomment if 
	// when object is deleted, the material pointer is also deleted
	// this will cause error if many objects are sharing same material
	/*
	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}
	*/
}


// ---------------------------------------------------------------- set_material

void 
GeometricObject::set_material(Material* mPtr) {
	material_ptr = mPtr;
}

// shadow test
bool GeometricObject::shadow_hit (const Ray& ray, float &tmin) const {
	return false;
}

bool GeometricObject::shadow_hit (const Ray& ray) const {
	return false;
}

// check if the object's aabb intersect with the aabb
bool GeometricObject::intersect (float x0, float y0, float z0, float x1, float y1, float z1) {
	BBox bb = get_bounding_box ();
	return !((x1<=bb.x0 || x0>=bb.x1) || (y1<=bb.y0 || y0>=bb.y1) || (z1<=bb.z0 || z0>=bb.z1));
}

bool GeometricObject::sat_intersect (float x0, float y0, float z0, float x1, float y1, float z1) const {
	return true;
}

void GeometricObject::clipping_bbox (const BBox& bb, float split_plane, int split_axis, BBox& left_bb, BBox& right_bb) {
}

void GeometricObject::intersectP (float x0, float y0, float z0, float x1, float y1, float z1, vector<Point3D>& v) {
}

