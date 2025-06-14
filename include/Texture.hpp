//
//  Texture.hpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/14/25.
//

#pragma once
#include <GL/glew.h>
#include <string>

class Texture {
public:
    GLuint textureID;
    int width, height, channels;
    
    Texture();
    ~Texture();
    
    bool loadFromFile(const std::string& path);
    bool loadFromPPM(const std::string& path);  // PPM 전용 로더 추가
    void bind(int unit = 0);
    void unbind();
    void deleteTexture();

private:
    bool loadSTB(const std::string& path);  // stb_image용 로더
};