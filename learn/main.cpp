/*
 *  main.cpp
 *  ImageSaver
 *
 *  Created by Daniel Beard on 6/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Image.h"

int main(int argc, char **argv)
{
	
	//declare image
	short width = 800;
	short height = 800;
	TGAImage *img = new TGAImage(width,height);
	
	//declare a temporary color variable
	Colour c;
	
	//Loop through image and set all pixels to red
	for(int x=0; x<width; x++)
		for(int y=0; y<width; y++)
		{
	      c.r = x % 256;  /* red */
	      c.g = y % 256;  /* green */
	      c.b = (x * y) % 256;  /* blue */
			c.a = 255;
			img->setPixel(c,x,y);
		}
	
	//write the image to disk
	string filename = "test.tga";
	img->WriteImage(filename);
	
	string command = "open " + filename;
system(command.c_str());

	return 0;	
}
