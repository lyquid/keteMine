#include "camera.hpp"

ktp::Camera::Camera(const Point3D& position, const Vector3& up, float yaw, float pitch) {
  m_position = position;
  m_world_up = up;
  m_yaw = yaw;
  m_pitch = pitch;
  updateCameraVectors();
  // updateViewMatrix();
}

ktp::Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) {
  m_position = Point3D(pos_x, pos_y, pos_z);
  m_world_up = Vector3(up_x, up_y, up_z);
  m_yaw = yaw;
  m_pitch = pitch;
  updateCameraVectors();
  // updateViewMatrix();
}

void ktp::Camera::look(float x_offset, float y_offset, bool constrain_pitch) {
  m_yaw   += (x_offset * m_mouse_sensitivity);
  m_pitch += (y_offset * m_mouse_sensitivity);
  // Make sure that when pitch is out of bounds, screen doesn't get flipped.
  if (constrain_pitch) {
    if (m_pitch >  89.f) m_pitch =  89.f;
    if (m_pitch < -89.f) m_pitch = -89.f;
  }
  updateCameraVectors();
  // updateViewMatrix();
}

void ktp::Camera::move(CameraMovement direction, float delta_time) {
  const auto velocity {m_movement_speed * delta_time};
  if (direction == CameraMovement::Forward)  m_position += m_front * velocity;
  if (direction == CameraMovement::Backward) m_position -= m_front * velocity;
  if (direction == CameraMovement::Left)     m_position -= m_right * velocity;
  if (direction == CameraMovement::Right)    m_position += m_right * velocity;
  // updateViewMatrix();
}

void ktp::Camera::updateCameraVectors() {
  // Calculate the new front_ vector
  const Vector3 new_front {
    glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch)),  // x
    glm::sin(glm::radians(m_pitch)),                                  // y
    glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch))   // z
  };
  m_front = glm::normalize(new_front);
  // Also re-calculate the right_ and up_ vectors.
  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  m_right = glm::normalize(glm::cross(m_front, m_world_up));
  m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void ktp::Camera::zoom(float how_much) {
  m_zoom -= how_much;
  if (m_zoom < 1.f) m_zoom = 1.f;
  if (m_zoom > 45.f) m_zoom = 45.f;
  m_projection = glm::perspective(glm::radians(m_zoom), m_ratio, 0.1f, 100.f);
}
