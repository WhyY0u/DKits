#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

#define TX vec2(TexCoord.x, 1.0 - TexCoord.y)

uniform sampler2D sampler;
uniform float blurRadius;
uniform float alpha;
uniform float total;
uniform float Wtotal;

void main() {
    vec2 tex_offset = 1.0 / textureSize(sampler, 0);
    vec4 blurredColor = vec4(0.0);
    float totalWeight = 0.0;
 
    for (float i = -blurRadius; i <= blurRadius; i += 1.0) {
         float distance;
         float is = i * i;
        for (float j = -blurRadius; j <= blurRadius; j += 1.0) {
            distance = sqrt(is + j * j);
            if (distance <= blurRadius) {
                float weight = 1.0 - (distance / blurRadius);
                blurredColor += texture(sampler, TX + vec2(tex_offset.x * i, tex_offset.y * j)) * Wtotal * weight;
                totalWeight += weight;
            }
        }
    }

    FragColor = vec4(blurredColor.rgb, blurredColor.a * alpha) / totalWeight * total;
}