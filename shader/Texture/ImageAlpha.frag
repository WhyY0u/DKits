#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 textureSize;
uniform vec2 scale;
uniform sampler2D atlasSampler;
uniform vec4 textureInfo;
uniform float alpha;
void main() {
    vec2 atlasTexCoord = ((TexCoord * textureSize) + textureInfo.xy) * scale  / textureSize;
    vec4 texColor = texture(atlasSampler, atlasTexCoord);
    FragColor = vec4(texColor.rgb, alpha);
  }
