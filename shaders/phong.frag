//
//  phong.frag
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/13/25.
//

#version 120

varying vec3 FragPos;
varying vec3 Normal;
varying vec2 TexCoord;
varying float MaterialID;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform float shininess;

uniform sampler2D barkTexture;
uniform sampler2D leafTexture;
uniform int useTexture;

void main()
{
    // 재질에 따른 색상 결정
    vec3 materialColor;
    
    if (useTexture == 1) {
        // 텍스처 사용
        if (MaterialID < 0.5) {
            materialColor = texture2D(barkTexture, TexCoord).rgb;  // 나무
        } else {
            materialColor = texture2D(leafTexture, TexCoord).rgb;  // 나뭇잎
        }
    } else {
        // 기본 색상 사용
        if (MaterialID < 0.5) {
            materialColor = vec3(0.6, 0.3, 0.1);  // 갈색 (나무)
        } else {
            materialColor = vec3(0.2, 0.8, 0.2);  // 녹색 (나뭇잎)
        }
    }
    
    // Phong Shading 계산
    vec3 ambient = lightAmbient * materialColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * materialColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = lightSpecular * spec * 0.3;

    vec3 result = ambient + diffuse + specular;
    gl_FragColor = vec4(result, 1.0);
}