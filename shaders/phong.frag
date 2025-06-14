//
//  phong.frag
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/13/25.
//

#version 120

varying vec3 FragPos;
varying vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float shininess;

void main()
{
    // Ambient
    vec3 ambient = lightAmbient * materialAmbient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * (diff * materialDiffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = lightSpecular * (spec * materialSpecular);

    vec3 result = ambient + diffuse + specular;
    
    // 이 부분이 누락되어 있었습니다!
    gl_FragColor = vec4(result, 1.0);
}