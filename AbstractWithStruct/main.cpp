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
            // x, y, texture-left, texture-right
            -0.5f, -0.5f, 0.0f, 0.0f,   //0
            0.5f, -0.5f, 1.0f, 0.0f,   //1
            0.5f, 0.5f, 1.0f, 1.0f,    //2
            -0.5f, 0.5f, 0.0f, 1.0f,   //3
    };

    GLuint indices[] = {
            0, 1, 2, //first triangle
            2, 3, 0, //second triangle
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


    VertexArray VAO;

    VertexBuffer VBO(quadrangle, sizeof(quadrangle));

    IndexBuffer IBO(indices, 6);

    VertexBufferLayout VBLayout;
    // push (x,y)
    VBLayout.push<GLfloat>(2);
    // push texture coordinate
    VBLayout.push<GLfloat>(2);
    VAO.setLayout(VBO, VBLayout);

    Shader shader("../../shader/vertexTextureShader.glsl",
                  "../../shader/fragmentTextureShader.glsl");
    shader.bind();

    Texture texture("../../texture/Morpho_didius_Male_Dos_MHNT.jpg");
    texture.bind(0);
    // value 0 of the 2nd parameter is the value of slot parameter of texture.bind()
    shader.setUniform1i("u_Texture", 0);

    // unbind
    VAO.unbind();
    VBO.unbind();
    IBO.unbind();
    shader.unbind();

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        renderer.clear();

        // sent color
        // uniform is per draw
        shader.bind();

        renderer.draw(VAO, IBO, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    return 0;
}