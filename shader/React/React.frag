#version 330 core

in vec2 TexCoord, VertexCord;
out vec4 FragColor;

uniform float radius;
uniform vec3 color;
uniform float alpha;
uniform vec2 size;

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius) {
    return length(max(abs(CenterPosition) - Size + Radius, 0.0)) - Radius;
}

void main() {
    float edgeSoftness = 1.0;
    vec2 center = size / 2.0;
    float distance = roundedBoxSDF(center - (vec2(TexCoord.x, 1.0 - TexCoord.y) * size), center, radius + 1.0);
    float smoothedAlpha = (1.0 - smoothstep(-edgeSoftness, edgeSoftness, distance)) * alpha;
    FragColor = vec4(color, smoothedAlpha);
}