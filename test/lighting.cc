/*
 * Copyright 2020 Maui Kelley
 */

#include <iostream>

#include "glm/vec3.hpp"
#include "engine/model.h"
#include "engine/camera.h"
#include "engine/rigid_body.h"
#include "engine/light.h"
#include "engine/helper.h"

int main(int argc, char **argv) {
  // Initialize the library
  if (!glfwInit()) {
    return -1;
  }

  // Create a windowed mode window and its OpenGL context
  GLFWwindow* window = glfwCreateWindow(640, 640, "Island Engine", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Load box1 Model
  std::string obj_file_name = "data/cube.obj";
  if (argc > 1) {
    obj_file_name = argv[1];
  }
  engine::Model md(obj_file_name);

  // Initialize necessary variables
  engine::RigidBody box1(&md);
  engine::RigidBody box2(&md);
  engine::Camera camera(45, 0.1, 100, false);

  camera.LookAt(glm::vec3(0.0, 0.0, 25.0),     // EYE
                glm::vec3(0.0f, 0.0f, 0.0f),  // CENTER
                glm::vec3(0.0f, 1.0f, 0.0f));  // UP
  box1.LookAt(glm::vec3(-2.0f, -2.0f, 0.0f),
              glm::vec3(2.0f, 2.0f, 0.0f),
              glm::vec3(0.0f, 1.0f, 0.0f));
  box2.LookAt(glm::vec3(2.0f, 2.0f, 0.0f),
              glm::vec3(-2.0f, -2.0f, 0.0f),
              glm::vec3(0.0f, 1.0f, 0.0f));
  engine::PrintVec3(box2.GetPosition());

  if (box1.Intersects(box2)) {
    std::cout << "True" << std::endl;
  } else {
    std::cout << "False" << std::endl;
  }

  // Set The Clear Color (Sky Box)
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Enable Depth
  glEnable(GL_DEPTH_TEST);

  // This should work
  engine::Light light9;
  light9.LookAt(glm::vec3(0.0f, 1.0f, 2.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
  float speed = -0.01f;

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // light
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 90.0 };
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Create The Camera Frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera.MultProjectionMatrix(width, height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the rigid bodies
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      // Draw UI
      // --here--
      // Transform The Camera
      camera.MultViewMatrix();
      // Draw The box1s
      glPushMatrix();
        if (box1.Intersects(box2)) {
          speed = 0.01f;
        }
        box1.Move(glm::vec3(0.0f, 0.0f, speed));
        box2.Move(glm::vec3(0.0f, 0.0f, speed));
        box1.Turn(-1.0, glm::vec3(0.0, 0.0, 1.0), false);
        box2.Turn(-1.0, glm::vec3(0.0, 0.0, 1.0), false);
        box1.Draw();
        box2.Draw();
      glPopMatrix();
    glPopMatrix();

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
