#include <iostream>
#include<glad/glad.h>
#include <gl/GL.h>
#include <cmath>
#include <GLFW/glfw3.h>
#include "helper.h"

void runWinProc(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void CreateOpenGLContext() {

	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1, 1, "YoutubeOpenGL", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	if (gladLoadGL() == GL_FALSE) {
		std::cout << "Can't load GL " << std::endl;

		return;
	}
	
	
}

void DestroyOpenGLContext() {
	glfwTerminate();
}

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

/*
* Barrier for all the things below except barrier
* memoryBarrier
* memoryBarrierAtomicCounter
* memoryBarrierImage
* memoryBarrierBuffer
* memoryBarrierShared
* 
* Create visibility for all unsynchronized memory operations
* groupMemoryBarrier
* Barrier for exclusion synchronization of invocations
* barrier
*/

template<typename T,size_t NELE=1>
struct LayoutBuffer {
	unsigned int index;
	unsigned int buffer;
	const size_t size= sizeof(T)*NELE;

	~LayoutBuffer() {
		glDeleteBuffers(1, &buffer);
	}
};

template<typename T,size_t NELE=1>
struct UniformBuffer {
	void* clientBuffer;
	unsigned int location;
	const size_t size = sizeof(T) * NELE;
};

struct GPUProgram {
	GLuint ID;
	bool isSafeRunning;
	bool isRunning;
	std::vector<ComputeShader> shaders;
	GPUProgram() {
		ID = glCreateProgram();
		isSafeRunning = false;
		isRunning = false;
	}

	~GPUProgram() {
		Delete();
	}

	void Link() {
		glLinkProgram(ID);
		CheckForProgramError(ID);
	}

	int GetWorkGroupCount(int index) {
		int wgcnt;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, index, &wgcnt);
		return wgcnt;
	}

	int GetWorkGroupSize(int index) {
		int wgsz;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, index, &wgsz);
		return wgsz;
	}

	int GetWorkGroupInv() {
		int wgi;
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &wgi);
		return wgi;
	}

	void Run(unsigned int xGroup, unsigned int yGroup, unsigned int zGroup) {
		std::cout << xGroup << "," << yGroup << "," << zGroup << std::endl;
		isRunning = true;
		glUseProgram(ID);
		glDispatchCompute(xGroup, yGroup, zGroup);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		isRunning = false;
	}

	void RunSafe() {
		isSafeRunning = true;
		glUseProgram(ID);
	}

	template<typename T,size_t NELE>
	void GetUniformLocation(const char* unfiromName,UniformBuffer<T,NELE>& outBuffer,void* dataToUpload) {
		if (shaders.empty())throw std::runtime_error("you haven't attached any shader");

		if (!isSafeRunning)throw std::runtime_error("gpu program isn't running in safe mode");

		GLuint location=glGetUniformLocation(ID, unfiromName);

		

	}

	void AttachShader(ComputeShader& shader) {
		if (!shader.IsValidShader())throw shader_exception(shader.ID,"Invalid shader!");


		shaders.push_back(shader);

		glAttachShader(ID, shader.ID);

		glDeleteShader(shader.ID);
	}

	template<typename T,size_t NELE=1>
	static void  CreateAndBindBuffer(int index, T* data,LayoutBuffer<T,NELE>& outBuffer) {
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T)*NELE, data, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		outBuffer.buffer = buffer;
		outBuffer.index = index;
	}

	template<typename T,size_t NELE=1>
	static void GetOutputBuffer(LayoutBuffer<T,NELE>& layout,T* outBuffer) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, layout.buffer);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, layout.size, outBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}


	template<typename T,size_t NELE=1>
	static void AssignDataToBuffer(LayoutBuffer<T, NELE>& layout, int offset, T* data,size_t customSize=sizeof(T)*NELE) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, layout.buffer);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, customSize, data);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

private:
	void Delete() {
		for (ComputeShader& shader : shaders) {
			glDetachShader(ID,shader.ID);
		}

		shaders.clear();
		glDeleteProgram(ID);
	}
};

void test1() {
	GPUProgram program;

	const char* source = "#version 430\n\
                      layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;\n\
                      layout(binding = 0) buffer InData { float a; float b; };\n\
                      layout(binding = 1) buffer OutData { float c; };\n\
                      void main() {\n\
                          c = a + b;\n\
                      }";

	float a = 1.0f;
	float b = 2.0f;
	float c = 0.0f;

	LayoutBuffer<float, 2> inputLayoutBuffer;
	LayoutBuffer<float> outputLayoutBuffer;

	GPUProgram::CreateAndBindBuffer(0, (float*)nullptr, inputLayoutBuffer);
	GPUProgram::CreateAndBindBuffer(1, (float*)nullptr, outputLayoutBuffer);

	GPUProgram::AssignDataToBuffer(inputLayoutBuffer, 0, &a, sizeof(float));
	GPUProgram::AssignDataToBuffer(inputLayoutBuffer, sizeof(float), &b, sizeof(float));

	ComputeShader shader(source);

	program.AttachShader(shader);
	program.Link();
	program.Run(1, 1, 1);



	GPUProgram::GetOutputBuffer(outputLayoutBuffer, &c);

	std::cout << c << std::endl;
}

void test2() {
	const char* computeShaderSource = R"(
								
#version 430

layout(local_size_x = 4, local_size_y = 4) in;

layout(std430, binding = 0) buffer BufferA {
    float A[];
};

layout(std430, binding = 1) buffer BufferB {
    float B[];
};

layout(std430, binding = 2) buffer BufferC {
    float C[];
};
layout (std430, binding =3 ) buffer Information{
 uint numColsA;
 uint numRowsA;
 uint numColsB;
};

void main() {
    uint row = gl_GlobalInvocationID.x;
    uint col = gl_GlobalInvocationID.y;

    float sum = 0.0;
    for (uint i = 0; i < numColsA; i++) {
        sum += A[row * numColsA + i] * B[i * numColsB + col];
    }

    C[row * numColsB + col] = sum;
}
)";

	GPUProgram program;

	ComputeShader computeShader(computeShaderSource);

	
	
	float a[] = {1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1,0};
	float b[] = { 1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1,1 };
	int colA=4;
	int rowA = 4;
	int colB=4;

	LayoutBuffer<float, 16> matrixALayout;
	LayoutBuffer<float, 16> matrixBLayout;
	LayoutBuffer<float, 16> outputCLayout;
	GPUProgram::CreateAndBindBuffer(0, a, matrixALayout);
	GPUProgram::CreateAndBindBuffer(1, b, matrixBLayout);
	GPUProgram::CreateAndBindBuffer(2, (float*)nullptr, outputCLayout);

	LayoutBuffer<int, 3> informationLayout;
	GPUProgram::CreateAndBindBuffer(3, (int*)nullptr, informationLayout);
	GPUProgram::AssignDataToBuffer(informationLayout, 0, &colA, sizeof(int));
	GPUProgram::AssignDataToBuffer(informationLayout, sizeof(float), &rowA, sizeof(int));
	GPUProgram::AssignDataToBuffer(informationLayout, 2*sizeof(float), &colB, sizeof(int));


	program.AttachShader(computeShader);

	program.Link();
	program.Run(4,4, 1);

	float* result = new float[16];
	GPUProgram::GetOutputBuffer(outputCLayout, result);

	for (int i = 0; i < 16; i++) {
		std::cout << result[i] << std::endl;
	}

}

int main(const int argc,const char** argv){
	
	CreateOpenGLContext();
	test1();

	test2();
	DestroyOpenGLContext();
	return 0;
}