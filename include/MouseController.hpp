//
//  MouseController.hpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/14/25.
//

#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class MouseController {
private:
    static bool mousePressed;
    static int lastMouseX, lastMouseY;
    static float cameraRotationX, cameraRotationY;
    static float cameraDistance;
    
public:
    // 초기화
    static void init();
    
    // 마우스 콜백 함수들
    static void mouseCallback(int button, int state, int x, int y);
    static void motionCallback(int x, int y);
    
    // 키보드 콜백 함수 (휠 대신)
    static void keyboardCallback(unsigned char key, int x, int y);
    
    // 카메라 정보 가져오기
    static glm::vec3 getCameraPosition();
    static glm::mat4 getViewMatrix();
    
    // 카메라 상태 출력 (디버깅용)
    static void printCameraInfo();
    
    // 줌 기능
    static void zoomIn();
    static void zoomOut();
};