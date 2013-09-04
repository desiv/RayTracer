//The MIT License (MIT)
//
//Copyright (c) 2013 Marius Savickas
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#pragma once
#include "common.h"

class MATERIAL
{
	BRDF brdf;

public:
	MATERIAL(){}

	MATERIAL(vec3 ka, vec3 kd, vec3 ks, vec3 kr, float shine)
	{
		brdf.ka = ka;
		brdf.kd = kd;
		brdf.ks = ks;
		brdf.kr = kr;
		brdf.shine = shine;
	}

	MATERIAL(BRDF brdf)
	{
		this->brdf = brdf;
	}

	void getBRDF(LocalGeo &local, BRDF *brdf)
	{
		memcpy(brdf, &(this->brdf), sizeof(BRDF));
	}
};