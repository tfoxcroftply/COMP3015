#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "ShowcaseScene.h"
#include "helper/glutils.h"

#include "Camera.h"
#include "Models.h"
#include "ObjectGen.h"

#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>

Camera camera;
GLFWwindow* mainWindow;

Model object;
Model skybox;


using std::string;
using std::cerr;
using std::endl;
using std::cout;
using glm::vec3;

static int FrameRate = 120;
static int FOV = 70;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

ShowcaseScene::ShowcaseScene() : angle(0.0f) {}

void mouse_callback(GLFWwindow* Window, double X, double Y) {
    camera.MouseInput(X, Y);
}

void ShowcaseScene::initScene()
{
    compile();
    prog.printActiveUniforms();

    mainWindow = glfwGetCurrentContext();
    camera.Window = mainWindow;

    glfwSetCursorPosCallback(mainWindow, mouse_callback);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    prog.setUniform("FogColor", glm::vec3(0.8f,0.8f,0.8f));
    prog.setUniform("FogStart", 1.0f);
    prog.setUniform("FogEnd", 5.0f);

    prog.setUniform("AmbientStrength", 0.9f);
    prog.setUniform("AmbientColor", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("LightPosition", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("LightColor", vec3(1.0f, 1.0f, 1.0f));

    // Object gen
    object.Data = GenerateSquare();
    object.Transformation = glm::mat4(1.0f);
    //object.Transformation = glm::rotate(object.Transformation, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));

    skybox.Data = GenerateSkybox();
}

void ShowcaseScene::compile()
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

void ShowcaseScene::update(float t)
{
 
}
 
void ShowcaseScene::render() // Render loop
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.Update(deltaTime);

    //MVP
    prog.setUniform("ViewIn", camera.GetViewMatrix());
    prog.setUniform("ProjectionIn", camera.Projection);
    prog.setUniform("ModelIn", glm::mat4(1.0f));

    //Others
    prog.setUniform("CameraPos", camera.Position);

    //Skybox
    prog.setUniform("SkyboxActive", true);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(skybox.Data.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.Data.TextureID);
    glDrawArrays(GL_TRIANGLES, 0, skybox.Data.ArraySize);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
    prog.setUniform("SkyboxActive", false);

    //Square
    prog.setUniform("ModelIn", object.Transformation);
    glBindVertexArray(object.Data.VAO); // grab the assigned vao
    glDrawElements(GL_TRIANGLES, object.Data.ArraySize, GL_UNSIGNED_INT, 0); // draw 


    while (glfwGetTime() - currentFrame < 1 / FrameRate) {}
}

void ShowcaseScene::resize(int w, int h)
{
    camera.Projection = glm::perspective(glm::radians(float(FOV)), (float)w / (float)h, 0.01f, 100.0f);
    glViewport(0,0,w,h);
}


