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
#include <glm/glm.hpp>
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
  using Point3d = glm::fvec3;
  using Vector3 = Point3d;
  using FloatArray = std::vector<GLfloat>;
  using UintArray  = std::vector<GLuint>;

} // namespace ktp

#endif // KETEMINE_SRC_TYPES_HPP_
