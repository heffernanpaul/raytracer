#ifndef __RECTANGLE__
#define __RECTANGLE__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"
#include "GeometricObject.h"

class Rectangle3D: public GeometricObject {	
	public:
		
		Rectangle3D(void);   									
				
		Rectangle3D(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b);
		
		Rectangle3D(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b, const Normal& n);
		
		virtual Rectangle3D* 										
		clone(void) const;
	
		Rectangle3D(const Rectangle3D& r); 						

		virtual													
		~Rectangle3D(void);   									

		Rectangle3D& 												
		operator= (const Rectangle3D& rhs);
		
		//BBox	get_bounding_box(void);				
	
		virtual bool 												 
		hit(const Ray& ray, float& t, ShadeRec& s) const;	
				
		
		// the following functions are used when the rectangle is a light source
		
		virtual void 								
		set_sampler(Sampler* sampler); 
				
		virtual Point3D 											
		sample(void);
		
		virtual Normal 																
		get_normal(const Point3D& p);		
		
		virtual float												
		pdf(const ShadeRec& sr);
		
	private:
	
		Point3D 		p0;   			// corner vertex 
		Vector3D		a;				// side
		Vector3D		b;				// side
		float			a_len_squared;	// square of the length of side a
		float			b_len_squared;	// square of the length of side b
		Normal			normal;	
		
		float			area;			// for rectangular lights
		float			inv_area;		// for rectangular lights
		Sampler*		sampler_ptr;	// for rectangular lights 	
		
		static const float kEpsilon;   											
};

#endif
