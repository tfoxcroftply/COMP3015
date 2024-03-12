#version 460 core

in vec3 Color;
in vec3 Normal;
in vec3 TexCoords;
in vec3 FragPosition;

out vec4 FragColor;

uniform bool SkyboxActive;
uniform samplerCube skybox;

uniform vec3 CameraPos;
uniform vec3 FogColor;
uniform float FogStart;
uniform float FogEnd;

uniform vec3 LightPosition;
uniform vec3 LightColor;

uniform float AmbientStrength;
uniform vec3 AmbientColor;

void main() {
    if (SkyboxActive) {
        vec3 FlipCoords = vec3(-TexCoords.x, TexCoords.y, TexCoords.z);
        FragColor = texture(skybox, FlipCoords);
    } else {
        //vec3 Norm = normalize(Normal);
        //vec3 LightDirection = normalize(LightPosition - vec3(FragPosition));
       // float Diffuse = max(dot(Norm, LightDirection), 0.0);
        //vec3 DiffuseWithColor = Diffuse * LightColor;

       // vec3 ObjectWithAmbient = mix(Color, AmbientColor, AmbientStrength); // repurposed to act like a haze
       // vec3 ObjectAndDiffuse = Color + DiffuseWithColor;

        //float FogFactor = smoothstep(FogStart, FogEnd, length(FragPosition.xyz - CameraPos));

        //FragColor = vec4(ObjectAndDiffuse, 1.0);

        vec3 Ambient = AmbientStrength * LightColor;

        vec3 LightDirection = normalize(LightPosition - FragPosition);  
        float Diff = max(dot(normalize(Normal), LightDirection), 0.0);
        vec3 Diffuse = Diff * LightColor;
        vec3 Final = (Ambient + Diffuse) * Color;

        FragColor = vec4(Final, 1.0);
    }
}