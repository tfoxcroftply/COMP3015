#version 460 core

in vec3 Color;
in vec3 Normal;
in vec3 TexCoords;
in vec3 FragPosition;

out vec4 FragColor;

uniform bool SkyboxActive;
uniform samplerCube skybox;

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
        vec3 FlipCoords = vec3(-TexCoords.x, TexCoords.y, TexCoords.z); // stbi seems to load it in the wrong way
        FragColor = texture(skybox, FlipCoords);
    } else {
        //vec3 Norm = normalize(Normal);
        //vec3 LightDirection = normalize(LightPosition - vec3(FragPosition));
       // float Diffuse = max(dot(Norm, LightDirection), 0.0);
        //vec3 DiffuseWithColor = Diffuse * LightColor;

       // vec3 ObjectWithAmbient = mix(Color, AmbientColor, AmbientStrength); // repurposed to act like a haze
       // vec3 ObjectAndDiffuse = Color + DiffuseWithColor;

        //FragColor = vec4(ObjectAndDiffuse, 1.0);

        float FogFactor = smoothstep(FogStart, FogEnd, length(FragPosition.xyz - CameraPos));

        vec3 Ambient = (Brightness * LightColor);

        vec3 LightDirection = normalize(LightPosition - FragPosition); // Derived from learnopengl
        float Diff = max(dot(normalize(Normal), LightDirection), 0.0);
        vec3 Diffuse = Diff * LightColor;
        vec3 Final = (Ambient + Diffuse) * Color;

        vec3 FinalWithHaze = mix(Final, HazeColor, HazeStrength);

        vec3 CameraDirection = normalize(CameraPos - FragPosition);
        vec3 ReflectionVector = reflect(-CameraDirection, normalize(Normal));
        vec3 ReflectedColor = texture(skybox, ReflectionVector).rgb;
        vec3 FinalWithReflect = mix(FinalWithHaze, ReflectedColor, SetReflection);
        

        FragColor = vec4(mix(FinalWithReflect, FogColor, FogFactor), 0.2f); // Fog colour last as overlay
    }
}