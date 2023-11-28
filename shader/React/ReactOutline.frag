#version 330 core

in vec2 TexCoord, VertexCord;
out vec4 FragColor;

uniform vec3 color, color2, color3, color4;
uniform vec3 outlines;
uniform float radius;
uniform float outlineWidth;

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius) {
    vec2 d = abs(CenterPosition) - Size;
    return length(max(d, 0.0)) - Radius + min(max(d.x, d.y), 0.0);
}

void main() {
    vec2 size = vec2(1.0, 1.0);
    vec2 location = vec2(0.5, 0.5);
    float distance = roundedBoxSDF((TexCoord - location) * size, location - radius, radius);
    
    // Calculate the distance to the inner edge of the rounded box
    float innerDistance = distance - outlineWidth;

    // Check if the pixel is inside the inner edge for outlining
    float outline = smoothstep(innerDistance, distance, 0.0);

    float smoothedAlpha = 1.0 - smoothstep(0.0, 0.01, distance);

    vec3 mix1 = mix(color, color2, TexCoord.x);
    vec3 mix2 = mix(color3, color4, TexCoord.x);
    vec3 mix3 = mix(mix1, mix2, TexCoord.y);

    // Mix the outline color with the regular color using the outline factor
    vec3 outlinedColor = mix(outlines, mix3, outline);

    FragColor = vec4(outlinedColor, smoothedAlpha);
}
