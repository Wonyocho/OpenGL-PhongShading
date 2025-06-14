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
        if (line.empty() || line[0] == '#') continue;  // 빈 줄이나 주석 스킵
        
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
            std::string vertex1, vertex2, vertex3;
            if (iss >> vertex1 >> vertex2 >> vertex3) {
                
                // 각 vertex string을 파싱 (변수명 변경: vertices -> vertex_strings)
                std::vector<std::string> vertex_strings = {vertex1, vertex2, vertex3};
                std::vector<unsigned int> pos_indices, norm_indices;
                
                bool validFace = true;
                
                for (const std::string& vertex_str : vertex_strings) {
                    unsigned int pos_idx = 0, tex_idx = 0, norm_idx = 0;
                    
                    // v/vt/vn 형태 파싱
                    size_t first_slash = vertex_str.find('/');
                    if (first_slash == std::string::npos) {
                        // v 형태 (position만)
                        pos_idx = std::stoi(vertex_str) - 1;
                    } else {
                        // position 파싱
                        pos_idx = std::stoi(vertex_str.substr(0, first_slash)) - 1;
                        
                        size_t second_slash = vertex_str.find('/', first_slash + 1);
                        if (second_slash == std::string::npos) {
                            // v/vt 형태
                            if (first_slash + 1 < vertex_str.length()) {
                                tex_idx = std::stoi(vertex_str.substr(first_slash + 1)) - 1;
                            }
                        } else {
                            // v/vt/vn 또는 v//vn 형태
                            if (second_slash > first_slash + 1) {
                                // v/vt/vn 형태
                                tex_idx = std::stoi(vertex_str.substr(first_slash + 1, second_slash - first_slash - 1)) - 1;
                            }
                            // normal 파싱
                            if (second_slash + 1 < vertex_str.length()) {
                                norm_idx = std::stoi(vertex_str.substr(second_slash + 1)) - 1;
                            }
                        }
                    }
                    
                    // 범위 체크
                    if (pos_idx >= temp_positions.size()) {
                        std::cerr << "Error: Position index " << pos_idx << " out of range at line " << lineNumber << std::endl;
                        validFace = false;
                        break;
                    }
                    
                    if (norm_idx > 0 && norm_idx >= temp_normals.size()) {
                        std::cerr << "Error: Normal index " << norm_idx << " out of range at line " << lineNumber << std::endl;
                        validFace = false;
                        break;
                    }
                    
                    pos_indices.push_back(pos_idx);
                    norm_indices.push_back(norm_idx);
                }
                
                if (validFace) {
                    // Vertex 생성
                    for (int i = 0; i < 3; i++) {
                        Vertex vertex;
                        
                        // Position 설정
                        glm::vec3 pos = temp_positions[pos_indices[i]];
                        vertex.position[0] = pos.x;
                        vertex.position[1] = pos.y;
                        vertex.position[2] = pos.z;
                        
                        // Normal 설정
                        if (!temp_normals.empty() && norm_indices[i] < temp_normals.size()) {
                            glm::vec3 normal = temp_normals[norm_indices[i]];
                            vertex.normal[0] = normal.x;
                            vertex.normal[1] = normal.y;
                            vertex.normal[2] = normal.z;
                        } else {
                            // 임시로 기본 normal 설정 (나중에 face normal로 교체됨)
                            vertex.normal[0] = 0.0f;
                            vertex.normal[1] = 1.0f;
                            vertex.normal[2] = 0.0f;
                        }
                        
                        // 클래스 멤버 변수에 추가
                        this->vertices.push_back(vertex);
                    }
                    
                    // Normal이 설정되지 않은 경우 face normal 계산 및 적용
                    if (temp_normals.empty() || norm_indices[0] >= temp_normals.size()) {
                        size_t start_idx = this->vertices.size() - 3;
                        glm::vec3 v1 = temp_positions[pos_indices[1]] - temp_positions[pos_indices[0]];
                        glm::vec3 v2 = temp_positions[pos_indices[2]] - temp_positions[pos_indices[0]];
                        glm::vec3 face_normal = glm::normalize(glm::cross(v1, v2));
                        
                        for (int i = 0; i < 3; i++) {
                            this->vertices[start_idx + i].normal[0] = face_normal.x;
                            this->vertices[start_idx + i].normal[1] = face_normal.y;
                            this->vertices[start_idx + i].normal[2] = face_normal.z;
                        }
                    }
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
    
    // aPos (location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // aNormal (location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
