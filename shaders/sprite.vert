#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 projection;
uniform mat4 transform;
uniform mat4 camera_view;

out vec2 texCoord;

void main()
{
	gl_Position = projection * camera_view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texCoord = aTex;
}