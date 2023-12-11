#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 textureSize;
uniform vec2 scale;
uniform vec4 textureInfo;
uniform sampler2D atlasSampler;

void main() {
   vec2 atlasTexCoord = ((TexCoord * textureSize) + textureInfo.xy) * scale  / textureSize;
    vec4 texColor = texture(atlasSampler, atlasTexCoord);
    FragColor = texColor;
  }
