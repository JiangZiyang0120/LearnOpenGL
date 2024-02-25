//
// Created by jasperyang on 24-2-21.
//

#include <Core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


int main(void) {
    GLFWwindow *window;

    /* Initialize the library */
    /* you can't use functions of glew until you successfully call glfwInit() */
    if (!glfwInit())
        return -1;

    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // set swap interval
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Important! all functions in glew should be called after glewInit()
    // And glewInit should be called after glfwMakeContextCurrent()
    glewInit();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLfloat quadrangle[] = {
            // x, y, texture-left, texture-right
            100.0f, 100.0f, 0.0f, 0.0f,   //0
            200.0, 100.0f, 1.0f, 0.0f,   //1
            200.0, 200.0, 1.0f, 1.0f,    //2
            100.0f, 200.0, 0.0f, 1.0f,   //3
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

    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

    glm::mat4 mvp = projection * view * model;

    Shader shader("../../shader/vertexShaderMath.glsl",
                  "../../shader/fragmentTextureShader.glsl");
    shader.bind();

    Texture texture("../../texture/00182280G31.jpg");
    texture.bind(0);
    // value 0 of the 2nd parameter is the value of slot parameter of texture.bind()
    shader.setUniform1i("u_Texture", 0);
    shader.setUniformMat4f("u_MVP", mvp);

    // unbind
    VAO.unbind();
    VBO.unbind();
    IBO.unbind();
    shader.unbind();

    Renderer renderer;

    ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::ColorEdit4("clear color", (float *) &clearColor);
        ImGui::End();

        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w,
                     clearColor.w);

        renderer.clear();

        // sent color
        // uniform is per draw
        shader.bind();

        renderer.draw(VAO, IBO, shader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}