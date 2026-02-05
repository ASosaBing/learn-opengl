#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <project/test.hpp>
#include <project/indexbuffer.hpp>
#include <project/renderer.hpp>
#include <project/shader.hpp>
#include <project/texture.hpp>
#include <project/vertexarray.hpp>
#include <project/vertexbuffer.hpp>
#include <project/vertexbufferlayout.hpp>
#include <project/testclearcolor.hpp>
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
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    test::TestClearColor test;
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");



    Renderer renderer;
    

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      renderer.Clear();
      
      test.OnUpdate(0.0);
      test.OnRender();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();



      ImGui::NewFrame();
      if(currentTest){
        currentTest->OnUpdate(0.0f);
        currentTest->OnRender();
        ImGui::Begin("Test");
        if(currentTest != testMenu && ImGui::Button("<-")){
          delete currentTest;
          currentTest = testMenu;
        }


        currentTest->OnImGuiRender();
        ImGui::End();



      }


      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    delete currentTest;
    if(currentTest != testMenu){
      delete testMenu;
    }

  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
