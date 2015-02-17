#pragma once

#include "Point2D.h"
#include "Point3D.h"

class Camera {

protected:
	Point3D eye;
	Vector3D		u, v, w;			// orthonormal basis vectors
	float	d;		// view plane distance

public:
	Camera(const Point3D& eyeIn, const float dIn) :
		eye(eyeIn),
		u(1, 0, 0),
		v(0, 1, 0),
		w(0, 0, 1),
		d(dIn)
		 {
	}

	const Point3D& getEye() const {
		return eye;
	}
	void getDirection(const Point2D& p, Vector3D& direction) const;

	float getDistanceToPlane() {
		return d;
	}
};