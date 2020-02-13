/*
 * Copyright 2020 Maui Kelley
 */

#include "src/engine/rigidbody.h"

namespace engine {

// model is a pointer to a model
RigidBody::RigidBody(const Model *model) {
  this->model = model;
}

// draws the rigid body’s model with it’s current position and orientation.
// make sure the matrix mode is GL_MODELVIEW
void RigidBody::draw() const {
  // Covert the quaternion into something readable for glfw
  glm::mat4 rot_mat = glm::toMat4(orientation);

  glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glMultMatrixf(glm::value_ptr(rot_mat));
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
