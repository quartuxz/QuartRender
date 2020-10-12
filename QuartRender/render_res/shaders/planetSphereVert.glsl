
//these are placeholders
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP * position;
}