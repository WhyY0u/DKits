#pragma once
#include <glad.h>
#include <vector>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <locale>
#include <codecvt>
#include <iostream>
#include "stb_image.h"
#include <map>
#include <cctype>
#include <algorithm>
#include <ft2build.h>
#include <freetype.h>
#include "MouseHelper.h"
enum Font {
    InterLight,
};
class Color {
public:
    Color() {
        red = 0.0f;
        green = 0.0f;
        blue = 0.0f;
    };
    Color(float red, float green, float blue) : red(red), green(green), blue(blue), alpha(1.0f) {}
    Color(float red, float green, float blue, float alpha) : red(red), green(green), blue(blue), alpha(alpha) {}
    Color(std::string hex) {
        if (hex.size() != 7 && hex.size() != 9) {
            std::cerr << "Invalid HEX color format." << std::endl;
            return;
        }
        std::istringstream iss(hex.substr(1));
        unsigned int hexValue;
        iss >> std::hex >> hexValue;
        red = static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;
        green = static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;
        blue = static_cast<float>(hexValue & 0xFF) / 255.0f;
        if (hex.size() == 9) {
            alpha = static_cast<float>((hexValue >> 24) & 0xFF) / 255.0f;
        }
        else {
            alpha = 1.0f;
        }
    }
    Color(int rgb) {
        int red = (rgb >> 16) & 0xFF;
        int green = (rgb >> 8) & 0xFF;
        int blue = rgb & 0xFF;
        int alpha = (rgb >> 24) & 0xFF;

        float redNormalized = static_cast<float>(red) / 255.0f;
        float greenNormalized = static_cast<float>(green) / 255.0f;
        float blueNormalized = static_cast<float>(blue) / 255.0f;
        float alphaNormalized = static_cast<float>(alpha) / 255.0f;

        this->red = redNormalized;
        this->green = greenNormalized;
        this->blue = blueNormalized;
        this->alpha = alphaNormalized;

    }
    void setRGB(int rgb) {
        int red = (rgb >> 16) & 0xFF;
        int green = (rgb >> 8) & 0xFF;
        int blue = rgb & 0xFF;
        int alpha = (rgb >> 24) & 0xFF;

        this->red = red;
        this->green = green;
        this->blue = blue;
        this->alpha = alpha;

    }
    void setHex(std::string hex) {
        if (hex.size() != 7 && hex.size() != 9) {
            std::cerr << "Invalid HEX color format." << std::endl;
            return;
        }
        std::istringstream iss(hex.substr(1));
        unsigned int hexValue;
        iss >> std::hex >> hexValue;
        red = static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;
        green = static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;
        blue = static_cast<float>(hexValue & 0xFF) / 255.0f;
        if (hex.size() == 9) {
            alpha = static_cast<float>((hexValue >> 24) & 0xFF) / 255.0f;
        }
        else {
            alpha = 1.0f;
        }
    }
    float getRed()
    {
        return red;
    }
    float getGreen()
    {
        return green;
    }
    float getBlue()
    {
        return blue;
    }
    float getAlpha() {
        return alpha;
    }
    void setRed(float red) {
        this->red = red;
    }
    void setGreen(float green) {
        this->green = green;
    }
    void setBlue(float blue) {
        this->blue = blue;
    }
    void setAlpha(float alpha) {
        this->alpha = alpha;
    }
    int getRGB() {
        int redInt = static_cast<int>(red * 255.0f);
        int greenInt = static_cast<int>(green * 255.0f);
        int blueInt = static_cast<int>(blue * 255.0f);
        int rgb = (redInt << 16) | (greenInt << 8) | blueInt;
        return rgb;
    }
private:
    float red, green, blue, alpha;
};
class RenderManager {

public:
    RenderManager(const std::string VertexURL, const std::string ShaderURL, const std::vector<float> vectors, std::vector<unsigned int> index) : vectors(vectors), index(index) {
        VertexShader = glCreateShader(GL_VERTEX_SHADER);
        FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* vertexShaderSource = readShaderFile(VertexURL);
        const char* fragmentShaderSource = readShaderFile(ShaderURL);

        if (!compileShader(VertexShader, GL_VERTEX_SHADER, vertexShaderSource) ||
            !compileShader(FragmentShader, GL_FRAGMENT_SHADER, fragmentShaderSource) ||
            !linkProgram(ShaderProgram, VertexShader, FragmentShader)) {
            std::cerr << "Shader creation failed!" << std::endl;
            ShaderProgram = 0;
        }
        delete vertexShaderSource;
        delete fragmentShaderSource;
        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vectors.size(), vectors.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index.size(), index.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    ~RenderManager() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
        glDeleteProgram(ShaderProgram);
    }
    void useShader(float width, float height) {
        glUseProgram(ShaderProgram);
        glm::mat4 projectionMatrix = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
        int projectionLoc = glGetUniformLocation(ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }
    void setUniform1i(const std::string name, int n) {
        glad_glUniform1i(glad_glGetUniformLocation(ShaderProgram, name.c_str()), n);
    }
    void setUniform1f(const std::string name, float n) {
        glad_glUniform1f(glad_glGetUniformLocation(ShaderProgram, name.c_str()), n);
    }
    void setUniform2f(const std::string name, float n, float n2) {
        glad_glUniform2f(glad_glGetUniformLocation(ShaderProgram, name.c_str()), n, n2);
    }
    void setUniform3f(const std::string name, float n, float n2, float n3) {
        glad_glUniform3f(glad_glGetUniformLocation(ShaderProgram, name.c_str()), n, n2, n3);
    }
    void setUniform4f(const std::string name, float n, float n2, float n3, float n4) {
        glad_glUniform4f(glad_glGetUniformLocation(ShaderProgram, name.c_str()), n, n2, n3, n4);
    }
    void PreRender() {

    }
    void Render() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    void StopRender() {
        glUseProgram(0);
    }

    void loadTexture(const char* filename, int width, int height) {
        int channels;
        unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
    }

    void renderTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    void deleteTexture() {
        glDeleteTextures(1, &textureID);
    }
private:
    std::vector<float> vectors;
    std::vector<unsigned int> index;
    GLuint VertexShader, FragmentShader, ShaderProgram;
    GLuint VAO, VBO, EBO;
    GLuint textureID;
    const char* readShaderFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file) {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return nullptr;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string shaderSource = buffer.str();

        char* shaderSourcePtr = new char[shaderSource.length() + 1];

        strcpy_s(shaderSourcePtr, shaderSource.length() + 1, shaderSource.c_str());

        return shaderSourcePtr;
    }
    const bool compileShader(GLuint& shader, GLenum type, const char* source) {
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation failed: " << infoLog << std::endl;
            return false;
        }
        return true;
    }
    const bool linkProgram(GLuint& programID, GLuint vertexShader, GLuint fragmentShader) {
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "Shader program linking failed: " << infoLog << std::endl;
            return false;
        }
        return true;
    }

};
struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};
class FontRender {
public:
    FontRender(std::string urlFont, std::wstring text, FT_Library ft, int size) {
        this->text = text;
        if (FT_New_Face(ft, urlFont.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        FT_Set_Pixel_Sizes(face, 0, size);
        vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertexShaderSource = readShaderFile("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/VertexText.vert");
        const char* fragmentShaderSource = readShaderFile("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Text/text.frag");


        if (!compileShader(vertexShaderId, GL_VERTEX_SHADER, vertexShaderSource) ||
            !compileShader(fragmentShaderId, GL_FRAGMENT_SHADER, fragmentShaderSource) ||
            !linkProgram(shaderProgram, vertexShaderId, fragmentShaderId)) {
            std::cerr << "Shader creation failed!" << std::endl;
            shaderProgram = 0;
        }
        delete[] vertexShaderSource;
        delete[] fragmentShaderSource;
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        createTextures();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }
    float textSize;
    void renderText(float x, float y, float scale, Color color, float alpha, float screenWidth, float screenHeight) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        glUseProgram(shaderProgram);
        glm::mat4 projectionMatrix = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        int colorLoc = glGetUniformLocation(shaderProgram, "color");
        int alphaLoc = glGetUniformLocation(shaderProgram, "alpha");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(colorLoc, color.getRed(), color.getGreen(), color.getBlue());
        glUniform1f(alphaLoc, alpha);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);
        std::wstring::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - ch.Bearing.y * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (ch.Advance >> 6) * scale;
            textSize += (ch.Advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
        FT_Done_Face(face);
    }
    int MaxTexture = 0;
    void clearTextures() {
        for (unsigned int i = 0; i < MaxTexture; i++) {
            glDeleteTextures(1, &i);
        }
        Characters.clear();
    }
    void createTextures() {
        clearTextures();
        for (unsigned int i = 0; i < text.size(); i++) {
            wchar_t c = text[i];
            if (FT_Load_Char(face, (FT_ULong)text[i], FT_LOAD_RENDER)) {
                FT_Error error = FT_Load_Char(face, (FT_ULong)text[i], FT_LOAD_RENDER);
                std::cout << error << std::endl;
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            if (texture > MaxTexture) MaxTexture = texture;
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );



            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<wchar_t, Character>(c, character));
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    ~FontRender() {
        clearTextures();
    }
    void setText(std::wstring textt) {
        text = textt;
    }
    std::wstring getText() {
        return text;
    }
private:
    Font f;
    const char* readShaderFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file) {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return nullptr;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string shaderSource = buffer.str();

        char* shaderSourcePtr = new char[shaderSource.length() + 1];

        strcpy_s(shaderSourcePtr, shaderSource.length() + 1, shaderSource.c_str());

        return shaderSourcePtr;
    }
    const bool compileShader(GLuint& shader, GLenum type, const char* source) {
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation failed: " << infoLog << std::endl;
            return false;
        }
        return true;
    }
    const bool linkProgram(GLuint& programID, GLuint vertexShader, GLuint fragmentShader) {
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "Shader program linking failed: " << infoLog << std::endl;
            return false;
        }
        return true;
    }
    std::map<wchar_t, Character> Characters;
    std::wstring text;
    FT_Face face;
    unsigned int VAO, VBO;
    GLuint vertexShaderId, fragmentShaderId, shaderProgram;
};
class FrameBuffer {
public:
    FrameBuffer(float width, float height) {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void renderTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    void StartRead() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }
    void StopRead() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    ~FrameBuffer() {
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteTextures(1, &texture);
    }

    GLuint getTexture() {
        return texture;
    }

private:
    GLuint texture;
    GLuint framebuffer;

};

class RenderUtils {
public:
    static RenderUtils& getInstance() {
        static RenderUtils instance;
        return instance;
    }
    float screenWidth;
    float screenHeight;

    std::string getFont(Font font) {
        switch (font) {
        case InterLight:
            return "C:/Users/User/Desktop/APISystem/DKIT/Project1/font/InterLight.ttf";
        default:
            return "Unknown";
        }
    }
    void drawImageAlpha(const char* str, float x, float y, float width, float height, float alpha) {
        std::vector<float> vertex = {
       x, y, 0.0f, 0.0f, 0.0f,
       x + width, y, 0.0f, 1.0f, 0.0f,
       x + width, y + height, 0.0f, 1.0f, 1.0f,
       x, y + height, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageAlpha.frag", vertex, indices);
        manager->PreRender();
        manager->loadTexture(str, width, height);
        manager->useShader(screenWidth, screenHeight);
        manager->renderTexture();
        manager->setUniform1f("sampler", GL_TEXTURE0);
        manager->setUniform1f("alpha", alpha);
        manager->Render();
        manager->deleteTexture();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();
    }
    void drawTripleGradient(float x, float y, float width, float height, Color color, Color color2, Color color3, float radius, float alpha) {
        std::vector<float> vertex = {
      x, y, 0.0f, 0.0f, 0.0f,
      x + width, y, 0.0f, 1.0f, 0.0f,
      x + width,  y + height, 0.0f, 1.0f, 1.0f,
      x, y + height, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };
        glEnable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/React/TripleGradient.frag", vertex, indices);
        manager->PreRender();
        manager->useShader(screenWidth, screenHeight);
        manager->setUniform3f("color", color.getRed(), color.getGreen(), color.getBlue());
        manager->setUniform3f("color2", color2.getRed(), color2.getGreen(), color2.getBlue());
        manager->setUniform3f("color3", color3.getRed(), color3.getGreen(), color3.getBlue());
        manager->setUniform1f("radius", radius);
        manager->setUniform1f("alpha", alpha);
        manager->Render();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();
    }
    void drawText(float x, float y, Font font, std::wstring text, FT_Library ft, float size, Color color, float alpha) {
        FontRender* ps = new FontRender(getFont(font), text, ft, size);
        ps->renderText(x, y, 1.0, color, alpha, screenWidth, screenHeight);
        delete ps;
    }

    void RectGlow(float x, float y, float width, float height, Color color, Color color2, Color color3, Color color4, float fadeStart, float fadeEnd, float shadowSoftness, float radius) {

        std::vector<float> vertex = {
        x, y, 0.0f, 0.0f, 0.0f,
        width, y, 0.0f, 1.0f, 0.0f,
        width,  height, 0.0f, 1.0f, 1.0f,
        x, height, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };
        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/React/GlowReact.frag", vertex, indices);
        manager->PreRender();
        manager->useShader(screenWidth, screenHeight);

        manager->setUniform3f("color", color.getRed(), color.getGreen(), color.getBlue());
        manager->setUniform3f("color2", color2.getRed(), color2.getGreen(), color2.getBlue());
        manager->setUniform3f("color3", color3.getRed(), color3.getGreen(), color3.getBlue());
        manager->setUniform3f("color4", color4.getRed(), color4.getGreen(), color4.getBlue());

        manager->setUniform3f("outlineColor", 1.0, 1.0, 1.0);
        manager->setUniform3f("outlineColor2", 1.0, 1.0, 1.0);
        manager->setUniform3f("outlineColor3", 1.0, 1.0, 1.0);
        manager->setUniform3f("outlineColor4", 1.0, 1.0, 1.0);

        manager->setUniform1f("fadeStart", fadeStart);
        manager->setUniform1f("fadeEnd", fadeEnd);
        manager->setUniform1f("shadowSoftness", shadowSoftness);
        manager->setUniform1f("radius", radius);

        manager->Render();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();
    }
    void Rect(float x, float y, float width, float height, Color color, Color color2, Color color3, Color color4, float radius, float alpha) {
        std::vector<float> vertex = {
        x, y, 0.0f, 0.0f, 0.0f,
        width, y, 0.0f, 1.0f, 0.0f,
        width,  height, 0.0f, 1.0f, 1.0f,
        x, height, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };
        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/React/React.frag", vertex, indices);
        manager->PreRender();
        manager->useShader(screenWidth, screenHeight);
        manager->setUniform3f("color", color.getRed(), color.getGreen(), color.getBlue());
        manager->setUniform3f("color2", color2.getRed(), color2.getGreen(), color2.getBlue());
        manager->setUniform3f("color3", color3.getRed(), color3.getGreen(), color3.getBlue());
        manager->setUniform3f("color4", color4.getRed(), color4.getGreen(), color4.getBlue());
        manager->setUniform1f("radius", radius);
        manager->setUniform1f("alpha", alpha);

        manager->Render();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();

    }
    void drawTextureColor(const char* str, float x, float y, float width, float height, Color color) {
        std::vector<float> vertex = {
        x, y, 0.0f, 0.0f, 0.0f,
        x + width, y, 0.0f, 1.0f, 0.0f,
        x + width, y + height, 0.0f, 1.0f, 1.0f,
        x, y + height, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageShadow.frag", vertex, indices);
        manager->PreRender();
        manager->loadTexture(str, width, height);
        manager->useShader(screenWidth, screenHeight);
        manager->renderTexture();
        manager->setUniform1f("sampler", GL_TEXTURE0);
        manager->setUniform3f("color", color.getRed(), color.getGreen(), color.getBlue());
        manager->Render();
        manager->deleteTexture();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();

    }
    void drawTexture(const char* str, float x, float y, float width, float height) {
        std::vector<float> vertex = {
        x, y, 0.0f, 0.0f, 0.0f,
        x + width, y, 0.0f, 1.0f, 0.0f,
        x + width, y + height, 0.0f, 1.0f, 1.0f,
        x, y + height, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/Image.frag", vertex, indices);
        manager->PreRender();
        manager->loadTexture(str, width, height);
        manager->useShader(screenWidth, screenHeight);
        manager->renderTexture();
        manager->setUniform1i("sampler", GL_TEXTURE0);
        manager->Render();
        manager->deleteTexture();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();

    }
    void drawGlowTexture(const char* str, float x, float y, float width, float height) {
        FrameBuffer* buffer = new FrameBuffer(screenWidth, screenHeight);
        buffer->StartRead();
        drawTexture(str, x, y, width, height);
        buffer->StopRead();


        std::vector<float> vertex = {
     0, 0, 0.0f, 0.0f, 0.0f,
     0 + screenWidth, 0, 0.0f, 1.0f, 0.0f,
     0 + screenWidth, 0 + screenHeight, 0.0f, 1.0f, 1.0f,
     0, 0 + screenHeight, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageGlow.frag", vertex, indices);
        manager->PreRender();
        manager->useShader(screenWidth, screenHeight);
        buffer->renderTexture();
        manager->setUniform1i("sampler", GL_TEXTURE0);
        manager->Render();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();
        delete buffer;
    }
    void drawFrameBuffer(GLuint texture, float x, float y, float width, float height, float textureWidth, float textureHeight) {
        std::vector<float> vertex = {
        x, y, 0.0f, 0.0f, 0.0f,
        width, y, 0.0f, 1.0f, 0.0f,
        width,  height, 0.0f, 1.0f, 1.0f,
        x, height, 0.0f, 0.0f, 1.0f
        };
        std::vector<unsigned int> indices = {
           0, 1, 2,
           2, 3, 0
        };
        RenderManager* manager = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/Image.frag", vertex, indices);
        manager->PreRender();
        manager->loadTexture("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/close.png", textureWidth, textureHeight);
        manager->useShader(screenWidth, screenHeight);
        manager->renderTexture();
        manager->setUniform1f("sampler", texture);
        manager->Render();
        manager->deleteTexture();
        manager->StopRender();
        delete manager;
        vertex.resize(0);
        vertex.shrink_to_fit();
        indices.resize(0);
        indices.shrink_to_fit();

    }
    void UpdateRenderInfo(float w, float h) {
        screenHeight = h;
        screenWidth = w;
    }
private:
    RenderUtils() : screenWidth(800), screenHeight(600) {}
    RenderUtils(const RenderUtils&) = delete;
    void operator=(const RenderUtils&) = delete;
};

