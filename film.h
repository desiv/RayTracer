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
#include "FreeImage.h"
#include <stack>

#pragma comment(lib, "lib/FreeImage.lib")

class FILM
{
private:

	int w, h;
	std::stack<vec3> *pixels;

public:

	FILM()
	{
		w = 0; h = 0;
		pixels = NULL;
	}

	~FILM()
	{
		for(int i = 0; i < w*h; i++)
			pixels[i].empty();

		if(pixels != NULL)
			delete [] pixels;
	}

	//Initialize
	void construct(int w, int h)
	{
		this->w = w;
		this->h = h;

		pixels = new std::stack<vec3>[w*h];
	}

	//Put a color in a bucket of colors
	void commit(Sample &sample, vec3 &color)
	{
		if(pixels != NULL && (sample.x * sample.y <= w*h))
		{
			int x = (int)floor(sample.x);
			int y = (int)floor(sample.y);
			pixels[y*w+x].push(color);
		}
	}

	//Finalizes image and produces png
	void writeImage(const char* name)
	{
		BYTE *pix = new BYTE[3*w*h];	
		FreeImage_Initialise();

		//Counting pixels
		for(int i = 0; i < w*h; i++)
		{

			pix[i*3] = 0;
			pix[i*3+1] = 0;
			pix[i*3+2] = 0;

			double size = pixels[i].size();

			float r = 0;
			float g = 0;
			float b = 0;

			//Converting to byte data
			//if colors are in range [0, 1]
			while(!pixels[i].empty())
			{
				r += (int)(255 * pixels[i].top().z);
				g += (int)(255 * pixels[i].top().y);
				b += (int)(255 * pixels[i].top().x);

				pixels[i].pop();
			}

			pix[i*3] = (BYTE)(r / size);
			pix[i*3+1] = (BYTE)(g / size);
			pix[i*3+2] = (BYTE)(b / size);
		}

		FIBITMAP *img = FreeImage_ConvertFromRawBits(pix, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

		FreeImage_Save(FIF_PNG, img, name, 0);

		printf("\nImage saved\n");

		delete pix;
		FreeImage_DeInitialise();
	}
};