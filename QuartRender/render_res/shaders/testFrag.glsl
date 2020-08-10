#version 460 core


layout(location = 0) out vec4 color;

layout(location = 0) in vec2 v_texCoord;

uniform vec4 u_color;
uniform sampler2D u_tex;

void main(){
	vec4 texColor = texture(u_tex, v_texCoord);
	color = texColor*u_color;
}