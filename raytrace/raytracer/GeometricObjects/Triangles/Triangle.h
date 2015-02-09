#ifndef __TRIANGLE__
#define __TRIANGLE__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// this is the triangle discussed in Section 19.3

#include "GeometricObject.h"

class Triangle: public GeometricObject {	
	public:
	
		Triangle(void);   												
				
		Triangle(const Point3D& a, const Point3D& b, const Point3D& c);

		virtual Triangle* 												
		clone(void) const;
	
		Triangle(const Triangle& triangle); 							

		~Triangle(void);   											

		Triangle& 														
		operator= (const Triangle& rhs);
		
		virtual BBox															
		get_bounding_box(void);

		void 
		compute_normal(void);
		
		virtual bool hit(const Ray& ray, float& tmin, ShadeRec& sr) const;		 					 
		
		virtual bool shadow_hit(const Ray& ray, float &tmin) const; 

		// a plane divide the object's abbb into half, resulting in 2 new abbb(s). this process is called
		// in a SAH Kd tree build
		virtual void clipping_bbox (const BBox& bb, float split_plane, int split_axis, BBox& left_bb, BBox& right_bb);

		virtual bool sat_intersect (float x0, float y0, float z0, float x1, float y1, float z1) const ; // with a aabb
		
	private:
	
		Point3D	v0,v1,v2;
		Normal	normal;
};

#endif

