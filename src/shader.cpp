#include <project/shader.hpp>
#include <project/renderer.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>





Shader::Shader(const std::string& filepath) : m_FilePath{filepath}{ 
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);


}

Shader::~Shader(){
  GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const{
  GLCall(glUseProgram(m_RendererID));
}
void Shader::UnBind() const{
  GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3){
  GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}
unsigned int Shader::GetUniformLocation(const std::string& name){
  if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];



  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if(location == -1)
    std::cout << "Warning: location " << name << " doesnt exist!" << std::endl;
  m_UniformLocationCache[name] = location;
  return location;
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source){
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


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  GLCall(unsigned int program = glCreateProgram());
  GLCall(unsigned int vs = (this)->CompileShader(GL_VERTEX_SHADER, vertexShader));
  GLCall(unsigned int fs = (this)->CompileShader(GL_FRAGMENT_SHADER, fragmentShader));
  

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  
  return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath){
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
