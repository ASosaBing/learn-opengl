#ifndef VRTX_ARRAY
#define VRTX_ARRAY
#include <project/vertexbufferlayout.hpp>
#include <project/vertexbuffer.hpp>
#include <project/renderer.hpp>
class VertexArray
{
private:
  unsigned int m_RendererID;



public:
  VertexArray();
  ~VertexArray();

  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);


  void Bind() const;
  void UnBind() const;
};





#endif
