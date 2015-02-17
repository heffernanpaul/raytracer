#include "Camera.h"

void
Camera::getDirection(const Point2D& p, Vector3D& direction) const {
    direction.x = p.x;
    direction.y = p.y;
    direction.z = d;
    direction.normalize();
}