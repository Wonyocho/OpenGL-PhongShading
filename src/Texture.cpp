//
//  Texture.cpp
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/14/25.
//

#include "Texture.hpp"
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Texture::Texture() : textureID(0), width(0), height(0), channels(0) {}

Texture::~Texture() {
    deleteTexture();
}

bool Texture::loadFromFile(const std::string& path) {
    // 파일 확장자 확인
    std::string ext = path.substr(path.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == "ppm") {
        return loadFromPPM(path);
    } else {
        return loadSTB(path);
    }
}

bool Texture::loadFromPPM(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open PPM file: " << path << std::endl;
        return false;
    }
    
    std::string magic;
    file >> magic;
    
    if (magic != "P3" && magic != "P6") {
        std::cerr << "Unsupported PPM format: " << magic << std::endl;
        return false;
    }
    
    // 주석 스킵
    std::string line;
    char c = file.peek();
    while (c == '#') {
        std::getline(file, line);
        c = file.peek();
    }
    
    // 크기 읽기
    file >> width >> height;
    
    int maxVal;
    file >> maxVal;
    
    if (maxVal != 255) {
        std::cerr << "Only 8-bit PPM files are supported" << std::endl;
        return false;
    }
    
    // 데이터 읽기
    std::vector<unsigned char> data(width * height * 3);
    
    if (magic == "P3") {
        // ASCII 형식
        for (int i = 0; i < width * height * 3; i++) {
            int val;
            file >> val;
            data[i] = static_cast<unsigned char>(val);
        }
    } else {
        // Binary 형식
        file.ignore(); // 개행 문자 스킵
        file.read(reinterpret_cast<char*>(data.data()), data.size());
    }
    
    file.close();
    
    // PPM은 위에서 아래로 저장되므로 Y축 뒤집기
    std::vector<unsigned char> flipped(data.size());
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int srcIdx = (y * width + x) * 3;
            int dstIdx = ((height - 1 - y) * width + x) * 3;
            flipped[dstIdx] = data[srcIdx];
            flipped[dstIdx + 1] = data[srcIdx + 1];
            flipped[dstIdx + 2] = data[srcIdx + 2];
        }
    }
    
    // OpenGL 텍스처 생성
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, flipped.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    channels = 3;
    std::cout << "PPM texture loaded: " << path << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

bool Texture::loadSTB(const std::string& path) {
    // stb_image를 사용한 기존 로더
    stbi_set_flip_vertically_on_load(true);
    
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum format = GL_RGB;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    std::cout << "Texture loaded: " << path << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
    return true;
}

void Texture::bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}