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

	ViewPlane vp(800, 800);
	Camera camera(Point3D(0,0, -100), 100);
	Tracer tracer;
	World world;
	
	TGAImage* img = tracer.render(camera, vp, world);

	string filename = "test.tga";
	img->WriteImage(filename);

}	
/*int main2(int argc, char **argv)
{
	
	//declare image
	short width = 800;
	short height = 800;
	TGAImage *img = new TGAImage(width,height);
	

	for (int i = 0; i < 50; i++) {
		createImage(width, height, img);
	}

	auto start = GetTime();
	createImage(width, height, img);

	auto end = GetTime();
	auto diff = end - start;
	cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;

	//write the image to disk
	string filename = "test.tga";
	img->WriteImage(filename);
	
	return 0;	
}
*/
