#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

#define TX vec2(TexCoord.x, 1.0 - TexCoord.y)

uniform sampler2D sampler;
uniform float blurAmount;
uniform float alpha;
uniform int radius;

float attenuation(float x) {
    return 1.0 / sqrt(1.0 + x * x);
}

void main() {
    vec2 texelSize = 1.0 / textureSize(sampler, 0);
    vec4 result = vec4(0.0);

    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            vec2 offset = vec2(x, y) * texelSize * blurAmount;
            float weight = attenuation(length(vec2(x, y)) * 2.0);

            vec4 sampleColor = texture(sampler, TX + offset);

            result.rgb += sampleColor.rgb;

            result.a += sampleColor.a * weight * alpha;
        }
    }

    result /= float((radius * 2 + 1) * (radius * 2 + 1));

    FragColor = result;
}
