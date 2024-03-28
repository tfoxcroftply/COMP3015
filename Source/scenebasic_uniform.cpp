#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>

#include "scenebasic_uniform.h"

#include "Camera.h"
#include "Models.h"
#include "ObjectGen.h"

Camera camera;
GLFWwindow* mainWindow;
Model object;
Model skybox;
Model sea;
std::unique_ptr<ObjMesh> boatMesh;
std::unique_ptr<ObjMesh> seaMesh;
int boatTexture;
int seaTexture;

using std::string;
using std::cerr;
using std::endl;
using std::cout;
using glm::vec3;
using glm::mat4;
using glm::scale;

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
    prog.setUniform("SetReflection", Reflectance);

    boatTexture = LoadTexture("resources/textures/boat.png");
    seaTexture = LoadTexture("resources/textures/sea.png");

    // Object gen
    //object.Data = GenerateSquare();
    object.Transformation = mat4(1.0f);
    ///object.Transformation = glm::rotate(object.Transformation, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));

    skybox.Data = GenerateSkybox();
    skybox.Transformation = mat4(1.0f);
    //skybox.Transformation = scale(skybox.Transformation, vec3(1.0f, 1.0f, 999.0f));

    boatMesh = ObjMesh::load("resources/models/boat.obj");
    seaMesh = ObjMesh::load("resources/models/sea.obj");

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

    //Timing
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //Camera
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

    glBindVertexArray(skybox.Data.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.Data.TextureID);
    glDrawArrays(GL_TRIANGLES, 0, skybox.Data.ArraySize);

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    prog.setUniform("SkyboxActive", false);


    // Boat
    float Calculation = ((sin((glfwGetTime() * SpeedMultiplier) - startTime) + 1 / 2) * MovementDistance);
    float RotationCalculation = (sin((glfwGetTime() * 5.0f) - startTime) + 1 / 2) * 2;

    float RotationDeg = fmod(glfwGetTime() * (360.0f / 15.0f), 360.0f); // remainder 

    mat4 Base = glm::rotate(mat4(1.0f), glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    Base = glm::scale(Base, glm::vec3(0.01f, 0.01f, 0.01f));
    Base = glm::rotate(Base, glm::radians(RotationDeg), vec3(0.0f, 0.0f, 1.0f));
    Base = glm::translate(Base, vec3(500.0f, 00.0f, 0.0f));

    mat4 Position = glm::translate(Base, vec3(0,0, Calculation - 4.5f));
    mat4 RotatedPosition = glm::rotate(Position, glm::radians(RotationCalculation), vec3(0.0f, 1.0f, 0.0f));
    
    prog.setUniform("ModelIn", RotatedPosition);
    glBindTexture(GL_TEXTURE_2D, boatTexture);
    boatMesh->render();


    // Sea
    mat4 SeaBase = glm::scale(mat4(1.0f), vec3(40.0f, 0.0f, 40.0f));
    mat4 SeaTranslated = glm::translate(SeaBase, vec3(sin(glfwGetTime()) * 0.004f, 0.0f, (sin(glfwGetTime()) * 0.004f) + 0.2f));

    prog.setUniform("ModelIn", SeaTranslated);
    glBindTexture(GL_TEXTURE_2D, seaTexture);
    seaMesh->render();

    //Timing
    while (glfwGetTime() - currentFrame < 1 / FrameRate) {}
}

void SceneBasic_Uniform::resize(int w, int h)
{
    camera.Projection = glm::perspective(glm::radians(float(FOV)), (float)w / (float)h, 0.01f, 100.0f);
    glViewport(0,0,w,h);
}


