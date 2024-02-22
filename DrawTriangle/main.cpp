//
// Created by jasperyang on 24-2-19.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>

// get the path relative to work path
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
    /*
     * ## Parameters
     * - shader: Specify the handle of the shader object whose source code is to be replaced.
     * - count: Specify the number of elements in the string and length arrays.
     * - String: Specify an array of pointers to strings containing the source code to be loaded into the shader.
     * - Length: Specify an array of string lengths.
     * ## Description
     * glShaderSource sets the source code in shader to the source code in the array of strings specified by string.
     * Any source code previously stored in the shader object is completely replaced. The number of strings in the
     * array is specified by count. If length is nullptr, each string is assumed to be null terminated. If length is
     * a value other than nullptr, it points to an array containing a string length for each of the corresponding
     * elements of string. Each element in the length array may contain the length of the corresponding string
     * (the null character is not counted as part of the string length) or a value less than 0 to indicate that the
     * string is null terminated. The source code strings are not scanned or parsed at this time; they are simply copied
     * into the specified shader object.
     */
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

    GLfloat triangle[] = {
            -0.5f, -0.5f,
            0.0f, 0.5f,
            0.5f, -0.5f
    };

    // define vertex buffer
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    /*
     * You usually need to alloc the buffer's size
     * There are two ways to give it data:
     * 1. You give it data.
     * 2. You give it nothing and later update it with data.
     * We will use the first method
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    /*
     * Then we need to describe how to decode the stream in the buffer
     * Notice: we need to tell the layout of *each* stat of vertex to OpenGL.
     * Once we call glVertexAttribPointer(), we describe the layout of a stat,
     * and we need to specify the index of the current property.
     * E.g., the vertex has the stats of position, texture and normal; we can
     * put the position into index0, texture into index1 and so on.
     */
    /*
     * ## Parameters
     * - index: Specifies the index of the generic vertex attribute to be modified.
     * - size:  Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally, the
     *          symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
     * - Type:  Specifies the data type of each component in the array. The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE,
     *          GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and
     *          glVertexAttribIPointer. Additionally, GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV,
     *          GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer.
     *          GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only token accepted by the type parameter
     *          for that function. The initial value is GL_FLOAT.
     * - Normalized: For glVertexAttribPointer, specify whether fixed-point data values should be normalized (GL_TRUE)
     *               or converted directly as fixed-point values (GL_FALSE) when they are accessed.
     * - Stride:     Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic
     *               vertex attributes are understood to be tightly packed in the array. The initial value is 0.
     * - Pointer:    Specifies an offset of the first component of the first generic vertex attribute in the array in the
     *               data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
     */
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

        // New OpenGL method to draw a triangle
        // two ways shown in below. We select the first
#define FIRST_WAY_TO_DRAW_A_TRIANGLE
#ifdef FIRST_WAY_TO_DRAW_A_TRIANGLE
        glDrawArrays(GL_TRIANGLES, 0, 3);
#else
        glDrawElements(GL_TRIANGLES, 3, GL_FLOAT, )
#endif

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    return 0;
}