#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"

class MultiJittered: public Sampler {
	public:
				
		MultiJittered(const int num_samples, const int m);	

		virtual
		~MultiJittered(void);								
		
	private:
		
		virtual void										
		generate_samples(void);		
};

