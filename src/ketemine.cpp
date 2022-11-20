#include "ketemine.hpp"

#include "gui/gui.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* ktp::keteMine::window {nullptr};
ktp::Size2D ktp::keteMine::window_size {1920, 1080};

void errorCallback(int error, const char* description) {
  std::cout << "GLFW error " << error << ": " << description << '\n';
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  ktp::keteMine::window_size = {width, height};
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
  ktp::keteMine::window_size = {width, height};
  // update any perspective matrices used here
}

void ktp::keteMine::init() {
  // GLFW
  glfwSetErrorCallback(errorCallback);
  if (!glfwInit()) exit(EXIT_FAILURE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  // window
  // GLFWmonitor* monitor {glfwGetPrimaryMonitor()};
  // const GLFWvidmode* video_mode {glfwGetVideoMode(monitor)};
  // window = glfwCreateWindow(video_mode->width, video_mode->height, "keteMine", monitor, nullptr);
  window = glfwCreateWindow(window_size.x, window_size.y, "keteMine", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glViewport(0, 0, window_size.x, window_size.y);
  // callbacks
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetWindowSizeCallback(window, windowSizeCallback);
  // GLEW
  glewExperimental = GL_TRUE;
  const auto err {glewInit()};
  if (GLEW_OK != err) {
    std::cerr << "GLEW error: " << glewGetErrorString(err) << '\n';
    exit(EXIT_FAILURE);
  }

  versionInfo();

  gui::init(window);
}

void ktp::keteMine::run() {
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, window_size.x, window_size.y);

    gui::layout();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}

void ktp::keteMine::versionInfo() {
  std::cout << glGetString(GL_RENDERER) << '\n';
  std::cout << "OpenGL " << glGetString(GL_VERSION) << '\n';
  std::cout << "GLEW " << glewGetString(GLEW_VERSION) << '\n';
  std::cout << "GLFW " << glfwGetVersionString() << '\n';
}
