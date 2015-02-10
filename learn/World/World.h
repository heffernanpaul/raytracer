#pragma once

#include "ShadeRec.h"

class World {

public:
	bool hit_objects(const Ray& ray, ShadeRec& sr) const;

};
