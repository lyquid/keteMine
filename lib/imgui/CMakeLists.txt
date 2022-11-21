add_library(imgui
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_impl_glfw.cpp
  imgui_impl_opengl3.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
  imgui.cpp
)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
  target_link_libraries(imgui PRIVATE
    GLEW::GLEW
  )
else()
  target_link_libraries(imgui PRIVATE
    glfw
  )
endif()
