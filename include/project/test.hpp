#ifndef TEST_HPP
#define TEST_HPP
#include <functional>
#include <iostream>
namespace test 
{
  class Test {
  public:
    Test() {}
    virtual ~Test() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
  };

  class TestMenu : public Test
  {
  public:
    TestMenu(Test*& current);
    ~TestMenu();
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    template<typename T>
    void RegisterTest(const std::string& str){
      std::cout << "Registering Test: " << str << std::endl;
      m_Tests.push_back(std::make_pair(str, []() {return new T();}));
    }
  private:
    Test*& m_CurrentTest;
    std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
  };
}




#endif


