#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <project/indexbuffer.hpp>
#include <project/renderer.hpp>
#include <project/shader.hpp>
#include <project/texture.hpp>
#include <project/vertexarray.hpp>
#include <project/vertexbuffer.hpp>
#include <project/vertexbufferlayout.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <alloca.h>
#include <cmath>
#include <iostream>

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit()) {
    std::cout << "failed to initalize GLFW";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwSwapInterval(1);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  {
    float vertices[]{// triangle 1
                    -50.0f, -50.0f, 0.0f, 0.0f, 
                     50.0f, -50.0f, 1.0f, 0.0f,
                     // triangle 2
                     50.0f, 50.0f, 1.0f, 1.0f, 
                    -50.0f, 50.0f, 0.0f, 1.0f};
    unsigned int indices[]{0, 1, 2, 2, 3, 0};

    VertexArray va{};
    VertexBuffer vb{vertices, 4 * 4 * sizeof(float)};
    IndexBuffer ib{indices, 6};
    Shader shader{"./res/shaders/basic.shader"};
    VertexBufferLayout layout;
    Renderer renderer;
    Texture texture{"./res/textures/flag.png"};
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    shader.Bind();
    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);
    shader.UnBind();

    layout.Push<float>(2);
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);
    float t = 0.01;

    float f = 0.0f;

    glm::vec3 translationA{0, 0, 0};
    glm::vec3 translationB{0, 0, 0};



    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      t += 0.01;
      /* Render here */

      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
      glm::mat4 mvp = proj * view * model;

      renderer.Clear();
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // shader.SetUniform4f("u_Color",  std::pow(std::sin(t), 2),
      // std::pow(std::cos(t + 0.5), 2), 0.8f, 1.0f);
     

      shader.Bind();
      {
       glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
       glm::mat4 mvp = proj * view * model;
       shader.SetUniformMat4f("u_MVP", mvp);
       renderer.Draw(va, ib, shader);
      }


      {
       glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
       glm::mat4 mvp = proj * view * model;
       shader.SetUniformMat4f("u_MVP", mvp);
       renderer.Draw(va, ib, shader);
      }



      


      ImGui::Begin("Matrix Transforms");
      ImGui::SliderFloat3("translationA", &translationA.x, 0.0f, 960.0f);
      ImGui::SliderFloat3("translationB", &translationB.x, 0.0f, 960.0f);

      ImGui::End();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
