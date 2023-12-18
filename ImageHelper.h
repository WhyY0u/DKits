#pragma once
#include <stb_image.h>
#include <string>
#include <unordered_map>
#include "Math.h"
struct ImageInfo {
    int width;
    int height;
    int channels;
    std::shared_ptr<unsigned char> data;
};
struct TextureInfo {
    vec4 pos;
    unsigned char* texture;
    int width, height;
};
class ImageHelper {
public:
    ImageHelper() {};
    static ImageHelper& getInstance() {
        static ImageHelper instance;
        return instance;
    }
    std::unordered_map<std::string, ImageInfo> imageCache;
    unsigned char* loadImage(const std::string& filepath, int& width, int& height, int& channels) {
        auto cachedImage = imageCache.find(filepath);
        if (cachedImage != imageCache.end()) {
            width = cachedImage->second.width;
            height = cachedImage->second.height;
            channels = cachedImage->second.channels;
            return cachedImage->second.data.get();
        }
        unsigned char* imageData = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        if (imageData) {
            ImageInfo info;
            info.width = width;
            info.height = height;
            info.channels = channels;
            info.data = std::shared_ptr<unsigned char>(imageData, stbi_image_free);
            imageCache[filepath] = info; 
        }

        return imageData; 
    }
    private:
     ImageHelper(const ImageHelper&) = delete;
     void operator=(const ImageHelper&) = delete;
};
class TextAtlas {
public:
    TextAtlas() : maxsize(2048), Ax(0), Ay(0), Aheight(0) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, maxsize, maxsize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
  }
    int Ax, Ay, Aheight;
    float maxsize;

private:

GLuint textureID;
};
class TextureAtlas {
public:
    TextureAtlas() : maxsize(2048), Ax(0), Ay(0), Aheight(0) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, maxsize, maxsize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    std::map<std::string, TextureInfo> textures;
    int Ax, Ay, Aheight;
    float maxsize;
    void addTexture(const std::string& filepath) {
        auto it = textures.find(filepath);
        if (it != textures.end()) {
            return;
        }
        int channels;
        int imageWidth, imageHeight;
        unsigned char* im = ImageHelper::getInstance().loadImage(filepath, imageWidth, imageHeight, channels);
        glBindTexture(GL_TEXTURE_2D, textureID);
        if (Ax + imageWidth > maxsize) {
            Ax = 0;
            Ay += Aheight;
            Aheight = 0;
        }
        if (Ay + imageHeight > maxsize) {
            if (getCommonAtlas().size() == 0) {
                TextureAtlas at = TextureAtlas();
                at.addTexture(filepath);
                getCommonAtlas().push_back(at);
            } else {
                TextureAtlas at = getCommonAtlas().back();
                if (at.Ay + imageHeight > maxsize) {
                    TextureAtlas at1 = TextureAtlas();
                    at1.addTexture(filepath);
                    getCommonAtlas().push_back(at1);
                    return;
                }
                at.addTexture(filepath);
               
            }

           
            return;
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, Ax, Ay, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, im);
        Ax += imageWidth;
        Aheight = std::max(Aheight, imageHeight);
        TextureInfo info;
        info.pos = vec4(Ax - imageWidth, Ay, Ax, Ay + imageHeight);
        info.texture = im;
        info.width = imageWidth;
        info.height = imageHeight;

        textures.insert(std::make_pair(filepath, info));
        
    }
    void deleteTexutre(std::string texture) {
        auto it = textures.find(texture.c_str());
        if (it != textures.end()) {
            textures.erase(it);
            delete[] it->second.texture;
            auto it2 = ImageHelper::getInstance().imageCache.find(texture);
            ImageHelper::getInstance().imageCache.erase(it2);
            it2->second.data = nullptr;
            return;
        } else {
          return;
        }
    }
    GLuint getTextureID() const {
        return textureID;
    }

private:
    GLuint textureID;
    static std::vector<TextureAtlas>& getCommonAtlas() {
        static std::vector<TextureAtlas> commonAtlas;
        return commonAtlas;
    }
};
class RenderTextureHelper {
public:
RenderTextureHelper() {};
RenderTextureHelper(TextureAtlas atlas) : atlas(atlas){};
static RenderTextureHelper getInstance() {
 static RenderTextureHelper instance;
 return instance;
    }
TextureAtlas atlas;
};
