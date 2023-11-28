#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform float alpha;
uniform vec3 color;
uniform sampler2D textur;


void main() {
    vec4 sampled = vec4(color, texture(textur, vec2(TexCoord.x, 1.0 - TexCoord.y)).r * alpha);
    FragColor = sampled;
      
}
