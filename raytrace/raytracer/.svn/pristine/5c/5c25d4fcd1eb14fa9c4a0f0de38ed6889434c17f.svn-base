#ifndef __NESTED_GRID__
#define __NESTED_GRID__

// This contains the declaration of the class NestedGrid.
// The copy constructor, assignment operator, and destructor, are dummy functions.
// The reason is that a grid can contain an arbitrary number of objects, can therefore be of arbitrary size.


// There is no set_bounding_box function because the bounding box is constructed in the setup_cells
// function.

// This NestedGrid class is also way too large. All the triangle and PLY file code should be placed in
// a separate class called TriangleMesh that inherits from NestedGrid.



#include "ShadeRec.h"
#include "BBox.h"
#include "Mesh.h"
#include "Grid.h"
//#include "Mesh.h"



//---------------------------------------------------------------------- class NestedGrid

class NestedGrid: public Grid {										  	
	public:

		NestedGrid(void); 

		NestedGrid(Mesh* _mesh_ptr);    										

		virtual NestedGrid* 										
		clone(void) const;

		NestedGrid(const NestedGrid& rg); 		

		NestedGrid& operator= (const NestedGrid& rhs);	

		virtual ~NestedGrid(void);   			
		virtual bool hit(const Ray& ray, float& tmin, ShadeRec& sr) const;	
		virtual bool cell_hit (int index, const Ray& ray, float& tmin, ShadeRec& s) const;
		virtual bool shadow_hit (const Ray& ray, float& tmin, GeometricObject* obj_ptr) const;
		virtual bool cell_shadow_hit (int index, const Ray& ray, float& tmin,const GeometricObject* obj_ptr) const;
		virtual void setup_cells();

		
	private: 
		long long***						count_x;
		long long***						count_y;
		long long***						count_z;
		vector<int>					partition_aabb;
		vector<Grid>				grids;

		void partition_grid (int x0, int y0, int z0, int x1, int y1, int z1);
};



#endif









