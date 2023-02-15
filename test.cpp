#include <iostream>
#include<glad/glad.h>
#include <gl/GL.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "helper.h"

struct ComputeShader {
	GLint ID;
	ComputeShader(std::string source) {
		
	}
};

struct GPUProgram {
	const GLint ID;
	GPUProgram():ID(glCreateProgram()) {}


};


int main(const int argc,const char** argv){
	if (gladLoadGL()!=0) return -1;

	return 0;
}