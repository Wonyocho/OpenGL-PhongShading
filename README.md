# OpenGL Phong Shading with Tree Model

이 프로젝트는 OpenGL을 사용하여 Phong 셰이딩, 텍스처 매핑, 마우스 제어 및 애니메이션을 구현한 3D 나무 모델 렌더링 프로그램입니다.

## 주요 기능

### 렌더링 기능
- **Phong 셰이딩**: Ambient, Diffuse, Specular 조명 모델 구현
- **텍스처 매핑**: 나무 줄기와 나뭇잎에 서로 다른 텍스처 적용
- **재질 분류**: Quad 면은 나무(갈색), Triangle 면은 나뭇잎(녹색)으로 자동 분류
- **실시간 애니메이션**: Y축을 중심으로 한 자동 회전

### 상호작용 기능
- **마우스 제어**: 드래그로 카메라 회전
- **키보드 제어**: 줌 인/아웃, 카메라 리셋 등
- **다중 텍스처 포맷 지원**: PPM, PNG, JPG 형식 자동 감지 및 로드

## 빌드 요구사항

### 필수 라이브러리
- OpenGL 3.3+
- GLEW (OpenGL Extension Wrangler)
- FreeGLUT (또는 GLUT)
- GLM (OpenGL Mathematics)

### 조작법
- **마우스 드래그**: 카메라 회전
- **`[` 키**: 줌 인
- **`]` 키**: 줌 아웃
- **`r` 키**: 카메라 리셋
- **`i` 키**: 카메라 정보 출력
- **ESC 키**: 프로그램 종료

## 기술적 세부사항

### 셰이더 구현
- **버텍스 셰이더**: 정점 변환, 법선 벡터 변환, 텍스처 좌표 전달
- **프래그먼트 셰이더**: Phong 조명 모델, 재질별 텍스처 매핑

### 텍스처 시스템
- PPM 파일 파서 자체 구현
- stb_image를 통한 PNG/JPG 지원
- 자동 밉맵 생성
- 다중 텍스처 유닛 활용

### 모델 로딩
- OBJ 파일 파서 구현
- 정점, 법선, 텍스처 좌표 지원
- 면 타입에 따른 자동 재질 분류
- Quad → Triangle 자동 분할

### 카메라 시스템
- 구면 좌표계 기반 카메라
- 마우스 드래그로 회전 제어
- 키보드로 줌 제어

## 조명 설정

```cpp
// 조명 매개변수
vec3 lightPos = (2.0, 4.0, 2.0);
vec3 lightAmbient = (0.2, 0.2, 0.2);
vec3 lightDiffuse = (0.8, 0.8, 0.8);
vec3 lightSpecular = (1.0, 1.0, 1.0);
float shininess = 32.0;
```

## 재질 설정

- **나무 줄기** (Quad 면): 갈색 기본색, bark_texture.ppm
- **나뭇잎** (Triangle 면): 녹색 기본색, leaf_texture.ppm
