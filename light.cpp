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


#include "light.h"

void POINTLIGHT::generateLightRay(LocalGeo &local, Ray *ray, vec3 *color)
{
	vec3 lightdir = normalize(this->pos - local.pos);
	
	ray->pos = local.pos + local.normal*CALC_ERROR_FIX;
	ray->direction = lightdir;

	ray->t_min = 0.0f;
	ray->t_max = distance(ray->pos, this->pos) + ray->t_min;

	memcpy(color,  &this->color, sizeof(vec3));
}

vec3 POINTLIGHT::shade(vec3 eyepos, LocalGeo &local, BRDF &brdf)
{
	vec3 direction = normalize(this->pos - local.pos); 
	vec3 eyedir = normalize(eyepos - local.pos);
	vec3 half = normalize (direction + eyedir); 

	float distance = glm::distance(local.pos, this->pos);
	vec3 attenuatedcolor = color / ((1/ (pow(distance, 2))*attenuation.z + distance*attenuation.y + attenuation.x));
	
	return ComputeLight(direction, attenuatedcolor, local.normal, half, brdf.kd, brdf.ks, brdf.shine) ;
}

void DIRECTIONALLIGHT::generateLightRay(LocalGeo &local, Ray *ray, vec3 *color)
{
	vec3 lightdir = normalize(this->direction);
	
	ray->pos = local.pos + local.normal*CALC_ERROR_FIX;
	ray->direction = lightdir;

	ray->t_min = 0.0f;
	ray->t_max = 1000.0f; 	//Infinite

	memcpy(color,  &this->color, sizeof(vec3));
}

vec3 DIRECTIONALLIGHT::shade(vec3 eyepos, LocalGeo &local, BRDF &brdf)
{
	vec3 dir = normalize(direction); // no attenuation 
	vec3 eyedir = normalize(eyepos - local.pos);
	vec3 half = normalize (dir + eyedir);  
	return ComputeLight(dir, this->color, local.normal, half, brdf.kd, brdf.ks, brdf.shine) ;
}