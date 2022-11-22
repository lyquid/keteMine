/**
 * @file resources.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief Resources loading utilities.
 * @version 0.1
 * @date 2022-11-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_RESOURCES_HPP_)
#define KETEMINE_SRC_RESOURCES_HPP_

#include "opengl.hpp"
#include "types.hpp"
#include <string>

namespace ktp { namespace Resources {

void loadResources();

// SHADERS

struct ShaderProgramInfo {
  GLuint id {};
  std::string vertex {};
  std::string fragment {};
  std::string geometry {};
};

extern ShaderPrograms shader_programs;

/**
 * @brief Loads and compiles a shader program.
 * @param name The name you wan to give to the shader program.
 * @param vertex_shader_path Vertex shader file path.
 * @param fragment_shader_path Fragment shader file path.
 * @param geometry_shader_path Geometry shader file path.
 */
bool createShaderProgram(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path = "");

/**
 * @brief Retrieves a shader by name.
 * @param name The name of the shader you want.
 * @return A ShaderProgram with the shader requested.
 */
inline auto getShaderProgram(const std::string& name) { return ShaderProgram{shader_programs.at(name).id}; }

/**
 * @brief Prints any problems with the shader program.
 * @param program The id of the shader program.
 */
bool printProgramLog(GLuint program);

/**
 * @brief Prints problems with the shader, if any.
 * @param shader The shader id.
 */
bool printShaderLog(GLuint shader);

} } // namespace resources/ktp

#endif // KETEMINE_SRC_RESOURCES_HPP_

