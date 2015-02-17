#pragma once

#include <vector>
#include "ShadeRec.h"
#include "Light.h"
#include "GeometricObject.h"

class World {

	std::vector<GeometricObject*> objects;
	std::vector<Light*> lights;

public:
	World();

	bool hit_objects(const Ray& ray, ShadeRec& sr) const;

    Light* ambient_ptr;

	const std::vector<Light*> getLights() const {
		return lights;
	}
};
