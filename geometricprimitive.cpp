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

#include "geometricprimitive.h"

GEOMETRICPRIMITIVE::GEOMETRICPRIMITIVE(SHAPE *shp, MATERIAL mat, mat4 transformation)
{
	this->shape = shp;
	this->mat = mat;
	this->shape->transform(transformation);
}

void GEOMETRICPRIMITIVE::getBDRF(LocalGeo &local, BRDF *brdf)
{
	mat.getBRDF(local, brdf);
}

bool GEOMETRICPRIMITIVE::intersect(Ray &ray, float *thit, Intersection *in)
{
	if(!shape->intersect(ray, thit, &in->local))
		return false;

	in->primitive = this;
	return true;
}

bool GEOMETRICPRIMITIVE::intersectP(Ray &ray)
{
	if(shape->intersectP(ray))
		return true;
	else
		return false;
}