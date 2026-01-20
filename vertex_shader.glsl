#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 MVP;
uniform mat4 MV;

out vec3 vPos;
out vec2 vUV;
out vec3 vNormal;

void main()
{
    // pozycja w view space
    vec4 posView = MV * vec4(aPos, 1.0);
    vPos = posView.xyz;

    // normal w view space
    vNormal = normalize(mat3(transpose(inverse(MV))) * aNormal);

    vUV = aUV;
    gl_Position = MVP * vec4(aPos, 1.0);
}
