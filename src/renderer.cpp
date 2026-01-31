#include <project/renderer.hpp>
#include <iostream>

void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line){
  while(GLenum error = glGetError()){
    std::cout << "[OpenGL Error] (" << std::hex << error << std::dec << ") "
              << function << " " << file << ": " << line << std::endl;
    return false;
  }
  return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{ 
    shader.Bind();
    va.Bind(); 
    ib.Bind();


    GLCall(glDrawElements(GL_TRIANGLES, ib.Count(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT)); 
}
    
