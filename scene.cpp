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

#include "scene.h"

SCENE::SCENE(const char* input)
{	
	//Initialize default values
	width = 480;
	height = 320;
	zNear = 1.8f;
	zFar = 1000.0f;

	//Don't make eye and lookat at the same location
	eye = vec3(0, 0, 0);
	lookat = vec3(0, 0, -1);
	up = vec3(0, 1, 0);

	attenuation = vec3(1, 0, 0);

	outputFileName = "";
	reflectionDepth = 1;
	antialiasing = 1;
	repeat = 1; 
	randomize = false;
	
	try{
		(readFile(input));
	}
	catch(int e)
	{
		if(e == 2)
			exit(1);
	}
	
	if(outputFileName.empty())
		outputFileName = "output.png";

	//Initialize necessary classes
	Sampler.construct(width, height, antialiasing);
	Film.construct(width, height);
	RayTracer.construct(&primitives, &lights, reflectionDepth);
	Camera.construct(eye , lookat, up, width, height, fovy, zNear, zFar);

}

SCENE::~SCENE()
{
	//Delete Objects
	std::vector<PRIMITIVE*>::iterator pIT = primitives.begin();
	while(pIT != primitives.end())
	{
		PRIMITIVE *sp = *pIT;
		pIT = primitives.erase(pIT);
		delete sp;
	}

	//Delete Lights
	std::vector<LIGHT*>::iterator lIT = lights.begin();
	while(lIT != lights.end())
	{
		LIGHT *lp = *lIT;
		lIT = lights.erase(lIT);
		delete lp;
	}
}


void SCENE::render()
{
	Sample sample;
	Ray ray;
	vec3 color(0, 0, 0);

	//How many times to render the scene. (Used for sloppy anti-aliasing)
	for(int i = 0; i < repeat; i++)
	{
		Sampler.reset();
		//Loops through all pixels, thus rendering whole scene
		while(Sampler.generateSample(sample))
		{
			Camera.generateRay(sample, &ray, randomize);
			RayTracer.trace(ray, 0, &color);
			Film.commit(sample, color);
		}
	}

	Film.writeImage(outputFileName.c_str());
}

// Function to read the input data values 
bool SCENE::readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}


void SCENE::rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

void SCENE::readFile(const char *filename)
{
	string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {

        BRDF brdf;
		stack <mat4> transfstack;	  //Transformation stack
		transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) 
			{
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd; 
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "directional") {
                    validinput = readvals(s, 6, values); // Position/color for lts.
                    if (validinput) 
						lights.push_back(new DIRECTIONALLIGHT(vec3(values[0],  values[1], values[2]), vec3(values[3], values[4], values[5])));
                    
                } else if (cmd == "point") {
                    validinput = readvals(s, 6, values); // Position/color for lts.
                    if (validinput) 
						lights.push_back(new POINTLIGHT(vec3(values[0],  values[1], values[2]), vec3(values[3], values[4], values[5]), attenuation));
                    
				} else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values); // Position/color for lts.
                    if (validinput) 
						this->attenuation = vec3(values[0], values[1], values[2]);
                    
				}

				//Material commands
                else if (cmd == "ambient"){
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput)
						brdf.ka = vec3(values[0], values[1], values[2]);

                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) 
						brdf.kd = vec3(values[0], values[1], values[2]);

                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) 
						brdf.ks = vec3(values[0], values[1], values[2]);
                    
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) 
						brdf.ke = vec3(values[0], values[1], values[2]);

                } else if (cmd == "reflection") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) 
						brdf.kr = vec3(values[0], values[1], values[2]);

                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) 
						brdf.shine = values[0];
				} 
				
				//Camera's commands
				else if (cmd == "size") {
                    validinput = readvals(s,2,values); 
                    if (validinput) {
						width = (int)values[0];
						height = (int)values[1];
					}
                     
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {

						eye = vec3(values[0], values[1], values[2]); 
						lookat = vec3(values[3], values[4], values[5]); 
						up = vec3(values[6], values[7], values[8]);
						fovy = values[9]; 
                    }
                
				} else if (cmd == "render_repeat") {
					validinput = readvals(s, 1, values);
					if(validinput)
						repeat = (int)values[0];
				
				} else if (cmd == "render_randomize") {
					validinput = readvals(s, 1, values);
					if(validinput)
						randomize = values[0] != 0;  //With != expression i'm getting rid of the performance warning in msvc
				}

              
				//Loading primitives
                else if (cmd == "vertex") {
                   validinput = readvals(s, 3, values); 
                    if (validinput)
						vertices.push_back(vec3(values[0], values[1], values[2]));

				
				} else if (cmd == "sphere") {
                   validinput = readvals(s, 4, values); 
                    if (validinput)
					{
			
						brdf.kr = brdf.ks;
						mat4 &M = transfstack.top();
						if(M[0].x == M[1].y && M[1].y == M[2].z)
							primitives.push_back(new GEOMETRICPRIMITIVE(new SPHERE(vec3(values[0], values[1], values[2]), values[3]), MATERIAL(brdf), transfstack.top()));
						else
							primitives.push_back(new GEOMETRICPRIMITIVE(new ELIPSOID(vec3(values[0], values[1], values[2]), values[3], transfstack.top()), MATERIAL(brdf), mat4(1)));
					}

                } else if (cmd == "ellipsoid") {
                   validinput = readvals(s, 4, values); 
				   if (validinput)
					   primitives.push_back(new GEOMETRICPRIMITIVE(new ELIPSOID(vec3(values[0], values[1], values[2]), values[3], transfstack.top()), MATERIAL(brdf), mat4(1)));
                    

                } else if (cmd == "tri") {
                   validinput = readvals(s, 3, values); 
                    if (validinput)
							primitives.push_back(new GEOMETRICPRIMITIVE(new TRIANGLE(vertices[(unsigned int)values[0]], vertices[(unsigned int)values[1]], vertices[(unsigned int)values[2]]), MATERIAL(brdf), transfstack.top()));
				}


				//Matrix commands
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput)
						rightmultiply( translate(mat4(1), vec3(values[0], values[1], values[2])), transfstack );
					
                    

                } else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput)
						rightmultiply(scale(mat4(1), vec3(values[0], values[1], values[2])), transfstack);
				

				} else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) 
					{
						//rightmultiply(rotate(mat4(1), radians(values[3]), /*normalize(*/vec3(values[0], values[1], values[2]/*)*/)), transfstack);
						mat4 &T = transfstack.top();
						float rotatef = values[3];
						T = rotate(T, rotatef, vec3(values[0], values[1], values[2]));
					}
					
                }


                // The basic push/pop code for matrix stacks
                else if (cmd == "pushTransform" || cmd == "push") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform" || cmd == "pop") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }


				else if (cmd == "maxdepth"){
					validinput = readvals(s,1,values); 
                    if (validinput)
						reflectionDepth = (int)values[0];

				}else if (cmd == "output"){
					s >> outputFileName;
				
				}else if (cmd == "antialiasing"){
					validinput = readvals(s,1,values); 
                    if (validinput)
						antialiasing = (int)values[0];
				}

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 

		}
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
