#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

//uniform mat4 Model;
uniform mat4 MVP;
//uniform mat3 NormalMatrix;

out vec3 vPos;
out vec2 vUV;
out vec3 vNormal;

void main()
{
    vPos = aPos;   
    vUV = aUV;
    vNormal = aNormal;

    //vec4 worldPos = Model * vec4(aPos, 1.0);
    /*vFragPos = worldPos.xyz;
    vNormal = normalize(NormalMatrix * aNormal);
    vUV = aUV;
*/
    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}