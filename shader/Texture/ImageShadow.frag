#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 color;
uniform vec3 ShadowColor;

uniform sampler2D sampler;

void main() {
    vec4 texColor = texture(sampler, TexCoord);
    FragColor = vec4(color, texColor.a);
}
