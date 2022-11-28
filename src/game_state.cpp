#include "game_state.hpp"

#include "ketemine.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "gui/gui.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "../lib/imgui/imgui_impl_glfw.h"

using namespace ktp;

// PLAYINGSTATE STATICS

keteMine::PlayingState keteMine::GameState::s_playing_state {};
bool keteMine::PlayingState::s_mouse_captured {false};

void keteMine::PlayingState::keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mode) {
  ImGui_ImplGlfw_KeyCallback(window, key, scan_code, action, mode);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    show_gui = !show_gui;
}

void keteMine::PlayingState::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
  if (ImGui::GetIO().WantCaptureMouse) return;
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    s_mouse_captured ? releaseMouse() : captureMouse();
    s_mouse_captured = !s_mouse_captured;
  }
}

void keteMine::PlayingState::mouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
  ImGui_ImplGlfw_CursorPosCallback(window, x_pos, y_pos);
  if (ImGui::GetIO().WantCaptureMouse || !s_mouse_captured) return;
  const float x {static_cast<float>(x_pos)}, y {static_cast<float>(y_pos)};
  static float last_x, last_y;
  static bool first_mouse {true};
  if (first_mouse) {
    last_x = x;
    last_y = y;
    first_mouse = false;
  }
  const float x_offset {x - last_x};
  const float y_offset {last_y - y};

  last_x = x;
  last_y = y;

  GameState::s_playing_state.camera().look(x_offset, y_offset);
}

void keteMine::PlayingState::scrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
  ImGui_ImplGlfw_ScrollCallback(window, x_offset, y_offset);
  if (ImGui::GetIO().WantCaptureMouse || !s_mouse_captured) return;
  GameState::s_playing_state.camera().zoom(static_cast<float>(y_offset));
}

// PLAYINGSTATE MEMBER FUNCTIONS

void keteMine::PlayingState::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, window_size.x, window_size.y);

  m_shader_program.use();
  m_vao.bind();
  glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertices_data.size(), kNumCubes);

  if (show_gui) gui::draw();

  glfwSwapBuffers(window);
}

keteMine::GameState* keteMine::PlayingState::enter() {
  m_shader_program = Resources::getShaderProgram("test");
  // vertices
  m_vertices_data = cube(0.05f);
  m_vertices.genBuffer();
  m_vertices.setup(m_vertices_data);
  m_vao.genVertexArray();
  m_vao.linkAttrib(m_vertices, 0, 3, GL_FLOAT, 0, nullptr);
  // colors
  FloatArray colors_data {};
  colors_data.resize(m_vertices_data.size());
  for (auto& color_c: colors_data) {
    color_c = rng::randomFloat(0.f, 0.7f);
  }
  m_colors.genBuffer();
  m_colors.setup(colors_data);
  m_vao.linkAttrib(m_colors, 1, 3, GL_FLOAT, 0, nullptr);
  // translations
  std::vector<Point3D> translations(kNumCubes);
  unsigned int index {};
  float offset {0.1f};
  for (int z = -10; z < 10; z += 2) {
    for (int y = -10; y < 10; y += 2) {
      for (int x = -10; x < 10; x += 2) {
        Point3D translation {};
        translation.x = (float)x * 0.1f + offset;
        translation.y = (float)y * 0.1f + offset;
        translation.z = (float)z * 0.1f + offset;
        translations[index++] = translation;
      }
    }
  }
  m_translations.genBuffer();
  m_translations.setup(translations.data(), kNumCubes * sizeof(Point3D));
  m_vao.linkAttrib(m_translations, 2, 3, GL_FLOAT, 0, nullptr);
  glVertexAttribDivisor(2, 1);
  // callbacks
  glfwSetKeyCallback(window, PlayingState::keyCallback);
  glfwSetCursorPosCallback(window, PlayingState::mouseCallback);
  glfwSetMouseButtonCallback(window, PlayingState::mouseButtonCallback);
  glfwSetScrollCallback(window, PlayingState::scrollCallback);
  return this;
}

void keteMine::PlayingState::handleInput(GLFWwindow* window) {}

void keteMine::PlayingState::update(double delta_time) {
  updateCamera(delta_time);
  updateMVP();
}

void keteMine::PlayingState::updateCamera(double delta_time) {
  if (ImGui::GetIO().WantCaptureKeyboard || !s_mouse_captured) return;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    m_camera.move(CameraMovement::Forward, static_cast<float>(delta_time));
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_camera.move(CameraMovement::Backward, static_cast<float>(delta_time));
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    m_camera.move(CameraMovement::Left, static_cast<float>(delta_time));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    m_camera.move(CameraMovement::Right, static_cast<float>(delta_time));
  }
}

void keteMine::PlayingState::updateMVP() {
  glm::mat4 model {1.f};
  const glm::mat4 mvp {m_camera.projectionMatrix() * m_camera.viewMatrix() * model};
  m_shader_program.use();
  m_shader_program.setMat4f("mvp", glm::value_ptr(mvp));
}
