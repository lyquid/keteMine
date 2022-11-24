/**
 * @file opengl.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief OpenGL utilities.
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_OPENGL_HPP_)
#define KETEMINE_SRC_OPENGL_HPP_

#include "types.hpp"
#include <GL/glew.h>
#include <utility>

namespace ktp {

/**
 * @brief Checks for OpenGL errors.
 */
GLenum glCheckError_(const char* file, int line);
#define glCheckError() ktp::glCheckError_(__FILE__, __LINE__ - 1)

/**
 * @brief Returns the coordinates for a 3D cube build with triangles.
 * @param size The desired size.
 * @return A std::vector filled with the coords {x, y, z, x, y, ...}
 */
FloatArray cube(GLfloat size = 1.f);

/**
 * @brief A wrapper for an OpenGL shader program.
 */
class ShaderProgram {

 public:

  ShaderProgram() = default;
  ShaderProgram(GLuint id): m_id(id) {}
  /**
   * @return The id of the shader program.
   */
  auto id() const { return m_id; }

  /**
   * @brief Gets the uniform location.
   * @param name The name of the uniform.
   * @return The id of the uniform.
   */
  auto getUniformLocation(const char* name) const { return glGetUniformLocation(m_id, name); }

  /**
   * @brief Sets a boolean uniform. Uses glUniform1i()
   * @param name The name of the uniform.
   * @param value The value to be set.
   */
  void setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name), (int)value);
  }

  /**
   * @brief Sets an int uniform. Uses glUniform1i()
   * @param name The name of the uniform.
   * @param value The value to be set.
   */
  void setInt(const char* name, GLint value) const {
    glUniform1i(glGetUniformLocation(m_id, name), value);
  }

  /**
   * @brief Sets a float uniform. Uses glUniform1f()
   * @param name The name of the uniform.
   * @param value The value to be set.
   */
  void setFloat(const char* name, GLfloat value) const {
    glUniform1f(glGetUniformLocation(m_id, name), value);
  }

  /**
   * @brief Sets a vec4 of floats uniform. Uses glUniform4f()
   * @param name The name of the uniform.
   * @param value A pointer to the values to be set.
   */
  void setFloat4(const char* name, const GLfloat* value) const {
    glUniform4f(glGetUniformLocation(m_id, name), value[0], value[1], value[2], value[3]);
  }

  /**
   * @brief Sets a 2x2 matrix uniform. Uses glUniformMatrix2fv()
   * @param name The name of the uniform.
   * @param value A pointer to the values to be set.
   */
  void setMat2f(const char* name, const GLfloat* value, GLboolean transpose = GL_FALSE) const {
    glUniformMatrix2fv(glGetUniformLocation(m_id, name), 1, transpose, value);
  }

  /**
   * @brief Sets a 3x3 matrix uniform. Uses glUniformMatrix3fv()
   * @param name The name of the uniform.
   * @param value A pointer to the values to be set.
   */
  void setMat3f(const char* name, const GLfloat* value, GLboolean transpose = GL_FALSE) const {
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, transpose, value);
  }

  /**
   * @brief Sets a 4x4 matrix uniform. Uses glUniformMatrix4fv()
   * @param name The name of the uniform.
   * @param value A pointer to the values to be set.
   */
  void setMat4f(const char* name, const GLfloat* value, GLboolean transpose = GL_FALSE) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, transpose, value);
  }

  /**
   * @brief Sets an unsigned int uniform. Uses glUniform1ui()
   * @param name The name of the uniform.
   * @param value The value to be set.
   */
  void setUint(const char* name, GLuint value) const {
    glUniform1ui(glGetUniformLocation(m_id, name), value);
  }

  /**
   * @brief Sets a 2 component vector uniform. Uses glUniform2fv()
   * @param name The name of the uniform.
   * @param value A pointer to the values to be set.
   */
  void setVec2(const char* name, const GLfloat* value) const {
    glUniform2fv(glGetUniformLocation(m_id, name), 1, value);
  }

  /**
   * @brief Sets a 2 component vector uniform. Uses glUniform2f()
   * @param name The name of the uniform.
   * @param x value.
   * @param y value.
   */
  void setVec2(const char* name, GLfloat x, GLfloat y) const {
    glUniform2f(glGetUniformLocation(m_id, name), x, y);
  }

  /**
   * @brief Sets a 3 component vector uniform. Uses glUniform3fv()
   * @param name The name of the uniform.
   * @param value A pointer to the values to be set.
   */
  void setVec3(const char* name, const GLfloat* value) const {
    glUniform3fv(glGetUniformLocation(m_id, name), 1, value);
  }

  /**
   * @brief Sets a 3 component vector uniform. Uses glUniform3f()
   * @param name The name of the uniform.
   * @param x value.
   * @param y value.
   * @param z value.
   */
  void setVec3(const char* name, GLfloat x, GLfloat y, GLfloat z) const {
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
  }

  /**
   * @brief Sets a 4 component vector uniform. Uses glUniform4fv()
   * @param name The name of the uniform.
   * @param value A pointer to the values to be set.
   */
  void setVec4(const char* name, const GLfloat* value) const {
    glUniform4fv(glGetUniformLocation(m_id, name), 1, value);
  }

  /**
   * @brief Sets a 4 component vector uniform. Uses glUniform4f()
   * @param name The name of the uniform.
   * @param x value.
   * @param y value.
   * @param z value.
   * @param w value.
   */
  void setVec4(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const {
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
  }

  /**
   * @brief Activates the shader.
   */
  void use() const { glUseProgram(m_id); }

 private:

  GLuint m_id {};
};

/**
 * @brief A RAII vertex buffer object wrapper.
 */
class VBO {

 public:

  VBO();
  VBO(const VBO& other) = delete;
  VBO(VBO&& other) { *this = std::move(other); }
  ~VBO() { if (m_id) glDeleteBuffers(1, &m_id); }
  VBO& operator=(const VBO& other) = delete;
  VBO& operator=(VBO&& other) {
    if (this != &other) {
      if (m_id) glDeleteBuffers(1, &m_id);
      m_id = std::exchange(other.m_id, 0);
    }
    return *this;
  }

  /**
   * @brief Binds the VBO.
   */
  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

  /**
   * @brief Sets up the data for the buffer.
   * @param vertices A pointer to an array of floats to use as data.
   * @param size The size in bytes of the data.
   * @param usage The usage type, default GL_STATIC_DRAW.
   */
  void setup(const GLfloat* vertices, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

  /**
   * @brief Sets up the data for the buffer.
   * @tparam T Things stored *contiguously* in memory.
   * @param vertices A pointer to an array of Ts to use as data.
   * @param size The size in bytes of the data.
   * @param usage The usage type, default GL_STATIC_DRAW.
   */
  template <typename T>
  void setup(const T* vertices, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
    glCheckError();
  }

  /**
   * @brief Sets up the data for the buffer.
   * @tparam T You might want this to some numeric, vector, etc type for good result,
   * @param vertices A std::vector of something to use as data.
   * @param usage The usage type, default GL_STATIC_DRAW.
   */
  template <typename T>
  void setup(const std::vector<T>& vertices, GLenum usage = GL_STATIC_DRAW) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), usage);
    glCheckError();
  }

  /**
   * @brief Sets up the data for the buffer avoiding the cost of reallocating
   *  the data store.
   * @tparam T Things stored *contiguously* in memory.
   * @param vertices A pointer to an array of Ts to use as data.
   * @param size The size in bytes of the data.
   * @param offset The offset into the buffer object's data store where data
   *  replacement will begin, measured in bytes.
   */
  template <typename T>
  void setupSubData(const T* vertices, GLsizeiptr size, GLintptr offset = 0) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
    glCheckError();
  }

  /**
   * @brief Sets up the data for the buffer avoiding the cost of reallocating
   *  the data store.
   * @tparam T You might want this to some numeric, vector, etc type for good result,
   * @param vertices A std::vector of floats to use as data.
   * @param offset The offset into the buffer object's data store where data
   *  replacement will begin, measured in bytes.
   */
  template <typename T>
  void setupSubData(const std::vector<T>& vertices, GLintptr offset = 0) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(T), vertices.data());
    glCheckError();
  }

  /**
   * @brief Unbinds the VBO.
   */
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

 private:

  GLuint m_id {};
};

/**
 * @brief A RAII element buffer object wrapper.
 */
class EBO {

 public:

  EBO();
  EBO(const EBO& other) = delete;
  EBO(EBO&& other) { *this = std::move(other); }
  ~EBO() { if (m_id) glDeleteBuffers(1, &m_id); }
  EBO& operator=(const EBO& other) = delete;
  EBO& operator=(EBO&& other) {
    if (this != &other) {
      if (m_id) glDeleteBuffers(1, &m_id);
      m_id = std::exchange(other.m_id, 0);
    }
    return *this;
  }

  /**
   * @brief Generates an EBO by removing duplicate coordinates and creating an index array.
   * @param vertices Vector of vertices.
   * @param indices Vector of indices.
   */
  static void generateEBO(FloatArray& vertices, UintArray& indices);

  /**
   * @brief Binds the EBO.
   */
  void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

  /**
   * @brief Sets up the data for the buffer.
   * @param vertices A std::vector of uints to use as data.
   */
  void setup(const UintArray& indices, GLenum usage = GL_STATIC_DRAW);

  /**
   * @brief Sets up the data for the buffer.
   * @param indices A pointer to an array of unsigned ints to use as data.
   * @param size The size in bytes of the data.
   */
  void setup(const GLuint* indices, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

  /**
   * @brief Unbinds the EBO.
   */
  void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

 private:

  GLuint m_id {};
};

/**
 * @brief A RAII vertex array object wrapper.
 */
class VAO {

 public:

  VAO();
  VAO(const VAO& other) = delete;
  VAO(VAO&& other) { *this = std::move(other); }
  ~VAO() { if (m_id) glDeleteVertexArrays(1, &m_id); }
  VAO& operator=(const VAO& other) = delete;
  VAO& operator=(VAO&& other) {
    if (this != &other) {
      if (m_id) glDeleteVertexArrays(1, &m_id);
      m_id = std::exchange(other.m_id, 0);
    }
    return *this;
  }

  /**
   * @brief Binds the VAO.
   */
  void bind() const { glBindVertexArray(m_id); }

  /**
   * @brief Specifies how OpenGL should interpret the vertex buffer data whenever a draw call is made.
   * @param vbo The vertex buffer object to be binded.
   * @param layout Specifies the index of the generic vertex attribute to be modified. Must match the layout in the shader.
   * @param components Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
   * @param type Type of the data.
   * @param stride Specifies the byte offset between consecutive generic vertex attributes.
   * @param offset Specifies a offset of the first component of the first generic vertex attribute in the array in the data store.
   * @param normalize Specifies whether fixed-point data values should be normalized.
   */
  void linkAttrib(const VBO& vbo, GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void* offset, GLboolean normalize = GL_FALSE) const;

  /**
   * @brief Specifies how OpenGL should interpret the vertex buffer data whenever a draw call is made. IT DOESN'T BIND ANYTHING!
   * @param vbo The vertex buffer object to be binded.
   * @param layout Specifies the index of the generic vertex attribute to be modified. Must match the layout in the shader.
   * @param components Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
   * @param type Type of the data.
   * @param stride Specifies the byte offset between consecutive generic vertex attributes.
   * @param offset Specifies a offset of the first component of the first generic vertex attribute in the array in the data store.
   * @param normalize Specifies whether fixed-point data values should be normalized.
   */
  void linkAttribFast(GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void* offset, GLboolean normalize = GL_FALSE) const;

  /**
   * @brief Unbinds the VAO.
   */
  void unbind() const { glBindVertexArray(0); }

 private:

  GLuint m_id {};
};

/**
 * @brief A texture wrapper.
 */
class Texture2D {

 public:

  Texture2D() = default;
  Texture2D(GLuint id): m_id(id) {}

  /**
   * @brief Bind the texture.
   */
  void bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }

  /**
   * @brief Unbinds the texture.
   */
  void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

 private:

  GLuint m_id {};
};

} // namespace ktp

#endif // KETEMINE_SRC_OPENGL_HPP_
