#ifndef __SPHERE__
#define __SPHERE__

// This file contains the declaration of the class Sphere

#include "GeometricObject.h"

//-------------------------------------------------------------------------------- class Sphere

class Sphere: public GeometricObject {	
								  	
	public:
		
		Sphere(void);   									// Default constructor
				
		Sphere(Point3D center, float r);					// Constructor 
					
		Sphere(const Sphere& sphere); 						// Copy constructor
		
		virtual Sphere* 									// Virtual copy constructor
		clone(void) const;

		virtual												// Destructor
		~Sphere(void);   									

		Sphere& 											// assignment operator
		operator= (const Sphere& sphere);				
																					
		void
		set_center(const Point3D& c);
		
		void
		set_center(const float x, const float y, const float z);
		
		void
		set_radius(const float r);
						
		virtual bool 												 
		hit(const Ray& ray, float& t, ShadeRec& s) const;	

		virtual BBox get_bounding_box ();

		virtual bool shadow_hit (const Ray& ray, float &tmin) const; // shadow test: secondary ray

		virtual bool shadow_hit (const Ray& ray) const; // for directional only

		
	private:
	
		Point3D 	center;   			// center coordinates as a point  
		float 		radius;				// the radius 
		
		static const float kEpsilon;   // for shadows and secondary rays
};


inline void
Sphere::set_center(const Point3D& c) {
	center = c;
}
		
inline void
Sphere::set_center(const float x, const float y, const float z) {
	center.x = x;
	center.y = y;
	center.z = z;
}
		
inline void
Sphere::set_radius(const float r) {
	radius = r;
}

#endif
