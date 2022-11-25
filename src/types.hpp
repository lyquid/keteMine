/**
 * @file types.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_TYPES_HPP_)
#define KETEMINE_SRC_TYPES_HPP_

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <map>
#include <string>
#include <vector>

// GLWF
struct GLFWwindow;

namespace ktp {

  class EBO;
  class ShaderProgram;
  class Texture2D;
  class VAO;
  class VBO;

  using Size2D  = glm::vec<2, GLint>;
  using Size2Du = glm::vec<2, GLuint>;
  using Point3D = glm::vec<3, GLfloat>;
  using Vector3 = Point3D;
  using FloatArray = std::vector<GLfloat>;
  using UintArray  = std::vector<GLuint>;

  namespace keteMine {
    class GameState;
    class PlayingState;
  } // namespace keteMine

  namespace Resources {
    struct ShaderProgramInfo;
    using ShaderPrograms = std::map<std::string, Resources::ShaderProgramInfo>;
  } // namespace Resources

} // namespace ktp

#endif // KETEMINE_SRC_TYPES_HPP_
