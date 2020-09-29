#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 ourcolor;

void main()
{	
    FragColor = texture(texture1, TexCoord) + vec4(ourColor,1.0) + ourcolor;
}