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

//Handles samples
class SAMPLER
{
private:
	float w, h;
	float indexX, indexY;
	int AA; //Anti-aliasing

	//Only for progress
	float previousPerc;

public:
	void construct(int w, int h, int aa)
	{
		this->w = (float)w;
		this->h = (float)h;

		if(!(aa % 2) && aa > 0)
			this->AA = aa;
		else
			this->AA = 1;

		indexX = 0;
		indexY = 0;

		previousPerc = 0;
	}

	void reset()
	{
		indexX = 0;
		indexY = 0;
	}

	//Gets a new sample. From pixel ranges: [0,x), [0,y) 
	bool generateSample(Sample &sample)
	{
		if(indexY*w + indexX < w*h)
		{
			sample.x = indexX;
			sample.y = indexY;
			
			indexX += 1.0f/AA;
			
			if((indexX / w) >= 1)
			{
				indexY += 1.0f/AA;
				indexX = 0;
			}
			

			if(indexY*w + indexX < w*h)
			{ 
				//TODO: This thing needs updating, because it does not represent percents correctly.
				float newPerc = ((indexY*w + indexX) / (w*h) * 100);
				bool state = fabs(newPerc - previousPerc) > 0.009f;
				if(fabs(newPerc - previousPerc) > 0.009f)
				{
					printf("%.2f%%\n", newPerc);
					previousPerc = newPerc;
				}
				return true;
			}
			else
			{
				printf("Done - 100%%\n");
				return false;
			}
		}
		else
			return false;
	}
};




