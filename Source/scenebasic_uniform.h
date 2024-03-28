#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include "helper/glutils.h"
#include "helper/glslprogram.h"
#include "helper/objmesh.h"

#include <GLFW/glfw3.h>


class SceneBasic_Uniform : public Scene
{
private:

    GLuint vaoHandle;
    GLSLProgram prog;
    float angle;

    const int FrameRate = 120;
    const int FOV = 70;

    const glm::vec3 FogColor = glm::vec3(0.8f, 0.8f, 0.8f);
    const float FogStartDist = 5.0f;
    const float FogEndDist = 30.0f;
    const float HazeStrength = 0.07f; // Sort of like a scene tint
    const glm::vec3 HazeColor = glm::vec3(0.9f, 1.0f, 1.0f); // very slightly blue
    const glm::vec3 LightPosition = glm::vec3(99.0f, 99.0f, 99.0f);
    const glm::vec3 LightColor = glm::vec3(0.6f, 0.6f, 0.6f);

    const float Brightness = 1.0f;
    const float Reflectance = 0.04f;

    float MovementDistance = 4.0f; //boat stuff
    float SpeedMultiplier = 3.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float startTime = glfwGetTime();

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
