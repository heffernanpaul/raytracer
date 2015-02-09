// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "Constants.h"
#include "Triangle.h"
#include "Maths.h"

// ----------------------------------------------------------------  default constructor

Triangle::Triangle (void)
	: 	GeometricObject(),
		v0(0, 0, 0), 
		v1(0,0,1), 
		v2(1,0,0),
		normal(0, 1, 0)
{}

// ---------------------------------------------------------------- constructor

Triangle::Triangle (const Point3D& a, const Point3D& b, const Point3D& c)
	: 	GeometricObject(),
		v0(a),
		v1(b),
		v2(c)
{
	compute_normal();	
}


// ---------------------------------------------------------------- clone

Triangle* 
Triangle::clone(void) const {
	return (new Triangle(*this));
}


// ---------------------------------------------------------------- copy constructor

Triangle::Triangle (const Triangle& triangle)
	:	GeometricObject(triangle),
		v0(triangle.v0),
		v1(triangle.v1),
		v2(triangle.v2),
		normal(triangle.normal)
{}


// ---------------------------------------------------------------- assignment operator

Triangle& 
Triangle::operator= (const Triangle& rhs) {
	if (this == &rhs)
		return (*this);

	GeometricObject::operator=(rhs);

	v0 		= rhs.v0;
	v1 		= rhs.v1;
	v2 		= rhs.v2;
	normal 	= rhs.normal;
		
	return (*this);
}


// ---------------------------------------------------------------- destructor

Triangle::~Triangle (void) {}



// ---------------------------------------------------------------- compute_normal

void 
Triangle::compute_normal(void) {
	normal = (v1 - v0) ^ (v2 - v0);  
	normal.normalize();
}


BBox
Triangle::get_bounding_box(void) {
	float delta = 0.000001f; 
	// we dont use +-delta here cos it will affect the kd-tree split candidates
	return (BBox(min(min(v0.x, v1.x), v2.x) , max(max(v0.x, v1.x), v2.x), 
				 min(min(v0.y, v1.y), v2.y) , max(max(v0.y, v1.y), v2.y), 
				 min(min(v0.z, v1.z), v2.z) , max(max(v0.z, v1.z), v2.z)));
}


// ------------------------------------------------------------------------------ hit

bool 
Triangle::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {	
	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
		
	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float inv_denom  = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * inv_denom;
	
	if (beta < 0.0)
	 	return (false);
	
	float r = r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * inv_denom;
	
	if (gamma < 0.0 )
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	float e3 = a * p - b * r + d * s;
	float t = e3 * inv_denom;
	
	if (t < kEpsilon) 
		return (false);
					
	tmin 				= t;
	sr.normal 			= normal;  	
	sr.local_hit_point 	= ray.o + t * ray.d;	
	
	return (true);	
}  		


// ------------------------------------------------------------------------------ shadow_hit

bool 																						 
Triangle::shadow_hit(const Ray& ray, float &tmin) const {	
	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
		
	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float inv_denom  = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * inv_denom;
	
	if (beta < 0.0)
	 	return (false);
	
	float r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * inv_denom;
	
	if (gamma < 0.0)
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	float e3 = a * p - b * r + d * s;
	float t = e3 * inv_denom;
	
	if (t < kEpsilon)
		return (false);
					
	tmin = t;
	
	return(true);	
}  

// use with a AABB of a triangle
// when divide the AABB into half, resulting in 2 AABBs
// this method is used to clip those AABBs to fit the triangle
// this method is called when constructing the SAH K-d tree
void Triangle::clipping_bbox (const BBox& bb, float split_plane, int split_axis, BBox& left_bb, BBox& right_bb) {
	vector<Point3D> p;
	Point3D p0,p1;
	float t;
	if (split_axis == X_AXIS) {
		if (v0.x == split_plane) {
			p.push_back (cut_x (v1,v2,split_plane,t));
			p.push_back (v0);
		}
		else if (v1.x == split_plane) {
			p.push_back (cut_x (v0,v1,split_plane,t));
			p.push_back (v1);
		}
		else if (v2.x == split_plane) {
			p.push_back (cut_x (v0,v1,split_plane,t));
			p.push_back (v2);
		}
		else {
			p0 = cut_x (v0,v1,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
			p0 = cut_x (v1,v2,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
			p0 = cut_x (v0,v2,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
		}
		left_bb = bb;
		right_bb = bb;
		left_bb.x1 = split_plane;
		left_bb.x0 = split_plane;

		p0 = p[0];
		p1 = p[1];
		// p0 is the possibly lower bound for y, p1 is the possibly upper bound for y
		if (p0.y >p1.y) {
			Point3D temp = p0;
			p0 = p1;
			p1 = temp;
		}
		bool cull_y0 = true, cull_y1 = true;
		if (v0.x<=split_plane && v0.y<=p0.y) cull_y0 = false; // v0 is in left aabb and v0.y >= new lower y bound 
		if (v1.x<=split_plane && v1.y<=p0.y) cull_y0 = false; // v1 is in left aabb and v1.y >= new lower y bound
		if (v2.x<=split_plane && v2.y<=p0.y) cull_y0 = false; // v2 is in left aabb and v2.y >= new lower y bound
		if (cull_y0) left_bb.y0 = p0.y;
		cull_y0 = true; // reset for the right aabb
		if (v0.x>split_plane && v0.y<=p0.y) cull_y0 = false; // v0 is in right aabb and v0.y >= new lower y bound 
		if (v1.x>split_plane && v1.y<=p0.y) cull_y0 = false; // v1 is in right aabb and v1.y >= new lower y bound
		if (v2.x>split_plane && v2.y<=p0.y) cull_y0 = false; // v2 is in right aabb and v2.y >= new lower y bound
		if (cull_y0) right_bb.y0 = p0.y;

		if (v0.x<=split_plane && v0.y>=p1.y) cull_y1 = false; // v0 is in left aabb and v0.y <= new upper y bound 
		if (v1.x<=split_plane && v1.y>=p1.y) cull_y1 = false; // v1 is in left aabb and v1.y <= new upper y bound
		if (v2.x<=split_plane && v2.y>=p1.y) cull_y1 = false; // v2 is in left aabb and v2.y <= new upper y bound
		if (cull_y1) left_bb.y1 = p1.y;
		cull_y1 = true; // reset for the right aabb
		if (v0.x>split_plane && v0.y>=p1.y) cull_y1 = false; // v0 is in right aabb and v0.y <= new upper y bound 
		if (v1.x>split_plane && v1.y>=p1.y) cull_y1 = false; // v1 is in right aabb and v1.y <= new upper y bound
		if (v2.x>split_plane && v2.y>=p1.y) cull_y1 = false; // v2 is in right aabb and v2.y <= new upper y bound
		if (cull_y1) right_bb.y1 = p1.y;

		
		// p0 is the possibly lower bound for z, p1 is the possibly upper bound for z
		if (p0.z >p1.z) {
			Point3D temp = p0;
			p0 = p1;
			p1 = temp;
		}
		bool cull_z0 = true, cull_z1 = true;
		if (v0.x<=split_plane && v0.z<=p0.z) cull_z0 = false; // v0 is in left aabb and v0.y >= new lower y bound 
		if (v1.x<=split_plane && v1.z<=p0.z) cull_z0 = false; // v1 is in left aabb and v1.y >= new lower y bound
		if (v2.x<=split_plane && v2.z<=p0.z) cull_z0 = false; // v2 is in left aabb and v2.y >= new lower y bound
		if (cull_z0) left_bb.z0 = p0.z;
		cull_z0 = true; // reset for the right aabb
		if (v0.x>split_plane && v0.z<=p0.z) cull_z0 = false; // v0 is in right aabb and v0.y >= new lower y bound 
		if (v1.x>split_plane && v1.z<=p0.z) cull_z0 = false; // v1 is in right aabb and v1.y >= new lower y bound
		if (v2.x>split_plane && v2.z<=p0.z) cull_z0 = false; // v2 is in right aabb and v2.y >= new lower y bound
		if (cull_z0) right_bb.z0 = p0.z;

		if (v0.x<=split_plane && v0.z>=p1.z) cull_z1 = false; // v0 is in left aabb and v0.z <= new upper z bound 
		if (v1.x<=split_plane && v1.z>=p1.z) cull_z1 = false; // v1 is in left aabb and v1.z <= new upper z bound
		if (v2.x<=split_plane && v2.z>=p1.z) cull_z1 = false; // v2 is in left aabb and v2.z <= new upper z bound
		if (cull_z0) left_bb.z1 = p1.z;
		cull_z1 = true; // reset for the right aabb
		if (v0.x>split_plane && v0.z>=p1.z) cull_z1 = false; // v0 is in right aabb and v0.z <= new upper z bound 
		if (v1.x>split_plane && v1.z>=p1.z) cull_z1 = false; // v1 is in right aabb and v1.z <= new upper z bound
		if (v2.x>split_plane && v2.z>=p1.z) cull_z1 = false; // v2 is in right aabb and v2.z <= new upper z bound
		if (cull_z1) right_bb.z1 = p1.z;
	}
	else if (split_axis == Y_AXIS) {
		if (v0.y == split_plane) {
			p.push_back (cut_y (v1,v2,split_plane,t));
			p.push_back (v0);
		}
		else if (v1.y == split_plane) {
			p.push_back (cut_y (v0,v1,split_plane,t));
			p.push_back (v1);
		}
		else if (v2.y == split_plane) {
			p.push_back (cut_y (v0,v1,split_plane,t));
			p.push_back (v2);
		}
		else {
			p0 = cut_y (v0,v1,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
			p0 = cut_y (v1,v2,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
			p0 = cut_y (v0,v2,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
		}
		left_bb = bb;
		right_bb = bb;
		left_bb.y1 = split_plane;
		left_bb.y0 = split_plane;

		p0 = p[0];
		p1 = p[1];
		// p0 is the possibly lower bound for x, p1 is the possibly upper bound for x
		if (p0.x >p1.x) {
			Point3D temp = p0;
			p0 = p1;
			p1 = temp;
		}
		bool cull_x0 = true, cull_x1 = true;
		if (v0.y<=split_plane && v0.x<=p0.x) cull_x0 = false; // v0 is in left aabb and v0.y >= new lower y bound 
		if (v1.y<=split_plane && v1.x<=p0.x) cull_x0 = false; // v1 is in left aabb and v1.y >= new lower y bound
		if (v2.y<=split_plane && v2.x<=p0.x) cull_x0 = false; // v2 is in left aabb and v2.y >= new lower y bound
		if (cull_x0) left_bb.x0 = p0.x;
		cull_x0 = true; // reset for the right aabb
		if (v0.y>split_plane && v0.x<=p0.x) cull_x0 = false; // v0 is in right aabb and v0.y >= new lower y bound 
		if (v1.y>split_plane && v1.x<=p0.x) cull_x0 = false; // v1 is in right aabb and v1.y >= new lower y bound
		if (v2.y>split_plane && v2.x<=p0.x) cull_x0 = false; // v2 is in right aabb and v2.y >= new lower y bound
		if (cull_x0) right_bb.x0 = p0.x;

		if (v0.y<=split_plane && v0.x>=p1.x) cull_x1 = false; // v0 is in left aabb and v0.y <= new upper y bound 
		if (v1.y<=split_plane && v1.x>=p1.x) cull_x1 = false; // v1 is in left aabb and v1.y <= new upper y bound
		if (v2.y<=split_plane && v2.x>=p1.x) cull_x1 = false; // v2 is in left aabb and v2.y <= new upper y bound
		if (cull_x1) left_bb.x1 = p1.x;
		cull_x1 = true; // reset for the right aabb
		if (v0.y>split_plane && v0.x>=p1.x) cull_x1 = false; // v0 is in right aabb and v0.y <= new upper y bound 
		if (v1.y>split_plane && v1.x>=p1.x) cull_x1 = false; // v1 is in right aabb and v1.y <= new upper y bound
		if (v2.y>split_plane && v2.x>=p1.x) cull_x1 = false; // v2 is in right aabb and v2.y <= new upper y bound
		if (cull_x1) right_bb.x1 = p1.x;

		
		// p0 is the possibly lower bound for z, p1 is the possibly upper bound for z
		if (p0.z >p1.z) {
			Point3D temp = p0;
			p0 = p1;
			p1 = temp;
		}
		bool cull_z0 = true, cull_z1 = true;
		if (v0.y<=split_plane && v0.z<=p0.z) cull_z0 = false; // v0 is in left aabb and v0.y >= new lower y bound 
		if (v1.y<=split_plane && v1.z<=p0.z) cull_z0 = false; // v1 is in left aabb and v1.y >= new lower y bound
		if (v2.y<=split_plane && v2.z<=p0.z) cull_z0 = false; // v2 is in left aabb and v2.y >= new lower y bound
		if (cull_z0) left_bb.z0 = p0.z;
		cull_z0 = true; // reset for the right aabb
		if (v0.y>split_plane && v0.z<=p0.z) cull_z0 = false; // v0 is in right aabb and v0.y >= new lower y bound 
		if (v1.y>split_plane && v1.z<=p0.z) cull_z0 = false; // v1 is in right aabb and v1.y >= new lower y bound
		if (v2.y>split_plane && v2.z<=p0.z) cull_z0 = false; // v2 is in right aabb and v2.y >= new lower y bound
		if (cull_z0) right_bb.z0 = p0.z;

		if (v0.y<=split_plane && v0.z>=p1.z) cull_z1 = false; // v0 is in left aabb and v0.z <= new upper z bound 
		if (v1.y<=split_plane && v1.z>=p1.z) cull_z1 = false; // v1 is in left aabb and v1.z <= new upper z bound
		if (v2.y<=split_plane && v2.z>=p1.z) cull_z1 = false; // v2 is in left aabb and v2.z <= new upper z bound
		if (cull_z0) left_bb.z1 = p1.z;
		cull_z1 = true; // reset for the right aabb
		if (v0.y>split_plane && v0.z>=p1.z) cull_z1 = false; // v0 is in right aabb and v0.z <= new upper z bound 
		if (v1.y>split_plane && v1.z>=p1.z) cull_z1 = false; // v1 is in right aabb and v1.z <= new upper z bound
		if (v2.y>split_plane && v2.z>=p1.z) cull_z1 = false; // v2 is in right aabb and v2.z <= new upper z bound
		if (cull_z1) right_bb.z1 = p1.z;
	}
	else {
		if (v0.z == split_plane) {
			p.push_back (cut_z (v1,v2,split_plane,t));
			p.push_back (v0);
		}
		else if (v1.z == split_plane) {
			p.push_back (cut_z (v0,v1,split_plane,t));
			p.push_back (v1);
		}
		else if (v2.z == split_plane) {
			p.push_back (cut_z (v0,v1,split_plane,t));
			p.push_back (v2);
		}
		else {
			p0 = cut_z (v0,v1,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
			p0 = cut_z (v1,v2,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
			p0 = cut_z (v0,v2,split_plane,t);
			if (t > 0 && t<1) p.push_back (p0);
		}
		left_bb = bb;
		right_bb = bb;
		left_bb.y1 = split_plane;
		left_bb.y0 = split_plane;

		p0 = p[0];
		p1 = p[1];
		// p0 is the possibly lower bound for x, p1 is the possibly upper bound for x
		if (p0.x >p1.x) {
			Point3D temp = p0;
			p0 = p1;
			p1 = temp;
		}
		bool cull_x0 = true, cull_x1 = true;
		if (v0.z<=split_plane && v0.x<=p0.x) cull_x0 = false; // v0 is in left aabb and v0.y >= new lower y bound 
		if (v1.z<=split_plane && v1.x<=p0.x) cull_x0 = false; // v1 is in left aabb and v1.y >= new lower y bound
		if (v2.z<=split_plane && v2.x<=p0.x) cull_x0 = false; // v2 is in left aabb and v2.y >= new lower y bound
		if (cull_x0) left_bb.x0 = p0.x;
		cull_x0 = true; // reset for the right aabb
		if (v0.z>split_plane && v0.x<=p0.x) cull_x0 = false; // v0 is in right aabb and v0.y >= new lower y bound 
		if (v1.z>split_plane && v1.x<=p0.x) cull_x0 = false; // v1 is in right aabb and v1.y >= new lower y bound
		if (v2.z>split_plane && v2.x<=p0.x) cull_x0 = false; // v2 is in right aabb and v2.y >= new lower y bound
		if (cull_x0) right_bb.x0 = p0.x;

		if (v0.z<=split_plane && v0.x>=p1.x) cull_x1 = false; // v0 is in left aabb and v0.y <= new upper y bound 
		if (v1.z<=split_plane && v1.x>=p1.x) cull_x1 = false; // v1 is in left aabb and v1.y <= new upper y bound
		if (v2.z<=split_plane && v2.x>=p1.x) cull_x1 = false; // v2 is in left aabb and v2.y <= new upper y bound
		if (cull_x1) left_bb.x1 = p1.x;
		cull_x1 = true; // reset for the right aabb
		if (v0.z>split_plane && v0.x>=p1.x) cull_x1 = false; // v0 is in right aabb and v0.y <= new upper y bound 
		if (v1.z>split_plane && v1.x>=p1.x) cull_x1 = false; // v1 is in right aabb and v1.y <= new upper y bound
		if (v2.z>split_plane && v2.x>=p1.x) cull_x1 = false; // v2 is in right aabb and v2.y <= new upper y bound
		if (cull_x1) right_bb.x1 = p1.x;

		
		// p0 is the possibly lower bound for z, p1 is the possibly upper bound for z
		if (p0.z >p1.z) {
			Point3D temp = p0;
			p0 = p1;
			p1 = temp;
		}
		bool cull_y0 = true, cull_y1 = true;
		if (v0.z<=split_plane && v0.y<=p0.y) cull_y0 = false; // v0 is in left aabb and v0.y >= new lower y bound 
		if (v1.z<=split_plane && v1.y<=p0.y) cull_y0 = false; // v1 is in left aabb and v1.y >= new lower y bound
		if (v2.z<=split_plane && v2.y<=p0.y) cull_y0 = false; // v2 is in left aabb and v2.y >= new lower y bound
		if (cull_y0) left_bb.y0 = p0.y;
		cull_y0 = true; // reset for the right aabb
		if (v0.z>split_plane && v0.y<=p0.y) cull_y0 = false; // v0 is in right aabb and v0.y >= new lower y bound 
		if (v1.z>split_plane && v1.y<=p0.y) cull_y0 = false; // v1 is in right aabb and v1.y >= new lower y bound
		if (v2.z>split_plane && v2.y<=p0.y) cull_y0 = false; // v2 is in right aabb and v2.y >= new lower y bound
		if (cull_y0) right_bb.y0 = p0.y;

		if (v0.z<=split_plane && v0.y>=p1.z) cull_y1 = false; // v0 is in left aabb and v0.z <= new upper z bound 
		if (v1.z<=split_plane && v1.y>=p1.z) cull_y1 = false; // v1 is in left aabb and v1.z <= new upper z bound
		if (v2.z<=split_plane && v2.y>=p1.z) cull_y1 = false; // v2 is in left aabb and v2.z <= new upper z bound
		if (cull_y1) left_bb.y1 = p1.y;
		cull_y1 = true; // reset for the right aabb
		if (v0.z>split_plane && v0.y>=p1.y) cull_y1 = false; // v0 is in right aabb and v0.z <= new upper z bound 
		if (v1.z>split_plane && v1.y>=p1.y) cull_y1 = false; // v1 is in right aabb and v1.z <= new upper z bound
		if (v2.z>split_plane && v2.y>=p1.y) cull_y1 = false; // v2 is in right aabb and v2.z <= new upper z bound
		if (cull_y1) right_bb.y1 = p1.y;
	}
}
// intersection with a aabb using separate axis theorem
bool Triangle::sat_intersect (float x0, float y0, float z0, float x1, float y1, float z1) const { 
	float cx,cy,cz,hx,hy,hz;
	cx=(x0+x1)*0.5f;
	cy=(y0+y1)*0.5f;
	cz=(z0+z1)*0.5f;
	hx = (x1-x0)*0.5f;
	hy = (y1-y0)*0.5f;
	hz = (z1-z0)*0.5f;

	float v[3][3];
	// translate the triangle to origin
	v[0][0] = v0.x - cx;
	v[0][1] = v0.y - cy;
	v[0][2] = v0.z - cz;

	v[1][0] = v1.x - cx;
	v[1][1] = v1.y - cy;
	v[1][2] = v1.z - cz;

	v[2][0] = v2.x - cx;
	v[2][1] = v2.y - cy;
	v[2][2] = v2.z - cz;

	float pmin=1e10f,pmax=1e10f; // projection of the triangle
	// project onto the X-axis
	pmin = min (v[0][0],v[1][0]);
	pmin = min (pmin,v[2][0]);
	pmax = max (v[0][0],v[1][0]);
	pmax = max (pmax,v[2][0]);
	if (pmin>=hx || pmax<=-hx) return false;
	// project onto the Y-axis
	pmin = min (v[0][1],v[1][1]);
	pmin = min (pmin,v[2][1]);
	pmax = max (v[0][1],v[1][1]);
	pmax = max (pmax,v[2][1]);
	if (pmin>=hy || pmax<=-hy) return false;
	// project onto the Y-axis
	pmin = min (v[0][2],v[1][2]);
	pmin = min (pmin,v[2][2]);
	pmax = max (v[0][2],v[1][2]);
	pmax = max (pmax,v[2][2]);
	if (pmin>=hz || pmax<=-hz) return false;

	// TO-DO list
	// ....
}