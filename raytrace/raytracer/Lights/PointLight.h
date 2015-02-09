#ifndef __POINTLIGHT__
#define __POINTLIGHT__

#include "Light.h"
#include "Vector3D.h"
#include "RGBColor.h"

#include "World.h"			// you will need this later on for shadows
#include "ShadeRec.h"


class PointLight: public Light {
public:
    
    PointLight(void);
    
    PointLight(const PointLight& dl);
    
    virtual Light*
    clone(void) const;
    
    PointLight&
    operator= (const PointLight& rhs);
    
    virtual
    ~PointLight(void);
    
    virtual Vector3D
    get_direction(const ShadeRec& sr);
    
    virtual RGBColor
    L(const ShadeRec& sr);

    Point3D
    get_location() {
        return Point3D(location.x, location.y, location.z);
    }

    void
    set_location(double x, double y, double z) {
        this->location.x = x;
        this->location.y = y;
        this->location.z = z;
    }
    
    void
    set_location(const Vector3D& location);
    
    void
    set_color(const float c);
    
    void
    set_color(const RGBColor& c);
    
    void
    set_color(const float r, const float g, const float b) {
        this->color.r = r;
        this->color.g = g;
        this->color.b = b;
    }
    
    void
    scale_radiance(const float _ls) {
        this->ls = _ls;
    }
    
    void
    set_shadows(bool flag) {
        
    }
    
    
    


private:
    
    float 		ls;
    RGBColor 	color;
    Vector3D 	location;
};




#endif