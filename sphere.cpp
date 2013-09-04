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

#include "sphere.h"

bool SPHERE::intersect(Ray &ray, float *thit, LocalGeo *local)
{
	//Line-sphere intersection

	vec3 P0 = ray.pos;
	vec3 P1 = ray.direction;


	//Solving quadratic equation
	float a = dot(ray.direction, ray.direction);
	float b = 2 * (dot(ray.direction, (ray.pos - center)));
	float c = dot(ray.pos - center, ray.pos - center) - pow(radius, 2);

	float D = pow(b, 2) - (4*a*c);

	//Ray hit sphere
	if(D > 0)
	{
	
		float t1 = (-b + sqrt(D)) / (2 * a);
		float t2 = (-b - sqrt(D)) / (2 * a);

		float t = 0;

		//Intersections are in opposite direction of the direction vector.
		//Starting point is outside and going in opposite direction of sphare.
		if(t1 < 0 && t2 < 0)
			return false;

		//Ray is inside the sphere
		if(t1 < 0 || t2 < 0)
		{
			if(t1 > 0)
				t = t1;
			else
				t = t2;
		}
		//Get the smalles (closest) value from those 2 points
		else if(t1 < t2)
			t = t1;

		else if(t2 < t1)
			t = t2;
	

		//Hit points are not in range
		if(t < ray.t_min || t > ray.t_max)
			return false;

		*thit = t;
		local->pos = P0 + P1*t;
		local->normal = normalize(local->pos - center);

		return true;
	}
	//Only interseted at one point (ray is touching sphere).
	else if(D == 0)
	{
		float t = -dot(P1, P0 - center);
		*thit = t;
		local->pos = P0 + P1*t;
		local->normal = normalize(local->pos - center);
		
		return true;
	}
		
	return false;
}

bool SPHERE::intersectP(Ray &ray)
{
	float max = ray.t_max;
	vec3 pos = ray.pos;
	vec3 pos0 = ray.pos;
	vec3 dir = ray.direction;

	vec3 P0 = pos;
	vec3 P1 = dir;

	//Solving quadratic equation
	float D = (dot(P1, P0 - center)*dot(P1, P0 - center)) - dot(P0 - center, P0 - center)+(radius*radius);

	//Ray hit sphere
	if(D >= 0)
	{
		float t1 = (-dot(P1, P0 - center) - glm::sqrt(D));
		float t2 = (-dot(P1, P0 - center) + glm::sqrt(D));

		float t = 0;
		
		//If both hit points are above zero distance
		if(t1 > 0 && t2 > 0)
			t = t1 < t2 ? t1 : t2;

		//Only if one point is above zero distance
		else if (t1 > 0 || t2 > 0)
			t = t1 > 0 ? t1 : t2;

		//Both points are behind ray
		else
			return false;


		//Intersections are in opposite direction of the direction vector.
		//Starting point is outside and going in opposite direction of sphare.
		if(ray.t_min < t && t < ray.t_max)
			return true;
		else
			return false;
	}
	else
		return false;
}


void SPHERE::transform(mat4 T)
{
	//Translating, scaling, rotating object
	float scaleX = T[0].x;
	float scaleY = T[1].y;
	float scaleZ = T[2].z;

	if(scaleX == scaleY && scaleY == scaleZ)
		radius = radius * scaleX;
	else
		//Get the biggest weight
		radius = (scaleX > scaleY)? ((scaleX > scaleZ)? scaleX : scaleZ) : ((scaleY > scaleZ)? scaleY : scaleZ);

	center = vec3(T*vec4(center, 1));
}