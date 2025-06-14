//
//  Model.cpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/13/25.
//

#include "Model.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

Model::Model() : VBO(0) {}

bool Model::loadFromOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texcoords;

    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            if (iss >> x >> y >> z) {
                temp_positions.push_back(glm::vec3(x, y, z));
            }
        } 
        else if (prefix == "vn") {
            float x, y, z;
            if (iss >> x >> y >> z) {
                temp_normals.push_back(glm::vec3(x, y, z));
            }
        } 
        else if (prefix == "vt") {
            float u, v;
            if (iss >> u >> v) {
                temp_texcoords.push_back(glm::vec2(u, v));
            }
        }
        else if (prefix == "f") {
            // 한 줄의 모든 vertex 읽기
            std::vector<std::string> face_vertices;
            std::string vertex_str;
            
            while (iss >> vertex_str) {
                face_vertices.push_back(vertex_str);
            }
            
            if (face_vertices.size() < 3) {
                std::cerr << "Error: Face with less than 3 vertices at line " << lineNumber << std::endl;
                continue;
            }
            
            // Face vertex 정보 파싱
            std::vector<unsigned int> pos_indices, norm_indices, tex_indices;
            bool validFace = true;
            
            for (const std::string& vertex : face_vertices) {
                unsigned int pos_idx = 0, tex_idx = 0, norm_idx = 0;
                
                // v/vt/vn 형태 파싱
                size_t first_slash = vertex.find('/');
                if (first_slash == std::string::npos) {
                    // v 형태
                    pos_idx = std::stoi(vertex) - 1;
                } else {
                    // position 파싱
                    pos_idx = std::stoi(vertex.substr(0, first_slash)) - 1;
                    
                    size_t second_slash = vertex.find('/', first_slash + 1);
                    if (second_slash == std::string::npos) {
                        // v/vt 형태
                        if (first_slash + 1 < vertex.length()) {
                            tex_idx = std::stoi(vertex.substr(first_slash + 1)) - 1;
                        }
                    } else {
                        // v/vt/vn 또는 v//vn 형태
                        if (second_slash > first_slash + 1) {
                            // v/vt/vn 형태
                            tex_idx = std::stoi(vertex.substr(first_slash + 1, second_slash - first_slash - 1)) - 1;
                        }
                        // normal 파싱
                        if (second_slash + 1 < vertex.length()) {
                            norm_idx = std::stoi(vertex.substr(second_slash + 1)) - 1;
                        }
                    }
                }
                
                // 범위 체크
                if (pos_idx >= temp_positions.size()) {
                    std::cerr << "Error: Position index " << pos_idx << " out of range at line " << lineNumber << std::endl;
                    validFace = false;
                    break;
                }
                
                if (!temp_normals.empty() && norm_idx >= temp_normals.size()) {
                    std::cerr << "Error: Normal index " << norm_idx << " out of range at line " << lineNumber << std::endl;
                    validFace = false;
                    break;
                }
                
                pos_indices.push_back(pos_idx);
                norm_indices.push_back(norm_idx);
                tex_indices.push_back(tex_idx);
            }
            
            if (!validFace) continue;
            
            // Face를 삼각형으로 분할 (Fan triangulation)
            // Face가 n개의 vertex를 가질 때, (n-2)개의 삼각형으로 분할
            for (size_t i = 1; i < face_vertices.size() - 1; i++) {
                // 삼각형: 0, i, i+1
                std::vector<int> triangle_indices = {0, (int)i, (int)i+1};
                
                for (int tri_idx : triangle_indices) {
                    Vertex vertex;
                    
                    // Position 설정
                    glm::vec3 pos = temp_positions[pos_indices[tri_idx]];
                    vertex.position[0] = pos.x;
                    vertex.position[1] = pos.y;
                    vertex.position[2] = pos.z;
                    
                    // Normal 설정
                    if (!temp_normals.empty() && norm_indices[tri_idx] < temp_normals.size()) {
                        glm::vec3 normal = temp_normals[norm_indices[tri_idx]];
                        vertex.normal[0] = normal.x;
                        vertex.normal[1] = normal.y;
                        vertex.normal[2] = normal.z;
                    } else {
                        // Face normal 계산
                        glm::vec3 v0 = temp_positions[pos_indices[0]];
                        glm::vec3 v1 = temp_positions[pos_indices[1]];
                        glm::vec3 v2 = temp_positions[pos_indices[2]];
                        
                        glm::vec3 edge1 = v1 - v0;
                        glm::vec3 edge2 = v2 - v0;
                        glm::vec3 face_normal = glm::normalize(glm::cross(edge1, edge2));
                        
                        vertex.normal[0] = face_normal.x;
                        vertex.normal[1] = face_normal.y;
                        vertex.normal[2] = face_normal.z;
                    }
                    
                    this->vertices.push_back(vertex);
                }
            }
        }
    }

    std::cout << "Successfully loaded:" << std::endl;
    std::cout << "- Positions: " << temp_positions.size() << std::endl;
    std::cout << "- Normals: " << temp_normals.size() << std::endl;
    std::cout << "- Texture coordinates: " << temp_texcoords.size() << std::endl;
    std::cout << "- Final vertices: " << this->vertices.size() << std::endl;
    std::cout << "- Final triangles: " << this->vertices.size() / 3 << std::endl;

    file.close();
    
    if (this->vertices.empty()) {
        std::cerr << "No vertices loaded!" << std::endl;
        return false;
    }
    
    return true;
}

void Model::setupBuffers() {
    if (vertices.empty()) {
        std::cerr << "No vertices to setup!" << std::endl;
        return;
    }
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    std::cout << "VBO created with ID: " << VBO << " for " << vertices.size() << " vertices" << std::endl;
}

void Model::draw() {
    if (vertices.empty() || VBO == 0) {
        std::cerr << "Nothing to draw!" << std::endl;
        return;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
