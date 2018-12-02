#version 410 core

layout (location = 0) in vec3 v;
layout (location = 1) in vec3 color;

out vec3 vertexColor;

uniform mat4 projection;

void main()
{    
    gl_Position = projection * vec4(v.x, v.y, 0.0, 1.0);   
    vertexColor = color;
};