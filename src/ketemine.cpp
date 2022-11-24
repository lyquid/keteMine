#include "ketemine.hpp"

#include "opengl.hpp"
#include "resources.hpp"
#include "gui/gui.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool ktp::keteMine::show_gui {true};
GLFWwindow* ktp::keteMine::window {nullptr};
ktp::Size2D ktp::keteMine::window_size {1920, 1080};

// CALLBACKS

void glfwErrorCallback(int error, const char* description) {
  ktp::gui::log.addError("GLFW error %d: %s\n", error, description);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  ktp::keteMine::window_size = {width, height};
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    ktp::keteMine::show_gui = !ktp::keteMine::show_gui;
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
  gui::log.add("GL context params:\n");
  // integers - only works if the order is 0-10 integer return types
  for (int i = 0; i < 10; i++) {
    int v {0};
    glGetIntegerv(params[i], &v);
    gui::log.add(" %s %d\n", names[i], v);
  }
  // others
  int v[2];
  v[0] = v[1] = 0;
  glGetIntegerv(params[10], v);
  gui::log.add(" %s %d %d\n", names[10], v[0], v[1]);
  unsigned char s {0};
  glGetBooleanv(params[11], &s);
  gui::log.add(" %s %u\n", names[11], (unsigned int)s);
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
  glfwWindowHint(GLFW_MAXIMIZED, true);
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
    gui::log.addError("GLEW error: %s\n", glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }

  versionInfo();

  gui::init(window);

  contextInfo();

  Resources::loadResources();
}

void ktp::keteMine::run() {
  FloatArray points {
     0.0f,  0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
  };
  VBO vbo_points {};
  vbo_points.setup(points, GL_STATIC_DRAW);

  FloatArray colors {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };
  VBO vbo_colors {};
  vbo_colors.setup(colors, GL_STATIC_DRAW);

  VAO vao {};
  vao.linkAttrib(vbo_points, 0, 3, GL_FLOAT, 0, nullptr);
  vao.linkAttrib(vbo_colors, 1, 3, GL_FLOAT, 0, nullptr);

  ShaderProgram shader {Resources::getShaderProgram("interpolation")};;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, window_size.x, window_size.y);

    shader.use();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    if (show_gui) gui::draw();

    glfwSwapBuffers(window);
  }
  gui::clean();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void ktp::keteMine::versionInfo() {
  gui::log.add("%s\n", glGetString(GL_RENDERER));
  gui::log.add("OpenGL %s\n", glGetString(GL_VERSION));
  gui::log.add("GLEW %s\n", glewGetString(GLEW_VERSION));
  gui::log.add("GLFW %s\n", glfwGetVersionString());
}
