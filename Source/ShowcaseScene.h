#pragma once

#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"

class ShowcaseScene : public Scene
{
private:
    GLuint vaoHandle;
    GLSLProgram prog;
    float angle;

    void compile();

public:
    ShowcaseScene();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

