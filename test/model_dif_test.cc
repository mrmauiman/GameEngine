/*
 * Copyright 2020 Maui Kelley
 */

#include <iostream>
#include "engine/model.h"
#include "engine/ui_model.h"
#include "engine/rigid_body.h"
#include "engine/ui.h"
#include "engine/project.h"
#include "engine/light.h"

int main(int argc, char **argv) {
  // Initialize the library
  if (!glfwInit()) {
    return -1;
  }
  // Initialize Window
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                        "game", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  // Make the window's context current
  glfwMakeContextCurrent(window);


  // Set The Clear Color (Sky Box)

  // Make star
  engine::UI* ret = new engine::UI("data/reticle.pam");
  ret->SetAttributes(0.5, 1, UI_FIX_WIDTH, UI_CENTER_CENTER);
  ret->SetPosition(0.75, 0.5, 0);

  engine::Light light9;
  light9.LookAt(glm::vec3(0.0f, 10.0f, 10.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

  while (!glfwWindowShouldClose(window)) {
    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    float ratio = width/static_cast<float>(height);

    // Create The Camera Frustum
    // Enable Depth
    // glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1, 1, 0, 10);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
      // Draw UI
      ret->SetScreenRatio(ratio);
      ret->Draw();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }
  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
