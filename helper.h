#pragma once
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

std::string get_file_contents(const char* filename) {

	std::ifstream in(filename, std::ios::in);

	if (in) {
		std::string contents;

		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());

		in.seekg(0, std::ios::beg);

		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}

	throw(errno);
}

static bool EnCounterShaderError(GLuint shader) {
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	
	if (isCompiled != GL_FALSE) {
		return false;
	}

	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	std::vector<GLchar> errorLog(maxLength);

	glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
	for (GLchar c : errorLog) {
		std::clog << c;
	}
	std::clog << std::endl;
	glDeleteShader(shader);
	return true;
}


static void CheckForProgramError(GLuint program) {
	GLint linked = 0;

	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (linked != GL_FALSE) {
		return;
	}

	GLint maxLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);


	std::vector<GLchar> errorLog(maxLength);

	glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

	glDeleteProgram(program);

	throw std::runtime_error(&errorLog[0]);
}


template<typename T>
void setUniformValue(GLint loc, const T& value) {
	if constexpr (std::is_same_v<T, int>) {
		glUniform1i(loc, value);
	}
	else if constexpr (std::is_same_v<T, float>) {
		glUniform1f(loc, value);
	}
	else if constexpr (std::is_same_v<T, bool>) {
		glUniform1i(loc, value);
	}
	else if constexpr (std::is_same_v<T, glm::mat2>) {
		glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}
	else if constexpr (std::is_same_v<T, glm::mat3>) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}
	else if constexpr (std::is_same_v<T, glm::mat4>) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	} // Add additional cases for other types as needed
}
