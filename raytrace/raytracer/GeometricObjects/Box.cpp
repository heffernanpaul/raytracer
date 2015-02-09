#include "Box.h"
#include "Constants.h"


Box::Box(void):
	GeometricObject (), x0(-1), x1(1), y0(-1), y1(1), z0(-1), z1(1)
{}

Box::Box (const float _x0, const float _y0,			
				const float _z0, const float _x1, 
				const float _y1, const float _z1)
	: GeometricObject (), x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1)
{}

Box::Box (const Point3D p0, const Point3D p1)
	: GeometricObject (), x0(p0.x), x1(p1.x), y0(p0.y), y1(p1.y), z0(p0.z), z1(p1.z)
{}
			
Box* Box::clone(void) const {
	return (new Box(*this));
}


// --------------------------------------------------------------------- copy constructor

Box::Box (const Box& Box)
	: GeometricObject (), x0(Box.x0), x1(Box.x1), y0(Box.y0), y1(Box.y1), z0(Box.z0), z1(Box.z1)
{}		


Box& Box::operator= (const Box& rhs) {
	if (this == &rhs)
		return (*this);

	x0	= rhs.x0;
	x1	= rhs.x1;
	y0	= rhs.y0;
	y1	= rhs.y1;
	z0	= rhs.z0;
	z1	= rhs.z1;	
	
	return (*this);
}	
Box::~Box(void)
{
}


Normal Box::get_normal (const int face_hit) const {
		switch (face_hit) {
			case 0: return Normal (-1,0,0); // -x face
			case 1: return Normal (0,-1,0); // -y 
			case 2: return Normal (0,0,-1); // -z 
			case 3: return Normal (1,0,0); // +x 
			case 4: return Normal (0,1,0); // +y
			case 5: return Normal (0,0,1); // +z
		}
}

// --------------------------------------------------------------------- hit

bool Box::hit(const Ray& ray, float &tmin, ShadeRec& sr) const {	
	float ox = ray.o.x; float oy = ray.o.y; float oz = ray.o.z;
	float dx = ray.d.x; float dy = ray.d.y; float dz = ray.d.z;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max; 

	float a = 1.0f / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}
	
	float b = 1.0f / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}
	
	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}
	
	float t0, t1;
	
	// find largest entering t value
	int face_in, face_out;

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a>=0.0)?0:3;
	}
	else {
		t0 = ty_min;
		face_in = (b>=0.0)?1:4;
	}
		
	if (tz_min > t0) {
		t0 = tz_min;	
		face_in = (c>=0.0)?2:5;
	}
		
	// find smallest exiting t value
		
	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a>=0.0)?3:0;
	}
	else {
		t1 = ty_max;
		face_out = (b>=0.0)?4:1;
	}
		
	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c>=0.0)?5:2;
	}
		

	if (t0<t1&&t1>kEpsilon) {
		if (t0>kEpsilon) { 
			tmin = t0;
			sr.normal = get_normal (face_in);
		}
		else {
			tmin = t1;
			sr.normal = get_normal (face_out);
		}

		sr.local_hit_point = ray.o + tmin*ray.d;
		return true;
	}
	else return false;
}

bool Box::shadow_hit(const Ray& ray, float &tmin) const {	
	float ox = ray.o.x; float oy = ray.o.y; float oz = ray.o.z;
	float dx = ray.d.x; float dy = ray.d.y; float dz = ray.d.z;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max; 

	float a = 1.0f / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}
	
	float b = 1.0f / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}
	
	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}
	
	float t0, t1;
	
	// find largest entering t value
	int face_in, face_out;

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a>=0.0)?0:3;
	}
	else {
		t0 = ty_min;
		face_in = (b>=0.0)?1:4;
	}
		
	if (tz_min > t0) {
		t0 = tz_min;	
		face_in = (c>=0.0)?2:5;
	}
		
	// find smallest exiting t value
		
	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a>=0.0)?3:0;
	}
	else {
		t1 = ty_max;
		face_out = (b>=0.0)?4:1;
	}
		
	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c>=0.0)?5:2;
	}
		

	if (t0<t1&&t1>kEpsilon) {
		if (t0>kEpsilon) { 
			tmin = t0;
		}
		else {
			tmin = t1;
		}

		return true;
	}
	else return false;
}

void Box::clipping_bbox (const BBox& bb, float split_plane, int split_axis, BBox& left_bb, BBox& right_bb) {
	left_bb = bb;
	right_bb = bb;
	if (split_axis==X_AXIS) {
		left_bb.x1 = split_plane;
		right_bb.x0 = split_plane;
	}
	else if (split_axis==Y_AXIS) {
		left_bb.y1 = split_plane;
		right_bb.y0 = split_plane;
	}
	else {
		left_bb.z1 = split_plane;
		right_bb.z0 = split_plane;
	}
}