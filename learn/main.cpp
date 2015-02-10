/*
 *  main.cpp
 *  ImageSaver
 *
 *  Created by Daniel Beard on 6/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <chrono>

#include "Image.h"

void createImage(short width, short height, TGAImage* img) {
	RGBColor c;
			//c.a = 255;
	//Loop through image and set all pixels to red
		for(int y=0; y<width; y++)
		{
	      c.g = y % 256;  
	for(int x=0; x<width; x++)
		{
	      c.r = x % 256; 
	      c.b = (x * y) % 256;  
			img->setPixel(c,x,y);
		}
	}

}

int main(int argc, char **argv)
{
	
	//declare image
	short width = 800;
	short height = 800;
	TGAImage *img = new TGAImage(width,height);
	

	for (int i = 0; i < 50; i++) {
		createImage(width, height, img);
	}

	auto start = chrono::monotonic_clock::now();
	createImage(width, height, img);

	auto end = chrono::monotonic_clock::now();
	auto diff = end - start;
	cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;

	//write the image to disk
	string filename = "test.tga";
	img->WriteImage(filename);
	
	return 0;	
}
