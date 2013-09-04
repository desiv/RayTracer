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
#include <Windows.h>
#include <WinBase.h>
#include "sampler.h"
#include "film.h"
#include "raytracer.h"
#include "camera.h"
#include "common.h"
#include "sphere.h"
#include "elipsoid.h"
#include "triangle.h"
#include "material.h"
#include "light.h"
#include "geometricprimitive.h"
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class SCENE
{
private:
	SAMPLER Sampler;
	CAMERA Camera;
	RAYTRACER RayTracer;
	FILM Film;

	string outputFileName;

	int width;
	int height;
	float zNear;
	float zFar;
	float fovy;

	//Don't make eye and lookat at the same location
	vec3 eye;
	vec3 lookat;
	vec3 up;

	vec3 attenuation;

	int antialiasing;
	int reflectionDepth;
	int repeat;		//repeat the render arbitrary times
	bool randomize; //randomizes hit points of a pixels. False - will hit in the middle of a pixel.

	std::vector<PRIMITIVE*> primitives;
	std::vector<LIGHT*> lights;
	std::vector<vec3> vertices;

public:
	SCENE(const char* input);
	SCENE();
	~SCENE();

	void render();

	//File reading, helping fuctions
	void readFile(const char *filename);
	bool readvals (stringstream &s, const int numvals, float * values) ;
	void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
};