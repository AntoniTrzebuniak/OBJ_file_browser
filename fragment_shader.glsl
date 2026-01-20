#version 330 core

in vec3 vPos;
in vec2 vUV;
in vec3 vNormal;

uniform int uRenderMode;
uniform sampler2D tex0;

out vec4 FragColor;

uniform vec3 uKa;   // ambient (MTL: Ka)
uniform vec3 uKd;   // diffuse (MTL: Kd)
uniform vec3 uKs;   // specular (MTL: Ks)
uniform float uNs;  // shininess (MTL: Ns)
uniform float uAlpha; // d
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uCol;

uniform bool uHasTexture;

void main()
{
    if (uRenderMode==0)
    {
        vec3 N = normalize(vNormal);
        vec3 L = normalize(uLightPos - vPos);
        vec3 V = normalize(uViewPos - vPos);
        vec3 R = reflect(-L, N);

        vec4 texCol = texture(tex0, vUV);

        // Ambient (Ka)
        vec3 ambient = uHasTexture ? uKa * texCol.rgb : uKa;

        // Diffuse (Kd)
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = uHasTexture ? uKd * diff * texCol.rgb : uKd * diff;


        // Specular (Ks + Ns)
        float spec = pow(max(dot(V, R), 0.0), max(1.0, uNs));
        vec3 specular = uKs * spec;

        vec3 color = ambient + diffuse + specular;
        float outAlpha = (uHasTexture ? texCol.a : 1.0) * uAlpha;
        FragColor = vec4(color, outAlpha);
    } else{
        FragColor = vec4( uCol, uAlpha);
    }
    
}


