#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D sampler;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);


void main() {
    vec4 texColor = texture(sampler, TexCoord);
    vec4 bluredColor = vec4(0.0);

    vec2 tex_offset = 1.0 / textureSize(sampler, 0);
    vec3 result = texture(sampler, TexCoord).rgb * weight[0];
    vec4 alpha;
        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampler, TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(sampler, TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            
            alpha  += texture(sampler, TexCoord + vec2(tex_offset.x * i, 0.0)).rgba * weight[i];
            alpha  += texture(sampler, TexCoord - vec2(tex_offset.x * i, 0.0)).rgba * weight[i];
        }

        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampler, TexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(sampler, TexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            alpha += texture(sampler, TexCoord + vec2(0.0, tex_offset.y * i)).rgba * weight[i];
            alpha += texture(sampler, TexCoord - vec2(0.0, tex_offset.y * i)).rgba * weight[i];
        }
  
        FragColor = alpha;
}
