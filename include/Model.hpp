//
//  Model.hpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/13/25.
//

#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
    int materialID;
};

class Model {
public:
    std::vector<Vertex> vertices;
    GLuint VBO;

    Model();
    bool loadFromOBJ(const std::string& path);
    void setupBuffers();
    void draw();
};