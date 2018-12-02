#version 410 core

out vec4 out_color;
in vec3 vertexColor;
 
 void main() {
 	out_color = vec4(vertexColor, 1.0);
 }