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

    cout << endl;

    object.Data = GenerateSquare();
    object.Transformation = glm::mat4(1.0f);

    prog.printActiveUniforms();

    mainWindow = glfwGetCurrentContext();
    camera.Window = mainWindow;

    glfwSetCursorPosCallback(mainWindow, mouse_callback);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    float currentFrame = glfwGetTime(); // Calculate current times and time taken
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.Update(deltaTime);

    prog.setUniform("ModelIn", object.Transformation);
    prog.setUniform("ViewIn", camera.GetViewMatrix());
    prog.setUniform("ProjectionIn", camera.Projection);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(object.Data.VAO); // grab the assigned vao
    glDrawElements(GL_TRIANGLES, object.Data.ArraySize, GL_UNSIGNED_INT, 0); // and then draw it

    glBindVertexArray(0);

    while (glfwGetTime() - currentFrame < 1 / FrameRate) {}
}

void ShowcaseScene::resize(int w, int h)
{
    camera.Projection = glm::perspective(glm::radians(float(FOV)), (float)w / (float)h, 0.01f, 100.0f);
    glViewport(0,0,w,h);
}


