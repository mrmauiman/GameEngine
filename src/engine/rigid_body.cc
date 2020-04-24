/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/rigid_body.h"

namespace engine {

// model is a pointer to a model
RigidBody::RigidBody(const Model *model) {
  this->model = model;
  SetBoundingBox(model->GetBoundMin(), model->GetBoundMax());
  tags.push_back("rigidbody");
}

// draws the rigid body’s model with it’s current position and orientation.
// make sure the matrix mode is GL_MODELVIEW
void RigidBody::Draw() const {
  // Covert the quaternion into something readable for glfw
  glm::mat4 rot_mat = glm::toMat4(orientation);
  glm::mat4 anim_rot_mat = glm::toMat4(animation_controller.GetOrientation());
  glm::vec3 anim_pos = animation_controller.GetPosition();
  glm::vec3 anim_scale = animation_controller.GetScale();

  glPushMatrix();
    // Apply Transformations
    glTranslatef(position.x, position.y, position.z);
    glTranslatef(anim_pos.x, anim_pos.y, anim_pos.z);
    glMultMatrixf(glm::value_ptr(rot_mat));
    glMultMatrixf(glm::value_ptr(anim_rot_mat));
    glScalef(scale.x, scale.y, scale.z);
    glScalef(anim_scale.x, anim_scale.y, anim_scale.z);

    // Set the color matrix based on color of rigid_body
    // std::vector<GLfloat> colors;
    // for (int i = 0; i < model->GetNumVerticies(); i++) {
    //   colors.push_back(color.r);
    //   colors.push_back(color.g);
    //   colors.push_back(color.b);
    //   colors.push_back(color.a);
    // }
    // glEnableClientState(GL_COLOR_ARRAY);
    // glColorPointer(COLOR_SIZE, GL_FLOAT,  0, colors.data());

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
  animation_controller.Update(delta);
}

}  // namespace engine
