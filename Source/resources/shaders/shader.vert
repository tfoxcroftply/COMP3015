#version 460 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexColor;

out vec3 TexCoords;
out vec3 Color;
out vec3 Normal;
out vec3 FragPosition;

uniform mat4 ModelIn;
uniform mat4 ViewIn;
uniform mat4 ProjectionIn;
uniform bool SkyboxActive;

void main() {
    if (!SkyboxActive) {
        Color = vec3(0.5f,0.5f,0.5f); //VertexColor;
        Normal = VertexNormal;
        FragPosition = vec3(ModelIn * vec4(VertexPosition, 1.0));
        gl_Position = ProjectionIn * ViewIn * ModelIn * vec4(VertexPosition, 1.0);
    } else {
        TexCoords = VertexPosition;
        gl_Position = ProjectionIn * mat4(mat3(ViewIn)) * vec4(VertexPosition, 1.0);
    }
}