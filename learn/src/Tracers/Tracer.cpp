#include "Tracer.h"
#include "Point2D.h"
#include "Ray.h"
#include "Timing.h"
#include "math.h"

TGAImage* Tracer::render(const Camera& camera, const ViewPlane& vp, const World& world, TGAImage *img,
                         int startRow, int blockSize) {



	Point2D 	pp;		// sample point on a pixel
	Vector3D direction;
	Ray ray;
	float halfWidth = 0.5f * vp.getHRes();
	float halfHeight = 0.5f * vp.getVRes();

	RGBColor rgb;
	ShadeRec sr(world);

//std::cout << "rendering" << std::endl;
    
    int n = (int)sqrt((float)vp.getNumSamples());
    int depth = 3;
	// for each Row
	for(int r=startRow; r<(startRow+blockSize); r++)
	{
        if (r%100 == 0) {
            std::cout << "Tracing row " << r << " of " << vp.getVRes() << std::endl;
        }
		pp.y = r - halfHeight;

		// for each pixel in row
		for(int c=0; c<vp.getHRes(); c++)
		{
            rgb = RGBColor::black;
            
            for (int p = 0; p < n; p++)	 {		// up pixel
                for (int q = 0; q < n; q++) {	// across pixel
                    pp.x = (c - (0.5f * vp.getHRes()) + (q + 0.5f) / n);
                    pp.y = (r - (0.5f * vp.getVRes()) + (p + 0.5f) / n);

                    camera.getDirection(pp, ray.d);

                    // does this ray Hit an object
                    bool hit_an_object = world.hit_objects(ray, sr);

                    if (hit_an_object) {
                        sr.depth = depth;
                        rgb += sr.obj->getMaterial().area_light_shade(sr);

                    } else {

                        rgb = RGBColor::black;

                    }
                }
            }
            rgb /= (float) vp.getNumSamples();
            rgb = rgb.clamp ();

			img->setPixel(rgb,c,r);

		}
	}



	return img;
}


