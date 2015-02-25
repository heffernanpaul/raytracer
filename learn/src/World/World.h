#pragma once

#include <vector>
#include "ShadeRec.h"
#include "Light.h"
#include "Sampler.h"
#include "GeometricObject.h"

class World {

	std::vector<GeometricObject*> objects;
	std::vector<Light*> lights;
    Sampler* sampler_ptr;

public:
	World();

	bool hit_objects(const Ray& ray, ShadeRec& sr) const;

    Light* ambient_ptr;

	const std::vector<Light*> getLights() const {
		return lights;
	}
};
