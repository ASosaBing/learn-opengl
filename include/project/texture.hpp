#ifndef TEXTR_HPP
#define TEXTR_HPP
#include <project/renderer.hpp>


class Texture{
public:
  Texture(const std::string& path);
  ~Texture();

  void Bind(unsigned int slot = 0) const;
  void UnBind();

  inline int getWidth() { return m_Width;}
  inline int getHeight() { return m_Height;}
private:
  unsigned int m_RendererID;
  std::string m_FilePath;
  unsigned char* m_LocalBuffer;
  int m_Width, m_Height, m_BPP;

};



#endif
