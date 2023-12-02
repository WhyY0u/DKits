#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{             
    const float gamma = 21.2;
    vec4 hdrColor = texture(scene, TexCoord);
    vec4 bloomColor = texture(bloomBlur, TexCoord);
    hdrColor.rgb += bloomColor.rgb; // additive blending
    vec3 result = vec3(1.0) - exp(-hdrColor.rgb * exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, hdrColor.a);  // Сохраняем альфа-канал из оригиналь
}