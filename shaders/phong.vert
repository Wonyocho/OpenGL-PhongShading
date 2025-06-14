//
//  phong.vert
//  과제5_20010493_최원영
//
//  Created by 최원영 on 6/13/25.
//

#version 120

attribute vec3 aPos;        // location 0
attribute vec3 aNormal;     // location 1
attribute vec2 aTexCoord;   // location 2
attribute float aMaterialID; // location 3

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec3 FragPos;
varying vec3 Normal;
varying vec2 TexCoord;
varying float MaterialID;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;
    TexCoord = aTexCoord;
    MaterialID = aMaterialID;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}