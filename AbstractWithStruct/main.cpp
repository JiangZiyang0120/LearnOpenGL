//
// Created by jasperyang on 24-2-21.
//

#include <Core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


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


    VertexArray VAO;

    VertexBuffer VBO(quadrangle, sizeof(quadrangle));

    IndexBuffer IBO(indices, 6);

    VertexBufferLayout VBLayout;
    VBLayout.push<GLfloat>(2);
    VAO.setLayout(VBO, VBLayout);

    Shader shader("../../Shader/VertexShader.glsl",
                  "../../Shader/uniformFS.glsl");
    shader.bind();

    shader.setUniform4f("u_Color", Eigen::Vector4f(0.2f, 0.3f, 0.8f, 1.0f));

    // unbind
    VAO.unbind();
    VBO.unbind();
    IBO.unbind();
    shader.unbind();

    Renderer renderer;

    // generate a carton
    GLfloat r = 0.5f;
    GLfloat increment = 0.03f;
    while (!glfwWindowShouldClose(window)) {
        renderer.clear();

        // sent color
        // uniform is per draw
        shader.bind();
        shader.setUniform4f("u_Color", Eigen::Vector4f(r, 0.3f, 0.8f, 1.0f));

        renderer.draw(VAO, IBO, shader);

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

    return 0;
}