#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

#define TX vec2(TexCoord.x, 1.0 - TexCoord.y)

uniform sampler2D sampler;

uniform vec2 textureSize;
uniform vec2 scale;
uniform vec4 textureInfo;

void main() {
    vec2 atlasTexCoord = ((TX * textureSize) + textureInfo.xy) * scale  / textureSize;

    vec4 sceneColor = texture(sampler, atlasTexCoord);
    vec4 brightPixels = max(sceneColor - vec4(0.5), vec4(0.0)) * 2.0;
    FragColor = brightPixels;
}
