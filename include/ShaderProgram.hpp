//
//  ShaderProgram.hpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/13/25.
//

// ShaderProgram.hpp
#pragma once
#include <string>
#include <GL/glew.h>

class ShaderProgram {
public:
    GLuint ID;

    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    void use();
    void deleteProgram();
    GLint getAttribLocation(const char* name);
    GLint getUniformLocation(const char* name);
};