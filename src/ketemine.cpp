#include "ketemine.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* ktp::keteMine::window {nullptr};
ktp::Size2D ktp::keteMine::window_size {1920, 1080};

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

bool ktp::keteMine::init() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(window_size.x, window_size.y, "keteMine", nullptr, nullptr);
  if (!window) return false;

  glfwMakeContextCurrent(window);
  glClearColor(0.7f, 0.9f, 0.1f, 1.0f);

  glewExperimental = GL_TRUE;
  const auto err {glewInit()};
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << '\n';
    return false;
  }

  glViewport(0, 0, window_size.x, window_size.y);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetKeyCallback(window, keyCallback);

  return true;
}

void ktp::keteMine::run() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
  }
  glfwTerminate();
}
