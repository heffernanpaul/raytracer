/*
 *  main.cpp
 *  ImageSaver
 *
 *  Created by Daniel Beard on 6/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "Image.h"
#include "Camera.h"
#include "Tracer.h"
#include "ViewPlane.h"
#include "Timing.h"



int main(int argc, char** argv) {

	ViewPlane vp(800, 800, 1);
	Camera camera(Point3D(0 ,0, -100), 300);
	Tracer tracer;
	World world;
	
    TGAImage* img = tracer.render(camera, vp, world);
	string filename = "test.tga";
	img->WriteImage(filename);

}	
