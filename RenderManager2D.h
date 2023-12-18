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
#include <map>
#include <cctype>
#include <algorithm>
#include <ft2build.h>
#include <freetype.h>
#include "MouseHelper.h"
#include <future>
#include "ImageHelper.h"


enum Font {
    InterLight,
    InterBold,
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
    RenderManager() {};
    RenderManager(const std::string VertexURL, const std::string ShaderURL, const std::vector<float> vectors, const std::vector<unsigned int> index) : vectors(vectors), index(index) {
        VertexShader = glCreateShader(GL_VERTEX_SHADER);
        FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* vertexShaderSource = readShaderFile(VertexURL);
        const char* fragmentShaderSource = readShaderFile(ShaderURL);

        if (!compileShader(VertexShader, GL_VERTEX_SHADER, vertexShaderSource) ||
            !compileShader(FragmentShader, GL_FRAGMENT_SHADER, fragmentShaderSource) ||
            !linkProgram(ShaderProgram, VertexShader, FragmentShader)) {
            std::cerr << "Shader creation failed!" + ShaderURL << std::endl;
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
    void UpdateVectors(std::vector<float> v, std::vector<unsigned int> i) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(int), i.data(), GL_STATIC_DRAW);
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
        GLint location = glad_glGetUniformLocation(ShaderProgram, name.c_str());
        if (location != -1) {
            GLint currentIntValue;
            glGetUniformiv(ShaderProgram, location, &currentIntValue);
            if (currentIntValue != n) {
               glad_glUniform1i(location, n);
            } else {
                return;
            }
        }
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
        int channels = 0;
        unsigned char* imageData = ImageHelper::getInstance().loadImage(filename, width, height, channels);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

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
struct Characters {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
    float fontsize;
};
struct FontInfo {
    FT_Face face;
    std::map<wchar_t, Characters> characters;
    std::vector<GLuint> textureDel;
    std::string urlFont;     
};
class NewFontRender {
public:
    NewFontRender(FT_Library ft): ft(ft) {
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
        std::cout << "Suces Compiled Shader" << std::endl;
        delete[] vertexShaderSource;
        delete[] fragmentShaderSource;
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    
    };

    std::string getFont(Font font) {
        switch (font) {
        case InterBold:
            return "C:/Users/User/Desktop/APISystem/DKIT/Project1/font/InterBold.ttf";
        case InterLight:
            return "C:/Users/User/Desktop/APISystem/DKIT/Project1/font/InterLight.ttf";
        default:
            return "Unknown";
        }
    }
    FT_Library getFontLibrary() {
        return ft;
    }
    void renderText(Font font, std::wstring text, float x, float y, float fontsize, float scale, Color color, float screenWidth, float screenHeight) {

        FontInfo f;
        std::map<Font, FontInfo>::iterator textur = fonts.find(font);
        if (textur == fonts.end()) {
           f = createFont(font, getFont(font));
           createTexture(f, fontsize, text, font);
        } else {
            f = textur->second;
                updateTextureSize(font, f, text, fontsize);
            
        }

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
        glUniform1f(alphaLoc, color.getAlpha());
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);
        std::wstring::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Characters ch = f.characters[*c];

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
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

         
    }
   

    void delteFont(Font font) {
        std::map<Font, FontInfo>::iterator texturs = fonts.find(font);
        texturs->second.characters.clear();
        for (int i = 0; i < texturs->second.textureDel.size(); i++) {
            glDeleteTextures(1, &texturs->second.textureDel[i]);
        }
        FT_Done_Face(texturs->second.face);
    }

    void ClearAll() {
        for (auto it = fonts.begin(); it != fonts.end(); it++) {
            FT_Done_Face(it->second.face);
            for (int i = 0; i < it->second.textureDel.size(); i++) {
                glDeleteTextures(1, &it->second.textureDel[i]);
            }
        }
    }

private:

    FontInfo createFont(Font font, std::string urlFont) {
        /*Если фонта нет в списе вызываем этот метод и возращает все готовое*/
        FontInfo info = FontInfo();
        FT_Face face;
        info.urlFont = urlFont;
        if (FT_New_Face(ft, urlFont.c_str(), 0, &face)) {
            std::cout << "Ошибка не удалось создать FT_Face Для " + urlFont << std::endl;
        }
        info.face = face;
        return info;
    }

    void createTexture(FontInfo info, float size, std::wstring text, Font font) {

        FT_Set_Pixel_Sizes(info.face, 0, size);

        for (unsigned int i = 0; i < text.size(); i++) {
            wchar_t c = text[i];
            if (FT_Load_Char(info.face, (FT_ULong)text[i], FT_LOAD_RENDER)) {
                FT_Error error = FT_Load_Char(info.face, (FT_ULong)text[i], FT_LOAD_RENDER);
                std::cout << error << std::endl;
                continue;
            }
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                info.face->glyph->bitmap.width,
                info.face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                info.face->glyph->bitmap.buffer
            );
            info.textureDel.push_back(texture);



            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Characters character = {
                texture,
                glm::ivec2(info.face->glyph->bitmap.width, info.face->glyph->bitmap.rows),
                glm::ivec2(info.face->glyph->bitmap_left, info.face->glyph->bitmap_top),
                static_cast<unsigned int>(info.face->glyph->advance.x),
                size,
            };
            info.characters.insert(std::pair<wchar_t, Characters>(c, character));
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        fonts.insert(std::pair<Font, FontInfo>(font, info));
    }

    void updateTextureSize(Font font, FontInfo info, std::wstring text, float size) {
        /*Обновляем текстуру если размер изменился*/
        FT_Set_Pixel_Sizes(info.face, 0, size);
      
        for (unsigned int i = 0; i < text.size(); i++) {
            wchar_t c = text[i];
            std::map<wchar_t, Characters>::iterator ch = info.characters.find(c);
            if (ch != info.characters.end()) {
                if (ch->second.fontsize == size) {

                    continue;
                }
                if (FT_Load_Char(info.face, (FT_ULong)text[i], FT_LOAD_RENDER)) {
                    FT_Error error = FT_Load_Char(info.face, (FT_ULong)text[i], FT_LOAD_RENDER);
                    std::cout << error << std::endl;
                    continue;
                }
                ch->second.Size = glm::ivec2(info.face->glyph->bitmap.width, info.face->glyph->bitmap.rows);
                ch->second.Bearing = glm::ivec2(info.face->glyph->bitmap_left, info.face->glyph->bitmap_top);
                glBindTexture(GL_TEXTURE_2D, ch->second.TextureID);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, info.face->glyph->bitmap.width, info.face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, info.face->glyph->bitmap.buffer);
            } else {
                
                if (FT_Load_Char(info.face, (FT_ULong)text[i], FT_LOAD_RENDER)) {
                    FT_Error error = FT_Load_Char(info.face, (FT_ULong)text[i], FT_LOAD_RENDER);
                    std::cout << error << std::endl;
                    continue;
                }
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    info.face->glyph->bitmap.width,
                    info.face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    info.face->glyph->bitmap.buffer
                );

                info.textureDel.push_back(texture);



                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Characters character = {
                    texture,
                    glm::ivec2(info.face->glyph->bitmap.width, info.face->glyph->bitmap.rows),
                    glm::ivec2(info.face->glyph->bitmap_left, info.face->glyph->bitmap_top),
                    static_cast<unsigned int>(info.face->glyph->advance.x),
                    size,
                };
                std::map<Font, FontInfo>::iterator textur = fonts.find(font);
                textur->second.characters.insert(std::pair<wchar_t, Characters>(c, character));

                
            }
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
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
    std::map<Font, FontInfo> fonts;
    GLuint vertexShaderId, fragmentShaderId, shaderProgram;
    GLuint VAO, VBO;
    FT_Library ft;
};
class FontRender {
public:
    FontRender() {  
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
    std::string getFont(Font font) {
    switch (font) {
    case InterBold:
        return "C:/Users/User/Desktop/APISystem/DKIT/Project1/font/InterBold.ttf";
    case InterLight:
        return "C:/Users/User/Desktop/APISystem/DKIT/Project1/font/InterLight.ttf";
    default:
        return "Unknown";
    }
}
    float textSize;
    void renderText(std::wstring text, std::string urlFont, float x, float y, float scale, Color color, float alpha, int size, float screenWidth, float screenHeight, FT_Library ft) {

        if (FT_New_Face(ft, urlFont.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        FT_Set_Pixel_Sizes(face, 0, size);
        createTextures(text);
   

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
        FT_Done_Face(face);
    }
    void ClearFontRender() {
        for (GLuint textureID : textureIDsToDelete) {
            glDeleteTextures(1, &textureID);
        }
        textureIDsToDelete.clear();
        Characters.clear();
    }
    void createTextures(std::wstring text) {
        for (unsigned int i = 0; i < text.size(); i++) {
            wchar_t c = text[i];
            if (FT_Load_Char(face, (FT_ULong)text[i], FT_LOAD_RENDER)) {
                FT_Error error = FT_Load_Char(face, (FT_ULong)text[i], FT_LOAD_RENDER);
                std::cout << error << std::endl;
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
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
            textureIDsToDelete.push_back(texture);



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
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
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
    std::vector<GLuint> textureIDsToDelete;
    FT_Face face;
    unsigned int VAO, VBO;
    GLuint vertexShaderId, fragmentShaderId, shaderProgram;
};
class FrameBuffer {
public:
    FrameBuffer(int width, int height) {
        this->width = width;
        this->height = height;

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
    void UpdateSizeBuffer(FrameBuffer buffer, int width, int height) {
        if (buffer.width != width || buffer.height != height) {
            glDeleteFramebuffers(1, &buffer.framebuffer);
            glDeleteTextures(1, &buffer.texture);

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
            return;
        }
    }
    void renderTexture(GLuint t) {
        glActiveTexture(t);
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
    int width, height;
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


     std::vector<unsigned int> indices = {
       0, 1, 2,
       2, 3, 0
    };
     std::vector<float> vertex;
     NewFontRender* newfontrender;
     FontRender* fontrender;
     RenderManager* Hyperbalic;
     RenderManager* ImageBrightness;
     RenderManager* rect;
     RenderManager* imageAlpha;
     RenderManager* image;
     RenderManager* imageColor;
     RenderManager* tripleGradient;
     RenderManager* glowRect;
     RenderManager* ImageGlow;
     RenderManager* ImageFBO;
     RenderManager* Bloom;
     RenderManager* ImageShadow;

    void initRenderUtils() {
     fontrender = new FontRender();

     FT_Library ft;
     if (FT_Init_FreeType(&ft)) {
         std::cout << "Error FT" << std::endl;
         glfwTerminate();
     }
     else {
         std::cout << "Suces Create FT" << std::endl;
     }
     newfontrender = new NewFontRender(ft);
     
     ImageBrightness = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageBrightness.frag", vertex, indices);
     Hyperbalic = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/Hyperbalic.frag", vertex, indices);
     ImageFBO = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageFBO.frag", vertex, indices);
     rect = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/React/React.frag", vertex, indices);
     imageAlpha = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageAlpha.frag", vertex, indices);
     tripleGradient = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/React/TripleGradient.frag", vertex, indices);
     glowRect = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/React/GlowReact.frag", vertex, indices);
     imageColor = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageColor.frag", vertex, indices);
     Bloom = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageBloom.frag", vertex, indices);
     ImageGlow = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageGlow.frag", vertex, indices);
     ImageShadow = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/ImageShadow.frag", vertex, indices);
     image = new RenderManager("C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Vertex/Vertex.vert", "C:/Users/User/Desktop/APISystem/DKIT/Project1/shader/Texture/Image.frag", vertex, indices);
    }
    void ClearShader() {
        delete ImageBrightness;
        delete Hyperbalic;
        delete ImageFBO;
        delete rect;
        delete imageAlpha;
        delete image;
        delete tripleGradient;
        delete glowRect;
        delete imageColor;
        delete Bloom;
        delete ImageGlow;
        delete ImageShadow;


    }
    
    void drawTextureAlpha(std::string str, float x, float y, float width, float height, float alpha) {
        RenderTextureHelper textureHelper = RenderTextureHelper::getInstance();
        TextureAtlas& atla = textureHelper.atlas;
        if (atla.textures.find(str) == atla.textures.end()) {
            atla.addTexture(str);
        }
        vertex = {
 x, y, 0.0f, 0.0f, 0.0f,
 x + width, y, 0.0f, 1.0f, 0.0f,
 x + width,  y + height, 0.0f, 1.0f, 1.0f,
 x, y + height, 0.0f, 0.0f, 1.0f
        };
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        TextureInfo& value = atla.textures.find(str)->second;
        imageAlpha->UpdateVectors(vertex, indices);
        imageAlpha->PreRender();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atla.getTextureID());
        imageAlpha->useShader(screenWidth, screenHeight);
        imageAlpha->setUniform1i("atlasSampler", GL_TEXTURE0);
        imageAlpha->setUniform1f("alpha", alpha);
        imageAlpha->setUniform2f("scale", value.width / atla.maxsize, value.height / atla.maxsize);
        imageAlpha->setUniform2f("textureSize", value.width, value.height);
        imageAlpha->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z, value.pos.w);
        imageAlpha->Render();
        imageAlpha->StopRender();
    }
    void drawTexture(std::string str, float x, float y, float width, float height) {
        RenderTextureHelper textureHelper = RenderTextureHelper::getInstance();
        TextureAtlas& atla = textureHelper.atlas;

        if (atla.textures.find(str) == atla.textures.end()) {
            atla.addTexture(str);
        }
        vertex = {
 x, y, 0.0f, 0.0f, 0.0f,
 x + width, y, 0.0f, 1.0f, 0.0f,
 x + width,  y + height, 0.0f, 1.0f, 1.0f,
 x, y + height, 0.0f, 0.0f, 1.0f
        };
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        TextureInfo& value = atla.textures.find(str)->second;
        image->UpdateVectors(vertex, indices);
        image->PreRender();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atla.getTextureID());
        image->useShader(screenWidth, screenHeight);
        image->setUniform1i("atlasSampler", GL_TEXTURE0);
        image->setUniform2f("scale", value.width / atla.maxsize, value.height / atla.maxsize);
        image->setUniform2f("textureSize", value.width, value.height);
        image->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z , value.pos.w);
        image->Render();
        image->StopRender();
    }
    void drawTripleGradient(float x, float y, float width, float height, Color color, Color color2, Color color3, float radius, float alpha) {
         vertex = {
      x, y, 0.0f, 0.0f, 0.0f,
      x + width, y, 0.0f, 1.0f, 0.0f,
      x + width,  y + height, 0.0f, 1.0f, 1.0f,
      x, y + height, 0.0f, 0.0f, 1.0f
        };
        glEnable(GL_BLEND);
        glEnable(GL_ALPHA_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        tripleGradient->UpdateVectors(vertex, indices);
        tripleGradient->PreRender();
        tripleGradient->useShader(screenWidth, screenHeight);
        tripleGradient->setUniform3f("color", color.getRed(), color.getGreen(), color.getBlue());
        tripleGradient->setUniform3f("color2", color2.getRed(), color2.getGreen(), color2.getBlue());
        tripleGradient->setUniform3f("color3", color3.getRed(), color3.getGreen(), color3.getBlue());
        tripleGradient->setUniform1f("radius", radius);
        tripleGradient->setUniform2f("size",  x + width, y + height);
        tripleGradient->setUniform1f("alpha", alpha);
        tripleGradient->Render();
        tripleGradient->StopRender();
    }
    
    void drawText(float x, float y, Font font, std::wstring text, float size, Color color, float alpha) {
       // fontrender->renderText(text, fontrender->getFont(font), x, y, 1.0, color, alpha, size, screenWidth, screenHeight, ft);
        newfontrender->renderText(font, text, x, y, size, 1.0, color, screenWidth, screenHeight);
    }
    void RectGlow(float x, float y, float width, float height, Color color, Color color2, Color color3, Color color4, float fadeStart, float fadeEnd, float shadowSoftness, float radius) {
        vertex = {
      x, y, 0.0f, 0.0f, 0.0f,
      x + width, y, 0.0f, 1.0f, 0.0f,
      x + width,  y + height, 0.0f, 1.0f, 1.0f,
      x, y + height, 0.0f, 0.0f, 1.0f
        };
        
        glowRect->UpdateVectors(vertex, indices);
        glowRect->PreRender();
        glowRect->useShader(screenWidth, screenHeight);

        glowRect->setUniform3f("color", color.getRed(), color.getGreen(), color.getBlue());
        glowRect->setUniform3f("color2", color2.getRed(), color2.getGreen(), color2.getBlue());
        glowRect->setUniform3f("color3", color3.getRed(), color3.getGreen(), color3.getBlue());
        glowRect->setUniform3f("color4", color4.getRed(), color4.getGreen(), color4.getBlue());

        glowRect->setUniform3f("outlineColor", 1.0, 1.0, 1.0);
        glowRect->setUniform3f("outlineColor2", 1.0, 1.0, 1.0);
        glowRect->setUniform3f("outlineColor3", 1.0, 1.0, 1.0);
        glowRect->setUniform3f("outlineColor4", 1.0, 1.0, 1.0);

        glowRect->setUniform1f("fadeStart", fadeStart);
        glowRect->setUniform1f("fadeEnd", fadeEnd);
        glowRect->setUniform1f("shadowSoftness", shadowSoftness);
        glowRect->setUniform1f("radius", radius);

        glowRect->Render();
        glowRect->StopRender();
    }
    void Rect(float x, float y, float width, float height, Color color, float radius, float alpha) {
       vertex = {
      x, y, 0.0f, 0.0f, 0.0f,
      x + width, y, 0.0f, 1.0f, 0.0f,
      x + width,  y + height, 0.0f, 1.0f, 1.0f,
      x, y + height, 0.0f, 0.0f, 1.0f
        };
       rect->UpdateVectors(vertex, indices);
       rect->PreRender();
       rect->useShader(screenWidth, screenHeight);
       rect->setUniform3f("color", color.getRed(), color.getGreen(), color.getBlue());
       rect->setUniform1f("radius", radius);
       rect->setUniform2f("size", x + width, y + height);
       rect->setUniform1f("alpha", alpha);
       rect->Render();
       rect->StopRender();
    }
    void drawTextureColor(std::string str, float x, float y, float width, float height, Color color) {
        RenderTextureHelper textureHelper = RenderTextureHelper::getInstance();
        TextureAtlas& atla = textureHelper.atlas;
        if (atla.textures.find(str) == atla.textures.end()) {
            atla.addTexture(str);
        }
        vertex = {
           x, y, 0.0f, 0.0f, 0.0f,
          x + float(width), y, 0.0f, 1.0f, 0.0f,
           x + float(width),y + float(height), 0.0f, 1.0f, 1.0f,
           x, y + float(height), 0.0f, 0.0f, 1.0f
        };
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        TextureInfo& value = atla.textures.find(str)->second;
        imageColor->UpdateVectors(vertex, indices);
        imageColor->PreRender();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atla.getTextureID());
        imageColor->useShader(screenWidth, screenHeight);
        imageColor->setUniform1i("atlasSampler", GL_TEXTURE0);
        imageColor->setUniform2f("scale", value.width / atla.maxsize, value.height / atla.maxsize);
        imageColor->setUniform2f("textureSize", value.width, value.height);
        imageColor->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z, value.pos.w);
        imageColor->setUniform4f("color", color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

        imageColor->Render();
        imageColor->StopRender();
    }
    void drawGlowTextureColor(std::string str, float x, float y, float width, float height, float alpha, float blurRadius,  float total, Color color) {
        RenderTextureHelper textureHelper = RenderTextureHelper::getInstance();
        TextureAtlas& atlas = textureHelper.atlas;
        FrameBuffer* buffer = new FrameBuffer(screenWidth, screenHeight);
        buffer->StartRead();
        drawTextureColor(str, x, y, width, height, color);
        buffer->StopRead();

        vertex = {
x, y, 0.0f, 0.0f, 0.0f,
x + width, y, 0.0f, 1.0f, 0.0f,
x + width,  y + height, 0.0f, 1.0f, 1.0f,
x, y + height, 0.0f, 0.0f, 1.0f
        };


        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
       
        ImageGlow->PreRender();
        ImageGlow->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE0);
        ImageGlow->setUniform1f("alpha", alpha);
        ImageGlow->setUniform1f("blurRadius", blurRadius);
        ImageGlow->setUniform1f("alpha", alpha);
        ImageGlow->setUniform1f("total", total);
        ImageGlow->setUniform1i("sampler", GL_TEXTURE0);
        ImageGlow->Render();
        ImageGlow->StopRender();

        if (atlas.textures.find(str) == atlas.textures.end()) {
            atlas.addTexture(str);
        }
        TextureInfo& value = atlas.textures.find(str)->second;
        Bloom->UpdateVectors(vertex, indices);
        Bloom->PreRender();
        Bloom->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas.getTextureID());
        Bloom->renderTexture();
        Bloom->setUniform2f("scale", value.width / atlas.maxsize, value.height / atlas.maxsize);
        Bloom->setUniform2f("textureSize", value.width, value.height);
        Bloom->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z, value.pos.w);
        Bloom->setUniform1i("bloomBlur", GL_TEXTURE1);
        Bloom->setUniform1i("scene", GL_TEXTURE0);
        Bloom->Render();
        Bloom->deleteTexture();
        Bloom->StopRender();
        delete buffer;


    }
    void drawTextureAttenuation(std::string str, float x, float y, float width, float height, float blurAmount, float alpha, int radius) {
        RenderTextureHelper textureHelper = RenderTextureHelper::getInstance();
        TextureAtlas& atlas = textureHelper.atlas;
        if (atlas.textures.find(str) == atlas.textures.end()) {
            atlas.addTexture(str);
        }
        TextureInfo& value = atlas.textures.find(str)->second;
        FrameBuffer* buffer = new FrameBuffer(screenWidth, screenHeight);
        std::vector<float> vertex2 = {
   x, y, 0.0f, 0.0f, 0.0f,
   x + width, y, 0.0f, 1.0f, 0.0f,
   x + width, y + height, 0.0f, 1.0f, 1.0f,
   x, y + height, 0.0f, 0.0f, 1.0f
        };
        buffer->StartRead();

        ImageBrightness->UpdateVectors(vertex2, indices);
        ImageBrightness->PreRender();
        ImageBrightness->useShader(screenWidth, screenHeight);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas.getTextureID());

        ImageBrightness->setUniform1i("sampler", GL_TEXTURE0);
        ImageBrightness->setUniform2f("scale", value.width / atlas.maxsize, value.height / atlas.maxsize);
        ImageBrightness->setUniform2f("textureSize", value.width, value.height);
        ImageBrightness->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z, value.pos.w);

        ImageBrightness->Render();
        ImageBrightness->StopRender();

        buffer->StopRead();

        vertex = {
           0, 0, 0.0f, 0.0f, 0.0f,
           0 + screenWidth, 0, 0.0f, 1.0f, 0.0f,
           0 + screenWidth, 0 + screenHeight, 0.0f, 1.0f, 1.0f,
           0, 0 + screenHeight, 0.0f, 0.0f, 1.0f
        };

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        Hyperbalic->UpdateVectors(vertex, indices);
        Hyperbalic->PreRender();
        Hyperbalic->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE0);
        Hyperbalic->setUniform1i("sampler", GL_TEXTURE0);
        Hyperbalic->setUniform1f("blurAmount", blurAmount);
        Hyperbalic->setUniform1f("alpha", alpha);
        Hyperbalic->setUniform1i("radius", radius);
        Hyperbalic->Render();
        Hyperbalic->StopRender();

      
        Bloom->UpdateVectors(vertex2, indices);
        Bloom->PreRender();
        Bloom->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas.getTextureID());
        Bloom->setUniform2f("scale", value.width / atlas.maxsize, value.height / atlas.maxsize);
        Bloom->setUniform2f("textureSize", value.width, value.height);
        Bloom->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z, value.pos.w);
        Bloom->setUniform1i("bloomBlur", GL_TEXTURE1);
        Bloom->setUniform1i("scene", GL_TEXTURE0);
        Bloom->Render();
        Bloom->StopRender();
        vertex2.resize(0);
        vertex2.shrink_to_fit();
        delete buffer;
    }
    void drawGlowTexture(std::string str, float x, float y, float width, float height, float alpha, float blurRadius, float total, float Wtotal) {
        RenderTextureHelper textureHelper = RenderTextureHelper::getInstance();
        TextureAtlas& atlas = textureHelper.atlas;
        if (atlas.textures.find(str) == atlas.textures.end()) {
            atlas.addTexture(str);
        }
        TextureInfo& value = atlas.textures.find(str)->second;
        FrameBuffer* buffer = new FrameBuffer(screenWidth , screenHeight);
        buffer->StartRead();
        drawTexture(str, x, y, width, height);  
        buffer->StopRead();
        vertex = {
           0, 0, 0.0f, 0.0f, 0.0f,
           0 + screenWidth, 0, 0.0f, 1.0f, 0.0f,
           0 + screenWidth, 0 + screenHeight, 0.0f, 1.0f, 1.0f,
           0, 0 + screenHeight, 0.0f, 0.0f, 1.0f
        };
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            
        ImageGlow->UpdateVectors(vertex, indices);
        ImageGlow->PreRender();
        ImageGlow->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE0);
        ImageGlow->setUniform1f("blurRadius", blurRadius);
        ImageGlow->setUniform1f("alpha", alpha);
        ImageGlow->setUniform1f("total", total);
        ImageGlow->setUniform1f("Wtotal", Wtotal);
        ImageGlow->setUniform1i("sampler", GL_TEXTURE0);
        ImageGlow->Render();
        ImageGlow->StopRender();

        std::vector<float> vertex2 = {
    x, y, 0.0f, 0.0f, 0.0f,
    x + width, y, 0.0f, 1.0f, 0.0f,
    x + width, y + height, 0.0f, 1.0f, 1.0f,
    x, y + height, 0.0f, 0.0f, 1.0f
        };
        Bloom->UpdateVectors(vertex2, indices);
        Bloom->PreRender();
        Bloom->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas.getTextureID());
        Bloom->setUniform2f("scale", value.width / atlas.maxsize, value.height / atlas.maxsize);
        Bloom->setUniform2f("textureSize", value.width, value.height);
        Bloom->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z, value.pos.w);
        Bloom->setUniform1i("bloomBlur", GL_TEXTURE1);
        Bloom->setUniform1i("scene", GL_TEXTURE0);
        Bloom->Render();
        Bloom->StopRender();
        vertex2.resize(0);
        vertex2.shrink_to_fit();
        delete buffer;
    }
    void drawShadowTexture(std::string str, float x, float y, float width, float height, float alpha, float blurRadius, float total, Color shadow) {
        FrameBuffer* buffer = new FrameBuffer(screenWidth, screenHeight);
        buffer->StartRead();
        drawTexture(str, x, y, width, height);
        buffer->StopRead();
        vertex = {
x, y, 0.0f, 0.0f, 0.0f,
x + width, y, 0.0f, 1.0f, 0.0f,
x + width,  y + height, 0.0f, 1.0f, 1.0f,
x, y + height, 0.0f, 0.0f, 1.0f
        };
 

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        ImageShadow->PreRender();
        ImageShadow->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE0);
        ImageShadow->setUniform3f("color", shadow.getRed(), shadow.getGreen(), shadow.getBlue());
        ImageShadow->setUniform1f("alpha", alpha);
        ImageShadow->setUniform1f("blurRadius", blurRadius);
        ImageShadow->setUniform1f("alpha", alpha);
        ImageShadow->setUniform1f("total", total);
        ImageShadow->setUniform1i("sampler", GL_TEXTURE0);
        ImageShadow->Render();
        ImageShadow->StopRender();
        RenderTextureHelper textureHelper = RenderTextureHelper::getInstance();
        TextureAtlas& atlas = textureHelper.atlas;
        if (atlas.textures.find(str) == atlas.textures.end()) {
            atlas.addTexture(str);
        }
        TextureInfo& value = atlas.textures.find(str)->second;
        Bloom->PreRender();
        Bloom->useShader(screenWidth, screenHeight);
        buffer->renderTexture(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas.getTextureID());
        Bloom->renderTexture();
        Bloom->setUniform2f("scale", value.width / atlas.maxsize, value.height / atlas.maxsize);
        Bloom->setUniform2f("textureSize", value.width, value.height);
        Bloom->setUniform4f("textureInfo", value.pos.x, value.pos.y, value.pos.z, value.pos.w);
        Bloom->setUniform1i("bloomBlur", GL_TEXTURE1);
        Bloom->setUniform1i("scene", GL_TEXTURE0);
        Bloom->Render();
        Bloom->deleteTexture();
        Bloom->StopRender();
        delete buffer;
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

