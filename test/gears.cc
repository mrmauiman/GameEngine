#include <iostream>
#include "../src/model.h"

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

  //Make star
  std::string obj_file_name = "data/gear.obj";
  engine::Model g1(obj_file_name);

  std::cout << g1 << std::endl;

  int rotation = 0;
  int rotation_speed = 1;


  // Loop until the user closes the window
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {

    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //Clear the color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    rotation += rotation_speed;

    glMatrixMode(GL_MODELVIEW);

    //Gear 1 Transformations
    g1.setColor(1.0f, 1.0f, 1.0f, 1.0f);
    glLoadIdentity();
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    g1.draw();

    //Gear 2 Transformations
    g1.setColor(1.0f, 0.0f, 0.0f, 1.0f);
    glLoadIdentity();
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glTranslatef(1.2f, 0.0f, 0.0f);
    glRotatef(-rotation, 0.0f, 0.0f, 1.0f);
    g1.draw();

    //Gear 3 Transformations
    g1.setColor(0.0f, 1.0f, 0.0f, 1.0f);
    glLoadIdentity();
    glRotatef(90+120, 0.0f, 0.0f, 1.0f);
    glTranslatef(1.2f, 0.0f, 0.0f);
    glRotatef(-rotation+12, 0.0f, 0.0f, 1.0f);
    g1.draw();

    //Gear 4 Transformations
    g1.setColor(0.0f, 0.0f, 1.0f, 1.0f);
    glLoadIdentity();
    glRotatef(90+240, 0.0f, 0.0f, 1.0f);
    glTranslatef(1.2f, 0.0f, 0.0f);
    glRotatef(-rotation-12, 0.0f, 0.0f, 1.0f);
    g1.draw();

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
