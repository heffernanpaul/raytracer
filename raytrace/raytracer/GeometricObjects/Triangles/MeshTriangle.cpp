// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// the copy constructor and assignment operator do not clone the mesh

#include "Constants.h"
#include "MeshTriangle.h"
#include "Maths.h"
	

// ----------------------------------------------------------------  default constructor

MeshTriangle::MeshTriangle(void)
	: 	GeometricObject(),
		mesh_ptr(NULL),
		index0(0), index1(0), index2(0),
		normal()
{}


// ---------------------------------------------------------------- constructor
// the normal is computed in Grid::read_ply_file

MeshTriangle::MeshTriangle(Mesh* _mesh_ptr, const int i0, const int i1, const int i2)
	: 	GeometricObject(),
		mesh_ptr(_mesh_ptr),
		index0(i0), index1(i1), index2(i2) 
{
	compute_normal (false);
}


// ---------------------------------------------------------------- copy constructor

MeshTriangle::MeshTriangle (const MeshTriangle& mt)
	:	GeometricObject(mt),
		mesh_ptr(mt.mesh_ptr), // just the pointer
		index0(mt.index0), 
		index1(mt.index1), 
		index2(mt.index2),
		normal(mt.normal)
{}


// ---------------------------------------------------------------- assignment operator

MeshTriangle& 
MeshTriangle::operator= (const MeshTriangle& rhs) {
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);
	
	mesh_ptr 	= rhs.mesh_ptr; // just the pointer
	index0 		= rhs.index0;
	index1 		= rhs.index1;
	index2 		= rhs.index2;
	normal 		= rhs.normal;
	
	return (*this);
}


// ---------------------------------------------------------------- destructor

MeshTriangle::~MeshTriangle (void) {
}


// ---------------------------------------------------------------- compute_normal

void MeshTriangle::compute_normal(const bool reverse_normal) {
	normal = (mesh_ptr->vertices[index1] - mesh_ptr->vertices[index0]) ^
			 (mesh_ptr->vertices[index2] - mesh_ptr->vertices[index0]);
	normal.normalize();
	
	if (reverse_normal)
		normal = -normal;
}


// ---------------------------------------------------------------- get_normal
// this is called in Grid::compute_mesh_normals

Normal
MeshTriangle::get_normal() const {
	return (normal);
}	


//---------------------------------------------------------------- get_bounding_box

BBox
MeshTriangle::get_bounding_box(void) {	
	float delta = 0.0001f;  // to avoid degenerate bounding boxes
	
	Point3D v1(mesh_ptr->vertices[index0]);
	Point3D v2(mesh_ptr->vertices[index1]);
	Point3D v3(mesh_ptr->vertices[index2]);
	
	return(BBox(min(min(v1.x, v2.x), v3.x)-delta, max(max(v1.x, v2.x), v3.x) +delta , 
				min(min(v1.y, v2.y), v3.y)-delta, max(max(v1.y, v2.y), v3.y) +delta , 
				min(min(v1.z, v2.z), v3.z)-delta, max(max(v1.z, v2.z), v3.z) +delta));
}


// ------------------------------------------------------------------------------ shadow_hit
// this function is independent of the derived triangle type:
// flat, smooth, flat uv, smooth uv

bool 															 
MeshTriangle::shadow_hit(const Ray& ray, float& tmin) const {	
	Point3D v0(mesh_ptr->vertices[index0]);
	Point3D v1(mesh_ptr->vertices[index1]);
	Point3D v2(mesh_ptr->vertices[index2]);
	/*
	Vector3D u = v1-v0;
	Vector3D v = v2-v0;
	Vector3D n = u^v;

	Vector3D w0 = ray.o - v0;
	float a = -(n*w0);
	float b = n*ray.d;
	if (fabs(b) < kEpsilon) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return true;
        else return false;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    float r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return false;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    Vector3D I = ray.o + r * ray.d;           // intersect point of ray and plane

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u*u;
    uv = u*v;
    vv = v*v;
    Vector3D w = I - v0;
    wu = w*u;
    wv = w*v;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return false;
	tmin = r;
	return (tmin > kEpsilon);                      // I is in T
	*/
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


// ---------------------------------------------------------------- interpolate_u
// this is used for texture mapping in Chapter 29

float 
MeshTriangle::interpolate_u(const float beta, const float gamma) const {	
	return( (1 - beta - gamma) * mesh_ptr->u[index0] 
				+ beta * mesh_ptr->u[index1] 
					+ gamma * mesh_ptr->u[index2] );
}


// ---------------------------------------------------------------- interpolate_v
// this is used for texture mapping in Chapter 29

float 
MeshTriangle::interpolate_v(const float beta, const float gamma) const {	
	return( (1 - beta - gamma) * mesh_ptr->v[index0] 
				+ beta * mesh_ptr->v[index1] 
					+ gamma * mesh_ptr->v[index2] );
}



void MeshTriangle::clipping_bbox (const BBox& bb, float split_plane, int split_axis, BBox& left_bb, BBox& right_bb) {
	vector<Point3D> p;
	Point3D p0,p1;
	float t;
	Point3D v0 = mesh_ptr->vertices[index0];
	Point3D v1 = mesh_ptr->vertices[index1];
	Point3D v2 = mesh_ptr->vertices[index2];

	if (split_axis == X_AXIS) {
		if (v0.x == split_plane) {
			p.push_back (cut_x (v1,v2,split_plane,t));
			p.push_back (v0);
		}
		else if (v1.x == split_plane) {
			p.push_back (cut_x (v0,v2,split_plane,t));
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
		right_bb.x0 = split_plane;

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
			p.push_back (cut_y (v0,v2,split_plane,t));
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
		right_bb.y0 = split_plane;

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
			p.push_back (cut_z (v0,v2,split_plane,t));
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
		left_bb.z1 = split_plane;
		right_bb.z0 = split_plane;

		p0 = p[0];
		p1 = p[1];
		// p0 is the possibly lower bound for x, p1 is the possibly upper bound for x
		if (p0.x >p1.x) {
			Point3D temp = p0;
			p0 = p1;
			p1 = temp;
		}
		bool cull_x0 = true, cull_x1 = true;
		if (v0.z<=split_plane && v0.x<=p0.x) cull_x0 = false; // v0 is in left aabb and v0.x >= new lower x bound 
		if (v1.z<=split_plane && v1.x<=p0.x) cull_x0 = false; // v1 is in left aabb and v1.x >= new lower x bound
		if (v2.z<=split_plane && v2.x<=p0.x) cull_x0 = false; // v2 is in left aabb and v2.x >= new lower x bound
		if (cull_x0) left_bb.x0 = p0.x;
		cull_x0 = true; // reset for the right aabb
		if (v0.z>split_plane && v0.x<=p0.x) cull_x0 = false; // v0 is in right aabb and v0.x >= new lower x bound 
		if (v1.z>split_plane && v1.x<=p0.x) cull_x0 = false; // v1 is in right aabb and v1.x >= new lower x bound
		if (v2.z>split_plane && v2.x<=p0.x) cull_x0 = false; // v2 is in right aabb and v2.x >= new lower x bound
		if (cull_x0) right_bb.x0 = p0.x;

		if (v0.z<=split_plane && v0.x>=p1.x) cull_x1 = false; // v0 is in left aabb and v0.x <= new upper x bound 
		if (v1.z<=split_plane && v1.x>=p1.x) cull_x1 = false; // v1 is in left aabb and v1.x <= new upper x bound
		if (v2.z<=split_plane && v2.x>=p1.x) cull_x1 = false; // v2 is in left aabb and v2.x <= new upper x bound
		if (cull_x1) left_bb.x1 = p1.x;
		cull_x1 = true; // reset for the right aabb
		if (v0.z>split_plane && v0.x>=p1.x) cull_x1 = false; // v0 is in right aabb and v0.x <= new upper x bound 
		if (v1.z>split_plane && v1.x>=p1.x) cull_x1 = false; // v1 is in right aabb and v1.x <= new upper x bound
		if (v2.z>split_plane && v2.x>=p1.x) cull_x1 = false; // v2 is in right aabb and v2.x <= new upper x bound
		if (cull_x1) right_bb.x1 = p1.x;

		
		// p0 is the possibly lower bound for y, p1 is the possibly upper bound for y
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

		if (v0.z<=split_plane && v0.y>=p1.y) cull_y1 = false; // v0 is in left aabb and v0.y <= new upper y bound 
		if (v1.z<=split_plane && v1.y>=p1.y) cull_y1 = false; // v1 is in left aabb and v1.y <= new upper y bound
		if (v2.z<=split_plane && v2.y>=p1.y) cull_y1 = false; // v2 is in left aabb and v2.y <= new upper y bound
		if (cull_y1) left_bb.y1 = p1.y;
		cull_y1 = true; // reset for the right aabb
		if (v0.z>split_plane && v0.y>=p1.y) cull_y1 = false; // v0 is in right aabb and v0.y <= new upper y bound 
		if (v1.z>split_plane && v1.y>=p1.y) cull_y1 = false; // v1 is in right aabb and v1.y <= new upper y bound
		if (v2.z>split_plane && v2.y>=p1.y) cull_y1 = false; // v2 is in right aabb and v2.y <= new upper y bound
		if (cull_y1) right_bb.y1 = p1.y;
	}
}

bool MeshTriangle::sat_intersect (float x0, float y0, float z0, float x1, float y1, float z1) const { // with a aabb
	float cx,cy,cz,hx,hy,hz;
	cx=(x0+x1)*0.5f;
	cy=(y0+y1)*0.5f;
	cz=(z0+z1)*0.5f;
	hx = (x1-x0)*0.5f;
	hy = (y1-y0)*0.5f;
	hz = (z1-z0)*0.5f;

	float v[3][3];
	// translate the triangle to origin
	v[0][0] = mesh_ptr->vertices[index0].x - cx;
	v[0][1] = mesh_ptr->vertices[index0].y - cy;
	v[0][2] = mesh_ptr->vertices[index0].z - cz;

	v[1][0] = mesh_ptr->vertices[index1].x - cx;
	v[1][1] = mesh_ptr->vertices[index1].y - cy;
	v[1][2] = mesh_ptr->vertices[index1].z - cz;

	v[2][0] = mesh_ptr->vertices[index2].x - cx;
	v[2][1] = mesh_ptr->vertices[index2].y - cy;
	v[2][2] = mesh_ptr->vertices[index2].z - cz;

	float pmin=1e10f,pmax=1e10f; // projection of the triangle
	// project onto the X-axis
	pmin = min (min (v[0][0],v[1][0]),v[2][0]);
	pmax = max (max (v[0][0],v[1][0]),v[2][0]);
	if (pmin>=hx || pmax<=-hx) return false;
	// project onto the Y-axis
	pmin = min (min (v[0][1],v[1][1]),v[2][1]);
	pmax = max (max (v[0][1],v[1][1]),v[2][1]);
	if (pmin>=hy || pmax<=-hy) return false;
	// project onto the Y-axis
	pmin = min (min (v[0][2],v[1][2]),v[2][2]);
	pmax = max (max (v[0][2],v[1][2]),v[2][2]);
	if (pmin>=hz || pmax<=-hz) return false;

	// project onto the triangle's normal
	Vector3D a(v[1][0]-v[0][0],v[1][1]-v[0][1],v[1][2]-v[0][2]);
	Vector3D b(v[2][0]-v[0][0],v[2][1]-v[0][1],v[2][2]-v[0][2]);
	Vector3D n = a^b;
	float p = hx*fabs(n.x) + hy*fabs(n.y) + hz*fabs(n.z);
	float s = -(n.x*v[0][0]+n.y*v[0][1]+n.z*v[0][2]);
	if (s-p>=0 || s+p<=0) return false;

	Vector3D e[3] = {Vector3D(1,0,0),Vector3D(0,1,0),Vector3D(0,0,1)};
	Vector3D f[3];
	f[0].x = v[1][0]-v[0][0];
	f[0].y = v[1][1]-v[0][1];
	f[0].z = v[1][2]-v[0][2];

	f[1].x = v[2][0]-v[1][0];
	f[1].y = v[2][1]-v[1][1];
	f[1].z = v[2][2]-v[1][2];

	f[2].x = v[0][0]-v[2][0];
	f[2].y = v[0][1]-v[2][1];
	f[2].z = v[0][2]-v[2][2];


	for (int i=0;i<3;++i)
		for (int j=0;j<3;++j) {
			Vector3D a = e[i]^f[j];
			float p0 = a.x*v[0][0] + a.y*v[0][1] + a.z*v[0][2];
			float p1 = a.x*v[1][0] + a.y*v[1][1] + a.z*v[1][2];
			float p2 = a.x*v[2][0] + a.y*v[2][1] + a.z*v[2][2];
			pmin = min (min (p0,p1),p2);
			pmax = max (max (p0,p1),p2);
			float r = hx*fabs(a.x) + hy*fabs(a.y) + hz*fabs(a.z);
			if (pmin>r || pmax<-r) return false;
		}
	return true;
}

void MeshTriangle::intersectP (float x0, float y0, float z0, float x1, float y1, float z1, vector<Point3D>& p) {
	Point3D v[3];
	
	float eps = 1e-4f;
	x0-=eps;
	y0-=eps;
	z0-=eps;
	x1+=eps;
	y1+=eps;
	z1+=eps;
	
	v[0].x = mesh_ptr->vertices[index0].x;
	v[0].y = mesh_ptr->vertices[index0].y;
	v[0].z = mesh_ptr->vertices[index0].z;

	v[1].x = mesh_ptr->vertices[index1].x;
	v[1].y = mesh_ptr->vertices[index1].y;
	v[1].z = mesh_ptr->vertices[index1].z;

	v[2].x = mesh_ptr->vertices[index2].x; 
	v[2].y = mesh_ptr->vertices[index2].y; 
	v[2].z = mesh_ptr->vertices[index2].z; 

	for (int i=0;i<3;++i) {
		// the vertex is inside the aabb
		if (v[i].x>=x0&&v[i].y>=y0&&v[i].z>=z0&&v[i].x<=x1&&v[i].y<=y1&&v[i].z<=z1) 
			p.push_back (Point3D(v[i]));
		int j=(i+1)%3;
		
		
		
		float dx,dy,dz;
		dx = v[j].x - v[i].x;
		dy = v[j].y - v[i].y;
		dz = v[j].z - v[i].z;
		
		if (fabs (dx)>1e-4f) {
			float t0,t1,y,z;
			t0 = (x0-v[i].x)/dx;
			if (t0>=0&&t0<=1) {
				y = v[i].y+t0*dy;
				z = v[i].z+t0*dz;
				if (y>=y0 && y<=y1 && z>=z0 && z<=z1) 
					p.push_back (Point3D (x0,y,z));
			}
			t1 = (x1-v[i].x)/dx;
			if (t1>=0&&t1<=1) {
				y = v[i].y+t1*dy;
				z = v[i].z+t1*dz;
				if (y>=y0 && y<=y1 && z>=z0 && z<=z1) 
					p.push_back (Point3D (x1,y,z));
			}
			
		}
		if (fabs(dy)>1e-4f) {
			float t0,t1,x,z;
			t0 = (y0-v[i].y)/dy;
			if (t0>=0&&t0<=1) {
				x = v[i].x+t0*dx;
				z = v[i].z+t0*dz;
				if (x>=x0 && x<=x1 && z>=z0 && z<=z1) 
					p.push_back (Point3D (x,y0,z));
			}
			t1 = (y1-v[i].y)/dy;
			if (t1>=0&&t1<=1) {
				x = v[i].x+t1*dx;
				z = v[i].z+t1*dz;
				if (x>=x0 && x<=x1 && z>=z0 && z<=z1) 
					p.push_back (Point3D (x,y1,z));
			}
			
		}
		if (fabs(dz)>1e-4f) {
			float t0,t1,x,y;
			t0 = (z0-v[i].z)/dz;
			if (t0>=0&&t0<=1) {
				x = v[i].x+t0*dx;
				y = v[i].y+t0*dy;
				if (x>=x0 && x<=x1 && y>=y0 && y<=y1) 
					p.push_back (Point3D (x,y,z0));
			}
			t1 = (z1-v[i].z)/dz;
			if (t1>=0&&t1<=1) {
				x = v[i].x+t1*dx;
				y = v[i].y+t1*dy;
				if (x>=x0 && x<=x1 && y>=y0 && y<=y1) 
					p.push_back (Point3D (x,y,z1));
			}
			
		}
		
	}
	// now check for intersection between aabb's edges and triangle
	Vector3D n = (v[2]-v[0])^(v[1]-v[0]);
	float D = -(n*v[0]);
	Ray e[12];
	e[0].o = Point3D (x0,y0,z0); e[0].d = Vector3D (0,y1-y0,0);
	e[1].o = Point3D (x1,y0,z0); e[1].d = Vector3D (0,y1-y0,0);
	e[2].o = Point3D (x0,y0,z1); e[2].d = Vector3D (0,y1-y0,0);
	e[3].o = Point3D (x1,y0,z1); e[3].d = Vector3D (0,y1-y0,0);

	e[4].o = Point3D (x0,y0,z0); e[4].d = Vector3D (x1-x0,0,0);
	e[5].o = Point3D (x1,y0,z0); e[5].d = Vector3D (0,0,z1-z0);
	e[6].o = Point3D (x1,y0,z1); e[6].d = Vector3D (x0-x1,0,0);
	e[7].o = Point3D (x0,y0,z1); e[7].d = Vector3D (0,0,z0-z1);

	e[8].o = Point3D (x0,y1,z0); e[8].d = Vector3D (x1-x0,0,0);
	e[9].o = Point3D (x1,y1,z0); e[9].d = Vector3D (0,0,z1-z0);
	e[10].o = Point3D (x1,y1,z1); e[10].d = Vector3D (x0-x1,0,0);
	e[11].o = Point3D (x0,y1,z1); e[11].d = Vector3D (0,0,z0-z1);
	
	for (int i=0;i<12;++i) {
		float t = (-D-n*e[i].o)/(n*e[i].d);
		if (t>=0 && t<=1) {
			Point3D P = e[i].o + t*e[i].d;
			Vector3D v0 = v[1]-v[0];
			Vector3D v1 = v[2]-v[0];
			Vector3D v2 = P - v[0];

			// Compute dot products
			float dot00,dot01,dot02,dot11,dot12;
			dot00 = v0*v0;
			dot01 = v0*v1;
			dot02 = v0*v2;
			dot11 = v1*v1;
			dot12 = v1*v2;

			// Compute barycentric coordinates
			float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
			float alpha = (dot11 * dot02 - dot01 * dot12) * invDenom;
			float beta = (dot00 * dot12 - dot01 * dot02) * invDenom;

			// Check if point is in triangle
			if ((alpha > 0) && (beta > 0) && (alpha + beta< 1)) 
				p.push_back (P);

		}
	}
}
