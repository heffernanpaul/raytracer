#include "World.h"
#include "Material.h"
#include "Matte.h"
#include "Sphere.h"
#include "Box.h"
#include "OpenCylinder.h"
#include "Plane.h"
#include "PointLight.h"
#include "AmbientLight.h"

bool World::hit_objects(const Ray& ray, ShadeRec& sr) const {

    static float max_float = std::numeric_limits<float>::max();
    
	sr.t = max_float;
	sr.obj = nullptr;
    float tmin = max_float;
    for (int i = 0; i < objects.size(); i++) {
        auto object = objects[i];
		if (object->hit(ray, tmin, sr)) {
			if (tmin < sr.t) {
				sr.obj = object;
				sr.t = tmin;
                sr.ray = ray;
			}
		}
	}
	if (sr.obj != nullptr) {
		sr.normal.normalize();
		return true;
	}
	return false;
}

World::World() {
	Material* redMatte = new Matte(1.0, 3.0, RGBColor(1.0, .4, .4));
	Material* yellowMatte = new Matte(1.0, 3.0, RGBColor(1.0, 1.0, .4));
	Material* greenMatte = new Matte(1.0, 3.0, RGBColor(0.4, .8, .6));
	Material* brownMatte = new Matte(1.0, 3.0, RGBColor(.4, .4, .1));
	Material* blueMatte = new Matte(1.0, 3.0, RGBColor(.4, .4, 1.0));

    // Balls
//	objects.push_back(new Sphere(Point3D(-100,  0,100), 20, *redMatte));
//	objects.push_back(new Sphere(Point3D(-50,   0,100), 20, *yellowMatte));
//	objects.push_back(new Sphere(Point3D(0,     0,100), 20, *greenMatte));
//	objects.push_back(new Sphere(Point3D(50,    0,100), 20, *blueMatte));
//	objects.push_back(new Sphere(Point3D(100,   0,100), 20, *redMatte));

    objects.push_back(new OpenCylinder(*redMatte, -10, -10, 10));

    // Planes
    objects.push_back(new Plane(Point3D(0,0,880), Normal(0,0,-1), *blueMatte));
    objects.push_back(new Plane(Point3D(0,-50,0), Normal(0,1,0), *greenMatte));
    
    // Lights
    lights.push_back(new PointLight(RGBColor(0.8, 0.8, 0.1), Point3D(100, 100, 0), 1.0));
//    lights.push_back(new PointLight(RGBColor(0.0, 0.6, 0.4), Point3D(-100, -35, 100), 0.2));
//    lights.push_back(new PointLight(RGBColor(0.6, 0.0, 0.0), Point3D(-100, 400, 850), 0.5));
//    lights.push_back(new PointLight(RGBColor(0.6, 0.6, 0.4), Point3D(50, -20, 50), 0.5));

    ambient_ptr = new AmbientLight(RGBColor(0.2,0.2,0.4));


}