#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;


out vec2 TexCoords;
out vec4 ParticleColor;

uniform vec3 offset;
uniform mat4 projection;
uniform vec4 color;
uniform mat4 model;
uniform mat4 view;


void main()
{
    float scale = 0.5f;
    TexCoords = aTexCoords;
    ParticleColor = color;
    gl_Position = projection  * view * model * vec4(aPos, 1.0);
}
