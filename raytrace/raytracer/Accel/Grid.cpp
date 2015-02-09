// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the definition of the Grid class

#include <iostream>
#include <vector>
#include <cmath>

#include "Constants.h"

#include "Vector3D.h"
#include "Point3D.h"
#include "Grid.h"
#include "World.h"
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

long GetTickCount()
	//define a function to return the time in decimal seconds
	{
	timeval t;
	gettimeofday(&t,NULL);
	return t.tv_sec*1000000+t.tv_usec;
	}
	
	

// ----------------------------------------------------------------  default constructor

Grid::Grid(void)	
	: 	Compound(),
		nx(0),
		ny(0),
		nz(0)
{
	// The cells array will be empty
}


// ----------------------------------------------------------------  constructor
// for rendering triangle meshes

Grid::Grid(Mesh* _mesh_ptr):
	Compound(),
	nx(0),
	ny(0),
	nz(0)
{}

// ---------------------------------------------------------------- clone

Grid* 
Grid::clone(void) const {
	return (new Grid (*this));
}


// ---------------------------------------------------------------- copy constructor
// not implemented

Grid::Grid(const Grid& grid) {}


// ---------------------------------------------------------------- assignment operator
// not implemented

Grid& 
Grid::operator= (const Grid& rhs)	{
	return (*this);
}	



// ---------------------------------------------------------------- destructor
// not implemented

Grid::~Grid(void) {}				


BBox 
Grid::get_bounding_box(void) {
	return (bbox);
}

// construct grid with known resolution and list of objects
void Grid::init_grid (const int& _nx, const int& _ny, const int& _nz, const float& x0, const float& y0,
						const float& z0, const float& x1,const float& y1, const float& z1,const vector<GeometricObject*>& objects) {
	nx = _nx;
	ny = _ny;
	nz = _nz;
	bbox.x0 = x0;
	bbox.y0 = y0;
	bbox.z0 = z0;
	bbox.x1 = x1;
	bbox.y1 = y1;
	bbox.z1 = z1;
	

	// set up the array of grid cells 
	int num_cells = nx * ny * nz;	
	int num_objects = objects.size();
	cells = new vector<GeometricObject*>[num_cells];	
				
	BBox obj_bBox; 	// object's bounding box
	int index;  	// cell's array index
	for (int j = 0; j < num_objects; j++) {
		objects[j]->set_ray_id (-1);
		obj_bBox =  objects[j]->get_bounding_box();
						
		float rx = (float) nx/(x1-x0);
		float ry = (float) ny/(y1-y0);
		float rz = (float) nz/(z1-z0);

		// compute the cell indices at the corners of the bounding box of the object
		int ixmin = clamp(int ((obj_bBox.x0 - x0) * rx), 0, nx - 1);
		int iymin = clamp(int ((obj_bBox.y0 - y0) * ry), 0, ny - 1);
		int izmin = clamp(int ((obj_bBox.z0 - z0) * rz), 0, nz - 1);
		int ixmax = clamp(int ((obj_bBox.x1 - x0) * rx), 0, nx - 1);
		int iymax = clamp(int ((obj_bBox.y1 - y0) * ry), 0, ny - 1);
		int izmax = clamp(int ((obj_bBox.z1 - z0) * rz), 0, nz - 1);
				
		// add the object to the cells
				
		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) 			// cells in x direction
					cells[ix + nx * iy + nx * ny * iz].push_back (objects[j]);									

	}  
}

//------------------------------------------------------------------ setup_cells
void Grid::setup_cells(bool partition) {
}

void Grid::setup_cells (void) {
	long long  tStart = GetTickCount ();
	FILE* fout = fopen ("PLY_FILE/stats.txt","w");
	// find the minimum and maximum coordinates of the grid
	
	Point3D p0 = find_min_bounds();
	Point3D p1 = find_max_bounds();
	
	bbox.x0 = p0.x;
	bbox.y0 = p0.y;
	bbox.z0 = p0.z;
	bbox.x1 = p1.x;
	bbox.y1 = p1.y;
	bbox.z1 = p1.z;
		
	// compute the number of grid cells in the x, y, and z directions
	
	int num_objects = objects.size();
	
	// dimensions of the grid in the x, y, and z directions
	
	float wx = p1.x - p0.x;
	float wy = p1.y - p0.y;
	float wz = p1.z - p0.z;  
	
	float multiplier = 2;  	// multiplyer scales the number of grid cells relative to the number of objects
								
	float s = pow(wx * wy * wz / num_objects, 0.3333333f);    
	nx = int (multiplier * wx / s) + 1;
	ny = int (multiplier * wy / s) + 1;
	nz = int (multiplier * wz / s) + 1;

	// set up the array of grid cells 
	
	int num_cells = nx * ny * nz;	
	cells = new vector<GeometricObject*>[num_cells];	
				
	BBox obj_bBox; 	// object's bounding box
	int index;  	// cell's array index
			
	for (int j = 0; j < num_objects; j++) {
		obj_bBox =  objects[j]->get_bounding_box();
		objects[j]->set_ray_id (-1);
		// compute the cell indices at the corners of the bounding box of the object
		
		int ixmin = clamp(int ((obj_bBox.x0 - p0.x) * nx / (p1.x - p0.x)), 0, nx - 1);
		int iymin = clamp(int ((obj_bBox.y0 - p0.y) * ny / (p1.y - p0.y)), 0, ny - 1);
		int izmin = clamp(int ((obj_bBox.z0 - p0.z) * nz / (p1.z - p0.z)), 0, nz - 1);
		int ixmax = clamp(int ((obj_bBox.x1 - p0.x) * nx / (p1.x - p0.x)), 0, nx - 1);
		int iymax = clamp(int ((obj_bBox.y1 - p0.y) * ny / (p1.y - p0.y)), 0, ny - 1);
		int izmax = clamp(int ((obj_bBox.z1 - p0.z) * nz / (p1.z - p0.z)), 0, nz - 1);
				
		// add the object to the cells
		float dx = (p1.x-p0.x)/nx;
		float dy = (p1.y-p0.y)/ny;
		float dz = (p1.z-p0.z)/nz;
		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {
					float x0 = p0.x+ix*dx;
					float y0 = p0.y+iy*dy;
					float z0 = p0.z+iz*dz;
					float x1 = x0+dx;
					float y1 = y0+dy;
					float z1 = z0+dz;
					// only push the object into cell if object really intersect with cell's aabb
					if (objects[j]->sat_intersect (x0,y0,z0,x1,y1,z1))
						cells[ix + nx * iy + nx * ny * iz].push_back (objects[j]);									
				}

	}  
	
	
	// erase the Compound::vector that stores the object pointers, but don't delete the objects
	objects.clear ();
	fprintf (fout,"time to add objects to initial grid = %lld us \n",GetTickCount()-tStart);
}


//------------------------------------------------------------------ find_min_bounds

// find the minimum grid coordinates, based on the bounding boxes of all the objects

Point3D 
Grid::find_min_bounds(void) {
	BBox 	object_box;
	Point3D p0(kHugeValue);

	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		object_box = objects[j]->get_bounding_box();
				
		if (object_box.x0 < p0.x)
			p0.x = object_box.x0;
		if (object_box.y0 < p0.y)
			p0.y = object_box.y0;
		if (object_box.z0 < p0.z)
			p0.z = object_box.z0;
	}
	
	p0.x -= kEpsilon; p0.y -= kEpsilon; p0.z -= kEpsilon;
		
	return (p0);
}


//------------------------------------------------------------------ find_max_bounds

// find the maximum grid coordinates, based on the bounding boxes of the objects

Point3D Grid::find_max_bounds(void) {
	BBox object_box;
	Point3D p1(-kHugeValue);

	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		object_box = objects[j]->get_bounding_box();
				
		if (object_box.x1 > p1.x)
			p1.x = object_box.x1;
		if (object_box.y1 > p1.y)
			p1.y = object_box.y1;
		if (object_box.z1 > p1.z)
			p1.z = object_box.z1;
	}
	
	p1.x += kEpsilon; p1.y += kEpsilon; p1.z += kEpsilon;	
		
	return (p1);
}

bool Grid::cell_hit (int index, const Ray& ray, float& tmin, ShadeRec& sr) const {
	int num_objects = cells[index].size();
	float		t = 1e10f; 
	Normal		normal;
	Point3D		local_hit_point;
	bool		hit 		= false;
	normal = sr.normal;
	for (int j = 0; j < num_objects; j++)
		if (cells[index][j]->get_ray_id () < ray.id) {
			if (cells[index][j]->hit(ray, t, sr) && (t < tmin)) {
				hit				= true;
				tmin 			= t;
				normal = sr.normal;
				sr.hit_an_object = true;
				sr.material_ptr	= cells[index][j]->get_material();	// lhs is GeometricObject::material_ptr
				sr.obj_ptr = cells[index][j] ;
				sr.hit_point	= ray.o + ray.d*t;
				sr.hit_an_object = true;
				sr.t = t;
				//local_hit_point	= sr.local_hit_point;  
			}
			cells[index][j]->set_ray_id (ray.id);
		}
	
	sr.normal = normal;
	
	return (hit);
}



// ---------------------------------------------------------------- hit

// The following grid traversal code is based on the pseudo-code in Shirley (2000)	
// The first part is the same as the code in BBox::hit

bool Grid::hit(const Ray& ray, float& t, ShadeRec& sr) const {
	float ox = ray.o.x;
	float oy = ray.o.y;
	float oz = ray.o.z;
	float dx = ray.d.x;
	float dy = ray.d.y;
	float dz = ray.d.z;

	float x0 = bbox.x0;
	float y0 = bbox.y0;
	float z0 = bbox.z0;
	float x1 = bbox.x1;
	float y1 = bbox.y1;
	float z1 = bbox.z1;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max; 
	
	// the following code includes modifications from Shirley and Morley (2003)
	
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
	
	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;
		
	if (tz_min > t0)
		t0 = tz_min;
		
	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;
		
	if (tz_max < t1)
		t1 = tz_max;
			
	if (t0 > t1)
		return(false);
	
			
	// initial cell coordinates
	
	int ix, iy, iz;
	
	if (bbox.inside(ray.o)) {  			// does the ray start inside the grid?
		ix = clamp(int ((ox - x0) * nx / (x1 - x0)), 0, nx - 1);
		iy = clamp(int ((oy - y0) * ny / (y1 - y0)), 0, ny - 1);
		iz = clamp(int ((oz - z0) * nz / (z1 - z0)), 0, nz - 1);
	}
	else {
		Point3D p = ray.o + t0 * ray.d;  // initial hit point with grid's bounding box
		ix = clamp(int ((p.x - x0) * nx / (x1 - x0)), 0, nx - 1);
		iy = clamp(int ((p.y - y0) * ny / (y1 - y0)), 0, ny - 1);
		iz = clamp(int ((p.z - z0) * nz / (z1 - z0)), 0, nz - 1);
	}
	
	// ray parameter increments per cell in the x, y, and z directions
	
	float dtx = (tx_max - tx_min) / nx;
	float dty = (ty_max - ty_min) / ny;
	float dtz = (tz_max - tz_min) / nz;
		
	float 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;
	
	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}
	
	if (dx == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}
	
	
	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}
	
	if (dy == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}
		
	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}
	
	if (dz == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}
	
		
	// traverse the grid
	t = 1e10f;
	while (true) {	
		sr.w.grid_iteration_count++;
		int index = ix + nx*iy + nx*ny*iz;
		bool nonempty = cells[index].size()>0;
		
		if (tx_next < ty_next && tx_next < tz_next) {
			if (nonempty) {
				//t = tx_next;
				if (cell_hit (index,ray, t, sr) && t<tx_next) return true;
			}
			
			tx_next += dtx;
			ix += ix_step;
						
			if (ix == ix_stop)
				return (sr.hit_an_object);
		} 
		else { 	
			if (ty_next < tz_next) {
				if (nonempty) {
					//t = ty_next;
					if (cell_hit(index,ray, t, sr)&& t<ty_next) return true;
				}
				
				ty_next += dty;
				iy += iy_step;
								
				if (iy == iy_stop)
					return (sr.hit_an_object);
		 	}
		 	else {		
				if (nonempty) {
					//t = tz_next;
					if (cell_hit(index,ray, t, sr) && t<tz_next) return true;
				}
				tz_next += dtz;
				iz += iz_step;
								
				if (iz == iz_stop)
					return (sr.hit_an_object);
		 	}
		}
	}
}	// end of hit

bool Grid::cell_shadow_hit (int index, const Ray& ray, float& tmin,const GeometricObject* obj_ptr) const {
	int num_objects = cells[index].size();
	float tt = 1e10f;
	bool hit = false;
	for (int j = 0; j < num_objects; j++)
		if (cells[index][j]!=obj_ptr && cells[index][j]->shadow_hit(ray, tt) && tt<tmin) {
			tmin = tt;
			hit = true;
		}
	
	return hit;
}

bool Grid::shadow_hit(const Ray& ray, float& t,const GeometricObject* obj_ptr) const {
	float ox = ray.o.x;
	float oy = ray.o.y;
	float oz = ray.o.z;
	float dx = ray.d.x;
	float dy = ray.d.y;
	float dz = ray.d.z;

	float x0 = bbox.x0;
	float y0 = bbox.y0;
	float z0 = bbox.z0;
	float x1 = bbox.x1;
	float y1 = bbox.y1;
	float z1 = bbox.z1;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max; 
	
	// the following code includes modifications from Shirley and Morley (2003)
	
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
	
	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;
		
	if (tz_min > t0)
		t0 = tz_min;
		
	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;
		
	if (tz_max < t1)
		t1 = tz_max;
			
	if (t0 > t1)
		return(false);
	
			
	// initial cell coordinates
	
	int ix, iy, iz;
	
	if (bbox.inside(ray.o)) {  			// does the ray start inside the grid?
		ix = clamp(int ((ox - x0) * nx / (x1 - x0)), 0, nx - 1);
		iy = clamp(int ((oy - y0) * ny / (y1 - y0)), 0, ny - 1);
		iz = clamp(int ((oz - z0) * nz / (z1 - z0)), 0, nz - 1);
	}
	else {
		Point3D p = ray.o + t0 * ray.d;  // initial hit point with grid's bounding box
		ix = clamp(int ((p.x - x0) * nx / (x1 - x0)), 0, nx - 1);
		iy = clamp(int ((p.y - y0) * ny / (y1 - y0)), 0, ny - 1);
		iz = clamp(int ((p.z - z0) * nz / (z1 - z0)), 0, nz - 1);
	}
	
	// ray parameter increments per cell in the x, y, and z directions
	
	float dtx = (tx_max - tx_min) / nx;
	float dty = (ty_max - ty_min) / ny;
	float dtz = (tz_max - tz_min) / nz;
		
	float 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;
	
	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}
	
	if (dx == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}
	
	
	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}
	
	if (dy == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}
		
	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}
	
	if (dz == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}
	
		
	// traverse the grid
	t = 1e10f;
	while (true) {	
		int index = ix + nx*iy + nx*ny*iz;
		bool nonempty = cells[index].size()>0;
		
		if (tx_next < ty_next && tx_next < tz_next) {
			if (nonempty) {
				t = tx_next;
				if (cell_shadow_hit (index,ray, t,obj_ptr))
					return (true);
			}
			tx_next += dtx;
			ix += ix_step;
						
			if (ix == ix_stop)
				return (false);
		} 
		else { 	
			if (ty_next < tz_next) {
				if (nonempty) {
					t = ty_next;
					if (cell_shadow_hit(index,ray, t,obj_ptr)) 
						return (true);
				}
				
				ty_next += dty;
				iy += iy_step;
								
				if (iy == iy_stop)
					return (false);
		 	}
		 	else {		
				
				if (nonempty) {
					t = tz_next;
					if (cell_shadow_hit(index,ray, t,obj_ptr)) 
						return (true);
				}
				tz_next += dtz;
				iz += iz_step;
								
				if (iz == iz_stop)
					return (false);
		 	}
		}
	}
}	// end of shadow hit
