//
//  SolidCylinder.cpp
//  raytrace
//
//  Created by Paul Heffernan on 26/01/2015.
//
//

#include "SolidCylinder.h"


SolidCylinder::SolidCylinder(	const double bottom,
                             const double top,
                             const double radius)
: Compound() {
    
//    objects.push_back(new Disk(	Point3D(0, bottom, 0), 			// bottom
//                               Normal(0, -1, 0),
//                               radius));
//    
//    objects.push_back(new Disk(	Point3D(0, top, 0), 			// top
//                               Normal(0, 1, 0),
//                               radius));   	
//    
//    objects.push_back(new OpenCylinder(bottom, top, radius));	// wall
}

bool
SolidCylinder::hit(const Ray& ray, float& tmin, ShadeRec& sr) const {
    if (bbox.hit(ray))
        return (Compound::hit(ray, tmin, sr));
    else
        return (false);
}

