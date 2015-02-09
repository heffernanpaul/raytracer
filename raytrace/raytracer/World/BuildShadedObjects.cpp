// This builds a scene that consists of 35 shaded spheres and a plane.
// The objects are illuminated by a directional light and rendered with
// ambient and diffuse shading.
// Perspective viewing is used with a pinhole camera.
// Jittered sampling for antialiasing is hardwired into the PinHole::render_scene function.
// There are no sampler classes in this project.
// These are in the Chapter 5 download file.
// The spheres are the same as those in the Chapter 14 page one image. 

#include "RGBColor.h"
#include "Pinhole.h"
#include "Raycast.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "Directional.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "EnvironmentLight.h"
#include "Matte.h"
#include "Sphere.h"
#include "SolidCylinder.h"
#include "World.h"
#include "Plane.h"
#include "Box.h"
#include "Whitted.h"
#include "AreaLighting.h"
#include "Reflective.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
void
World::build(void) {
    

 int num_samples = 1;
    
    render_ptr->vp.set_hres(600);
    render_ptr->vp.set_vres(400);
    render_ptr->vp.set_samples(num_samples);
    Sampler* sampler = new MultiJittered(num_samples);
    sampler->map_samples_to_hemisphere(0.1);
    render_ptr->set_sampler(sampler);
    //	vp.set_max_depth(0);			// for Figure 24.6(a)
    render_ptr->vp.set_max_depth(2);			// for Figure 24.6(b)
    
    render_ptr->tracer_ptr = new Whitted(this);
    render_ptr->background_color = RGBColor(0.0, 0.0, 0.0);
    
  //  render_ptr->render_option = DIRECT_DIFFUSE;

    Ambient* ambient_ptr = new Ambient;
    ambient_ptr->scale_radiance(0.5);
//    set_ambient_light(ambient_ptr);
    
    
    Pinhole* pinhole_ptr = new Pinhole;
    pinhole_ptr->set_eye(0, 0, 200);
    pinhole_ptr->set_lookat(-10, 0, 0);
    pinhole_ptr->set_view_distance(360);
    pinhole_ptr->compute_uvw();
    set_camera(pinhole_ptr);
    
    
    Matte* matte_ptr2 = new Matte(0.45, 4.0, RGBColor(0.9, 0.9, 0.9));   // orange

    Sphere* sphere_ptr3 = new Sphere(Point3D(15, 5, 0), 5);
    sphere_ptr3->set_material(matte_ptr2);
    add_object(sphere_ptr3);

    
    
    Directional* light_ptr1 = new Directional;
    light_ptr1->set_direction(100, 100, 200);
    light_ptr1->set_color(0.8, 0.8, 0.8);
    
    PointLight* point_light_ptr1 = new PointLight;
    point_light_ptr1->set_location(0, 0, 0);
    point_light_ptr1->set_color(0.4, 0.4, 0.4);
    point_light_ptr1->scale_radiance(3.0);
    add_light(point_light_ptr1);
    
    Matte* orange = new Matte(0.45, 4.0, RGBColor(0.7, 0.4, 0.2));
    Matte* green = new Matte(0.85, 2.0, RGBColor(0.2, 0.6, 0.2));
    Matte* skyBlue = new Matte(1.0, 2.0, RGBColor(0.1, 0.3, 1.0));
    Matte* red = new Matte(1.0, 2.0, RGBColor(1.0, 0.1, 0.1));
    Matte* brown = new Matte(1.0, 2.0, RGBColor(0.3, 0.3, 0.1));

//    for (int i = -10; i < 10; i+=2) {
//        for (int j = -10; j < 10; j+=2) {
//            Sphere* sphere_ptr2 = new Sphere(Point3D(i*10.0, j*10.0, -60), 10);
//            sphere_ptr2->set_material(orange);
//            add_object(sphere_ptr2);
//            
//        }
//    }

    Sphere* sphere_ptr2 = new Sphere(Point3D(-50, 0, 0), 20);
    sphere_ptr2->set_material(orange);
    add_object(sphere_ptr2);
    
    sphere_ptr2 = new Sphere(Point3D(50, 0, 0), 20);
    sphere_ptr2->set_material(orange);
    add_object(sphere_ptr2);
    
    Plane* floor = new Plane(Point3D(0,-100,0), Vector3D(0, 1, 0));
    floor->set_material(green);
    add_object(floor);
    
    Plane* ceiling = new Plane(Point3D(0,100,0), Vector3D(0, -1, 0));
    ceiling->set_material(green);
    add_object(ceiling);
    
    Plane* leftWall = new Plane(Point3D(-100,0,0), Vector3D(1, 0, 0));
    leftWall->set_material(brown);
    add_object(leftWall);
    
    Plane* rightWall = new Plane(Point3D(100,0,0), Vector3D(-1, 0, 0));
    rightWall->set_material(brown);
    add_object(rightWall);
    
    Plane* sky = new Plane(Point3D(0,0,-400), Vector3D(0, 0, 1));
    sky->set_material(skyBlue);
    add_object(sky);

    Sphere* sphere_ptr4 = new Sphere(point_light_ptr1->get_location(), 2.0);
    sphere_ptr4->set_material(red);
    add_object(sphere_ptr4);

    AreaLight* areaLight = new AreaLight();
    areaLight->set_object(floor);
   // add_light(areaLight);
/*
	int num_samples = 100;
	
    render_ptr->vp.set_hres(600);
    render_ptr->vp.set_vres(400);
    render_ptr->vp.set_samples(num_samples);
    Sampler* sampler = new MultiJittered(num_samples);
    sampler->map_samples_to_hemisphere(0.1);
    render_ptr->set_sampler(sampler);
	render_ptr->vp.set_max_depth(10);
 
	render_ptr->tracer_ptr = new AreaLighting(this);
 
	AmbientOccluder* ambient_occluder_ptr = new AmbientOccluder;
	ambient_occluder_ptr->set_sampler(new MultiJittered(num_samples));
	ambient_occluder_ptr->set_min_amount(0.5);
	set_ambient_light(ambient_occluder_ptr);
 
 
	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(100, 45, 100);
	pinhole_ptr->set_lookat(-10, 40, 0);
	pinhole_ptr->set_view_distance(400);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);
 
//	
	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(0.90);
	emissive_ptr->set_ce(RGBColor(1.0, 1.0, 0.5)); 	// lemon yellow
 
 
//	ConcaveSphere* sphere_ptr = new ConcaveSphere;
//	sphere_ptr->set_radius(1000000.0);
//	sphere_ptr->set_material(emissive_ptr);
//	sphere_ptr->set_shadows(false);
//	add_object(sphere_ptr);
	
    
	EnvironmentLight* environment_light_ptr = new EnvironmentLight;
	environment_light_ptr->set_material(emissive_ptr);
	environment_light_ptr->set_sampler(sampler);
	//environment_light_ptr->set_shadows(true);
	add_light(environment_light_ptr);

	
	// common reflective material for large sphere, medium sphere, and cylinder
	
//	Reflective* reflective_ptr = new Reflective;
//	reflective_ptr->set_ka(0.0);
//	reflective_ptr->set_kd(0.0);
//	reflective_ptr->set_cd(black);
//	reflective_ptr->set_ks(0.0);
//	reflective_ptr->set_exp(1.0);
//	reflective_ptr->set_kr(0.9);
//	reflective_ptr->set_cr(1.0, 0.75, 0.5);   // orange
//	
	float ka = 0.2;  // commom ambient reflection coefficient for other objects
	
 // large sphere
	
//	Sphere* sphere_ptr1 = new Sphere(Point3D(38, 20, -24), 20);
//	sphere_ptr1->set_material(reflective_ptr);
//	add_object(sphere_ptr1);
	
	
	// small sphere
	
	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(ka);
	matte_ptr2->set_kd(0.5);
	matte_ptr2->set_cd(0.85);
	
	Sphere* sphere_ptr2 = new Sphere(Point3D(34, 12, 13), 12);
	sphere_ptr2->set_material(matte_ptr2);
	add_object(sphere_ptr2);
	
	
	// medium sphere
	
//	Sphere* sphere_ptr3 = new Sphere(Point3D(-7, 15, 42), 16);
//	sphere_ptr3->set_material(reflective_ptr);
//	add_object(sphere_ptr3);
	
	
	// cylinder
//	
//	double bottom 	= 0.0;
//	double top 		= 85;
//	double radius	= 22;
//	SolidCylinder* cylinder_ptr = new SolidCylinder(bottom, top, radius);
//	cylinder_ptr->set_material(reflective_ptr);
//	add_object(cylinder_ptr);
// 
	
	// box
	
//	Matte* matte_ptr5 = new Matte;
//	matte_ptr5->set_ka(ka);
//	matte_ptr5->set_kd(0.5);
//	matte_ptr5->set_cd(0.95);
//	
//	Box* box_ptr = new Box(Point3D(-35, 0, -110), Point3D(-25, 60, 65));
//	box_ptr->set_material(matte_ptr5);
//	add_object(box_ptr);
	
 
	// ground plane
// 
//	Matte* matte_ptr6 = new Matte;
//	matte_ptr6->set_ka(0.15);
//	matte_ptr6->set_kd(0.5);
//	matte_ptr6->set_cd(0.7);
//	
//	Plane* plane_ptr = new Plane(Point3D(0, 0.01, 0), Normal(0, 1, 0));
//	plane_ptr->set_material(matte_ptr6);
//	add_object(plane_ptr);
//
 */
}

