#ifndef VRTX_ARRAY
#define VRTX_ARRAY
#include <project/vertexbuffer.hpp>


class VertexBufferLayout;

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
