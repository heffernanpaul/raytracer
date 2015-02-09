#ifndef __PLANE__
#define __PLANE__

#include "GeometricObject.h"

//-------------------------------------------------------------------- class Plane

class Plane: public GeometricObject {
	
	public:
	
		Plane(void);   												// default constructor
		
		Plane(const Point3D& point, const Normal& normal);			// constructor	
	
		Plane(const Plane& plane); 									// copy constructor
		
		virtual Plane* 												// virtual copy constructor
		clone(void) const;

		Plane& 														// assignment operator
		operator= (const Plane& rhs);	
		
		virtual														// destructor
		~Plane(void);   											
					
		virtual bool 																								 
		hit(const Ray& ray, float& tmin, ShadeRec& sr) const;

		virtual bool shadow_hit (const Ray& ray, float &tmin) const; // shadow test: secondary ray

		virtual bool shadow_hit (const Ray& ray) const; // for directional only
		
	
		Point3D 	p;   				// point through which plane passes 
		Normal 		n;					// normal to the plane
				
		static const float kEpsilon;   // for shadows and secondary rays
};

#endif
