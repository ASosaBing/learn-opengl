#ifndef TSTCLRCOLR_HPP
#define TSTCLRCOLR_HPP
#include <project/test.hpp>


namespace test{
  class TestClearColor : public Test{
  public:
    TestClearColor();
    ~TestClearColor();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    float m_ClearColor[4];
  };
}




#endif
