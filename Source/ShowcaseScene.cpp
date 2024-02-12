#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "ShowcaseScene.h"
#include "helper/glutils.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>

Camera camera;
GLFWwindow* mainWindow;

using std::string;
using std::cerr;
using std::endl;
using std::cout;
using glm::vec3;

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

    prog.printActiveUniforms();

    /////////////////// Create the VBO ////////////////////
    float positionData[] = {
        -1.0f, -1.0f, 0.0f, // t1
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, // t2
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f 
    };

    float colorData[] = {
     1.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 1.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 1.0f,
     1.0f, 1.0f, 0.0f 
    };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), colorData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

    glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat)*3);
    glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat)*3);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);

    glBindVertexArray(0);

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

    glm::mat4 Model = glm::mat4(1.0f);

    prog.setUniform("ModelIn", Model);
    prog.setUniform("ViewIn", camera.GetViewMatrix());
    prog.setUniform("ProjectionIn", camera.Projection);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

}

void ShowcaseScene::resize(int w, int h)
{
    camera.Projection = glm::perspective(glm::radians(70.0f), (float)w / (float)h, 0.01f, 100.0f);
    glViewport(0,0,w,h);
}


