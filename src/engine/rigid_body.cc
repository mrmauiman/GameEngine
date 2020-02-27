/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/rigid_body.h"

namespace engine {

// model is a pointer to a model
RigidBody::RigidBody(const Model *model) {
  this->model = model;
}

// draws the rigid body’s model with it’s current position and orientation.
// make sure the matrix mode is GL_MODELVIEW
void RigidBody::Draw() const {
  // Covert the quaternion into something readable for glfw
  glm::mat4 rot_mat = glm::toMat4(orientation);

  glPushMatrix();
    // Apply Transformations
    glTranslatef(position.x, position.y, position.z);
    glMultMatrixf(glm::value_ptr(rot_mat));
    glScalef(scale.x, scale.y, scale.z);

    // Set the color matrix based on color of rigid_body
    std::vector<GLfloat> colors;
    for (int i = 0; i < model->GetNumVerticies(); i++) {
      colors.push_back(color.r);
      colors.push_back(color.g);
      colors.push_back(color.b);
      colors.push_back(color.a);
    }
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(COLOR_SIZE, GL_FLOAT,  0, colors.data());

    // Draw the model
    model->Draw();
  glPopMatrix();
}

// color is a vector of size 4 representing rgba
// sets the color member data to color
void RigidBody::SetColor(glm::vec4 color) {
  this->color = color;
}

// r, g, b, a represent a color
// sets the color member data
void RigidBody::SetColor(float r, float g, float b, float a) {
  glm::vec4 c = {r, g, b, a};
  color = c;
}

// delta is the fraction of a second a frame takes
// draws the rigid_body
void RigidBody::Update(float delta) {
  this->Draw();
}

}  // namespace engine
