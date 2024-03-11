#version 460 core

in vec3 Color;
in vec3 TexCoords;

out vec4 FragColor;

uniform bool SkyboxActive;
uniform samplerCube skybox;

void main() {
    if (SkyboxActive) {
        vec3 FlipCoords = vec3(-TexCoords.x, TexCoords.y, TexCoords.z); // couldn't flip with stbi
        FragColor = texture(skybox, FlipCoords);
    } else {
        FragColor = vec4(Color, 1.0);
    }
}
