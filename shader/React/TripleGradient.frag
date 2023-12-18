#version 330 core

in vec2 TexCoord, VertexCord;
out vec4 FragColor;
uniform float radius;
uniform vec3 color, color2, color3;
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

float smoothStep1 = smoothstep(0.0, 0.5, TexCoord.x);
float smoothStep2 = smoothstep(0.33, 0.66, TexCoord.x);
float smoothStep3 = smoothstep(0.5, 1.0, TexCoord.x);

vec3 mix1 = mix(color, color2, smoothStep1);
vec3 mix2 = mix(color2, color3, smoothStep2);

vec3 finalColor = mix(mix1, mix2, smoothStep3);
FragColor = vec4(finalColor, smoothedAlpha);
}