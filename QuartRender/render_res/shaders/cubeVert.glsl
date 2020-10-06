#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

//DEPRECATED:
//colors I picked at my whim
//const vec3 vertexColors[] = vec3[](
//	vec3(0.5f,0.5f,0.5f),
//	vec3(0.3,0.6f,0.9f),
//	vec3(0.1f,0.3f,0.1f),
//	vec3(0.2f,0.3f,0.4f),
//	vec3(0.9f,0.8f,0.5f),
//	vec3(0.2f,0.2f,0.3f)
//)


layout(location = 0) out vec3 v_color;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP * vec4(position,1.0f);
	v_color = color;
}