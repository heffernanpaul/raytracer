//
//  SolidCylinder.h
//  raytrace
//
//  Created by Paul Heffernan on 26/01/2015.
//
//

#ifndef __raytrace__SolidCylinder__
#define __raytrace__SolidCylinder__

#include "Compound.h"

class SolidCylinder: public Compound {
public:
    
    SolidCylinder(void);
    
    SolidCylinder(const double bottom, const double top, const double radius);
    
    SolidCylinder(const SolidCylinder& cc);

    virtual bool hit(const Ray& ray, float& tmin, ShadeRec& sr) const;
    
};

#endif /* defined(__raytrace__SolidCylinder__) */
