//
// Created by jasperyang on 24-2-21.
//

#include <Tool.h>
#include <Shader.h>
#include <utility>
#include <iostream>
#include <fstream>

Shader::Shader(std::string vertexShaderSource, std::string fragmentShaderSource) :
        m_VertexShaderPath(std::move(vertexShaderSource)),
        m_FragmentShaderPath(std::move(fragmentShaderSource)),
        m_RenderID(0) {
    m_RenderID = createShader(m_VertexShaderPath, m_FragmentShaderPath);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RenderID));
}

void Shader::bind() const {
    GLCall(glUseProgram(m_RenderID));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string &name, Eigen::Vector4f value) {
    GLCall(GLint location = getUniformLocation(name));
    // check if uniform is found
    ASSERT(location == -1);
    // set the data of u_Color
    GLCall(glUniform4f(location, value[0], value[1], value[2], value[3]));
}

GLuint Shader::getUniformLocation(const std::string &name) {
    auto iter = m_UniformLocationCaches.find(name);
    if (iter != m_UniformLocationCaches.end())
        return iter->second;
    GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
    ASSERT(location == -1);
    m_UniformLocationCaches[name] = location;
    return location;
}

GLuint Shader::compileShader(GLenum type, const std::string &source) {
    GLuint id = glCreateShader(type);
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // check for error
    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << ((type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << " SHADER ERROR: \n" << message
                  << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint Shader::createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, parseShader(vertexShaderPath)),
            fs = compileShader(GL_FRAGMENT_SHADER, parseShader(fragmentShaderPath));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // Usually we don't need to do that
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

std::string Shader::parseShader(const std::string &filepath) {
    std::ifstream file(filepath);
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
                            (std::istreambuf_iterator<char>()));
        return content;
    } else {
        return "Error Occurred";
    }
}
