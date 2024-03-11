#version 460 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 TexCoords;
out vec3 Color;

uniform mat4 ModelIn;
uniform mat4 ViewIn;
uniform mat4 ProjectionIn;
uniform bool SkyboxActive;

void main() {
    if (!SkyboxActive) {
        Color = VertexColor;
        gl_Position = ProjectionIn * ViewIn * ModelIn * vec4(VertexPosition, 1.0);
    } else {
        TexCoords = VertexPosition;
        mat4 viewWithoutTranslation = mat4(mat3(ViewIn));
        gl_Position = ProjectionIn * viewWithoutTranslation * ModelIn * vec4(VertexPosition, 1.0);
    }
}