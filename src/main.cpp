#include <project/renderer.hpp>
#include <project/indexbuffer.hpp>
#include <project/vertexbuffer.hpp>
#include <alloca.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


struct ShaderProgramSource {
std::string VertexSource;
std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath){
  std::ifstream stream{filePath};

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while(getline(stream, line)){
    if(line.find("#shader") != std::string::npos){
      if(line.find("vertex") != std::string::npos){
        type = ShaderType::VERTEX;
      } else if(line.find("fragment") != std::string::npos){
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << "\n";
    }

    

  }

  return {ss[0].str(), ss[1].str()};
}



static unsigned int CompileShader(unsigned int type, const std::string& source){
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, NULL));

  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if(result == GL_FALSE){
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));


    char* message = (char*)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex " : "fragment ") << "shader!" << std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  GLCall(unsigned int program = glCreateProgram());
  GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
  GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));
  

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  
  return program;
}


int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()){
        std::cout << "failed to initalize GLFW";
        return -1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSwapInterval(1);


  
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
  
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }
   
    GLCall(glClear(GL_COLOR_BUFFER_BIT)); 

{
    float vertices[] {
  //triangle 1 
      -0.5f, -0.5f,
       0.5f, -0.5f,
  //triangle 2
       0.5f,  0.5f,
      -0.5f,  0.5f,
    };


    unsigned int indices[]{
      0, 1, 2,
      2, 3, 0
    };

    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));


//puts things on the GPU, but doesnt tell the gpu how to do anything
  
    VertexBuffer vb{vertices, 4 * 2 * sizeof(float)};



    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    GLCall(glEnableVertexAttribArray(0));

    IndexBuffer ib {indices, 6};

//after this we write some shaders
 
    ShaderProgramSource source = ParseShader("./res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));


    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
       float t = 0.01;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        t += 0.01;
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT)); 
         
        GLCall(glUniform4f(location, std::pow(std::sin(t), 2), std::pow(std::cos(t + 0.5), 2), 0.8f, 1.0f));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    GLCall(glDeleteProgram(shader));
}
    glfwTerminate();
    return 0;
}
