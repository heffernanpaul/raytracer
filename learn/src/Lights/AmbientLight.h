#pragma once

class AmbientLight : public Light {


public:
	AmbientLight(const RGBColor& color) : Light(color) {
	}

	virtual void getDirection(const Point3D& point, Vector3D& direction) const {
        direction.x = -point.x;
        direction.y = -point.y;
        direction.z = -point.z;
        direction.normalize();
	}

    RGBColor L(const ShadeRec& sr) {
        return (color);
    }

};