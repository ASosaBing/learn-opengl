#ifndef SHADR_HPP
#define SHADR_HPP
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
std::string VertexSource;
std::string FragmentSource;
};


class Shader {
public:
  Shader(const std::string& filepath);
  ~Shader();

  void Bind() const;
  void UnBind() const;
  void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);

private:
  unsigned int m_RendererID;
  std::string m_FilePath;
  std::unordered_map<std::string, unsigned int> m_UniformLocationCache;

  ShaderProgramSource ParseShader(const std::string& filepath);
  unsigned int GetUniformLocation(const std::string& name);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};


#endif

