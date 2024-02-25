//
// Created by jasperyang on 24-2-21.
//

#ifndef ABSTRACT_SHADER_H
#define ABSTRACT_SHADER_H

#include <string>
#include <GL/glew.h>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(std::string vertexShaderSource, std::string  fragmentShaderSource);

    ~Shader();

    void bind() const;

    void unbind() const;

    // set uniforms
    void setUniform4f(const std::string &name, const glm::vec4 value);

    void setUniformMat4f(const std::string &name, const glm::mat4 &matrix);

    void setUniform1i(const std::string &name, GLuint value);

private:
    GLuint m_RenderID;

    std::string m_VertexShaderPath, m_FragmentShaderPath;

    std::unordered_map<std::string, GLuint> m_UniformLocationCaches;

    GLuint getUniformLocation(const std::string &name);

    std::string parseShader(const std::string &filepath);

    GLuint compileShader(GLenum type, const std::string& source);

    GLuint createShader(const std::string &vertexShader, const std::string &fragmentShader);
};


#endif //ABSTRACT_SHADER_H
