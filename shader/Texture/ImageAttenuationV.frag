#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

#define TX vec2(TexCoord.x, 1.0 - TexCoord.y)

uniform sampler2D sampler;
uniform float blurAmount;
uniform int radius;

 float attenuation(float d) {
     return 1.0 / (1.0 + d * d);
  }
void main() {
    vec2 texelSize = 1.0 / textureSize(sampler, 0);
    vec4 result = vec4(0.0); 

    for (int i = -radius; i <= radius; ++i) {
        vec2 offset = vec2(0, i) * texelSize * blurAmount;
        vec4 sampleColor = texture(sampler, TX + offset);
        float weight = attenuation(abs(float(i)));
        result.rgb += sampleColor.rgb;
        result.a += sampleColor.a * weight;
    }

    FragColor = result / float(2 * radius + 1);  
}
