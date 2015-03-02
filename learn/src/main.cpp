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
#include <thread>
#include <vector>


int main(int argc, char** argv) {

    int numThreads = argc > 1 ? std::stoi(argv[1]) : 2;
    std::cout << "Running with " << numThreads << " threads." << std::endl;

    int numSamples = argc > 2 ? std::stoi(argv[2]) : 1;
    std::cout << "Num samples per pixel = " << numSamples << std::endl;

	ViewPlane vp(1200, 800, numSamples);
	Camera camera(Point3D(0 ,0, -100), 300);
	Tracer tracer;
	World world;
	
    std::vector<std::thread> threads;
    
    
    TGAImage *img = new TGAImage(vp.getHRes(), vp.getVRes());
    
    auto start = GetTime();

    int blockSize = vp.getVRes()/numThreads;
    for (int i = 0; i < numThreads; i++) {
        threads.push_back(std::thread([i,&camera,&vp,&img,blockSize, &world, &tracer](){
            tracer.render(camera, vp, world, img, i*blockSize, blockSize);
        }));
    }
    
    for(auto& thread : threads){
        thread.join();
    }
    auto end = GetTime();
    auto diff = end - start;
    cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
    
	string filename = "test.tga";
	img->WriteImage(filename);

}	
