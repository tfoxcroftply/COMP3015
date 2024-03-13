#include "Models.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* ModelData GenerateSquare() {
    float vertices[] = {
        -1.0f, 0.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f,  1.0f,
         1.0f, 0.0f,  1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    };

    float normals[] = {
        0.0f, 1.0f, 0.0f,  // Up
        0.0f, 1.0f, 0.0f,  // Up
        0.0f, 1.0f, 0.0f,  // Up
        0.0f, 1.0f, 0.0f   // Up
    };


    GLuint VAO, vertVBO, colVBO, normVBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertVBO);
    glGenBuffers(1, &colVBO);
    glGenBuffers(1, &normVBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, normVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return { int(VAO), sizeof(indices) / sizeof(indices[0]), 0 };
} */


ModelData GenerateSkybox() {
    std::string Extension = ".png";
    std::vector<std::string> Faces {
        "right",
        "left",
        "top",
        "bottom",
        "front",
        "back"
    };
    for (unsigned int i = 0; i < Faces.size(); i++)
    {
        Faces[i] = "resources/skybox/" + Faces[i] + Extension;
        std:: cout << Faces[i];
    }

    unsigned int TexID;
    glGenTextures(1, &TexID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TexID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < Faces.size(); i++) {
        unsigned char* data = stbi_load(Faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Couldn't load texture! Path: " << Faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    float vertices[] = {        
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    GLuint vao, vertVbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vertVbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return { int(vao), sizeof(vertices), TexID };
}