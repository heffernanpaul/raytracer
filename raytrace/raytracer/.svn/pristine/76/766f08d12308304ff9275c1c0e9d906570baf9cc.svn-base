#ifndef __RAY_CAST__
#define __RAY_CAST__

#include "Tracer.h"
#include "Grid.h"

class RayCast: public Tracer {
	public:
		
		RayCast(void);
		
		RayCast(World* _worldPtr);

		RayCast(const RayCast& rc);
				
		virtual											
		~RayCast(void);		

		virtual RGBColor	
		trace_ray(const Ray& ray) const;

		virtual RGBColor	
		trace_ray(const Ray ray, const int depth) const;

};

#endif