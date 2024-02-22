//
// Created by jasperyang on 24-2-19.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>

// two approaches to get error messages
#define USE_GL_GET_ERROR
//#define USE_GL_DEBUG_MESSAGE_CALLBACK

// USE_MACRO_FUNCTION offers many approaches to brief the code
#define USE_MACRO_FUNCTION

// get the path relative to the working path
static std::string getPath(const std::string &str) {
    static const std::string currentFilepath = "../../";
    return currentFilepath + str;
}

// read the shader file
static std::string getShader(const std::string &&filePath) {
    std::ifstream file(getPath(filePath));
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
                            (std::istreambuf_iterator<char>()));
        return content;
    } else {
        return "Error Occurred";
    }
}

#ifdef USE_GL_GET_ERROR

#ifdef USE_MACRO_FUNCTION
#define GLCheckError(x) GLCheckErrorWithLine(__FILE__, #x, __LINE__);

#define GLCall(x) GLClearError(); \
                  x;              \
                  GLCheckError(x)

static void GLCheckErrorWithLine(const char *file, const char *function, int line) {
    while (GLenum error = glGetError()) {
        std::cerr << "File " << file << "\nLine " << line << " " << function << "\nOpenGL Error: (" << error << ")" << std::endl;
    }
}
#else
static void GLCheckError() {
    while (GLenum error = glGetError()) {
        std::cerr << "OpenGL Error: (" << error << ")" << std::endl;
    }
}
#endif

static void GLClearError() {
    // keep calling glGetError() until there's no error code in OpenGL,
    // and we just throw all of them to clear error messages
    while (glGetError() != GL_NO_ERROR);
}

#endif

GLuint compileShader(GLenum type, const std::string &source) {
    GLuint id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

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

// compile and test shader
static GLuint createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader),
            fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Usually we don't need to do that
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void) {
    GLFWwindow *window;

    /* Initialize the library */
    /* you can't use functions of glew until you successfully call glfwInit() */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Important! all functions in glew should be called after glewInit()
    // And glewInit should be called after glfwMakeContextCurrent()
    glewInit();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLfloat quadrangle[] = {
            -0.5f, -0.5f,   //0
            0.5f, -0.5f,    //1
            0.5f, 0.5f,     //2
            -0.5f, 0.5f,    //3
    };

    // generate the indices
    GLuint indices[] = {
            0, 1, 2, //first triangle
            2, 3, 0, //second triangle
    };

    // define vertex buffer
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadrangle), quadrangle, GL_STATIC_DRAW);
    GLuint IBO; // index buffer object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    // We enable the data set defined above whose index is 0.
    // Notice, glEnableVertexAttribArray can be called before glVertexAttribPointer
    glEnableVertexAttribArray(0);

    GLuint shader = createShader(getShader("shader/VertexShader.glsl"),
                                 getShader("shader/fragmentShader.glsl"));
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

#ifdef USE_GL_GET_ERROR
#ifndef USE_MACRO_FUNCTION
        GLClearError();
        // We set GL_INT for the third parameter, which is a wrong type
        glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
        // The function print error code 1280 in Terminate
        // by checking GL/glew.h, we have
        // #define GL_INVALID_ENUM 0x0500
        // That means the error type is invalid enum
        // ----------------------------------------------
        // However, this check approach is somehow cumbersome, and we
        // actually don't know which line the error occurred(however,
        // we can modify our GLCheckError function with macro like below).
        // In any case, this approach adds a lot of extra error-handling parts
        // to the program, making it more difficult to read.
        GLCheckError();
#else
        // its output is:
        // ---
        // File ***/main.cpp
        // Line 192 glDrawElements(0x0004, 6, 0x1404, nullptr)
        // OpenGL Error: (1280)
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
#endif

#elif defined(USE_GL_DEBUG_MESSAGE_CALLBACK)

#endif

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    return 0;
}