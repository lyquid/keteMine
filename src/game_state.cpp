#include "game_state.hpp"

#include "ketemine.hpp"
#include "resources.hpp"
#include "gui/gui.hpp"
#include <GLFW/glfw3.h>

using namespace ktp;

// PLAYINGSTATE

keteMine::PlayingState keteMine::GameState::playing_state {};

keteMine::GameState* keteMine::PlayingState::enter() {
  FloatArray points {
     0.0f,  0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
  };
  m_points.genBuffer();
  m_points.setup(points, GL_STATIC_DRAW);

  FloatArray colors {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };
  m_colors.genBuffer();
  m_colors.setup(colors, GL_STATIC_DRAW);

  m_vao.genVertexArray();
  m_vao.linkAttrib(m_points, 0, 3, GL_FLOAT, 0, nullptr);
  m_vao.linkAttrib(m_colors, 1, 3, GL_FLOAT, 0, nullptr);

  m_shader = Resources::getShaderProgram("interpolation");

  return this;
}

void keteMine::PlayingState::draw() {
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, keteMine::window_size.x, keteMine::window_size.y);

  m_shader.use();
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);

  if (keteMine::show_gui) gui::draw();

  glfwSwapBuffers(keteMine::window);
}

void keteMine::PlayingState::handleInput(GLFWwindow* window) {}

void keteMine::PlayingState::update(double delta_time) {}
