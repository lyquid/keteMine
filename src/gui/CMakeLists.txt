add_library(keteMineGUI
  gui.cpp
  log.cpp
)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
  target_link_libraries(keteMineGUI PRIVATE
    GLEW::GLEW
    imgui
  )
else()
  target_link_libraries(keteMineGUI PRIVATE
    GLEW::GLEW
    imgui
  )
endif()
