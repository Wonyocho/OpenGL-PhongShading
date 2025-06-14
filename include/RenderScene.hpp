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

class ShaderProgram;
class Model;

// 함수 선언만 (전역 변수 선언 제거)
void renderScene();
void updateAnimation();
void setScenePointers(ShaderProgram* shader, Model* model);

