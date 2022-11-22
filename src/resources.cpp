#include "resources.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

void logError(const std::string& msg) {
  std::cerr << msg << "\n";
}

void logError(const std::string& msg, const std::string& path) {
  std::cerr << msg << " file: \"" << path << "\"\n";
}

void logMessage(const std::string& msg) {
  std::cout << msg << "\n";
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
}

// SHADERS

ktp::Resources::ShaderPrograms ktp::Resources::shader_programs {};

bool ktp::Resources::createShaderProgram(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path) {
  // Create the shaders
	GLuint vertex_shader_id {glCreateShader(GL_VERTEX_SHADER)};
	GLuint fragment_shader_id {glCreateShader(GL_FRAGMENT_SHADER)};
  bool geometry_shader_present {};
  GLuint geometry_shader_id {};
  if (geometry_shader_path != "") {
    geometry_shader_present = true;
    glCreateShader(GL_GEOMETRY_SHADER);
  }
  // Read the Vertex Shader code from the file
	std::string vertex_shader_code {};
	std::ifstream vertex_shader_stream {vertex_shader_path, std::ios::in};
	if (vertex_shader_stream.is_open()) {
		std::stringstream sstr {};
		sstr << vertex_shader_stream.rdbuf();
		vertex_shader_code = sstr.str();
		vertex_shader_stream.close();
	} else {
    logError("Could NOT open vertex shader file", vertex_shader_path);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(geometry_shader_id);
    return false;
	}
  // Read the Fragment Shader code from the file
	std::string fragment_shader_code {};
	std::ifstream fragment_shader_stream {fragment_shader_path, std::ios::in};
	if (fragment_shader_stream.is_open()) {
		std::stringstream sstr {};
		sstr << fragment_shader_stream.rdbuf();
		fragment_shader_code = sstr.str();
		fragment_shader_stream.close();
	} else {
    logError("Could NOT open fragment shader file", fragment_shader_path);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(geometry_shader_id);
    return false;
	}
  std::string geometry_shader_code {};
  if (geometry_shader_present) {
    // Read the Geometry Shader code from the file
    std::ifstream geometry_shader_stream {geometry_shader_path, std::ios::in};
    if (geometry_shader_stream.is_open()) {
      std::stringstream sstr {};
      sstr << geometry_shader_stream.rdbuf();
      geometry_shader_code = sstr.str();
      geometry_shader_stream.close();
    } else {
      logError("Could NOT open geometry shader file", geometry_shader_path);
      glDeleteShader(vertex_shader_id);
      glDeleteShader(fragment_shader_id);
      glDeleteShader(geometry_shader_id);
      return false;
    }
  }
  // Compile Vertex Shader
  logMessage("Compiling vertex shader " + vertex_shader_path);
	const auto vertex_source_pointer {vertex_shader_code.c_str()};
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr);
	glCompileShader(vertex_shader_id);
  glCheckError();
  if (!printShaderLog(vertex_shader_id)) {
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(geometry_shader_id);
    return false;
  }
  // Compile Fragment Shader
  logMessage("Compiling fragment shader " + fragment_shader_path);
	const auto fragment_source_pointer {fragment_shader_code.c_str()};
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr);
	glCompileShader(fragment_shader_id);
  glCheckError();
  if (!printShaderLog(fragment_shader_id)) {
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(geometry_shader_id);
    return false;
  }
  // Compile Geometry shader
  if (geometry_shader_present) {
    logMessage("Compiling geometry shader " + geometry_shader_path);
    const auto geometry_source_pointer {geometry_shader_code.c_str()};
    glShaderSource(geometry_shader_id, 1, &geometry_source_pointer, nullptr);
    glCompileShader(geometry_shader_id);
    glCheckError();
    if (!printShaderLog(geometry_shader_id)) {
      glDeleteShader(vertex_shader_id);
      glDeleteShader(fragment_shader_id);
      glDeleteShader(geometry_shader_id);
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
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(geometry_shader_id);
    return false;
  }
  // Clean
	glDeleteShader(vertex_shader_id);
  glCheckError();
	glDeleteShader(fragment_shader_id);
  glCheckError();
  if (geometry_shader_present) {
    glDeleteShader(geometry_shader_id);
    glCheckError();
  }
  // add shader to the maps
  shader_programs[name] = {id, vertex_shader_code, fragment_shader_code, geometry_shader_code};
  logMessage("Shader program \"" + name + "\" successfully compiled and linked.");
  return true;
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
