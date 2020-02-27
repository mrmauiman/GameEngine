/*
 * Copyright 2020 Maui Kelley
 */

#include <iostream>
#include "engine/model.h"

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

  // Make star
  std::string obj_file_name = "data/star.obj";
  if (argc > 1) {
    obj_file_name = argv[1];
  }
  engine::Model md(obj_file_name);

  int rotation = 0;
  int rotation_speed = 2;

  float dist = -10.1f;
  float dist_speed = -0.1f;

  // Loop until the user closes the window
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         !glfwWindowShouldClose(window)) {
    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    float ratio = width/static_cast<float>(height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ratio, ratio, -1.0, 1.0, 10, 100);

    // Clear the color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    rotation += rotation_speed;
    dist += dist_speed;

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
      glTranslatef(0.0f, 0.0f, dist);
      // Create Matrix for star a
      glPushMatrix();
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glScalef(0.3f, 0.3f, 0.3f);
        md.Draw();
      glPopMatrix();
      // Create Matrix for star b
      glPushMatrix();
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.7f, 0.0f, 0.0f);
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glScalef(0.2f, 0.2f, 0.2f);
        md.Draw();
      glPopMatrix();
      // Create Matrix for star c
      glPushMatrix();
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.7f, 0.0f, 0.0f);
        glRotatef(rotation*2, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.25f, 0.0f, 0.0f);
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glScalef(0.05f, 0.05f, 0.05f);
        md.Draw();
      glPopMatrix();
      // Create Matrix for star d
      glPushMatrix();
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.7f, 0.0f, 0.0f);
        glRotatef(rotation*2, 0.0f, 0.0f, 1.0f);
        glTranslatef(-0.25f, 0.0f, 0.0f);
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glScalef(0.05f, 0.05f, 0.05f);
        md.Draw();
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
