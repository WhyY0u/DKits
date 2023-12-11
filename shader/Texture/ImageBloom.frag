#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure = 1.0;

uniform vec2 textureSize;
uniform vec2 scale;
uniform vec4 textureInfo;


void main()
{
    vec2 atlasTexCoord = ((TexCoord * textureSize) + textureInfo.xy) * scale  / textureSize;
    const float gamma = 0.5;
    vec4 hdrColor = texture(scene, atlasTexCoord);
    vec4 bloomColor = texture(bloomBlur, atlasTexCoord);
    hdrColor.rgb += bloomColor.rgb;
    vec3 result = vec3(1.0) - exp(-hdrColor.rgb * exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, hdrColor.a);  
}
