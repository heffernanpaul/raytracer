#include "World.h"
#include "Material.h"
#include "Matte.h"
#include "Emissive.h"
#include "Sphere.h"
#include "Rectangle3D.h"
#include "Box.h"
#include "OpenCylinder.h"
#include "Plane.h"
#include "PointLight.h"
#include "AmbientLight.h"
#include "AreaLight.h"
#include "MultiJittered.h"

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
    
    sampler_ptr = new MultiJittered(256, 16);
    sampler_ptr->generate_samples();
	Material* redMatte = new Matte(1.0, 3.0, RGBColor(1.0, .4, .4));
	Material* yellowMatte = new Matte(1.0, 3.0, RGBColor(1.0, 1.0, .4));
	Material* pinkMatte = new Matte(1.5, 9.0, RGBColor(0.6, .2, .6));
	Material* whiteEmissive = new Emissive(1.0, RGBColor(1.0, 1.0, 0.6));
	Material* blueMatte = new Matte(1.0, 3.0, RGBColor(.4, .4, 1.0));
    Material* grassMatte = new Matte(1.0, 3.0, RGBColor(0.4, .8, .3));

    // Balls
//	objects.push_back(new Sphere(Point3D(-20,  0,60), 20, *redMatte));
    objects.push_back(new Sphere(Point3D(-80,   0,60), 20, *yellowMatte));
	objects.push_back(new Sphere(Point3D(0,     0,60), 20, *pinkMatte));
	objects.push_back(new Sphere(Point3D(50,    0,60), 20, *blueMatte));

//    objects.push_back(new OpenCylinder(*redMatte, -10, -10, 10));

    // Planes
    objects.push_back(new Plane(Point3D(0,0,100), Normal(0,0.0,-1), *blueMatte));
    objects.push_back(new Plane(Point3D(0,-20,0), Normal(0,1,0), *grassMatte));

    Rectangle3D* lightGlobe1 = new Rectangle3D(*whiteEmissive, Point3D(-100,   70,100), Vector3D(40,0,0), Vector3D(0,0,-80), Normal(0,-1,0), sampler_ptr);
    lightGlobe1->set_sampler(sampler_ptr);
    objects.push_back(lightGlobe1);
    lights.push_back(new AreaLight(lightGlobe1));
    
    Rectangle3D* lightGlobe2 = new Rectangle3D(*whiteEmissive, Point3D(0,   70,100), Vector3D(40,0,0), Vector3D(0,0,-80), Normal(0,-1,0), sampler_ptr);
    lightGlobe2->set_sampler(sampler_ptr);
    objects.push_back(lightGlobe2);
    lights.push_back(new AreaLight(lightGlobe2));
    
    Rectangle3D* lightGlobe3 = new Rectangle3D(*whiteEmissive, Point3D(100,   70,100), Vector3D(40,0,0), Vector3D(0,0,-80), Normal(0,-1,0), sampler_ptr);
    lightGlobe3->set_sampler(sampler_ptr);
    objects.push_back(lightGlobe3);
    lights.push_back(new AreaLight(lightGlobe3));
    
    
    // Lights
//    lights.push_back(new PointLight(RGBColor(0.8, 0.8, 0.8), Point3D(30, 30, 30), 0.4));
//    lights.push_back(new PointLight(RGBColor(0.8, 0.8, 0.8), Point3D(-100, 100, 20), 0.3));
//    lights.push_back(new PointLight(RGBColor(0.6, 0.0, 0.0), Point3D(-100, 400, 850), 0.5));
//    lights.push_back(new PointLight(RGBColor(0.6, 0.6, 0.4), Point3D(50, -20, 50), 0.5));

    ambient_ptr = new AmbientLight(RGBColor(0.2,0.2,0.2));


}
