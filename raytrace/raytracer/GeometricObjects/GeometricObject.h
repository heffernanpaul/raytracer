#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

class Material;
	
#include "Point3D.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "Sampler.h"
#include "BBox.h"
#include "Constants.h"

//----------------------------------------------------------------------------------------------------- Class GeometricObject
class ShadeRec;

class GeometricObject {	
	
	public:	

		GeometricObject(void);									// default constructor
		
		GeometricObject(const GeometricObject& object);			// copy constructor
	
		virtual GeometricObject*								// virtual copy constructor
		clone(void) const = 0;

		virtual 												// destructor
		~GeometricObject (void);	
			
		virtual bool 												 
		hit(const Ray& ray, float& t, ShadeRec& s) const = 0;
				
		Material*						
		get_material(void) const;

		virtual void 							// needs to virtual so that it can be overriden in Compound
		set_material(Material* mPtr); 			

		int get_ray_id () const;

		void set_ray_id (const int& _id);

		virtual bool shadow_hit (const Ray& ray, float &tmin) const; // shadow test: secondary ray

		virtual bool shadow_hit (const Ray& ray) const; // for directional only

		virtual bool shadow_hit (const Ray& ray, float& tmin,const GeometricObject* obj_ptr) const;
	
		// the following methods are for Arealight's object
		virtual float pdf(const ShadeRec& sr);

		virtual void set_sampler (Sampler* sampler_ptr);  

		virtual Point3D sample(void);
		
		virtual Normal get_normal(const Point3D& p); 

		virtual Normal get_normal ();

		virtual BBox get_bounding_box ();

		virtual void add_object (GeometricObject* obj_ptr);

		bool cast_shadow () const;

		void set_cast_shadow (bool cast);

		bool intersect (float x0, float y0, float z0, float x1, float y1, float z1); // with a aabb

		virtual bool sat_intersect (float x0, float y0, float z0, float x1, float y1, float z1) const; // with a aabb

		virtual void intersectP (float x0, float y0, float z0, float x1, float y1, float z1, vector<Point3D>& v);

		// a plane divide the object's abbb into half, resulting in 2 new abbbs
		virtual void clipping_bbox (const BBox& bb, float split_plane, int split_axis, BBox& left_bb, BBox& right_bb);

		
		Point3D cut_x (Point3D v0, Point3D v1, float x,float& t) {
			float temp = v1.x-v0.x;
			Point3D ret;
			if (fabs (temp) < kEpsilon) { t = -1; return 0; }
			else {
				t = (x-v0.x) / temp;
				ret = (1-t)*v0 + t*v1;
				return ret;
			}
		}

		Point3D cut_y (Point3D v0, Point3D v1, float y, float& t) {
			float temp = v1.y-v0.y;
			Point3D ret;
			if (fabs (temp) < kEpsilon) { t = -1; return 0; }
			else {
				t = (y-v0.y) / temp;
				ret = (1-t)*v0 + t*v1;
				return ret;
			}
		}

		Point3D cut_z (Point3D v0, Point3D v1, float z, float& t) {
			float temp = v1.z-v0.z;
			Point3D ret;
			if (fabs (temp) < kEpsilon) { t = -1; return 0; }
			else {
				t = (z-v0.z) / temp;
				ret = (1-t)*v0 + t*v1;
				return ret;
			}
		}

	protected:
		int rayid;		 // object's id for mail boxing
		bool shadow; // cast shadow?
		BBox bbox;
		mutable Material*   material_ptr;   	// mutable allows Compound::hit, Instance::hit and Grid::hit to assign to material_ptr. hit functions are const
		Sampler* sampler_ptr; 
		GeometricObject&						// assignment operator
		operator= (const GeometricObject& rhs);
};

inline bool GeometricObject::cast_shadow () const {
	return shadow;
}

inline void GeometricObject::set_cast_shadow (bool cast) {
	shadow = cast;
}

inline void GeometricObject::add_object (GeometricObject* obj_ptr) {
	// main code is in Compound class	
}

inline BBox GeometricObject::get_bounding_box () {
	return bbox;
}

// ------------------------------------------------------------------------- get_material
inline Material* 
GeometricObject::get_material(void) const {
	return (material_ptr);
}

inline void GeometricObject::set_sampler (Sampler* s_ptr) {
	sampler_ptr = s_ptr;
}

inline Point3D GeometricObject::sample(void) {
	return Point3D(0);
}

inline Normal GeometricObject::get_normal(const Point3D& p) {
	return Normal ();
}

inline Normal GeometricObject::get_normal() {
	return Normal ();
}

inline float GeometricObject::pdf(const ShadeRec& sr) {
	return 0;
}

inline int GeometricObject::get_ray_id () const {
	return rayid;
}

inline void GeometricObject::set_ray_id (const int& _id) {
	rayid = _id;
}

inline bool GeometricObject::shadow_hit (const Ray& ray, float& tmin,const GeometricObject* obj_ptr) const {
	return true;
}

#endif

