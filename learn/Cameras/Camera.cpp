#include "Camera.h"

Vector3D
Camera::getDirection(const Point2D& p) const {
	Vector3D dir = p.x * u + p.y * v - d * w;
	dir.normalize();
	
	return(dir);
}