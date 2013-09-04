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


#include "elipsoid.h"


bool ELIPSOID::intersect(Ray &ray, float *thit, LocalGeo *local)
{
	//Transforming ray into object's local coordinates
	Ray R = ray*worldToObj;
	LocalGeo L;
	
	if(!SPHERE::intersect(R, thit, &L))
		return false;

	//Transforming intersection points into world's coordinates
	*local = L*objToWorld;
	
	//Calculating distance
	vec3 pos = local->pos - ray.pos;
	*thit = glm::distance(pos, vec3(0, 0, 0));



	return true;
}

bool ELIPSOID::intersectP(Ray &ray)
{
	//Transforming ray into object's local coordinates
	Ray R = ray*worldToObj;
	return SPHERE::intersectP(R);
}


void ELIPSOID::transform(mat4 T)
{
	SPHERE::transform(T);
}