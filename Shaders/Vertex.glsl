#version 410

layout (location = 0) in vec3 v;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(v.x, v.y, v.z, 1.0);
};