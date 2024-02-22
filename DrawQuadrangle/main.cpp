//
// Created by jasperyang on 24-2-19.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>

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

#define SECOND_WAY_TO_DRAW_QUADRANGLE

#ifdef FIRST_WAY_TO_DRAW_QUADRANGLE
    /*
     * This is the first way to draw a quadrangle: draw two triangles.
     * However, this method is not convenient and elegant, because we need to define two
     * triangles where there are two similar points of the triangles.
     * Thus, we need to reduce the amount of data stored on the GPU and the difficulty of writing
     */
    GLfloat triangle[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,

            0.5f, 0.5f,
            -0.5f, 0.5f,
            -0.5f, -0.5f,
    };
#elif defined(SECOND_WAY_TO_DRAW_QUADRANGLE)
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
#endif

    // define vertex buffer
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

#ifdef FIRST_WAY_TO_DRAW_QUADRANGLE
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
#elif defined(SECOND_WAY_TO_DRAW_QUADRANGLE)
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadrangle), quadrangle, GL_STATIC_DRAW);
    GLuint IBO; // index buffer object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#endif


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


#ifdef FIRST_WAY_TO_DRAW_QUADRANGLE
        glDrawArrays(GL_TRIANGLES, 0, 6);
#elif defined(SECOND_WAY_TO_DRAW_QUADRANGLE)
        // 6 of the second parameter is the size of indices
        // ---
        // GL_UNSIGNED_INT of the third parameter, notice that, every index of GPU is an unsigned integer.
        // You will miss some mistakes when using GL_INT
        // ---
        // nullptr of the forth parameter is because we have bind indices with GL_ELEMENT_ARRAY_BUFFER
        // thus we only need to use it in the shader
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
#endif

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    return 0;
}