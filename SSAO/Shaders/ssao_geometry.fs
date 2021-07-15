#version 450 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // diffuse per-fragment color
    gAlbedo.rgb = vec3(0.95);
}