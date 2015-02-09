#include <iostream>
#include <vector>
#include <cmath>
#include "Material.h"
#include "Constants.h"

#include "Vector3D.h"
#include "Point3D.h"
#include "NestedGrid.h"
#include "World.h"
#include "Box.h"

// ----------------------------------------------------------------  default constructor

NestedGrid::NestedGrid(void)	
	: 	Grid ()
{
	// The cells array will be empty
}


// ---------------------------------------------------------------- clone

NestedGrid* 
NestedGrid::clone(void) const {
	return (new NestedGrid (*this));
}


// ---------------------------------------------------------------- copy constructor
// not implemented

NestedGrid::NestedGrid(const NestedGrid& grid) {}


// ---------------------------------------------------------------- assignment operator
// not implemented

NestedGrid& 
NestedGrid::operator= (const NestedGrid& rhs)	{
	return (*this);
}	



// ---------------------------------------------------------------- destructor
// not implemented

NestedGrid::~NestedGrid(void) {}				



void NestedGrid::partition_grid (int x0, int y0, int z0, int x1, int y1, int z1) {
	long long c1 = count_x[x0][y1][z1]-(y0>0?count_x[x0][y0-1][z1]:0)-(z0>0?count_x[x0][y1][z0-1]:0)
					 +(y0>0&&z0>0?count_x[x0][y0-1][z0-1]:0);
	long long c2 = x1>x0?count_x[x1][y1][z1]-(y0>0?count_x[x1][y0-1][z1]:0)-(z0>0?count_x[x1][y1][z0-1]:0)
					 +(y0>0&&z0>0?count_x[x1][y0-1][z0-1]:0):0;
	long long c3 = x1>x0+1?count_y[x1-1][y0][z1]-(count_y[x0][y0][z1])-(z0>0?count_y[x1-1][y0][z0-1]:0)
					 + (z0>0?count_y[x0][y0][z0-1]:0):0;
	long long c4 = (x1>x0+1&&y1>y0)?count_y[x1-1][y1][z1]-(count_y[x0][y1][z1])-(z0>0?count_y[x1-1][y1][z0-1]:0)
					 + (z0>0?count_y[x0][y1][z0-1]:0):0;
	long long c5 = (x1>x0+1&&y1>y0+1)?count_z[x1-1][y1-1][z0]-(count_z[x0][y1-1][z0])-(count_z[x1-1][y0][z0])
					 + (count_z[x0][y0][z0]):0;
	long long c6 = (x1>x0+1&&y1>y0+1&&z1>z0)?count_z[x1-1][y1-1][z1]-(count_z[x0][y1-1][z1])-(count_z[x1-1][y0][z1])
						 + (count_z[x0][y0][z1]):0;
	long long count_boundary = c1+c2+c3+c4+c5+c6;						 
	long long boundary = (x1-x0+1)*(y1-y0+1)*(z1-z0+1) - (x1>x0+1?x1-x0-1:0)*(y1>y0+1?y1-y0-1:0)*(z1>z0+1?z1-z0-1:0);
	if (count_boundary*10 >= boundary*9) {
		partition_aabb.push_back (x0);
		partition_aabb.push_back (y0);
		partition_aabb.push_back (z0);
		partition_aabb.push_back (x1);
		partition_aabb.push_back (y1);
		partition_aabb.push_back (z1);
	}
	else {
		long long size_xy = (x1-x0+1)*(y1-y0+1);
		long long size_yz = (z1-z0+1)*(y1-y0+1);
		long long size_xz = (x1-x0+1)*(z1-z0+1);
		long long count_left, count_right;
		float c_max = 0, temp;
		long long split_axis, split_plane;
		
		count_left = count_x[x0][y1][z1]-(y0>0?count_x[x0][y0-1][z1]:0)-(z0>0?count_x[x0][y1][z0-1]:0)
						+(y0>0&&z0>0?count_x[x0][y0-1][z0-1]:0);
		long long count_aabb = count_left; // number of non-empty cells in aabb (x0,y0,z0,x1,y1,z1)
		for (int x=x0+1;x<=x1;++x) {
			count_right = count_x[x][y1][z1]-(y0>0?count_x[x][y0-1][z1]:0)-(z0>0?count_x[x][y1][z0-1]:0)
						   +(y0>0&&z0>0?count_x[x][y0-1][z0-1]:0);
			temp = max ((float)(size_yz-count_left)*count_right,(float)count_left*(size_yz-count_right))/((float)size_yz*size_yz);
			if ( temp > c_max) {
				c_max = temp;
				split_axis = X_AXIS;
				split_plane = x;
			}
			count_left = count_right;
			count_aabb+=count_right;
		}
		if (count_aabb == 0) return; // if aabb contains no non-empty cell, quit
		count_left =  count_y[x1][y0][z1]-(x0>0?count_y[x0-1][y0][z1]:0)-(z0>0?count_y[x1][y0][z0-1]:0)
						 + (x0>0&&z0>0?count_y[x0-1][y0][z0-1]:0);
		for (int y=y0+1;y<=y1;++y) {
			count_right =  count_y[x1][y][z1]-(x0>0?count_y[x0-1][y][z1]:0)-(z0>0?count_y[x1][y][z0-1]:0)
						 + (x0>0&&z0>0?count_y[x0-1][y][z0-1]:0);
			temp = max ((float)(size_xz-count_left)*count_right,(float)count_left*(size_xz-count_right))/((float)size_xz*size_xz);
			if (temp > c_max) {
				c_max = temp;
				split_axis = Y_AXIS;
				split_plane = y;
			}
			count_left = count_right;
		}
		count_left =  count_z[x1][y1][z0]-(x0>0?count_z[x0-1][y1][z0]:0)-(y0>0?count_z[x1][y0-1][z0]:0)
						 + (x0>0&&y0>0?count_z[x0-1][y0-1][z0]:0);
		for (int z=z0+1;z<=z1;++z) {
			count_right = count_z[x1][y1][z]-(x0>0?count_z[x0-1][y1][z]:0)-(y0>0?count_z[x1][y0-1][z]:0)
						 + (x0>0&&y0>0?count_z[x0-1][y0-1][z]:0);
			temp = max ((float)(size_xy-count_left)*count_right,(float)count_left*(size_xy-count_right))/((float)size_xy*size_xy);
			if (temp > c_max) {
				c_max = temp;
				split_axis = Z_AXIS;
				split_plane = z;
			}
		}

		if (split_axis == X_AXIS) {
			if (split_plane > x0) partition_grid (x0,y0,z0,split_plane-1,y1,z1);
			partition_grid (split_plane,y0,z0,x1,y1,z1);
		}
		else if (split_axis == Y_AXIS) {
			if (split_plane > y0) partition_grid (x0,y0,z0,x1,split_plane-1,z1);
			partition_grid (x0,split_plane,z0,x1,y1,z1);
		}
		else {
			if (split_plane > z0) partition_grid (x0,y0,z0,x1,y1,split_plane-1);
			partition_grid (x0,y0,split_plane,x1,y1,z1);
		}
	}
}
//------------------------------------------------------------------ setup_cells
void
NestedGrid::setup_cells() {
	int tStart = GetTickCount ();
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
	
	float multiplier =2.0;  	// multiplier scales the number of grid cells relative to the number of objects
								
	float s = pow(wx * wy * wz / num_objects, 0.3333333f);    
	/*
	nx = int (multiplier * wx / s) + 1;
	ny = int (multiplier * wy / s) + 1;
	nz = int (multiplier * wz / s) + 1;
	*/
	nx = ny = nz = multiplier * int (pow ((float) num_objects,0.33f)) + 1;
	

	fprintf (fout,"%.2f %.2f %.2f %.2f %d %d %d\n",wx,wy,wz,s,nx,ny,nz);
	multiplier = 2.0; // scale back to 2.0 for inner and outer grid
	int num_cells = nx * ny * nz;	
	cells = new vector<GeometricObject*>[num_cells];
	
	BBox obj_bBox; 	// object's bounding box
	int index;  	// cell's array index

	// count_x,count_y,count_z store the number of non-empty cells
	count_x = new long long**[nx];
	count_y = new long long**[nx];
	count_z = new long long**[nx];
	for (int i=0;i<nx;++i) {
		count_x[i] = new long long*[ny];
		count_y[i] = new long long*[ny];
		count_z[i] = new long long*[ny];
		for (int j=0;j<ny;++j) {
			count_x[i][j] = new long long[nz];
			count_y[i][j] = new long long[nz];
			count_z[i][j] = new long long[nz];
			memset (count_x[i][j], 0, nz*sizeof (long long));
			memset (count_y[i][j], 0, nz*sizeof (long long));
			memset (count_z[i][j], 0, nz*sizeof (long long));
		}
	}

	vector<int>* cell_obj_id = new vector<int>[nx*ny*nz];
	for (int j = 0; j < num_objects; j++) {
		obj_bBox =  objects[j]->get_bounding_box();
				
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
				for (int ix = ixmin; ix <= ixmax; ix++) { 			// cells in x direction
					int index = ix + nx * iy + nx * ny * iz; 
					float x0 = p0.x+ix*dx;
					float y0 = p0.y+iy*dy;
					float z0 = p0.z+iz*dz;
					float x1 = x0+dx;
					float y1 = y0+dy;
					float z1 = z0+dz;
					// only push the object into cell if object really intersect with cell's aabb
					if (objects[j]->sat_intersect (x0,y0,z0,x1,y1,z1)) {
						cells[index].push_back (objects[j]);				
						cell_obj_id[index].push_back (j);
					}
				}

	}  // end of for (int j = 0; j < num_objects; j++)

	fprintf (fout,"time to add objects to initial grid = %d ms \n",GetTickCount()-tStart);
	tStart = GetTickCount ();

	// count the number of non-empty cells for each x-cut
	#pragma omp parallel for
	for (int x=0;x<nx;++x) {
		count_x[x][0][0] = (cells[x].size()>0);
		for (int y=1;y<ny;++y)
			count_x[x][y][0]+=count_x[x][y-1][0] + (cells[x+nx*y].size()>0);
		for (int z=1;z<nz;++z)
			count_x[x][0][z]+=count_x[x][0][z-1] + (cells[x+nx*ny*z].size()>0);
		for (int y=1;y<ny;++y)
			for (int z=1;z<nz;++z)
				count_x[x][y][z]+=count_x[x][y-1][z]+count_x[x][y][z-1]-count_x[x][y-1][z-1] + (cells[x+nx*y+nx*ny*z].size()>0);
	}
	// count the number of non-empty cells for each y-cut
	#pragma omp parallel for
	for (int y=0;y<ny;++y) {
		count_y[0][y][0] = cells[nx*y].size() > 0;
		for (int x=1;x<nx;++x)
			count_y[x][y][0]+=count_y[x-1][y][0] + (cells[x+nx*y].size()>0);
		for (int z=1;z<nz;++z)
			count_y[0][y][z]+=count_y[0][y][z-1] + (cells[nx*y+nx*ny*z].size()>0);
		for (int x=1;x<nx;++x)
			for (int z=1;z<nz;++z)
				count_y[x][y][z]+=count_y[x-1][y][z]+count_y[x][y][z-1]-count_y[x-1][y][z-1] + (cells[x+nx*y+nx*ny*z].size()>0);
	}
	// count the number of non-empty cells for each z-cut
	#pragma omp parallel for
	for (int z=0;z<nz;++z) {
		count_z[0][0][z] = cells[nx*ny*z].size() > 0;
		for (int x=1;x<nx;++x)
			count_z[x][0][z]+=count_z[x-1][0][z] + (cells[x+nx*ny*z].size()>0);
		for (int y=1;y<ny;++y)
			count_z[0][y][z]+=count_z[0][y-1][z] + (cells[nx*y+nx*ny*z].size()>0);
		for (int x=1;x<nx;++x)
			for (int y=1;y<ny;++y)
				count_z[x][y][z]+=count_z[x-1][y][z]+count_z[x][y-1][z]-count_z[x-1][y-1][z] + (cells[x+nx*y+nx*ny*z].size()>0);
	}
	
	
	partition_grid (0,0,0,nx-1,ny-1,nz-1);
	fprintf (fout,"time to partition aabbs = %ld ms \n",GetTickCount()-tStart);
	tStart = GetTickCount ();
	int count = partition_aabb.size ()/6;
	Material* mat_ptr = objects[0]->get_material ();
	objects.clear ();
	float dx = (bbox.x1-bbox.x0)/nx;
	float dy = (bbox.y1-bbox.y0)/ny;
	float dz = (bbox.z1-bbox.z0)/nz;
	FILE *f1 = fopen ("C:\\Users\\htvu\\Documents\\maya\\2009\\scripts\\visAABBs.mel","w");
	//FILE *f1 = fopen ("PLY_FILE/stats_volume.txt","w");
	int num_sub_grids = partition_aabb.size()/6;
	Grid** nested_grid_objects = new Grid*[num_sub_grids]; // hold the sub-grid pointers

	float vol1=.0f,vol2 = .0f;
	#pragma omp parallel for
	for (int i = 0;i<num_sub_grids;++i) {
		// write to MEL
		int ii=i*6;
		float bx0,by0,bz0,bx1,by1,bz1; // coordinates of the aabb
		bx0 = bbox.x0+partition_aabb[ii]*dx;
		by0 = bbox.y0+partition_aabb[ii+1]*dy;
		bz0 = bbox.z0+partition_aabb[ii+2]*dz;
		bx1 = bbox.x0+(partition_aabb[ii+3]+1)*dx;
		by1 = bbox.y0+(partition_aabb[ii+4]+1)*dy;
		bz1 = bbox.z0+(partition_aabb[ii+5]+1)*dz;
		//fprintf (f1, "drawAABB(%.4f,%.4f,%.4f,%.4f,%.4f,%.4f);\n",bx0,by0,bz0,bx1,by1,bz1);
		//fprintf (f1,"%.4f ",(bx1-bx0)*(by1-by0)*(bz1-bz0));	
		vol1+=(bx1-bx0)*(by1-by0)*(bz1-bz0);
		int x0,y0,z0,x1,y1,z1;
		
		x0 = partition_aabb[ii];
		y0 = partition_aabb[ii+1];
		z0 = partition_aabb[ii+2];
		x1 = partition_aabb[ii+3];
		y1 = partition_aabb[ii+4];
		z1 = partition_aabb[ii+5];
		Grid* grid_ptr = new Grid;
		vector<GeometricObject*> objs;
		vector<int> hash[1003];
			
		for (int x=x0;x<=x1;++x)
			for (int y=y0;y<=y1;++y)
				for (int z=z0;z<=z1;++z) {
					int index = x+nx*y+nx*ny*z;
								
					int num_objects = cells[index].size();
					for (int j=0;j<num_objects;++j)  {
						int id = cell_obj_id[index][j];
						int h = id % 1003;
						if (!hash[h].size()) {
							objs.push_back (cells[index][j]);
							hash[h].push_back (id);
						}
						else {
							bool found = false;
							for (int k=0;k<hash[h].size();++k)
								if (hash[h][k]== id) {
									found = true;
									break;
								}
							if (!found) {
								objs.push_back (cells[index][j]);
								hash[h].push_back (id);
							}
						}
					}
				}
		
		
		int num_objs = objs.size ();
		//fprintf (f1, "drawAABB(%.7f,%.7f,%.7f,%.7f,%.7f,%.7f);\n",bx0,by0,bz0,bx1,by1,bz1);
		/*
		float xx0,yy0,zz0,xx1,yy1,zz1;
		xx0=yy0=zz0=1e6f;
		xx1=yy1=zz1=-1e6f;
		bool intersect = false;
		for (int j=0;j<num_objs;++j) {
			vector<Point3D> p;
			p.clear ();
			objs[j]->intersectP(bx0,by0,bz0,bx1,by1,bz1,p);
			if (!intersect) intersect = p.size() > 0;
			for (int k=0;k<p.size();++k) {
				xx0 = min (xx0,p[k].x);
				yy0 = min (yy0,p[k].y);
				zz0 = min (zz0,p[k].z);
				xx1 = max (xx1,p[k].x);
				yy1 = max (yy1,p[k].y);
				zz1 = max (zz1,p[k].z);
			}
		}
		
		if (intersect) {
			bx0 = max (bx0,xx0);
			by0 = max (by0,yy0);
			bz0 = max (bz0,zz0);
			bx1 = min (bx1,xx1);
			by1 = min (by1,yy1);
			bz1 = min (bz1,zz1);
		}

		float eps = 1e-4f;
		bx0-=eps;
		by0-=eps;
		bz0-=eps;
		bx1+=eps;
		by1+=eps;
		bz1+=eps;
		*/
		
	
		fprintf (f1, "drawAABB(%.4f,%.4f,%.4f,%.4f,%.4f,%.4f);\n",bx0,by0,bz0,bx1,by1,bz1);
		//fprintf (f1, "%.4f %.4f %.4f %.4f %.4f %.4f\n",bx0,by0,bz0,bx1,by1,bz1);
		vol2 +=(bx1-bx0)*(by1-by0)*(bz1-bz0);
		// compute the sub-grid resolution
		wx = bx1 - bx0;
		wy = by1 - by0;
		wz = bz1 - bz0;  
		
		s = pow(wx * wy * wz / num_objs, 0.3333333f);    
		int nnx = int (multiplier * wx / s) + 1;
		int nny = int (multiplier * wy / s) + 1;
		int nnz = int (multiplier * wz / s) + 1;
		// construct the sub-grid
		grid_ptr->init_grid (nnx,nny,nnz,bx0,by0,bz0,bx1,by1,bz1,objs);

		nested_grid_objects[i] = grid_ptr;

		// delete objs
		objs.clear ();
		
		
	}

	//fprintf (f1,"%.4f %.4f\n",vol1,vol2);
	fprintf (fout,"time to construct inner grid = %d ms \n",GetTickCount()-tStart);
	tStart = GetTickCount ();
	// Now reconstruct the outter grid by adding the sub-grids to it
	// First recalculate the resolution
	// The enclosing aabb remains the same
	wx = p1.x-p0.x;
	wy = p1.y-p0.y;
	wz = p1.z-p0.z;
	s = pow(wx * wy * wz / num_sub_grids, 0.3333333f);    
	nx = int (multiplier * wx / s) + 1;
	ny = int (multiplier * wy / s) + 1;
	nz = int (multiplier * wz / s) + 1;
	cells = new vector<GeometricObject*>[nx*ny*nz];
	// adding sub-grids to the outter grid
	for (int i=0;i<num_sub_grids;++i) {
		nested_grid_objects[i]->set_ray_id (-1);
		float rx = (float) nx/(p1.x-p0.x);
		float ry = (float) ny/(p1.y-p0.y);
		float rz = (float) nz/(p1.z-p0.z);
		obj_bBox = nested_grid_objects[i]->get_bounding_box();
		// compute the cell indices at the corners of the bounding box of the object
		int ixmin = clamp(int ((obj_bBox.x0 - p0.x) * rx), 0, nx - 1);
		int iymin = clamp(int ((obj_bBox.y0 - p0.y) * ry), 0, ny - 1);
		int izmin = clamp(int ((obj_bBox.z0 - p0.z) * rz), 0, nz - 1);
		int ixmax = clamp(int ((obj_bBox.x1 - p0.x) * rx), 0, nx - 1);
		int iymax = clamp(int ((obj_bBox.y1 - p0.y) * ry), 0, ny - 1);
		int izmax = clamp(int ((obj_bBox.z1 - p0.z) * rz), 0, nz - 1);
			
		// add the object to the cells
			
		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) 			// cells in x direction
					cells[ix + nx * iy + nx * ny * iz].push_back (nested_grid_objects[i]);			
	}
	delete nested_grid_objects;
		
	// erase the Compound::vector that stores the object pointers, but don't delete the objects
	
	objects.erase (objects.begin(), objects.end());
	fprintf (fout,"time to construct outter grid = %d ms \n",GetTickCount()-tStart);
	fclose (fout);
	fclose (f1);
}

bool NestedGrid::cell_hit (int index, const Ray& ray, float& tmin, ShadeRec& sr) const {
	sr.w.grid_intersection_count++;
	int num_objects = cells[index].size();
	float		t=1e10f;
	bool		hit 		= false;
	Point3D		local_hit_point = sr.hit_point;
	Normal normal = sr.normal;
	Material* material_ptr = sr.material_ptr;
	GeometricObject* obj_ptr = sr.obj_ptr;
	float tt = sr.t;
	for (int j = 0; j < num_objects; j++)
		if (cells[index][j]->get_ray_id () < ray.id) {
			if (cells[index][j]->hit(ray, t, sr) && (t < tt)) {
				hit = true;
				normal = sr.normal;
				material_ptr = sr.material_ptr;
				local_hit_point = sr.hit_point;
				tt = t;
				obj_ptr = sr.obj_ptr;
				//local_hit_point	= sr.local_hit_point;  
			}
			cells[index][j]->set_ray_id (ray.id);
		}
	sr.obj_ptr = obj_ptr;
	sr.t = tt;
	sr.normal = normal;
	sr.material_ptr = material_ptr;
	sr.hit_point = local_hit_point;
	return (hit);
}

// ---------------------------------------------------------------- hit

// The following grid traversal code is based on the pseudo-code in Shirley (2000)	
// The first part is the same as the code in BBox::hit

bool NestedGrid::hit(const Ray& ray, float& t, ShadeRec& sr) const {
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
				cell_hit (index,ray, sr.t, sr);
				if (sr.t<tx_next) return true;
			}
			tx_next += dtx;
			ix += ix_step;
						
			if (ix == ix_stop)
				return (false);
		} 
		else { 	
			if (ty_next < tz_next) {
				if (nonempty) {
					//t = ty_next;
					cell_hit(index,ray, sr.t, sr);
					if (sr.t<ty_next) return true;
				}
				ty_next += dty;				
				iy += iy_step;
				if (iy == iy_stop)
					return (false);
		 	}
		 	else {		
				if (nonempty) {
					//t = tz_next;
					cell_hit(index,ray, sr.t, sr);
					if (sr.t<tz_next) return true;
				}
				tz_next += dtz;
				iz += iz_step;
				if (iz == iz_stop)
					return (false);
		 	}
		}
	}
}	// end of hit


bool NestedGrid::cell_shadow_hit (int index, const Ray& ray, float& tmin,const GeometricObject* obj_ptr) const {
	int num_objects = cells[index].size();
	float tt = 1e10f;
	bool found = false;
	for (int j = 0; j < num_objects; j++)
		if (cells[index][j]->shadow_hit(ray, tt,obj_ptr) && tt < tmin) {
			tmin = tt;
			found = true;
		}
	
	return found;
}

bool NestedGrid::shadow_hit(const Ray& ray, float& t, GeometricObject* obj_ptr) const {
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
					if (cell_shadow_hit(index,ray, t, obj_ptr)) 
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

