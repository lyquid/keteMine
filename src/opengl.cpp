#include "opengl.hpp"

#include <glm/common.hpp>
#include <iostream>
#include <string>

GLenum ktp::glCheckError_(const char* file, int line) {
  GLenum error_code {};
  while ((error_code = glGetError())) {
    std::string error_msg {};
    switch (error_code) {
      case GL_INVALID_ENUM:                  error_msg = "INVALID_ENUM"; break;
      case GL_INVALID_VALUE:                 error_msg = "INVALID_VALUE"; break;
      case GL_INVALID_OPERATION:             error_msg = "INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:                error_msg = "STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:               error_msg = "STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:                 error_msg = "OUT_OF_MEMORY"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error_msg = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }
    error_msg = error_msg + " in file " + file + " (" + std::to_string(line) + ")\n";
    std::cerr << error_msg;
  }
  return error_code;
}

ktp::FloatArray ktp::cube(GLfloat size) {
  const auto good_size {glm::abs(size)};
  const FloatArray vertices {
    -1.f * good_size, -1.f * good_size, -1.f * good_size, // triangle 1 : begin
    -1.f * good_size, -1.f * good_size,  1.f * good_size,
    -1.f * good_size,  1.f * good_size,  1.f * good_size, // triangle 1 : end
     1.f * good_size,  1.f * good_size, -1.f * good_size, // triangle 2 : begin
    -1.f * good_size, -1.f * good_size, -1.f * good_size,
    -1.f * good_size,  1.f * good_size, -1.f * good_size, // triangle 2 : end
     1.f * good_size, -1.f * good_size,  1.f * good_size,
    -1.f * good_size, -1.f * good_size, -1.f * good_size,
     1.f * good_size, -1.f * good_size, -1.f * good_size,
     1.f * good_size,  1.f * good_size, -1.f * good_size,
     1.f * good_size, -1.f * good_size, -1.f * good_size,
    -1.f * good_size, -1.f * good_size, -1.f * good_size,
    -1.f * good_size, -1.f * good_size, -1.f * good_size,
    -1.f * good_size,  1.f * good_size,  1.f * good_size,
    -1.f * good_size,  1.f * good_size, -1.f * good_size,
     1.f * good_size, -1.f * good_size,  1.f * good_size,
    -1.f * good_size, -1.f * good_size,  1.f * good_size,
    -1.f * good_size, -1.f * good_size, -1.f * good_size,
    -1.f * good_size,  1.f * good_size,  1.f * good_size,
    -1.f * good_size, -1.f * good_size,  1.f * good_size,
     1.f * good_size, -1.f * good_size,  1.f * good_size,
     1.f * good_size,  1.f * good_size,  1.f * good_size,
     1.f * good_size, -1.f * good_size, -1.f * good_size,
     1.f * good_size,  1.f * good_size, -1.f * good_size,
     1.f * good_size, -1.f * good_size, -1.f * good_size,
     1.f * good_size,  1.f * good_size,  1.f * good_size,
     1.f * good_size, -1.f * good_size,  1.f * good_size,
     1.f * good_size,  1.f * good_size,  1.f * good_size,
     1.f * good_size,  1.f * good_size, -1.f * good_size,
    -1.f * good_size,  1.f * good_size, -1.f * good_size,
     1.f * good_size,  1.f * good_size,  1.f * good_size,
    -1.f * good_size,  1.f * good_size, -1.f * good_size,
    -1.f * good_size,  1.f * good_size,  1.f * good_size,
     1.f * good_size,  1.f * good_size,  1.f * good_size,
    -1.f * good_size,  1.f * good_size,  1.f * good_size,
     1.f * good_size, -1.f * good_size,  1.f * good_size
  };
  return vertices;
}

/* VBO */

ktp::VBO::VBO() {
  glGenBuffers(1, &m_id);
}

// this setup is needed when you pass nullptr for a later use with subData
void ktp::VBO::setup(const GLfloat* vertices, GLsizeiptr size, GLenum usage) {
  glBindBuffer(GL_ARRAY_BUFFER, m_id);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

/* EBO */

ktp::EBO::EBO() {
  glGenBuffers(1, &m_id);
}

void ktp::EBO::generateEBO(FloatArray& vertices, UintArray& indices) {
  FloatArray unique_coords {};
  indices.clear();
  // push the first element
  indices.push_back(0);
  // push the first three coords
  unique_coords.push_back(vertices[0]);
  unique_coords.push_back(vertices[1]);
  unique_coords.push_back(vertices[2]);
  // find repeated coords by groups of 3
  for (std::size_t i = 3; i < vertices.size(); i += 3) {
    bool found {false};
    for (std::size_t j = 0; j < unique_coords.size(); j += 3) {
      if (vertices[i] == unique_coords[j] && vertices[i + 1] == unique_coords[j + 1] && vertices[i + 2] == unique_coords[j + 2]) {
        // coord already in the list
        found = true;
        // so, new index will point to...
        indices.push_back(j / 3u);
        break;
      }
    }
    if (!found) {
      // new unique coord
      unique_coords.push_back(vertices[i]);
      unique_coords.push_back(vertices[i + 1]);
      unique_coords.push_back(vertices[i + 2]);
      // the new index points at...
      indices.push_back((unique_coords.size() - 1u) / 3u);
    }
  }
  // the new vertices
  vertices = std::move(unique_coords);
}

void ktp::EBO::setup(const UintArray& indices, GLenum usage) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), usage);
}

void ktp::EBO::setup(const GLuint* indices, GLsizeiptr size, GLenum usage) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}

/* VAO */

ktp::VAO::VAO() {
  glGenVertexArrays(1, &m_id);
}

void ktp::VAO::linkAttrib(const VBO& vbo, GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void* offset, GLboolean normalize) const {
  glBindVertexArray(m_id);
  vbo.bind();
  glEnableVertexAttribArray(layout);
  glVertexAttribPointer(
    layout,       // index: specifies the index of the generic vertex attribute to be modified. Must match the layout in the shader
    components,   // size: specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
    type,         // type of the data
    normalize,    // normalize: specifies whether fixed-point data values should be normalized
    stride,       // stride: specifies the byte offset between consecutive generic vertex attributes
    offset        // pointer: specifies a offset of the first component of the first generic vertex attribute in the array in the data store
  );
}

void ktp::VAO::linkAttribFast(GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void* offset, GLboolean normalize) const {
  glEnableVertexAttribArray(layout);
  glVertexAttribPointer(
    layout,       // index: specifies the index of the generic vertex attribute to be modified. Must match the layout in the shader
    components,   // size: specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
    type,         // type of the data
    normalize,    // normalize: specifies whether fixed-point data values should be normalized
    stride,       // stride: specifies the byte offset between consecutive generic vertex attributes
    offset        // pointer: specifies a offset of the first component of the first generic vertex attribute in the array in the data store
  );
}
