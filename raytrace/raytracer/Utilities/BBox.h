#ifndef __BBOX__
#define __BBOX__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Ray.h"
#include "Point3D.h"

class BBox {
	public:
		float x0, x1, y0, y1, z0, z1;
		
		BBox(void);										
						
		BBox(	const float x0, const float x1,
				const float y0, const float y1,
				const float z0, const float z1);

		BBox(const Point3D p0, const Point3D p1);											

		BBox(const BBox& bbox);							

		BBox& 												
		operator= (const BBox& rhs);			

		~BBox(void);										
		
		bool 															
		hit(const Ray& ray) const;
	
		bool
		inside(const Point3D& p) const;
};
	
#endif

