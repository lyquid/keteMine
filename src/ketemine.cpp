#include "ketemine.hpp"

#include "gui/gui.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* ktp::keteMine::window {nullptr};
ktp::Size2D ktp::keteMine::window_size {1920, 1080};

void glfwErrorCallback(int error, const char* description) {
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

void ktp::keteMine::contextInfo() {
  GLenum params[] = {
    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE,
    GL_MAX_DRAW_BUFFERS,
    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    GL_MAX_TEXTURE_IMAGE_UNITS,
    GL_MAX_TEXTURE_SIZE,
    GL_MAX_VARYING_FLOATS,
    GL_MAX_VERTEX_ATTRIBS,
    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
    GL_MAX_VERTEX_UNIFORM_COMPONENTS,
    GL_MAX_VIEWPORT_DIMS,
    GL_STEREO,
  };
  const char* names[] = {
    "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
    "GL_MAX_DRAW_BUFFERS",
    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
    "GL_MAX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_TEXTURE_SIZE",
    "GL_MAX_VARYING_FLOATS",
    "GL_MAX_VERTEX_ATTRIBS",
    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
    "GL_MAX_VIEWPORT_DIMS",
    "GL_STEREO",
  };
  std::cout << "GL context params:\n";
  // integers - only works if the order is 0-10 integer return types
  for (int i = 0; i < 10; i++) {
    int v = 0;
    glGetIntegerv(params[i], &v);
    std::cout << "  " << names[i] << " " << v << "\n";
  }
  // others
  int v[2];
  v[0] = v[1] = 0;
  glGetIntegerv(params[10], v);
  std::cout << "  " << names[10] << " " << v[0] << " " << v[1] << "\n";
  unsigned char s = 0;
  glGetBooleanv(params[11], &s);
  std::cout << "  " << names[11] << " " << (unsigned int)s << "\n";
}

void ktp::keteMine::init() {
  // GLFW
  glfwSetErrorCallback(glfwErrorCallback);
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

  contextInfo();
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
