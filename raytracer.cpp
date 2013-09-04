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

#include "raytracer.h"

RAYTRACER::RAYTRACER(){}

void RAYTRACER::construct(std::vector<PRIMITIVE*> *o, std::vector<LIGHT*> *lights, int reflecThreshold)
{ 
	this->reflectionThreshold = reflecThreshold;
	this->objects = o; 
	this->lights = lights;
	numLights = lights->size();
}

void RAYTRACER::generateReflectedRay(Ray &incomingRay, LocalGeo &local, Ray *reflectedRay)
{
	//Specular reflection
	reflectedRay->t_min = 0;
	reflectedRay->t_max = incomingRay.t_max;
	reflectedRay->pos = local.pos + local.normal*CALC_ERROR_FIX;
	reflectedRay->direction = -(2.0f * local.normal * dot(local.normal, incomingRay.direction) - incomingRay.direction); 
}

void RAYTRACER::trace(Ray &ray, int depth, vec3 *color)
{
	//Recursion exceeded
	if(depth > reflectionThreshold)
	{
		*color = vec3(0);
		return;
	}

	vec3 col = vec3(0, 0, 0);
	Intersection in;
	BRDF brdf;
	float prevthit = ray.t_max+1;

	//Iterate through shapes
	for(std::vector<PRIMITIVE*>::iterator it = objects->begin(); it != objects->end(); ++it)
	{
		vec3 lcolor(0);
		float thit = 0.0f;
		//Check for intersection
		if(!(*it)->intersect(ray, &thit, &in))
			continue;

		
		//Last object was closer
		if(prevthit < thit)
			continue;
		//Else continue with calculations
		else
			prevthit = thit;

		//Get information of a shape
		in.primitive->getBDRF(in.local, &brdf);

		col = brdf.ka;
		col += brdf.ke;
		//Loop through lights sources
		for(std::vector<LIGHT*>::iterator lit = lights->begin(); lit != lights->end(); ++lit)
		{
			Ray lray;
			(*lit)->generateLightRay(in.local, &lray, &lcolor);

			//Check if it's blocked
			bool block = false;
			for(std::vector<PRIMITIVE*>::iterator pit2 = objects->begin(); pit2 != objects->end(); ++pit2)
			{
				//If there is an intersection - make the color black, shadow
				if((*pit2)->intersectP(lray))
				{
					block = true;
					break;
				}
			}
			//Position is blocked, skipping color adding, thus returning black color
			if(!block)
			{
				col += (*lit)->shade(ray.pos, in.local, brdf);
			}
		}


		//Reflection
		if(brdf.kr.x + brdf.kr.y + brdf.kr.z > 0)
		{
			Ray refray;
			vec3 tmpcol = brdf.ka;

			generateReflectedRay(ray, in.local, &refray);
			trace(refray, depth+1, &tmpcol);

			col += brdf.kr * tmpcol;
		}

	}


	//Finalize color
	if(col.x < 1.0f)
		color->x = col.x;
	else 
		color->x = 1.0f;

	if(col.y <  1.0f)
		color->y = col.y;
	else 
		color->y = 1.0f;
	
	if(col.z <  1.0f)
		color->z = col.z;
	else 
		color->z = 1.0f;
}
