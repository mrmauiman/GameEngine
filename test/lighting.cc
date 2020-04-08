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

  // Load Star Model
  std::string obj_file_name = "data/cube.obj";
  if (argc > 1) {
    obj_file_name = argv[1];
  }
  engine::Model star_md(obj_file_name);

  // Initialize necessary variables
  engine::RigidBody star(&star_md);
  engine::RigidBody cube(&star_md);
  engine::Camera camera(90, 0.1, 100, false);

  camera.LookAt(glm::vec3(0.0, 0.0, 2.0),     // EYE
                glm::vec3(0.0f, 0.0f, 0.0f),  // CENTER
                glm::vec3(0.0f, 1.0f, 0.0f));  // UP
  star.LookAt(glm::vec3(-2.0f, -2.0f, 0.0f),
              glm::vec3(2.0f, 2.0f, 0.0f),
              glm::vec3(0.0f, 1.0f, 0.0f));
  cube.LookAt(glm::vec3(2.0f, 2.0f, 0.0f),
              glm::vec3(-2.0f, -2.0f, 0.0f),
              glm::vec3(0.0f, 1.0f, 0.0f));

  if (star.Intersects(cube)) {
    std::cout << "True" << std::endl;
  } else {
    std::cout << "False" << std::endl;
  }

  // Set The Clear Color (Sky Box)
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Enable Depth
  glEnable(GL_DEPTH_TEST);

  // engine::Light light1;
  // light1.SetPosition(glm::vec3(1.0f, 1.0f, 0.0f));
  // light1.SetActivation(false);
  // engine::Light light2(1.0f, 0.0f, 0.0f);
  // light2.SetPosition(glm::vec3(-1.0f, -1.0f, -2.0f));
  // engine::Light light3(0.0f, 1.0f, 0.0f);
  // light3.SetPosition(glm::vec3(0.0f, 2.0f, -2.0f));
  // engine::Light light4(0.0f, 0.0f, 1.0f);
  // light4.SetPosition(glm::vec3(1.0f, -1.0f, -2.0f));
  // engine::Light light5;
  // engine::Light light6;
  // engine::Light * light8 = new engine::Light;
  // engine::Light light7;
  // light5.SetActivation(false);
  // light6.SetActivation(false);
  // light7.SetActivation(false);
  // light8->SetActivation(false);
  // delete light8;

  // This should work
  engine::Light light9;
  light9.LookAt(glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
  light9.SetSpot(45.0, 1.0);



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

    // Get the mouse position
    double c_pos_x = 0.0;
    double c_pos_y = 0.0;
    glfwGetCursorPos(window, &c_pos_x, &c_pos_y);

    GLfloat x_scaler = c_pos_x/width;
    GLfloat y_scaler = c_pos_y/height;

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the rigid bodies
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      // Transform The Camera
      camera.MultViewMatrix();
      // Draw The Stars
      glPushMatrix();
        // star.Turn(-1, glm::vec3(0.0, 1.0, 0.0), false);
        if (!star.Intersects(cube)) {
          star.Move(glm::vec3(0.0f, 0.0f, -0.01f));
          cube.Move(glm::vec3(0.0f, 0.0f, -0.01f));
        }
        star.Draw();
        cube.Draw();
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
