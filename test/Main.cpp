#include <iostream>
#include "../src/Model.h"

using namespace std;

int main(int argc, char **argv){

  // Initialize the library
  if (!glfwInit()) {
    return -1;
  }

  // Create a windowed mode window and its OpenGL context
  GLFWwindow* window = glfwCreateWindow(640, 640, "IslandEngine", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  //Make star
  string obj_file_name = "../data/star.obj";
  if(argc > 1){
    obj_file_name = argv[1];
  }
  Model md(obj_file_name);
  cout << md << endl;

  int rotation = 0;
  int rotation_speed = 5;


  // Loop until the user closes the window
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {

    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //Clear the color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Apply transformations
    glLoadIdentity();
    rotation += rotation_speed;
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.75f, 0.0f, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(0.25f, 0.25f, 0.25f);

    //Draw the star
    md.draw();

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
