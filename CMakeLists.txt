cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)




include(FetchContent)
project(opengl)



#finding opengl
find_package(OpenGL REQUIRED)

#glfw library
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3.8
  GIT_SHALLOW true
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)


#finding OpenGL package

add_executable(opengl src/shader.cpp src/main.cpp src/glad.c src/renderer.cpp 
                      src/vertexbuffer.cpp src/indexbuffer.cpp 
                      src/vertexarray.cpp src/stb_image.cpp src/texture.cpp
                      ./include/imgui/imgui.cpp  ./include/imgui/imgui_demo.cpp  ./include/imgui/imgui_draw.cpp
                      ./include/imgui/imgui_impl_glfw.cpp ./include/imgui/imgui_impl_opengl3.cpp
                      ./include/imgui/imgui_tables.cpp ./include/imgui/imgui_widgets.cpp)


set_target_properties(opengl PROPERTIES 
    CXX_STANDARD 20 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)






target_include_directories(opengl PRIVATE include)

#working on this
target_link_libraries(opengl PRIVATE glfw OpenGL::GL glm)

#adding other directories that arent necessary to  


# 1. Define the source and destination
set(RES_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(RES_DEST "${CMAKE_CURRENT_BINARY_DIR}/res")

add_custom_target(update_res ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RES_SOURCE} ${RES_DEST}
    COMMENT "Syncing resources to build directory..."
)

add_dependencies(opengl update_res)
