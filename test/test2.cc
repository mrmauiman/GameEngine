/*
 * Copyright 2020 Maui Kelley
 */

#include <cstdlib>
#include <cstdio>

#include "GLFW/glfw3.h"

#include "engine/model.h"
#include "engine/rigid_body.h"
#include "engine/camera.h"

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  // Create window
  GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  // Initialize OpenGL
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);

  // create rigid body and camera
  engine::Model model("data/star.obj");
  engine::RigidBody rigidBody(&model);
  engine::Camera camera(35.0f, 0.1f, 10.0f);
  camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f),   // eye
                glm::vec3(0.0f, 0.0f, 0.0f),   // center
                glm::vec3(0.0f, 1.0f, 0.0f));  // up
  rigidBody.Turn(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  rigidBody.Move(glm::vec3(0.0, 0.0, -1.0));
  rigidBody.Turn(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  // Loop until the user closes the window
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         !glfwWindowShouldClose(window)) {
    // Set the rendering viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Select and setup the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera.MultProjectionMatrix(width, height);

    // Select and setup the view matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    camera.MultViewMatrix();

    // Rotate and draw rigid body
    rigidBody.Turn(10.0f, glm::vec3(0.0f, 1.0f, 0.0f));  // 10 deg around y axis
    rigidBody.Draw();

    // Prepare for next frame
    glPopMatrix();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
