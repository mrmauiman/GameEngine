#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../src/model.h" //Ask how to get #include "model.h" to work

int main(int argc, char **argv) {

  //Seed the random
  srand(time(NULL));

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
  std::string obj_file_name = "data/star.obj";
  if (argc > 1) {
    obj_file_name = argv[1];
  }
  engine::Model md(obj_file_name);

  obj_file_name = "data/cube.obj";
  engine::Model fl(obj_file_name);

  md.setColor(1.0f, 0.0f, 0.0f, 1.0f);

  int plane_size = 50;
  int num_stars = 20;
  std::vector<GLfloat> positions;
  for (int i = 0; i < num_stars; i++) {
    GLfloat p1 = (rand()%plane_size)-(plane_size/2);
    GLfloat p2 = -1*((rand()%plane_size)+1);
    positions.push_back(p1);
    positions.push_back(p2);
  }

  std::vector<std::vector<GLfloat>> fl_positions;
  for(int i = 0; i < plane_size; i++){
    std::vector<GLfloat> line;
    for(int j = 0; j < plane_size; j++){
      int x = i - (plane_size/2);
      int z = -1 * j;
      line.push_back(x);
      line.push_back(z);
    }
    fl_positions.push_back(line);
  }

  GLfloat camera_x = 0;
  GLfloat camera_z = 0;
  GLfloat movespeed = 0.4f;


  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  // Loop until the user closes the window
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {

    //Inputs
    int h_input = glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D);
    int v_input = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);

    camera_x += h_input * movespeed;
    camera_z += v_input * movespeed;

    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    float ratio = width/(float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ratio, ratio, -1.0, 1.0, 1, 100);

    //Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
      glTranslatef(camera_x, 0.0f, camera_z);
      //Create Matrix for star
      for(int i = 0; i < num_stars; i++){
        glPushMatrix();
          glTranslatef(positions[i*2], 0.0f, positions[(i*2)+1]);
          md.draw();
        glPopMatrix();
      }
      //Create Floor
      for (int i = 0; i < plane_size; i++){
        for (int j = 0; j < plane_size; j++){
          glPushMatrix();

            if (i%2 == 0) {
              if (j%2 == 0) {
                fl.setColor(0.2f, 0.2f, 0.2f, 1.0f);
              }else{
                fl.setColor(0.5f, 0.5f, 0.5f, 1.0f);
              }
            } else{
              if (j%2 == 0) {
                fl.setColor(0.5f, 0.5f, 0.5f, 1.0f);
              }else{
                fl.setColor(0.2f, 0.2f, 0.2f, 1.0f);
              }
            }
            glTranslatef(fl_positions[i][j*2], -1.5f, fl_positions[i][(j*2)+1]);
            glScalef(1.0f, 0.1f, 1.0f);
            fl.draw();
          glPopMatrix();
        }
      }
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
