// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Constants.h"
#include "FlatMeshTriangle.h"
#include "World.h"						

// ----------------------------------------------------------------  default constructor

FlatMeshTriangle::FlatMeshTriangle(void)
	: 	MeshTriangle()
{}


// ---------------------------------------------------------------- constructor

FlatMeshTriangle::FlatMeshTriangle (Mesh* _mesh_ptr, const int i0, const int i1, const int i2)
	: 	MeshTriangle(_mesh_ptr, i0, i1, i2)
{}


// ---------------------------------------------------------------- clone

FlatMeshTriangle* 
FlatMeshTriangle::clone(void) const {
	return (new FlatMeshTriangle(*this));
}


// ---------------------------------------------------------------- copy constructor

FlatMeshTriangle::FlatMeshTriangle(const FlatMeshTriangle& fmt)
	:	MeshTriangle(fmt)
{}


// ---------------------------------------------------------------- assignment operator

FlatMeshTriangle& 
FlatMeshTriangle::operator= (const FlatMeshTriangle& rhs) {
	if (this == &rhs)
		return (*this);

	MeshTriangle::operator= (rhs);
	
	return (*this);
}


// ---------------------------------------------------------------- destructor

FlatMeshTriangle::~FlatMeshTriangle(void) {}


// ---------------------------------------------------------------- hit
bool  FlatMeshTriangle::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {   
	
	sr.w.triangle_intersection_count++;
	Point3D v0(mesh_ptr->vertices[index0]);
    Point3D v1(mesh_ptr->vertices[index1]);
    Point3D v2(mesh_ptr->vertices[index2]);
	
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

	
	/*
	Vector3D u = v1-v0;
	Vector3D v = v2-v0;
	Vector3D n = u^v;

	Vector3D w0 = ray.o - v0;
	float a = -(n*w0);
	float b = n*ray.d;
	if (fabs(b) < kEpsilon) {     // ray is parallel to triangle plane
        return false;
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
	//if (normal*(-ray.d) > 0) sr.normal      = normal;                               // for flat shading
    //else sr.normal = -normal; // reverse normal
    sr.local_hit_point      = ray.o + t * ray.d;    
    return (true);  
	*/
}