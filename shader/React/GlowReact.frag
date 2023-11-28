#version 330 core

in vec2 TexCoord, VertexCord;
out vec4 FragColor;

uniform float fadeStart, fadeEnd, shadowSoftness, radius;
uniform vec3 color, color2, color3, color4;

uniform vec3 outlineColor, outlineColor2, outlineColor3, outlineColor4;

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius) {
    vec2 d = abs(CenterPosition) - Size+Radius;
    return length(max(d, 0.0)) - Radius + min(max(d.x, d.y), 0.0);
}

void main() {
    vec2 size = vec2(1.0,1.0);
    vec2 location = vec2(0.5, 0.5);
    float distance = roundedBoxSDF((TexCoord - location) * size, location - radius, radius);
    float smoothedAlpha = mix(1.0, 0.0, smoothstep(fadeStart, fadeEnd, distance));
    float smoothedAlphaShadow = 1.0 - smoothstep(0.0, shadowSoftness, sqrt(distance));

    vec3 mix1 = mix(vec3(1.0, 0.0, 0.0), vec3(0.4, 0.0, 1.0), TexCoord.x);
    vec3 mix2 = mix(vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), TexCoord.x);
    vec3 mix3 = mix(mix1, mix2 , TexCoord.y);

    vec3 outmix = mix(outlineColor,outlineColor2,TexCoord.x);
    vec3 outmix2 = mix(outlineColor3,outlineColor4,TexCoord.x);
    vec3 outmix3 = mix(outmix,outmix2,TexCoord.y);


    FragColor = vec4(mix3,  smoothedAlphaShadow * smoothedAlpha);
}
