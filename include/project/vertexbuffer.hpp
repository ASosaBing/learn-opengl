#ifndef VERTEXBUFF_HPP
#define VERTEXBUFF_HPP
class VertexBuffer {
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;
};
#endif
