#version 460 core

in vec2 TexCoords; 
in vec3 SkyboxCoords;
in vec3 Color;
in vec3 Normal;
in vec3 FragPosition;

out vec4 FragColor;

uniform bool SkyboxActive;
uniform samplerCube skybox;
uniform sampler2D Texture; 

uniform vec3 FogColor;
uniform float FogStart;
uniform float FogEnd;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float HazeStrength;
uniform vec3 HazeColor;
uniform float Brightness;

uniform vec3 CameraPos;
uniform float SetReflection;


void main() {
    if (SkyboxActive) {
        vec3 FlipCoords = vec3(-SkyboxCoords.x, SkyboxCoords.y, SkyboxCoords.z);
        FragColor = texture(skybox, FlipCoords);
    } else {
        vec2 FlipCoords = vec2(TexCoords.x, -TexCoords.y);
        float FogFactor = smoothstep(FogStart, FogEnd, length(FragPosition.xyz - CameraPos));

        vec3 Ambient = (Brightness * LightColor);

        vec3 LightDirection = normalize(LightPosition - FragPosition);
        float Diff = max(dot(normalize(Normal), LightDirection), 0.0);
        vec3 Diffuse = Diff * LightColor;
        vec3 Final = (Ambient + Diffuse) * texture(Texture, FlipCoords).rgb;

        vec3 FinalWithHaze = mix(Final, HazeColor, HazeStrength);

        vec3 CameraDirection = normalize(CameraPos - FragPosition);
        vec3 ReflectionVector = reflect(-CameraDirection, normalize(Normal));
        vec3 ReflectedColor = texture(skybox, ReflectionVector).rgb;
        vec3 FinalWithReflect = mix(FinalWithHaze, ReflectedColor, SetReflection);
        

        FragColor = vec4(mix(FinalWithReflect, FogColor, FogFactor), 0.2f);
        //FragColor = texture(Texture,FlipCoords);
    }
}