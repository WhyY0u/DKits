#version 330 core
out vec2 TexCoord; 
out vec3 VertexCord; 

layout (location = 0) in vec3 vertexcord;
layout (location = 1) in vec2 texCoord;
uniform mat4 projection; 

void main() {
    TexCoord = texCoord;
    VertexCord = vertexcord;
    gl_Position = projection * vec4(VertexCord, 1.0);
}
