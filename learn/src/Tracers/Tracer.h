#pragma once

#include "Camera.h"
#include "ViewPlane.h"
#include "Image.h"
#include "Vector3D.h"
#include "World.h"

class Tracer {
	
public:
	Tracer() {
	}

	TGAImage *render(const Camera& camera, const ViewPlane& vp, const World& world, TGAImage *img,
                     int startRow, int blockSize);

};