//
//  main.cpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/13/25.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "ShaderProgram.hpp"
#include "Model.hpp"
#include "RenderScene.hpp"
#include "MouseController.hpp"  // 마우스 컨트롤러 헤더 추가

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    // Back-face culling 활성화
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    std::cout << "OpenGL initialized with depth test and back-face culling" << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Phong Shading + Animation + Mouse Control");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "GLEW init failed: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    initOpenGL();
    
    // 1. 먼저 ShaderProgram 생성
    ShaderProgram* shaderProgram = new ShaderProgram("shaders/phong.vert", "shaders/phong.frag");
    
    // 2. 그 다음 Model 생성
    Model* model = new Model();
    
    // 3. OBJ 파일 로드
    if (!model->loadFromOBJ("resources/Lowpoly_tree_sample.obj")) {
        std::cerr << "Failed to load OBJ file" << std::endl;
        delete shaderProgram;
        delete model;
        return -1;
    }
    
    std::cout << "Program start" << std::endl;
    model->setupBuffers();
    
    // 4. RenderScene에 포인터 전달
    setScenePointers(shaderProgram, model);
    
    // 5. 마우스 컨트롤러 초기화
    MouseController::init();
    
    // 6. 콜백 함수 등록
    extern void renderScene();
    extern void updateAnimation();
    
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateAnimation);
    
    // 마우스 및 키보드 콜백 함수 등록
    glutMouseFunc(MouseController::mouseCallback);
    glutMotionFunc(MouseController::motionCallback);
    glutKeyboardFunc(MouseController::keyboardCallback);  // 키보드 콜백 추가
    
    std::cout << "Starting main loop" << std::endl;
    glutMainLoop();
    
    // 7. 정리
    shaderProgram->deleteProgram();
    delete shaderProgram;
    delete model;

    return 0;
}