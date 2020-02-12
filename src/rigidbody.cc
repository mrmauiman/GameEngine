/*
 * Copyright 2020 Maui Kelley
 */

#include "src/rigidbody.h"

namespace engine {

// model is a pointer to a model
RigidBody::RigidBody(const Model *model) {
  this->model = model;
}

// draws the rigid body’s model with it’s current position and orientation.
// make sure the matrix mode is GL_MODELVIEW
void RigidBody::draw() const {
  // Covert the quaternion into something readable for glfw
  float angle = rad2deg(glm::angle(orientation));
  glm::vec3 axis = glm::axis(orientation);

  glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(angle, axis.x, axis.y, axis.z);
    glScalef(scale.x, scale.y, scale.z);
    model->draw();
  glPopMatrix();
}

// delta is the fraction of a second a frame takes
// draws the rigidbody
void RigidBody::update(float delta) {
  this->draw();
}

}  // namespace engine
