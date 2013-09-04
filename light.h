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


class POINTLIGHT : public LIGHT
{
	vec3 color;
	vec3 pos;
	vec3 attenuation;
public:
	//POINTLIGHT(){}
	POINTLIGHT(vec3 pos, vec3 color, vec3 attenuation){ this->color = color; this->pos = pos; this->attenuation = attenuation;}

	void generateLightRay(LocalGeo &local, Ray *ray, vec3 *color);
	vec3 shade(vec3 eyepos, LocalGeo &local, BRDF &brdf);
};

class DIRECTIONALLIGHT : public LIGHT
{
	vec3 color;
	vec3 direction;
public:
	DIRECTIONALLIGHT(){}
	DIRECTIONALLIGHT(vec3 dir, vec3 color){ this->color = color; this->direction = normalize(-dir);}

	void generateLightRay(LocalGeo &local, Ray *ray, vec3 *color);
	vec3 shade(vec3 eyepos, LocalGeo &local, BRDF &brdf);
};