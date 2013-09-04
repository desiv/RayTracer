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


#include "camera.h"

void CAMERA::construct(vec3 eye, vec3 lookat, vec3 up, int w, int h, float fovy, float zn, float zf)
{
	this->eye = eye;
    this->lookat = lookat;
	this->up = up;
	this->w = w;
	this->h = h;
	zNear = zn;
	zFar = zf;

	if(distance(eye, lookat) == 0)
	{
		printf("Eye is too close to lookat location\n");
		exit(1);
	}

	vec3 lookatDirection = normalize(lookat - eye);
	
	vec3 upvector(0);
	upvector = cross(up, lookatDirection);
	upvector = cross(lookatDirection, upvector);
	upvector = normalize(upvector);

	this->up = upvector;

	//Getting a vector which is parallel to the plane
	vec3 crossprod = glm::cross(upvector, glm::normalize(lookatDirection));
	crossprod = normalize(crossprod);
	
	float fieldOfView = tan(radians(fovy));
	//float ratio = (float)w/(float)h;
	float ratio = (float)h/(float)w;

	//Constructing plane
	plane.UL = (upvector*ratio + crossprod/**ratio*/ + eye + lookatDirection*zn) * fieldOfView;  
	plane.UR = (upvector*ratio + -crossprod/**ratio*/ + eye + lookatDirection*zn) * fieldOfView;
	plane.LL = (-upvector*ratio + crossprod/**ratio*/ + eye + lookatDirection*zn) * fieldOfView;
	plane.LR = (-upvector*ratio + -crossprod/**ratio*/ + eye + lookatDirection*zn) * fieldOfView;
}


void CAMERA::generateRay(Sample &sample, Ray *ray, bool randomize)
{
	//Get magnitude of a plane
	vec3 unitX = plane.UR - plane.UL;
	vec3 unitY = plane.LL - plane.UL;

	//Normalize with image's h and w
	unitX = unitX/(float)w;
	unitY = unitY/(float)h;

	vec3 offsetX(0);
	vec3 offsetY(0);

	//Randomizes where the ray hits the pixel
	if(randomize)
	{
		offsetX = (unitX*(rand()/(float)RAND_MAX));
		offsetY = (unitY*(rand()/(float)RAND_MAX));
	}
	//Otherwise the ray will hit the pixel at the center
	else
	{
		offsetX = unitX/2.0f;
		offsetY = unitY/2.0f;
	}

	vec3 pos = plane.UL + (unitX * (float)sample.x) + offsetX + 
	   					  (unitY * (float)sample.y) + offsetY;

	ray->pos = eye;
	ray->direction = normalize(pos - eye);
	ray->t_min = 0.0f;//zNear;
	ray->t_max = zFar;
}

