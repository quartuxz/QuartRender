#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec2 v_texCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP*vec4(position.x, position.y, 1.0f,1.0f);
	v_texCoord = texCoord;
}