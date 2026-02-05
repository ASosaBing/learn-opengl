#include <project/test.hpp>
#include <imgui/imgui.h>
namespace test
{
  TestMenu::TestMenu(Test*& current) : m_CurrentTest(current){
    
  }
  TestMenu::~TestMenu(){}
  
  void TestMenu::OnImGuiRender(){
    for(auto& test : m_Tests){
      if(ImGui::Button(test.first.c_str()))
        m_CurrentTest = test.second();
    }
  }

  void TestMenu::OnRender(){}
  void TestMenu::OnUpdate(float deltaTime){}


}
