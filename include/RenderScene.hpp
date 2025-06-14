//
//  RenderScene.hpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/14/25.
//

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 전방 선언
class ShaderProgram;
class Model;
class Texture;

// 렌더링 함수들
void renderScene();
void updateAnimation();

// 포인터 설정 함수들
void setScenePointers(ShaderProgram* shader, Model* model);
void setTexturePointers(Texture* bark, Texture* leaf);  // 텍스처 설정 함수 추가

