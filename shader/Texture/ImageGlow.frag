#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

#define TX vec2(TexCoord.x, 1.0 - TexCoord.y)

uniform sampler2D sampler;
uniform float blurRadius;
uniform float alpha;
uniform float total;
uniform float Wtotal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);


void main() {
    vec4 texColor = texture(sampler, TexCoord);
    vec4 bluredColor = vec4(0.0);

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

        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampler, TexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(sampler, TexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            alpha += texture(sampler, TexCoord + vec2(0.0, tex_offset.y * i)).rgba * weight[i];
            alpha += texture(sampler, TexCoord - vec2(0.0, tex_offset.y * i)).rgba * weight[i];
        }
  
    FragColor = vec4(blurredColor.rgb, blurredColor.a * alpha) / totalWeight * total;
}
