#include <project/indexbuffer.hpp>
#include <project/renderer.hpp>

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) {

  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                      data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  GLCall(glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_RendererID));
}

void IndexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
