#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Log.h"

int Res[] = { 1280,720 };

void resize_callback(GLFWwindow* window, int ResX, int ResY) {
	glViewport(0, 0, ResX, ResY);
}
void mouse_callback(GLFWwindow* window, double XPos, double YPos) {
	//
}

int main() {
	if (glfwInit()) {
		log("GLFW started successfully.");
	} else {
		log("GLFW failed to start.", LogType::Fatal);
		glfwTerminate();
		return -1;
	}

	GLFWwindow* mainWindow = glfwCreateWindow(Res[0], Res[1], "COMP3015 Assignment by Tobey Foxcroft", NULL, NULL);
	glfwMakeContextCurrent(mainWindow);

	if (glewInit() == GLEW_OK) {
		log("GLEW started successfully.");
	} else {
		log("GLEW failed to start.", LogType::Fatal);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, Res[0], Res[1]);
	glfwSetFramebufferSizeCallback(mainWindow, resize_callback);
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // prevents mouse from escaping window

}