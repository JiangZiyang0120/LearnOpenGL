//
// Created by jasperyang on 24-2-21.
//

#include <Core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

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

    // set swap interval
    glfwSwapInterval(3);

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

    GLuint indices[] = {
            0, 1, 2, //first triangle
            2, 3, 0, //second triangle
    };

    /*
     * A VAO contains a buffer bind to GL_ARRAY_BUFFER and its layout (which we indicate in glVertexAttribPointer)
     */
    GLuint VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    VertexBuffer VBO(quadrangle, sizeof(quadrangle));

    // bind VBO to VAO
    // index 0 of the first parameter link to ABO, and bind it with VAO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    IndexBuffer IBO(indices, 6);

    GLuint shader = createShader(getShader("VertexShader.glsl"),
                                 getShader("uniformFS.glsl"));
    GLCall(glUseProgram(shader));

    // index for vec4 u_Color in "uniformFS.glsl"
    GLCall(GLint location = glGetUniformLocation(shader, "u_Color"));
    // check if u_Color is found
    ASSERT(location == -1);
    // set the data of u_Color
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    // unbind
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    // generate a carton
    GLfloat r = 0.5f;
    GLfloat increment = 0.03f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // sent color
        // uniform is per draw
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        GLCall(glBindVertexArray(VAO));
        IBO.bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        // change the color
        if (r > 1.0f)
            increment = -0.03f;
        else if (r < 0.0f)
            increment = 0.03f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    return 0;
}