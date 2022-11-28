#include "ketemine.hpp"

#include "game_state.hpp"
#include "opengl.hpp"
#include "resources.hpp"
#include "gui/gui.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace ktp;

bool keteMine::show_gui {true};
keteMine::GameState* keteMine::state {nullptr};
GLFWwindow* keteMine::window {nullptr};
Size2D keteMine::window_size {1920, 1080};

// CALLBACKS

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  // keteMine::window_size = {width, height};
  glViewport(0, 0, width, height);
}

void glfwErrorCallback(int error, const char* description) {
  gui::log.addError("GLFW error %d: %s\n", error, description);
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
  keteMine::window_size = {width, height};
  // update any perspective matrices used here
}

// main functions

void keteMine::contextInfo() {
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

void keteMine::init() {
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
  GLFWmonitor* monitor {glfwGetPrimaryMonitor()};
  const GLFWvidmode* video_mode {glfwGetVideoMode(monitor)};
  window_size.x = video_mode->width;
  window_size.y = video_mode->height;
  // window = glfwCreateWindow(video_mode->width, video_mode->height, "keteMine", monitor, nullptr);
  window = glfwCreateWindow(window_size.x, window_size.y, "keteMine", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glViewport(0, 0, window_size.x, window_size.y);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // callbacks
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
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

  state = GameState::goToState(GameState::playing_state);
}

void keteMine::run() {
  constexpr double dt {1.0 / 60.0};
  double current_time {glfwGetTime()};
  double accumulator {0.0};

  while (!glfwWindowShouldClose(window)) {
    // events
    glfwPollEvents();
    state->handleInput(window);
    // fixed time step logic
    double new_time {glfwGetTime()};
    double frame_time {new_time - current_time};
    if (frame_time > 0.25) frame_time = 0.25;

    current_time = new_time;
    accumulator += frame_time;
    // update logic
    while (accumulator >= dt) {
      state->update(dt);
      accumulator -= dt;
    }
    // render game
    state->draw();
  }
  gui::clean();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void keteMine::versionInfo() {
  gui::log.add("keteMine v0.1\n");
  gui::log.add("%s\n", glGetString(GL_RENDERER));
  gui::log.add("OpenGL %s\n", glGetString(GL_VERSION));
  gui::log.add("GLEW %s\n", glewGetString(GLEW_VERSION));
  gui::log.add("GLFW %s\n", glfwGetVersionString());
}
