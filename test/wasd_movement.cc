#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../lib/glm/gtc/quaternion.hpp"
#include "../lib/glm/vec3.hpp"
#include "../src/model.h" //Ask how to get #include "model.h" to work
#include "../src/constants.h"

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
  float camera_x_angle = 0;
  float camera_y_angle = 0;
  GLfloat movespeed = 0.4f;
  GLfloat view_range = 180.0;


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

    //Get Camera Rotation
    double c_pos_x = 0.0;
    double c_pos_y = 0.0;

    glfwGetCursorPos(window, &c_pos_x, &c_pos_y);

    camera_y_angle = (float)((c_pos_x/width)*view_range) - (view_range/2);
    camera_x_angle = (float)((c_pos_y/height)*view_range) - (view_range/2);

    glm::vec3 y_axis = {0.0f, 1.0f, 0.0f};
    glm::vec3 x_axis = {1.0f, 0.0f, 0.0f};

    glm::quat y_quat = glm::angleAxis((camera_y_angle/180)*PI, y_axis);
    glm::quat x_quat = glm::angleAxis((camera_x_angle/180)*PI, x_axis);

    glm::quat rot = x_quat * y_quat;
    glm::vec3 axis = glm::axis(rot);
    // std::cout << axis.x << ", " << axis.y << ", " << axis.z << ", " << glm::angle(rot) << std::endl;


    float ratio = width/(float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ratio*0.1, ratio*0.1, -0.1, 0.1, 0.1, 100);

    //Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
      glRotatef((glm::angle(rot)/PI)*180, axis.x, axis.y, axis.z);
      glTranslatef(camera_x, 0.0f, camera_z);
      // glRotatef(camera_y_angle, 0.0f, 1.0f, 0.0f);
      // glRotatef(camera_x_angle, 1.0f, 0.0f, 0.0f);
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
