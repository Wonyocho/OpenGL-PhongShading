//
//  renderScene.cpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/14/25.
//

#include "RenderScene.hpp"
#include "ShaderProgram.hpp"
#include "Model.hpp"
#include "MouseController.hpp"  // 마우스 컨트롤러 헤더 추가
#include <iostream>

static float angle = 0.0f;

// 전역 포인터들 (외부에서 설정됨)
static ShaderProgram* g_shaderProgram = nullptr;
static Model* g_model = nullptr;

void setScenePointers(ShaderProgram* shader, Model* model) {
    g_shaderProgram = shader;
    g_model = model;
    std::cout << "Scene pointers set - Shader: " << (shader ? "OK" : "NULL") 
              << ", Model: " << (model ? "OK" : "NULL") << std::endl;
}

void updateAnimation() {
    angle += 0.01f;
    if (angle > 6.28319f) angle -= 6.28319f; // 2*PI
    glutPostRedisplay();
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

    // --- 조명 및 재질 uniform ---
    glm::vec3 lightPos(2.0f, 4.0f, 2.0f);
    glm::vec3 viewPos = MouseController::getCameraPosition();
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.8f, 0.8f, 0.8f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
    glm::vec3 materialAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 materialDiffuse(0.7f, 0.7f, 0.7f);
    glm::vec3 materialSpecular(1.0f, 1.0f, 1.0f);
    float shininess = 32.0f;

    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightAmbient"), 1, glm::value_ptr(lightAmbient));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightDiffuse"), 1, glm::value_ptr(lightDiffuse));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "lightSpecular"), 1, glm::value_ptr(lightSpecular));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "materialAmbient"), 1, glm::value_ptr(materialAmbient));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "materialDiffuse"), 1, glm::value_ptr(materialDiffuse));
    glUniform3fv(glGetUniformLocation(g_shaderProgram->ID, "materialSpecular"), 1, glm::value_ptr(materialSpecular));
    glUniform1f(glGetUniformLocation(g_shaderProgram->ID, "shininess"), shininess);

    // --- 드로우 ---
    g_model->draw();

    glutSwapBuffers();
}