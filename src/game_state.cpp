#include "game_state.hpp"

#include "ketemine.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "gui/gui.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

using namespace ktp;

// PLAYINGSTATE

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  const float x {static_cast<float>(xpos)}, y {static_cast<float>(ypos)};
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

  keteMine::GameState::playing_state.camera().look(x_offset, y_offset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  keteMine::GameState::playing_state.camera().zoom(static_cast<float>(yoffset));
}

keteMine::PlayingState keteMine::GameState::playing_state {};

void keteMine::PlayingState::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, keteMine::window_size.x, keteMine::window_size.y);

  m_shader_program.use();
  m_vao.bind();
  glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertices_data.size(), kNumCubes);

  if (keteMine::show_gui) gui::draw();

  glfwSwapBuffers(keteMine::window);
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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  return this;
}

void keteMine::PlayingState::handleInput(GLFWwindow* window) {}

void keteMine::PlayingState::update(double delta_time) {
  updateCamera(delta_time);
  updateMVP();
}

void keteMine::PlayingState::updateCamera(double delta_time) {
  if (ImGui::GetIO().WantCaptureKeyboard) return;

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
