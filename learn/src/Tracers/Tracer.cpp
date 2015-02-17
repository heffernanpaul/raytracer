#include "Tracer.h"
#include "Point2D.h"
#include "Ray.h"
#include "Timing.h"
#include "math.h"

TGAImage* Tracer::render(const Camera& camera, const ViewPlane& vp, const World& world) {

	TGAImage *img = new TGAImage(vp.getHRes(), vp.getVRes());


	Point2D 	pp;		// sample point on a pixel
	Vector3D direction;
	Ray ray;
	float halfWidth = 0.5f * vp.getHRes();
	float halfHeight = 0.5f * vp.getVRes();

	RGBColor rgb;
	ShadeRec sr(world);

//std::cout << "rendering" << std::endl;
    
    auto start = GetTime();
	// for each Row
	for(int r=0; r<vp.getVRes(); r++)
	{
		pp.y = r - halfHeight;

		// for each pixel in row
		for(int c=0; c<vp.getHRes(); c++)
		{
			pp.x = c - halfWidth;
			camera.getDirection(pp, ray.d);

			// does this ray Hit an object
			bool hit_an_object = world.hit_objects(ray, sr);

			if (hit_an_object) {

                rgb = sr.obj->getMaterial().shade(sr);

			} else {

				rgb = RGBColor::black;

			}
			img->setPixel(rgb,c,r);

		}
	}


	auto end = GetTime();
	auto diff = end - start;
	cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;

	return img;
}


