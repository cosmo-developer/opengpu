#version 460 core

layout (local_size_x=10,local_size_y=10,local_size_z=1) in;

layout (binding = 0) buffer readonly MatrixA{
	float data[];
} matrix_a;

layout (binding = 1) buffer readonly MatrixB{
	float data[];
} matrix_b;


layout (binding = 2) buffer readonly MatrixBColumnInfo{
	uint matrixBCols;
} matrix_b_info;

layout (binding = 3) buffer writeonly MatrixResult{
	float data[];
} matrix_result;

void main(){
	uvec2 globalThreadIndex=gl_GlobalInvocationID.xy;
	uvec2 localThreadIndex=gl_LocalInvocationID.xy;
	uvec2 matrixADimension=gl_NumWorkGroups.xy;
	uvec2 matrixBDimension=uvec2(gl_NumWorkGroups.y,matrix_b_info.matrixBCols);
	
}