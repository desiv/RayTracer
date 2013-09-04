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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

//Minor calculation error weight
#define CALC_ERROR_FIX 0.0008f;

struct Ray 
{
	vec3 pos; //Position of the ray
	vec3 direction;
	float t_min, t_max;

	Ray(){}
	
	Ray operator*(mat4 &matrix)
	{
		Ray retRay(*this);
		//Point w = 1;
		vec4  tranpos = vec4(pos, 1.0f);
		tranpos = matrix*tranpos;

		retRay.pos = vec3(tranpos/tranpos.w);

		mat4 modifiedMatrix = matrix;
		//Vector w = 0;
		vec4 transdir = vec4(direction, 0.0f);
		transdir = modifiedMatrix*transdir;

		retRay.direction = normalize(vec3(transdir));

		return retRay;
	}
};

//Container for point and normal of a primitive
struct LocalGeo
{
	vec3 pos;
	vec3 normal;

	LocalGeo operator*(mat4 &matrix)
	{
		LocalGeo retLocal(*this);
		vec4  tranpos = vec4(pos, 1.0);
		tranpos = matrix*tranpos;
		
		retLocal.pos = vec3(tranpos/tranpos.w);

		mat4 modifiedMatrix = glm::transpose(glm::inverse(matrix));
		//Vector w = 0;
		vec4 transnormal = vec4(normal, 0.0f);
		transnormal = modifiedMatrix*transnormal;
		retLocal.normal = normalize(vec3(transnormal));

		return retLocal;
	} 
};

//Container corresponding x and y on the screen/image
struct Sample
{
	float x, y;
};

//Container for plane information
struct Plane
{
	vec3 UL, UR, LL, LR;

	Plane()
	{
		UR = vec3(0, 0, 0);
		UL = vec3(0, 0, 0);
		LR = vec3(0, 0, 0);
		LL = vec3(0, 0, 0);
	}

	Plane(vec3 ul, vec3 ur, vec3 ll, vec3 lr)
	{
		UL = ul;
		UR = ur;
		LL = ll;
		LR = lr;
	}
};

//Container for color information
struct BRDF
{
	vec3 kd, //diffuse
		 ks, //specular
		 ka, //ambient
		 kr, //mirror reflection coefficient
		 ke; //emission

	float shine;
};

struct Intersection;

//Class for primitives with virtual functions
class PRIMITIVE
{
public:
	//Returns information about colors
	virtual void getBDRF(LocalGeo &local, BRDF *brdf)=0;

	//Intersections with arbitrary primitive and ray
	virtual bool intersect(Ray &ray, float *thit, Intersection *in)=0;
	virtual bool intersectP(Ray &ray)=0;
};

//Container for intersection point/normal with pointer to primitive data
struct Intersection
{
	LocalGeo local;
	PRIMITIVE *primitive;
};

class SHAPE
{
public:
	//Check intersection between arbitrary primitive and ray
	virtual bool intersect(Ray &ray, float *thit, LocalGeo *in)=0;
	virtual bool intersectP(Ray &ray)=0;

	//Transforms arbitrary primitive accordingly
	virtual void transform(mat4 T)=0;
};

//Light class with virtual functions
class LIGHT
{
public:
	//Creates ray from arbitrary light
	virtual void generateLightRay(LocalGeo &local, Ray *ray, vec3 *color)=0;
	//Computes color from arbitrary light
	virtual vec3 shade(vec3 eyepos, LocalGeo &local, BRDF &brdf)=0;

	//Static class to compute color
	static vec3 ComputeLight (const vec3 direction, const vec3 lightcolor, const vec3 normal, const vec3 halfvec, const vec3 mydiffuse, const vec3 myspecular, const float myshininess) 
	{
        float nDotL = dot(normal, direction)  ;         
        vec3 lambert = mydiffuse * lightcolor * glm::max(nDotL, 0.0f) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec3 phong = myspecular * lightcolor * pow(glm::max(nDotH, 0.0f), myshininess) ; 

        vec3 retval = lambert + phong ; 
        return retval ;            
	}   
};
