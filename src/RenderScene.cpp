//
//  renderScene.cpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/14/25.
//

#include "RenderScene.hpp"
#include "ShaderProgram.hpp"
#include "Model.hpp"
#include "MouseController.hpp"
#include "Texture.hpp"  // 텍스처 헤더 추가
#include <iostream>

// 전역 포인터들
ShaderProgram* g_shaderProgram = nullptr;
Model* g_model = nullptr;
Texture* g_barkTexture = nullptr;   // 나무 텍스처
Texture* g_leafTexture = nullptr;   // 나뭇잎 텍스처

// 애니메이션 변수
float angle = 0.0f;

void setScenePointers(ShaderProgram* shader, Model* model) {
    g_shaderProgram = shader;
    g_model = model;
    std::cout << "Scene pointers set - Shader: " << (shader ? "OK" : "NULL") 
              << ", Model: " << (model ? "OK" : "NULL") << std::endl;
}

void setTexturePointers(Texture* bark, Texture* leaf) {
    g_barkTexture = bark;
    g_leafTexture = leaf;
    std::cout << "Texture pointers set - Bark: " << (bark ? "OK" : "NULL") 
              << ", Leaf: " << (leaf ? "OK" : "NULL") << std::endl;
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!g_shaderProgram || !g_model) {
        std::cerr << "Shader or model not initialized!" << std::endl;
        return;
    }
    
    // 셰이더 사용
    g_shaderProgram->use();

    // 변환 행렬 설정
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 viewMat = MouseController::getViewMatrix();
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram->ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projMat));

    // 조명 및 재질 uniform
    glm::vec3 lightPos(2.0f, 4.0f, 2.0f);
    glm::vec3 viewPos = MouseController::getCameraPosition();
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.8f, 0.8f, 0.8f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
    float shininess = 32.0f;

    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightAmbient"), 1, glm::value_ptr(lightAmbient));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightDiffuse"), 1, glm::value_ptr(lightDiffuse));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightSpecular"), 1, glm::value_ptr(lightSpecular));
    glUniform1f(glGetUniformLocation(g_shaderProgram->ID, "shininess"), shininess);

    // 텍스처 바인딩 (있는 경우)
    if (g_barkTexture && g_leafTexture) {
        g_barkTexture->bind(0);  // GL_TEXTURE0
        g_leafTexture->bind(1);  // GL_TEXTURE1
        
        // 셰이더에 텍스처 유닛 전달
        glUniform1i(glGetUniformLocation(g_shaderProgram->ID, "barkTexture"), 0);
        glUniform1i(glGetUniformLocation(g_shaderProgram->ID, "leafTexture"), 1);
        glUniform1i(glGetUniformLocation(g_shaderProgram->ID, "useTexture"), 1);
    } else {
        // 텍스처가 없는 경우
        glUniform1i(glGetUniformLocation(g_shaderProgram->ID, "useTexture"), 0);
    }

    // 드로우
    g_model->draw();

    // 텍스처 언바인드
    if (g_barkTexture && g_leafTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glutSwapBuffers();
}

void updateAnimation() {
    angle += 0.01f;  // 회전 속도
    if (angle >= 360.0f) {
        angle -= 360.0f;
    }
    glutPostRedisplay();
}