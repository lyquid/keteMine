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

#include "opengl.hpp"
#include "types.hpp"

namespace ktp { namespace keteMine {

class GameState {
 public:
  virtual ~GameState() {}
  virtual void draw() = 0;
  virtual void handleInput(GLFWwindow* window) = 0;
  virtual void update(double delta_time) = 0;

  static GameState* goToState(GameState& new_state) { return new_state.enter(); }

  static PlayingState playing_state;

 protected:
  virtual GameState* enter() { return this; }
};

class PlayingState: public GameState {
 public:
  void draw() override;
  void handleInput(GLFWwindow* window) override;
  void update(double delta_time) override;
 private:
  GameState* enter() override;
  VBO m_points {};
  VBO m_colors {};
  VAO m_vao {};
  ShaderProgram m_shader {};
};

} } // namespace State / ktp

#endif // KETEMINE_SRC_GAME_STATE_HPP_
