#ifndef SRC_ENGINE_PROJECT_H_
#define SRC_ENGINE_PROJECT_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C++/C Standard Libraby
#include <GLFW/glfw3.h>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <typeindex>
// lib
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
// src
#include "engine/camera.h"
#include "engine/rigid_body.h"
#include "engine/model.h"
#include "engine/constants.h"
#include "engine/helper.h"
#include "engine/ui.h"

namespace engine {

class Project {
 private:
  std::string name;
  int current_id;
  std::map<std::string, std::vector<int>> cameras;
  std::map<std::string, std::vector<int>> rigidbodies;
  std::map<std::string, std::vector<int>> uis;
  std::vector<std::string> scenes;
  std::string current_scene;
  std::map<int, GameObject*> objects;
  std::vector<int> trashcan;
  glm::vec3 center;

  GLFWwindow* window;
  float delta;
  int ticks;
  glm::vec2 previos_cursor_position;

  // type is a type_index and types is a list of type_indexs
  // returns if type is in types
  bool ShouldIgnore(GameObject* obj, std::vector<std::string> tags);

  // Run the trash collector
  void TrashCollector();

 public:
  // Inputs
  std::map<std::string, std::map<int, int>> button_inputs;
  std::map<std::string, std::map<int, std::vector<int>>> vector_inputs;
  float deadzone;
  float mouse_sensitivity;
  float render_distance;
  float collision_radius;

  // Default Constructor
  Project();

  // Constructor
  // pname is the name to assign this project
  // name member data is assigned pname
  explicit Project(std::string pname);

  // initializes glwf and openGL for drawing
  int Initialize();

  // runs the update and draw functions for all game objects
  void GameLoop();

  // Cameras
  // fov is in degrees and render_distance is a positve number > 0.1
  // creates a new camera in cameras and returns its index
  int AddCamera(Camera* camera);

  // id is an id in objects
  // if id is a camera is activates it
  void ActivateCamera(int id);

  // rigidbody is a pointer to a RigidBody
  // create a new rigidbody to rigidbodies with model as its model
  // and return its index
  int AddRigidBody(RigidBody* rigidbody);

  // ui is a pointer to a UI
  // adds ui to objects and puts its id in uis
  int AddUI(GameObject* ui);

  // scene is a name of a scene
  // returns whether scene is in scenes
  bool SceneExists(std::string scene);

  // scene is a string
  // adds scene to the scenes and returns whether it was successful
  bool AddScene(std::string scene);

  // scene is a name of a scene
  // sets current scene to scene if it is in scenes
  void SetCurrentScene(std::string scene);

  // index is a position in rigidbodies
  // returns a pointer to the rigidbody at index in rigidbodies
  GameObject * GetObject(int index);

  // input is the name of the input in vector_inputs
  // returns a vec2 with a maximum magnitude of 1
  glm::vec2 GetVectorInput(std::string input);

  // input is the name of the input in button_inputs
  // returns a bool whether that button is being pressed
  bool GetButtonInput(std::string input);

  // start and end are positions in 3d space and ignore is a rigidbody id
  // to ignore
  // returns the position of the first intersection along the line from start to
  // end of a rigidbosy
  float RayCast(glm::vec3 start, glm::vec3 end,
  std::vector<std::string> ignore);

  // id is an index in objects, and ignore is a list of indices to ignore
  // returns if the rigidbody is colliding with another
  int Collides(int id, std::vector<std::string> ignore);

  // id is an index in rigidbodies
  // removes that rigidbody from existance
  void RemoveRigidBody(int id);

  // id is an index in cameras
  // removes that camera from existance
  void RemoveCamera(int id);

  // Prints a readable list of all game objects
  void PrintGameObjects();

  // obj is the point we are looking for
  // returns true is obj is in the render box
  bool WithInRender(glm::vec3 obj);

  // Deconstructor
  // Cleans up glfw/openGL
  ~Project();
};

}  // namespace engine

#endif  // SRC_ENGINE_PROJECT_H_
