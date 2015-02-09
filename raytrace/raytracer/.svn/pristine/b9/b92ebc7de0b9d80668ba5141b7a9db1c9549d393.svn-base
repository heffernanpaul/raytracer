#ifndef __BOX__
#define __BOX__

#include "GeometricObject.h"

class Box :	public GeometricObject {
public:
	Box(void);
	Box (const float _x0, const float _y0,const float _z0, const float _x1,const float _y1, const float _z1);
	Box(const Point3D p0, const Point3D p1);											
	Box(const Box& box);	
	virtual Box* clone(void) const; // virtual copy constructor
	virtual ~Box(void);

	Box& operator= (const Box& rhs);
	Normal get_normal (const int face_hit) const;
	virtual bool hit(const Ray& ray, float& t, ShadeRec& sr) const;
	virtual bool shadow_hit(const Ray& ray, float& t) const;
	virtual BBox get_bounding_box () {
		return BBox (x0,x1,y0,y1,z0,z1);
	}
	virtual void clipping_bbox (const BBox& bb, float split_plane, int split_axis, BBox& left_bb, BBox& right_bb);
protected:
	float x0, x1, y0, y1, z0, z1;
};

#endif