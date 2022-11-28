#include "resources.hpp"

#include "gui/gui.hpp"
#include <fstream>
#include <sstream>

void logError(const std::string& msg) {
  ktp::gui::log.addError("%s\n", msg.c_str());
}

void logError(const std::string& msg, const std::string& path) {
  ktp::gui::log.addError("%s file: \"%s\"\n", msg.c_str(), path.c_str());
}

void logMessage(const std::string& msg) {
  ktp::gui::log.add("%s\n", msg.c_str());
}

void ktp::Resources::loadResources() {
  Resources::createShaderProgram(
    "basic",
    "resources/shaders/basic.vert",
    "resources/shaders/basic.frag"
  );
  Resources::createShaderProgram(
    "interpolation",
    "resources/shaders/basic.vert",
    "resources/shaders/interpolation.frag"
  );
  Resources::createShaderProgram(
    "test",
    "resources/shaders/test.vert",
    "resources/shaders/test.frag"
  );
}

// SHADERS

ktp::Resources::ShaderPrograms ktp::Resources::shader_programs {};

bool ktp::Resources::compileShader(GLuint shader, const std::string& source) {
  const auto source_pointer {source.c_str()};
  glShaderSource(shader, 1, &source_pointer, nullptr);
  glCompileShader(shader);
  glCheckError();
  if (!printShaderLog(shader)) return false;
  return true;
}

bool ktp::Resources::createShaderProgram(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path) {
  // Create the shaders
	GLuint vertex_shader_id {glCreateShader(GL_VERTEX_SHADER)};
  glCheckError();
	GLuint fragment_shader_id {glCreateShader(GL_FRAGMENT_SHADER)};
  glCheckError();
  bool geometry_shader_present {};
  GLuint geometry_shader_id {};
  if (geometry_shader_path != "") {
    geometry_shader_present = true;
    glCreateShader(GL_GEOMETRY_SHADER);
    glCheckError();
  }
  // Read the Vertex Shader code from the file
	const std::string vertex_shader_code {loadShaderSource(vertex_shader_path)};
  if (vertex_shader_code == "") {
    logError("Could NOT open vertex shader file", vertex_shader_path);
    deleteShaders({vertex_shader_id, fragment_shader_id, geometry_shader_id});
    return false;
  }
  // Read the Fragment Shader code from the file
	const std::string fragment_shader_code {loadShaderSource(fragment_shader_path)};
  if (fragment_shader_code == "") {
    logError("Could NOT open fragment shader file", fragment_shader_path);
    deleteShaders({vertex_shader_id, fragment_shader_id, geometry_shader_id});
    return false;
  }
  std::string geometry_shader_code {};
  if (geometry_shader_present) {
    // Read the Geometry Shader code from the file
    geometry_shader_code = loadShaderSource(geometry_shader_path);
    if (geometry_shader_code == "") {
      logError("Could NOT open geometry shader file", geometry_shader_path);
      deleteShaders({vertex_shader_id, fragment_shader_id, geometry_shader_id});
      return false;
    }
  }
  // compile shaders
  if (!compileShader(vertex_shader_id, vertex_shader_code)
   || !compileShader(fragment_shader_id, fragment_shader_code)) {
    deleteShaders({vertex_shader_id, fragment_shader_id, geometry_shader_id});
    return false;
  }
  if (geometry_shader_present) {
    if (!compileShader(geometry_shader_id, geometry_shader_code)) {
      deleteShaders({vertex_shader_id, fragment_shader_id, geometry_shader_id});
      return false;
    }
  }
  // Link the program
	const GLuint id {glCreateProgram()};
  glCheckError();
	glAttachShader(id, vertex_shader_id);
  glCheckError();
	glAttachShader(id, fragment_shader_id);
  glCheckError();
  if (geometry_shader_present) {
    glAttachShader(id, geometry_shader_id);
    glCheckError();
  }
	glLinkProgram(id);
  glCheckError();
  if (!printProgramLog(id)) {
    deleteShaders({vertex_shader_id, fragment_shader_id, geometry_shader_id});
    return false;
  }
  // Clean
	deleteShaders({vertex_shader_id, fragment_shader_id, geometry_shader_id});
  // add shader to the maps
  shader_programs[name] = {id, vertex_shader_code, fragment_shader_code, geometry_shader_code};
  logMessage("Shader program \"" + name + "\" successfully compiled and linked.");
  return true;
}

void ktp::Resources::deleteShaders(const std::initializer_list<GLuint>& list) {
  for(auto id: list) {
    glDeleteShader(id);
    glCheckError();
  }
}

std::string ktp::Resources::loadShaderSource(const std::string path) {
  std::ifstream file {path};
  if (!file.is_open()) return "";
  std::stringstream sstr {};
	sstr << file.rdbuf();
  file.close();
  return sstr.str();
}

bool ktp::Resources::printProgramLog(GLuint program) {
  // Make sure name is program
  if (glIsProgram(program)) {
    // Program log length
    int info_log_length{}, max_length{};
    // Get info string length
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
    std::string info_log {};
    info_log.resize((unsigned int)max_length);
    // Get info log
    glGetProgramInfoLog(program, max_length, &info_log_length, info_log.data());
    if (info_log_length > 0) {
      logError(info_log);
      return false;
    }
  } else {
    logError("printProgramLog(): Name " + std::to_string(program) + " is not a shader program.");
    return false;
  }
  return true;
}

bool ktp::Resources::printShaderLog(GLuint shader) {
  // Make sure name is shader
  if (glIsShader(shader)) {
    // Shader log length
    int info_log_length {}, max_length {};
    // Get info string length
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
    std::string info_log {};
    info_log.resize((unsigned int)max_length);
    // Get info log
    glGetShaderInfoLog(shader, max_length, &info_log_length, info_log.data());
    if (info_log_length > 0) {
      logError(info_log);
      return false;
    }
  } else {
    logError("printShaderLog(): Name " + std::to_string(shader) + " is not a shader.");
    return false;
  }
  return true;
}
