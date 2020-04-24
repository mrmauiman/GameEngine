/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/ui.h"

namespace engine {

UI::UI() {
  Initialize();
}
UI::UI(const std::string &image_file) {
  Initialize();
  Load(image_file);
}

// called by all constructors
void UI::Initialize() {
  tags.push_back("ui");
  screen_ratio = 1.0f;
}

void UI::Load(const std::string &image_file) {
  image.Load(image_file);
}

void UI::Draw() const {
  // Covert the quaternion into something readable for glfw
  glm::mat4 rot_mat = glm::toMat4(orientation);
  glm::vec3 pos = GetScreenPosition();

  glPushMatrix();
    // Apply Transformations
    glTranslatef(pos.x, pos.y, pos.z);
    glMultMatrixf(glm::value_ptr(rot_mat));
    glScalef(scale.x, scale.y, scale.z);

    // Draw the model
    image.Draw();
  glPopMatrix();
}

}  // namespace engine
