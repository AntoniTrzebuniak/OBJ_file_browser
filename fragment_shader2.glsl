#version 330 core

in vec3 vPos;
in vec2 vUV;
in vec3 vNormal;

uniform int uRenderMode; // 0 = triangles, 1 = points
uniform sampler2D tex0;

out vec4 FragColor;

void main()
{

    vec3 n = normalize(vPos);
    vec3 color = n * 0.5 + 0.5;


    if (uRenderMode == 1)
    {
        color = vec3(1.0) - color;
    }
    FragColor = vec4(color, 1.0);
}