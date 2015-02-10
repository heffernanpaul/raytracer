#include "Tracer.h"
#include "Point2D.h"
#include "Ray.h"

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
	      c.b = 0; //(x * y) % 256;  
			img->setPixel(c,x,y);
		}
	}

}
TGAImage* Tracer::render(const Camera& camera, const ViewPlane& vp, const World& world) {

	TGAImage *img = new TGAImage(vp.getHRes(), vp.getVRes());

	Point2D 	pp;		// sample point on a pixel
	Vector3D direction;
	Ray ray;
	float halfWidth = 0.5f * vp.getHRes();
	float halfHeight = 0.5f * vp.getVRes();

	RGBColor rgb;
	ShadeRec sr;
	
	// for each Row
	for(int r=0; r<vp.getVRes(); r++)
	{
		pp.y = r - halfHeight;

		// for each pixel in row
		for(int c=0; c<vp.getHRes(); c++)
		{
			pp.x = c - halfWidth;
			ray.d = camera.getDirection(pp);

			// does this ray Hit an object
			bool hit_an_object = world.hit_objects(ray, sr);
			if (hit_an_object) {

			} else {
				rgb = RGBColor::black;

			}
			img->setPixel(rgb,r,c);

		}
	}

	return img;
}


