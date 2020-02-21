#include <cstdlib>
#include <cstdio>

#include "GLFW/glfw3.h"

#include "src/engine/model.h"

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  // Create a windowed mode window and its OpenGL context
  GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Clear the color buffer to black
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  // Create and load model
  engine::Model model("data/model1.obj");

  // Loop until the user closes the window
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         !glfwWindowShouldClose(window)) {
    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Clear the color and depth buffers before drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Select and setup the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = width / static_cast<float>(height);
    glOrtho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

    // Select and setup the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw model
    model.Draw();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
