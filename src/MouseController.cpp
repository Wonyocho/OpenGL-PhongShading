//
//  MouseController.cpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/14/25.
//

#include "MouseController.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

// 정적 멤버 변수 초기화
bool MouseController::mousePressed = false;
int MouseController::lastMouseX = 0;
int MouseController::lastMouseY = 0;
float MouseController::cameraRotationX = 0.0f;
float MouseController::cameraRotationY = 0.0f;
float MouseController::cameraDistance = 50.0f;

void MouseController::init() {
    mousePressed = false;
    lastMouseX = 0;
    lastMouseY = 0;
    cameraRotationX = 0.0f;
    cameraRotationY = 0.0f;
    cameraDistance = 50.0f;
    
    std::cout << "MouseController initialized" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "- Left click + drag: Rotate camera" << std::endl;
    std::cout << "- '[' key: Zoom in" << std::endl;
    std::cout << "- ']' key: Zoom out" << std::endl;
    std::cout << "- Right click: Print camera info" << std::endl;
    std::cout << "- 'r' key: Reset camera" << std::endl;
    std::cout << "- ESC: Exit program" << std::endl;
}

void MouseController::mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
            std::cout << "Mouse pressed at (" << x << ", " << y << ")" << std::endl;
        } else if (state == GLUT_UP) {
            mousePressed = false;
            std::cout << "Mouse released" << std::endl;
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        printCameraInfo();
    }
}

void MouseController::motionCallback(int x, int y) {
    if (mousePressed) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;
        
        // 마우스 이동을 카메라 회전으로 변환
        cameraRotationY += deltaX * 0.01f;  // 좌우 회전 (Y축)
        cameraRotationX += deltaY * 0.01f;  // 상하 회전 (X축)
        
        // X축 회전 제한 (위아래 회전 제한)
        if (cameraRotationX > 1.5f) cameraRotationX = 1.5f;
        if (cameraRotationX < -1.5f) cameraRotationX = -1.5f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        std::cout << "Camera rotation: X=" << cameraRotationX 
                  << ", Y=" << cameraRotationY << std::endl;
        
        glutPostRedisplay();  // 화면 다시 그리기
    }
}

void MouseController::keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
        case '[':  // 줌 인
            zoomIn();
            break;
        case ']':  // 줌 아웃
            zoomOut();
            break;
        case 'r':  // 카메라 리셋
        case 'R':
            cameraRotationX = 0.0f;
            cameraRotationY = 0.0f;
            cameraDistance = 50.0f;
            std::cout << "Camera reset!" << std::endl;
            glutPostRedisplay();
            break;
        case 'i':  // 카메라 정보 출력
        case 'I':
            printCameraInfo();
            break;
        case 27:   // ESC 키
            std::cout << "ESC pressed - Exiting..." << std::endl;
            exit(0);
            break;
        default:
            break;
    }
}

void MouseController::zoomIn() {
    cameraDistance -= 0.5f;
    if (cameraDistance < 1.0f) cameraDistance = 1.0f;
    std::cout << "Zoom in - Camera distance: " << cameraDistance << std::endl;
    glutPostRedisplay();
}

void MouseController::zoomOut() {
    cameraDistance += 0.5f;
    if (cameraDistance > 100.0f) cameraDistance = 100.0f;
    std::cout << "Zoom out - Camera distance: " << cameraDistance << std::endl;
    glutPostRedisplay();
}

glm::vec3 MouseController::getCameraPosition() {
    // 구면 좌표계를 사용하여 카메라 위치 계산
    float x = cameraDistance * cos(cameraRotationX) * sin(cameraRotationY);
    float y = cameraDistance * sin(cameraRotationX);
    float z = cameraDistance * cos(cameraRotationX) * cos(cameraRotationY);
    
    return glm::vec3(x, y, z);
}

glm::mat4 MouseController::getViewMatrix() {
    glm::vec3 cameraPos = getCameraPosition();
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);  // 원점을 바라봄
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);      // Y축이 위쪽
    
    return glm::lookAt(cameraPos, target, up);
}

void MouseController::printCameraInfo() {
    glm::vec3 pos = getCameraPosition();
    std::cout << "=== Camera Info ===" << std::endl;
    std::cout << "Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
    std::cout << "Distance: " << cameraDistance << std::endl;
    std::cout << "Rotation X: " << cameraRotationX << " rad (" << (cameraRotationX * 180.0f / 3.14159f) << " deg)" << std::endl;
    std::cout << "Rotation Y: " << cameraRotationY << " rad (" << (cameraRotationY * 180.0f / 3.14159f) << " deg)" << std::endl;
    std::cout << "===================" << std::endl;
}
