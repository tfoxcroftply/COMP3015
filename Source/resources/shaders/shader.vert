#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;
uniform mat4 ModelIn;
uniform mat4 ViewIn;
uniform mat4 ProjectionIn;

void main()
{
    Color = VertexColor;
    gl_Position = ProjectionIn * ViewIn * ModelIn * vec4(VertexPosition, 1.0);
}
