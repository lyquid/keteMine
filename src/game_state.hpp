/**
 * @file game_state.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief Game states FSM.
 * @version 0.1
 * @date 2022-11-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_GAME_STATE_HPP_)
#define KETEMINE_SRC_GAME_STATE_HPP_

#include "camera.hpp"
#include "opengl.hpp"
#include "types.hpp"

namespace ktp { namespace keteMine {

class GameState {
 public:
  virtual ~GameState() {}

  static GameState* goToState(GameState& new_state) { return new_state.enter(); }

  virtual void draw() = 0;
  virtual void handleInput(GLFWwindow* window) = 0;
  virtual void update(double delta_time) = 0;

  static PlayingState playing_state;

 protected:
  virtual GameState* enter() { return this; }
};

class PlayingState: public GameState {
 public:
  auto& camera() { return m_camera; }
  void draw() override;
  void handleInput(GLFWwindow* window) override;
  void update(double delta_time) override;

 private:
  static void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mode);
  static void mouseCallback(GLFWwindow* window, double x_pos, double y_pos);
  static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset);
  void updateCamera(double delta_time);
  void updateMVP();
  GameState* enter() override;

  static constexpr int kNumCubes {1000};

  VAO m_vao {};
  VBO m_vertices {};
  VBO m_colors {};
  VBO m_translations {};
  FloatArray m_vertices_data {};
  ShaderProgram m_shader_program {};

  Camera m_camera {glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f};
};

} } // namespace State / ktp

#endif // KETEMINE_SRC_GAME_STATE_HPP_
