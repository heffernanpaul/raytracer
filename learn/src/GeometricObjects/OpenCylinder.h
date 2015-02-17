#ifndef __OPEN_CYLINDER__
#define __OPEN_CYLINDER__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// An open cylinder is defined by its extent in the y direction and its radius.
// It is centered on the y axis.
// The classes ConcaveOpenCylinder and ConvexOpenCylinder can inherit from OpenCylinder.
// The classes ConcavePartCylinder and ConvexPartCylinder can in turn inherit from these.

#include "GeometricObject.h"

class OpenCylinder: public GeometricObject {
	
	public:

		OpenCylinder(const Material& materialIn,
                     const float bottom, const float top, const float radius) :
    GeometricObject(materialIn), y0(bottom), y1(top), radius(radius), inv_radius(1.0/radius) {
    
    }

		virtual bool
		hit(const Ray& ray, float& t, ShadeRec& sr) const;

		virtual bool shadow_hit (const Ray& ray, float &tmin) const;
		
	protected:
	
		float		y0;				// bottom y value
		float		y1;				// top y value
		float		radius;			// radius
		float		inv_radius;  	// one over the radius	
};

#endif
