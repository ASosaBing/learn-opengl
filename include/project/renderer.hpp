#ifndef RENDR_HPP
#define RENDR_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if defined(_WIN32) | defined(_WIN64)
#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    __debug_break();
#endif
#if (defined(__APPLE__) || defined(__MACH__) || defined(__linux__))
#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    __builtin_trap();
#endif

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

#endif
