#ifndef __GRID__
#define __GRID__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This contains the declaration of the class Grid.
// The copy constructor, assignment operator, and destructor, are dummy functions.
// The reason is that a grid can contain an arbitrary number of objects, can therefore be of arbitrary size.


// There is no set_bounding_box function because the bounding box is constructed in the setup_cells
// function.

// This Grid class is also way too large. All the triangle and PLY file code should be placed in
// a separate class called TriangleMesh that inherits from Grid.


#include "Compound.h"
#include "ShadeRec.h"
#include "BBox.h"
#include "Mesh.h"

//#include "Mesh.h"



//---------------------------------------------------------------------- class Grid

class Grid: public Compound {										  	
	public:

		Grid(void); 

		Grid(Mesh* _mesh_ptr);    										

		virtual Grid* 										
		clone(void) const;

		Grid(const Grid& rg); 		

		Grid& 								
		operator= (const Grid& rhs);	

		virtual 										
		~Grid(void);   			

		virtual BBox 
		get_bounding_box(void);
							 

		
		virtual void setup_cells(void);
		virtual void setup_cells(bool partition);
		virtual bool hit(const Ray& ray, float& tmin, ShadeRec& sr) const;		 
		virtual bool cell_hit (int index, const Ray& ray, float& tmin, ShadeRec& s) const;
		virtual bool shadow_hit (const Ray& ray, float& tmin,const GeometricObject* obj_ptr) const;
		bool cell_shadow_hit (int index, const Ray& ray, float& tmin,const GeometricObject* obj_ptr) const;

		void store_material(Material* material, const int index); 	

		float clamp (float x, float min, float max) const;

		int clamp (int x, int min, int max) const;

		void init_grid (const int& _nx, const int& _ny, const int& _nz, const float& x0, const float& y0,
						const float& z0, const float& x1,const float& y1, const float& z1,const vector<GeometricObject*>& objects);

	protected: 

		vector<GeometricObject*>*	cells;			// grid of cells
		int							nx, ny, nz;    	// number of cells in the x, y, and z directions
		BBox						bbox;			// bounding box

		Point3D	find_min_bounds(void);

		Point3D	find_max_bounds(void);

		
		

		
};

inline int Grid::clamp (int x, int min, int max) const {
	return (x<min?min:(x>max?max:x));
}

inline float Grid::clamp (float x, float min, float max) const {
	return (x<min?min:(x>max?max:x));
}

// ------------------------------------------------------------------------------ store_material

inline void
Grid::store_material(Material* material_ptr, const int index) {
	objects[index]->set_material(material_ptr); 
}

#endif









