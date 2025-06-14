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
#include "MouseController.hpp"
#include "Texture.hpp"  // 텍스처 헤더 추가

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
    glutCreateWindow("Phong Shading + Animation + Mouse Control + Texture");

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
    
    // 4. 텍스처 로드
    Texture* barkTexture = new Texture();
    Texture* leafTexture = new Texture();
    
    // 텍스처 파일 로드 (PPM, PNG, JPG 등 자동 감지)
    bool barkLoaded = false;
    bool leafLoaded = false;
    
    // 나무 텍스처 로드 시도 (여러 형식)
    if (barkTexture->loadFromFile("resources/bark_texture.ppm")) {
        barkLoaded = true;
        std::cout << "Bark texture loaded (PPM)" << std::endl;
    } else if (barkTexture->loadFromFile("resources/bark_texture.png")) {
        barkLoaded = true;
        std::cout << "Bark texture loaded (PNG)" << std::endl;
    } else if (barkTexture->loadFromFile("resources/bark_texture.jpg")) {
        barkLoaded = true;
        std::cout << "Bark texture loaded (JPG)" << std::endl;
    } else {
        std::cerr << "Warning: Failed to load bark texture (using default)" << std::endl;
    }
    
    // 나뭇잎 텍스처 로드 시도 (여러 형식)
    if (leafTexture->loadFromFile("resources/leaf_texture.ppm")) {
        leafLoaded = true;
        std::cout << "Leaf texture loaded (PPM)" << std::endl;
    } else if (leafTexture->loadFromFile("resources/leaf_texture.png")) {
        leafLoaded = true;
        std::cout << "Leaf texture loaded (PNG)" << std::endl;
    } else if (leafTexture->loadFromFile("resources/leaf_texture.jpg")) {
        leafLoaded = true;
        std::cout << "Leaf texture loaded (JPG)" << std::endl;
    } else {
        std::cerr << "Warning: Failed to load leaf texture (using default)" << std::endl;
    }
    
    // 5. RenderScene에 포인터들 전달
    setScenePointers(shaderProgram, model);
    
    // 텍스처가 로드된 경우에만 설정
    if (barkLoaded && leafLoaded) {
        setTexturePointers(barkTexture, leafTexture);
        std::cout << "Textures applied successfully" << std::endl;
    } else {
        std::cout << "Running without textures (solid color mode)" << std::endl;
    }
    
    // 6. 마우스 컨트롤러 초기화
    MouseController::init();
    
    // 7. 콜백 함수 등록
    extern void renderScene();
    extern void updateAnimation();
    
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateAnimation);
    
    // 마우스 및 키보드 콜백 함수 등록
    glutMouseFunc(MouseController::mouseCallback);
    glutMotionFunc(MouseController::motionCallback);
    glutKeyboardFunc(MouseController::keyboardCallback);
    
    std::cout << "Starting main loop" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "- Mouse drag: Rotate camera" << std::endl;
    std::cout << "- '[' key: Zoom in" << std::endl;
    std::cout << "- ']' key: Zoom out" << std::endl;
    std::cout << "- 'r' key: Reset camera" << std::endl;
    std::cout << "- 'i' key: Print camera info" << std::endl;
    std::cout << "- ESC: Exit program" << std::endl;
    
    glutMainLoop();
    
    // 8. 정리
    std::cout << "Cleaning up resources..." << std::endl;
    
    shaderProgram->deleteProgram();
    delete shaderProgram;
    delete model;
    
    if (barkTexture) {
        barkTexture->deleteTexture();
        delete barkTexture;
    }
    
    if (leafTexture) {
        leafTexture->deleteTexture();
        delete leafTexture;
    }
    
    std::cout << "Program terminated successfully" << std::endl;
    return 0;
}