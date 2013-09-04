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


#include "triangle.h"

#define EPSILON 0.000001

bool TRIANGLE::intersect(Ray &ray, float *thit, LocalGeo *in)
{
	//Line-plane intersection
	float t = (glm::dot(A, normal) - glm::dot(ray.pos, normal)) / glm::dot(ray.direction, normal);

	if(!(ray.t_min < t && t < ray.t_max))
		return false;


	if(t > 0)
	{
		//Line-triangle intersection
		//Intersection point
		vec3 P = ray.pos + ray.direction*t;

		//Ray-Triangle Intersection method taken from:
		//http://geomalgorithms.com/a06-_intersect-2.html
		vec3 v = B-A;
		vec3 u = C-A;
		vec3 w = P-A;

		//Weights
		float s1 = (dot(u, v)*dot(w, v) - dot(v, v)*dot(w, u)) / (pow(dot(u, v), 2) - dot(u, u)*dot(v, v));
		float t1 = (dot(u, v)*dot(w, u) - dot(u, u)*dot(w, v)) / (pow(dot(u, v), 2) -  dot(u, u)*dot(v, v));

		//The intersection point is in the triangle
		if(s1 >= 0 && s1 <= 1 &&
			t1 >= 0 && t1 <= 1 &&
			s1 + t1 <= 1)
		{
			*thit = t;
			in->normal = (distance(ray.pos, P+normal) < distance(ray.pos, P+-normal))? normal : -normal;
			in->pos = P;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool TRIANGLE::intersectP(Ray &ray)
{
	//Line-plane intersection
	float t = (glm::dot(A, normal) - glm::dot(ray.pos, normal)) / glm::dot(ray.direction, normal);

	if(!(ray.t_min < t && t < ray.t_max))
		return false;

	if(t > 0)
	{
		//Line-triangle intersection
		//Intersection point
		vec3 P = ray.pos + ray.direction*t;

		//Ray-Triangle Intersection method taken from:
		//http://geomalgorithms.com/a06-_intersect-2.html
		vec3 v = B-A;
		vec3 u = C-A;
		vec3 w = P-A;

		//Weights
		float s1 = (dot(u, v)*dot(w, v) - dot(v, v)*dot(w, u)) / (pow(dot(u, v), 2) - dot(u, u)*dot(v, v));
		float t1 = (dot(u, v)*dot(w, u) - dot(u, u)*dot(w, v)) / (pow(dot(u, v), 2) -  dot(u, u)*dot(v, v));

		//The intersection point is in the triangle
		if(s1 >= 0 && s1 <= 1 &&
			t1 >= 0 && t1 <= 1 &&
			s1 + t1 <= 1)
			return true;
		else
			return false;

		
	}
	else
		return false;
}


void TRIANGLE::transform(mat4 T)
{
	//Translating, scaling, rotating
	//Transform points
	vec4 A4 = vec4(A, 1);
	vec4 B4 = vec4(B, 1);
	vec4 C4 = vec4(C, 1);

	A = vec3((T*A4)/A4.w);
	B = vec3((T*B4)/B4.w);
	C = vec3((T*C4)/C4.w);

	//Transforming normal
	mat4 modifiedMatrix = transpose(inverse(T));
	vec4 transnormal = vec4(normal, 0.0f); //In computer graphics, vectors are defined by w = 0, points by w = 1
	transnormal = modifiedMatrix*transnormal;

	normal = normalize(vec3(transnormal));
}