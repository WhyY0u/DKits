#version 330 core

in vec2 TexCoord, VertexCord;
out vec4 FragColor;
uniform float radius;
uniform vec3 color, color2, color3, color4;
uniform float alpha;

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius) {
    vec2 d = abs(CenterPosition) - Size;
    return length(max(d, 0.0)) - Radius + min(max(d.x, d.y), 0.0);
}

void main() {
    vec2 size = vec2(1.0, 1.0);
    vec2 location = vec2(0.5, 0.5);
    float distance = roundedBoxSDF((TexCoord - location) * size, location - radius, radius);
    
    float smoothedAlpha = (1.0 - smoothstep(0.0, 0.002, (distance))) * alpha;


   vec3 mix1 = mix(color, color2, TexCoord.x);
   vec3 mix2 = mix(color3, color4, TexCoord.x);
   vec3 finalColor = mix(mix1, mix2, TexCoord.y);

    FragColor = vec4(finalColor, smoothedAlpha);
}