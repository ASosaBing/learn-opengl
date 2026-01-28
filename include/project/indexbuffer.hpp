#ifndef IDXBUFF_HPP
#define IXDBUFF_HPP
class IndexBuffer {
public:
  IndexBuffer(const unsigned int *data, unsigned int count);
  ~IndexBuffer();

  void Bind() const;
  void UnBind() const;

  inline unsigned int Count() const { return m_Count; }

private:
  unsigned int m_RendererID;
  unsigned int m_Count;
};

#endif
