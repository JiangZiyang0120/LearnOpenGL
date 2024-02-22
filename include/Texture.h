//
// Created by jasperyang on 24-2-22.
//

#ifndef ABSTRACT_TEXTURE_H
#define ABSTRACT_TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture(const std::string &path);

    ~Texture();

    // sometimes we need to bind more than one texture at once.
    // for instance, you might need slot 32 for PC and slot 8 for Iphone
    void bind(GLuint slot = 0) const;

    void unbind() const;

    inline int getWidth() const { return m_Width; }

    inline int getHeight() const { return m_Height; }

private:
    GLuint m_RenderID;
    std::string m_FilePath;
    // local storage for the texture
    GLubyte *m_LocalBuffer;
    // m_BPP for the bits of per pixel
    int m_Width, m_Height, m_BPP;
};


#endif //ABSTRACT_TEXTURE_H
