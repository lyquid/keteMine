/**
 * @file camera.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief An abstract camera class that processes input and calculates the corresponding Euler angles, vectors and matrices for use in OpenGL.
 * @version 0.1
 * @date 2022-11-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_CAMERA_HPP_)
#define KETEMINE_SRC_CAMERA_HPP_

#include "types.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ktp {

enum class CameraMovement {
  Forward,
  Backward,
  Left,
  Right
};

/**
 * @brief An abstract camera class that processes input and calculates the corresponding Euler angles,
 *        vectors and matrices for use in OpenGL.
 */
class Camera {
 public:

  Camera(const Point3D& position = Point3D(0.f, 0.f, 0.f), const Vector3& up = Vector3(0.f, 1.f, 0.f), float yaw = -90.f, float pitch = 0.f);
  Camera (float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

  /**
   * @brief Points the camera to directions.
   * @param x_offset
   * @param y_offset
   * @param constrain_pitch To make sure that when pitch is out of bounds, screen doesn't get flipped.
   */
  void look(float x_offset, float y_offset, bool constrain_pitch = true);

  /**
   * @brief Moves the camera.
   * @param direction Where should the camera go.
   * @param delta_time The delta time!
   */
  void move(CameraMovement direction, float delta_time);

  /**
   * @brief
   * @return auto
   */
  auto projectionMatrix() const { return m_projection; }

  /**
   * @brief Sets the ratio for the perspective projection.
   * @param ratio desired.
   */
  void setRatio(float ratio) { m_ratio = ratio; }

  /**
   * @brief Calculates the view matrix using Euler angles and the glm::lookAt() matrix.
   * @return The view matrix.
   */
  auto viewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }

  /**
   * @brief Increases or decreases the zoom level.
   * @param how_much you want.
   */
  void zoom(float how_much);

 private:

  /**
   * @brief Calculates the front vector from the camera's (updated) Euler angles.
   */
  void updateCameraVectors();

  // camera attributes
  Point3D m_position {0.f, 0.f, 0.f};
  Vector3 m_front {0.f, 0.f, -1.f};
  Vector3 m_up {};
  Vector3 m_right {};
  Vector3 m_world_up {0.f, 1.f, 0.f};
  // Euler angles
  float m_yaw {-90.f};
  float m_pitch {0.f};
  // camera options
  float m_movement_speed {2.5f};
  float m_mouse_sensitivity {0.1f};
  // projection options
  float m_ratio {16.f / 9.f};
  float m_zoom {45.f};
  // projection matrix
  glm::mat4 m_projection {glm::perspective(glm::radians(m_zoom), m_ratio, 0.1f, 100.f)};
};

} // namespace ktp

#endif // KETEMINE_SRC_CAMERA_HPP_
