
#include "scenebasic_uniform.h"
#include "Camera.h"
#include "ObjectGen.h"

using std::string;
using std::cerr;
using std::endl;
using std::cout;
using glm::vec3;
using glm::mat4;
using glm::scale;

// Would have defined it in the header however it seemed to break STBI
Camera camera;
GLFWwindow* mainWindow;
Model object;
Model skybox;
Model sea;
std::unique_ptr<ObjMesh> boatMesh;
std::unique_ptr<ObjMesh> seaMesh;
int boatTexture;
int seaTexture;
int seaTexture2;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

void mouse_callback(GLFWwindow* Window, double X, double Y) {
    camera.MouseInput(X, Y); // Send data to the camera for processing
}

void SceneBasic_Uniform::initScene()
{
    compile();
    prog.printActiveUniforms();

    mainWindow = glfwGetCurrentContext(); // Get the window location. Had to be called here since the lab libraries didn't seem to set it anywhere.
    camera.Window = mainWindow;

    glfwSetCursorPosCallback(mainWindow, mouse_callback); // Define mouse callback function
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    prog.setUniform("FogColor", FogColor); // Pass all lighting variables to the fragment shader
    prog.setUniform("FogStart", FogStartDist);
    prog.setUniform("FogEnd", FogEndDist);
    prog.setUniform("HazeStrength", HazeStrength);
    prog.setUniform("HazeColor", HazeColor);
    prog.setUniform("Brightness", Brightness);
    prog.setUniform("LightPosition", LightPosition);
    prog.setUniform("LightColor", LightColor);
    prog.setUniform("SetReflection", Reflectance);

    prog.setUniform("skybox", 0); // i found that explicit linking is required on nvidia opengl api, but seems to work without on intel
    prog.setUniform("Texture", 1);
    prog.setUniform("Texture2", 2);


    // Object gen
    //object.Data = GenerateSquare();
    //object.Transformation = mat4(1.0f);
    ///object.Transformation = glm::rotate(object.Transformation, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));

    // Skybox object
    skybox.Data = GenerateSkybox(); // Grab skybox data from ObjectGen
    skybox.Transformation = mat4(1.0f);
    //skybox.Transformation = scale(skybox.Transformation, vec3(1.0f, 1.0f, 999.0f));

    // Boat object
    boatMesh = ObjMesh::load("resources/models/boat.obj");
    boatTexture = LoadTexture("resources/textures/boat.png");

    // Sea object
    seaMesh = ObjMesh::load("resources/models/sea.obj");
    seaTexture = LoadTexture("resources/textures/sea.png");
    seaTexture2 = LoadTexture("resources/textures/seaoverlay.png");
    sea.Transformation = glm::rotate(sea.Transformation, glm::radians(180.0f), vec3(0.0f, 0.0f, 0.0f));
    sea.Transformation = glm::scale(mat4(1.0f), vec3(40.0f, 1.0f, 40.0f));

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
}

void SceneBasic_Uniform::compile() // Provided by labs
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
 // unused
}
 
void SceneBasic_Uniform::render() // Render loop
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Scene clearing/refreshing

    //Timing
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //Camera
    camera.Update(deltaTime); // Delta time is used to not have abnormal movement depending on frame time
    prog.setUniform("CameraPos", camera.Position); // Send camera position for lighting calculations

    //MVP
    prog.setUniform("ViewIn", camera.GetViewMatrix()); // Send MVP base to shader, but leave models to be set per model
    prog.setUniform("ProjectionIn", camera.Projection);
    prog.setUniform("ModelIn", mat4(1.0f));

    //Others
    prog.setUniform("MixEnabled", false); // Disable texture mixing to not cause issues 

    //Skybox
    glDisable(GL_DEPTH_TEST);
    prog.setUniform("SkyboxActive", true); // Fragment shader setting to skybox
    glBindVertexArray(skybox.Data.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.Data.TextureID);
    glDrawArrays(GL_TRIANGLES, 0, skybox.Data.ArraySize);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    prog.setUniform("SkyboxActive", false);

    // Boat
    mat4 Base = glm::rotate(mat4(1.0f), glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    Base = glm::scale(Base, glm::vec3(0.025f, 0.025f, 0.025f));

    if (MovementEnabled) {
        float Calculation = ((sin((glfwGetTime() * SpeedMultiplier) - startTime) + 1 / 2) * MovementDistance);
        float RotationCalculation = (sin((glfwGetTime() * 5.0f) - startTime) + 1 / 2) * 2;
        float RotationDeg = fmod(glfwGetTime() * (360.0f / 20.0f), 360.0f); // remainder but i dont know if it could cause processing issues with a long run time
        Base = glm::rotate(Base, glm::radians(RotationDeg), vec3(0.0f, 0.0f, 1.0f));
        Base = glm::translate(Base, vec3(150.0f, 00.0f, 0.0f));
        mat4 Position = glm::translate(Base, vec3(0, 0, Calculation - 0.5f));
        Base = glm::rotate(Position, glm::radians(RotationCalculation), vec3(0.0f, 1.0f, 0.0f));
    }

    prog.setUniform("ModelIn", Base);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, boatTexture);
    boatMesh->render();

    // Sea
    mat4 SeaTranslated = glm::translate(sea.Transformation, vec3(sin(glfwGetTime()) * 0.004f, 0.0f, (sin(glfwGetTime()) * 0.004f) + 0.2f));
    prog.setUniform("ModelIn", SeaTranslated);
    prog.setUniform("MixEnabled", true);
    glActiveTexture(GL_TEXTURE1); // First texture
    glBindTexture(GL_TEXTURE_2D, seaTexture);
    glActiveTexture(GL_TEXTURE2); // Overlay texture
    glBindTexture(GL_TEXTURE_2D, seaTexture2);
    seaMesh->render();

    //Timing
    while (glfwGetTime() - currentFrame < 1 / FrameRate) {}
}

void SceneBasic_Uniform::resize(int w, int h)
{
    camera.Projection = glm::perspective(glm::radians(float(FOV)), (float)w / (float)h, 0.01f, 100.0f);
    glViewport(0,0,w,h);
}


