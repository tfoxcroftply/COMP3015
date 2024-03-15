#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>

#include <glm/ext/matrix_clip_space.hpp>

#include "scenebasic_uniform.h"
#include "helper/glutils.h"
#include "helper/glslprogram.h"
#include "helper/objmesh.h"

#include "Camera.h"
#include "Models.h"
#include "ObjectGen.h"

Camera camera;
GLFWwindow* mainWindow;

Model object;
Model skybox;
std::unique_ptr<ObjMesh> mesh;

using std::string;
using std::cerr;
using std::endl;
using std::cout;

using glm::vec3;
using glm::mat4;
using glm::scale;

static int FrameRate = 120;
static int FOV = 70;

static vec3 FogColor = vec3(0.8f, 0.8f, 0.8f);
static float FogStartDist = 1.0f;
static float FogEndDist = 10.0f;
static float HazeStrength = 0.07f; // Sort of like a scene tint
static vec3 HazeColor = vec3(0.9f, 1.0f, 1.0f); // very slightly blue
static vec3 LightPosition = vec3(1.0f, 1.0f, 1.0f);
static vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);

static float Brightness = 0.9f;
static float Reflectance = 0.04f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float startTime = glfwGetTime();

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

void mouse_callback(GLFWwindow* Window, double X, double Y) {
    camera.MouseInput(X, Y);
}

void SceneBasic_Uniform::initScene()
{
    compile();
    prog.printActiveUniforms();

    mainWindow = glfwGetCurrentContext();
    camera.Window = mainWindow;

    glfwSetCursorPosCallback(mainWindow, mouse_callback);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    prog.setUniform("FogColor", FogColor);
    prog.setUniform("FogStart", FogStartDist);
    prog.setUniform("FogEnd", FogEndDist);
    prog.setUniform("HazeStrength", HazeStrength);
    prog.setUniform("HazeColor", HazeColor);
    prog.setUniform("Brightness", Brightness);
    prog.setUniform("LightPosition", LightPosition);
    prog.setUniform("LightColor", LightColor);


    // Object gen
    //object.Data = GenerateSquare();
    object.Transformation = mat4(1.0f);
    ///object.Transformation = glm::rotate(object.Transformation, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));

    skybox.Data = GenerateSkybox();
    skybox.Transformation = mat4(1.0f);
    skybox.Transformation = scale(skybox.Transformation, vec3(999.0f, 999.0f, 999.0f));

    mesh = ObjMesh::load("resources/models/pig_triangulated.obj");

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("resources/shaders/shader.vert");
		prog.compileShader("resources/shaders/shader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}

void SceneBasic_Uniform::update(float t)
{
 
}
 
void SceneBasic_Uniform::render() // Render loop
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.Update(deltaTime);

    //MVP
    prog.setUniform("ViewIn", camera.GetViewMatrix());
    prog.setUniform("ProjectionIn", camera.Projection);
    prog.setUniform("ModelIn", mat4(1.0f));

    //Others
    prog.setUniform("CameraPos", camera.Position);

    //Skybox
    glDisable(GL_DEPTH_TEST);
    prog.setUniform("SkyboxActive", true);
    //glDepthFunc(GL_LEQUAL);
    glBindVertexArray(skybox.Data.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.Data.TextureID);
    glDrawArrays(GL_TRIANGLES, 0, skybox.Data.ArraySize);
    glBindVertexArray(0);
    //glDepthFunc(GL_LESS);
    prog.setUniform("SkyboxActive", false);
    glEnable(GL_DEPTH_TEST);

    //Square
    prog.setUniform("SkyboxActive", false);
    prog.setUniform("SetReflection", Reflectance);
    //glBindVertexArray(object.Data.VAO); // grab the assigned vao
    //glDrawElements(GL_TRIANGLES, object.Data.ArraySize, GL_UNSIGNED_INT, 0); // draw 

    float MovementDistance = 0.1f;
    float SpeedMultiplier = 3.0f;

    float Calculation = (sin((glfwGetTime() * SpeedMultiplier) - startTime) + 1 / 2) * MovementDistance;
    float RotationCalculation = (sin((glfwGetTime() * 5.0f) - startTime) + 1 / 2) * 2;

    mat4 Base = mat4(1.0f);
    mat4 Position = glm::translate(Base, vec3(0,Calculation,0));    
    mat4 RotatedPosition = glm::rotate(Position, glm::radians(RotationCalculation), vec3(0.0f, 0.0f, 1.0f));
    
    prog.setUniform("ModelIn", RotatedPosition);
    mesh->render();


    while (glfwGetTime() - currentFrame < 1 / FrameRate) {}
}

void SceneBasic_Uniform::resize(int w, int h)
{
    camera.Projection = glm::perspective(glm::radians(float(FOV)), (float)w / (float)h, 0.01f, 100.0f);
    glViewport(0,0,w,h);
}


