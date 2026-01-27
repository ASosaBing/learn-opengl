#include <alloca.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#if defined(_WIN32) | defined(_WIN64)
  #define ASSERT(x) if(!(x)) __debug_break(); 
#endif
#if(defined(__APPLE__) || defined(__MACH__) || defined(__linux__))
  #define ASSERT(x) if(!(x)) __builtin_trap();
#endif


#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__));






static void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line){
  while(GLenum error = glGetError()){
    std::cout << "[OpenGL Error] (" << std::hex << error << std::dec << ") "
              << function << " " << file << ": " << line << std::endl;
    return false;
  }
  return true;
}




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


    float vertices[] {
  //triangle 1 
      -0.5f, -0.5f,
       0.5f, -0.5f,
  //triangle 2
       0.5f,  0.5f,
      -0.5f,  0.5f,
    };


    int indices[]{
      0, 1, 2,
      2, 3, 0
    };

    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));


//puts things on the GPU, but doesnt tell the gpu how to do anything
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW)); 
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    GLCall(glEnableVertexAttribArray(0));

    unsigned int IBO;
    GLCall(glGenBuffers(1, &IBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

//after this we write some shaders
 
    ShaderProgramSource source = ParseShader("./res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));




    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT)); 

    
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    GLCall(glDeleteProgram(shader));
    glfwTerminate();
    return 0;
}
