#version 330 core

in vec2 TexCoord, VertexCord;
out vec4 FragColor;

const float msdiv = 3.312;
const vec3 msat = vec3(.3,.3,.3);


vec3 saturate( vec3 v ) {
  float sv = dot(v,msat);
  vec3 hs = v/sv*msdiv;
  float ls = (1.-exp(-sv))*.9;
  float s = 1.-exp(-sv*sv/10.);
  vec3 hs2 = mix(hs,vec3(1),s);
  return ls*hs2/mix(msdiv,1.,s);
}

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius) {
    vec2 d = abs(CenterPosition) - Size;
    return length(max(d, 0.0)) - Radius + min(max(d.x, d.y), 0.0);
}

void main() {
    vec2 size = vec2(1.0, 1.0);
    vec2 location = vec2(0.5, 0.5);
    float radius = 0.2;
    float distance = roundedBoxSDF((TexCoord - location) * size, location - radius, radius);
    
    float smoothedAlpha = 1.0 - smoothstep(0.0, 0.01, distance);


    float shadowSoftness = 22.0;
    vec3 mix1 = mix(vec3(1.0, 0.0, 0.0), vec3(0.4, 0.0, 1.0), TexCoord.x);
    vec3 mix2 = mix(vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), TexCoord.x);
    vec3 mix3 = mix(mix1, mix2, TexCoord.y);

    FragColor = vec4(mix3, smoothedAlpha);
}