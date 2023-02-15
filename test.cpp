#include <iostream>
#include<glad/glad.h>
#include <gl/GL.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "helper.h"

struct shader_exception {
	const GLuint ID;
	const std::string what;
	shader_exception(GLuint ID, std::string what):ID(ID),what(what) {

	}
};

struct ComputeShader {
	GLuint ID;
	ComputeShader(std::string source) {
		ID=glCreateShader(GL_COMPUTE_SHADER);
		
		const char* shaderSource = source.c_str();
		glShaderSource(ID, 1, &shaderSource, NULL);
		glCompileShader(ID);
		if (EnCounterShaderError(ID)) {
			ID = 0;
			return;
		}
	}

	bool IsValidShader() {
		return ID != 0;
	}

};

struct GPUProgram {
	GLuint ID;
	GPUProgram() {
		ID = glCreateProgram();
	}

	void Link() {
		glLinkProgram(ID);
		CheckForProgramError(ID);
	}

	void AttachShader(ComputeShader& shader) {
		if (!shader.IsValidShader())throw shader_exception(shader.ID,"Invalid shader!");

	}
};


int main(const int argc,const char** argv){
	
	GLuint program = glCreateProgram();
	return 0;
}