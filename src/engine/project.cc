/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/project.h"

namespace engine {

// type is a type_index and types is a list of type_indexs
// returns if type is in types
bool Project::ShouldIgnore(GameObject* obj, std::vector<std::string> tags) {
  bool rv = true;
  if (WithInRender(obj->GetPosition()) || obj->HasTag("floor")) {
    rv = false;
    for (int i = 0; i < tags.size() && !rv; i++) {
      if (obj->HasTag(tags[i])) {
        rv = true;
      }
    }
  }
  return rv;
}

// Run the trash collector
void Project::TrashCollector() {
  // Trash Collection
  for (int i = 0; i < trashcan.size(); i++) {
    GameObject* to_delete = objects[trashcan[i]];
    // std::cout << *to_delete << std::endl;
    objects.erase(trashcan[i]);
    std::vector<int>::iterator it;
    it = std::find(rigidbodies.begin(), rigidbodies.end(), trashcan[i]);
    if (it != rigidbodies.end()) rigidbodies.erase(it);
    it = std::find(cameras.begin(), cameras.end(), trashcan[i]);
    if (it != cameras.end()) cameras.erase(it);
    it = std::find(uis.begin(), uis.end(), trashcan[i]);
    if (it != uis.end()) uis.erase(it);
    delete to_delete;
  }
  trashcan.clear();
}

// obj is the point we are looking for
// returns true is obj is in the render box
bool Project::WithInRender(glm::vec3 obj) {
  PointInBox(obj, center, render_distance);
}

// Default Constructor
Project::Project() {
  deadzone = ENGINE_DEAD_ZONE;
  mouse_sensitivity = ENGINE_MOUSE_SENSITIVITY;
  current_id = 0;
}

// Constructor
// pname is the name to assign this project
// name member data is assigned pname
Project::Project(std::string pname) {
  name = pname;
  deadzone = ENGINE_DEAD_ZONE;
  mouse_sensitivity = ENGINE_MOUSE_SENSITIVITY;
  current_id = 0;
}

// initializes glwf and openGL for drawing
int Project::Initialize() {
  // Initialize the library
  if (!glfwInit()) {
    return -1;
  }
  // Initialize Window
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                        name.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Set The Clear Color (Sky Box)
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Set Shade Model
  glShadeModel(GL_SMOOTH);

  // Set Starting Cursor Position
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  previos_cursor_position = glm::vec2(xpos, ypos);

  // start the ticker
  ticks = 0;
  delta = 1.0f/60.0f;

  return 0;
}

// runs the update and draw functions for all game objects
void Project::GameLoop() {
  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    time_t start_time = time(NULL);
    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Create The Camera Frustum
    // Enable Depth
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    for (int i = 0; i < cameras.size(); i++) {
      Camera* camera = dynamic_cast<Camera*>(objects[cameras[i]]);
      if (camera) {
        if (camera->enabled) {
          camera->MultProjectionMatrix(width, height);
          center = camera->GetPosition();
        }
      }
    }

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the rigid bodies
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      // Transform The Camera
      for (int i = 0; i < cameras.size(); i++) {
        Camera* camera = dynamic_cast<Camera*>(objects[cameras[i]]);
        if (camera) {
          camera->Update(delta);
          if (camera->enabled) {
            camera->MultViewMatrix();
          }
        }
      }
      glPushMatrix();
        // std::cout << *objects[rigidbodies[257]] << std::endl;
        // Draw/Update Rigid Bodies
        for (int i = 0; i < rigidbodies.size(); i++) {
          // if (WithInRender(rigidbodies[i]->GetPosition())) {
            RigidBody* rb = dynamic_cast<RigidBody*>(objects[rigidbodies[i]]);
            if (rb) {
              rb->Update(delta);
              // std::cout << i+1 << "/" << rigidbodies.size() << std::endl;
              rb->Draw();
            }
          // }
        }
      glPopMatrix();
    glPopMatrix();

    // glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = width/static_cast<float>(height);
    glOrtho(-ratio, ratio, -1, 1, 0, render_distance);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      // Draw UI
      for (int i = 0; i < uis.size(); i++) {
        UI* ui = dynamic_cast<UI*>(objects[uis[i]]);
        if (ui) {
          ui->SetScreenRatio(ratio);
          ui->Update(delta);
          ui->Draw();
        }
      }
    glPopMatrix();

    TrashCollector();

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glm::vec2 cursor_position(xpos, ypos);
    previos_cursor_position = cursor_position;

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();

    // Make the game run a 60FPS
    ticks++;
    time_t end_time = time(NULL);
    if (end_time != start_time) {
      delta = 1.0f/ticks;
      ticks = 0;
    }
  }
}

// camera is a pointer to a Camera object
// adds camera to cameras and returns its index
int Project::AddCamera(Camera* camera) {
  cameras.push_back(current_id);
  objects[current_id] = camera;
  camera->project = this;
  camera->id = current_id;
  current_id++;
  return camera->id;
}

// id is an id in objects
// if id is a camera is activates it
void Project::ActivateCamera(int id) {
  Camera* new_cam = dynamic_cast<Camera*>(objects[id]);
  if (new_cam) {
    // disable all cameras
    for (int i = 0; i < cameras.size(); i++) {
      Camera* cam = dynamic_cast<Camera*>(objects[cameras[i]]);
      cam->enabled = false;
    }
    // enable the new camera
    new_cam->enabled = true;
  }
}

// model is a pointer to a Model
// create a new rigidbody to rigidbodies with model as its model
// and return its index
int Project::AddRigidBody(RigidBody* rigidbody) {
  rigidbodies.push_back(current_id);
  objects[current_id] = rigidbody;
  rigidbody->project = this;
  rigidbody->id = current_id;
  current_id++;
  return rigidbody->id;
}

// ui is a pointer to a UI
// adds ui to objects and puts its id in uis
int Project::AddUI(GameObject* ui) {
  uis.push_back(current_id);
  objects[current_id] = ui;
  ui->project = this;
  ui->id = current_id;
  current_id++;
  return ui->id;
}

// index is a position in rigidbodies
// returns a pointer to the rigidbody at index in rigidbodies
GameObject * Project::GetObject(int index) {
  return objects[index];
}

// input is the name of the input in vector_inputs
// returns a vec2 with a maximum magnitude of 1
glm::vec2 Project::GetVectorInput(std::string input) {
  glm::vec2 rv(0, 0);
  GLFWgamepadstate gp_state;
  glfwGetGamepadState(GLFW_JOYSTICK_1, &gp_state);
  for (auto const& in : vector_inputs[input]) {
    switch (in.first) {
      case ENGINE_GAMEPAD:
        rv.x += static_cast<int>(gp_state.buttons[in.second[1]] == GLFW_PRESS) -
                static_cast<int>(gp_state.buttons[in.second[0]] == GLFW_PRESS);
        rv.y += static_cast<int>(gp_state.buttons[in.second[3]] == GLFW_PRESS) -
                static_cast<int>(gp_state.buttons[in.second[2]] == GLFW_PRESS);
        break;
      case ENGINE_KEYBOARD:
        rv.x +=
        static_cast<int>(glfwGetKey(window, in.second[1]) == GLFW_PRESS) -
        static_cast<int>(glfwGetKey(window, in.second[0]) == GLFW_PRESS);
        rv.y +=
        static_cast<int>(glfwGetKey(window, in.second[3]) == GLFW_PRESS) -
        static_cast<int>(glfwGetKey(window, in.second[2]) == GLFW_PRESS);
        break;
      case ENGINE_MOUSE:
        rv.x +=
        static_cast<int>
        (glfwGetMouseButton(window, in.second[1]) == GLFW_PRESS) -
        static_cast<int>
        (glfwGetMouseButton(window, in.second[0]) == GLFW_PRESS);
        rv.y +=
        static_cast<int>
        (glfwGetMouseButton(window, in.second[3]) == GLFW_PRESS) -
        static_cast<int>
        (glfwGetMouseButton(window, in.second[2]) == GLFW_PRESS);
        break;
      case ENGINE_AXIS:
        rv.x += gp_state.axes[in.second[0]];
        rv.y -= gp_state.axes[in.second[1]];
        break;
      case ENGINE_CURSOR:
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 cursor_position(xpos, ypos);
        glm::vec2 cursor_offset = cursor_position - previos_cursor_position;
        cursor_offset = cursor_offset * (1.0f/mouse_sensitivity);
        rv.x += cursor_offset[in.second[0]];
        rv.y += cursor_offset[in.second[1]];
        break;
    }
  }
  if (glm::length(rv) > 1) {
    rv = glm::normalize(rv);
  } else if (glm::length(rv) < deadzone) {
    rv = glm::vec2(0, 0);
  }
  return rv;
}

// input is the name of the input in button_inputs
// returns a bool whether that button is being pressed
bool Project::GetButtonInput(std::string input) {
  bool rv = false;
  GLFWgamepadstate gp_state;
  glfwGetGamepadState(GLFW_JOYSTICK_1, &gp_state);
  for (auto const& in : button_inputs[input]) {
    switch (in.first) {
      case ENGINE_AXIS:
        rv = (rv || (gp_state.axes[in.second] > 0));
        break;
      case ENGINE_MOUSE:
        rv = (rv || (glfwGetMouseButton(window, in.second) == GLFW_PRESS));
        break;
      case ENGINE_GAMEPAD:
        rv = (rv || (gp_state.buttons[in.second] == GLFW_PRESS));
        break;
      case ENGINE_KEYBOARD:
        rv = (rv || (glfwGetKey(window, in.second) == GLFW_PRESS));
        break;
    }
  }
  return rv;
}

// start and end are positions in 3d space and ignore is a rigidbody id
// to ignore
// returns the position of the first intersection along the line from start to
// end of all rigidbodies and -1 if there are no intersections
float Project::RayCast(glm::vec3 start, glm::vec3 end,
std::vector<std::string> ignore) {
  float rv = -1;
  if (rigidbodies.size() > 0) {
    for (int i = 0; i < rigidbodies.size(); i++) {
      if (!ShouldIgnore(objects[rigidbodies[i]], ignore)) {
        RigidBody* rb = dynamic_cast<RigidBody*>(objects[rigidbodies[i]]);
        if (rb) {
          float temp = rb->RayCast(start, end);
          if (temp != -1) {
            if (rv == -1 || temp < rv) {
              rv = temp;
            }
          }
        }
      }
    }
  }
  return rv;
}

// id is an index in rigidbodies, and ignore is a list of indices to ignore
// returns if the rigidbody is colliding with another
int Project::Collides(int id, std::vector<std::string> ignore) {
  int rv = -1;
  // std::cout << rv << std::endl;
  GameObject* me = objects[id];
  for (int i = 0; i < rigidbodies.size() && rv == -1; i++) {
    if (!ShouldIgnore(objects[rigidbodies[i]], ignore)
    && rigidbodies[i] != id && PointInBox(objects[rigidbodies[i]]->GetPosition()
    , me->GetPosition(), collision_radius)) {
      GameObject* other = objects[rigidbodies[i]];
      if (me->Intersects(*other)) {
        rv = other->id;
        // std::cout << *me << " Collided with " << *other << std::endl;
        // std::cout << ShouldIgnore(other, ignore) << std::endl;
      }
    }
  }
  return rv;
}

// id is an index in rigidbodies
// removes that rigidbody from existance
void Project::RemoveRigidBody(int id) {
  trashcan.push_back(id);
}

// id is an index in rigidbodies
// removes that rigidbody from existance
void Project::RemoveCamera(int id) {
  trashcan.push_back(id);
}

// Prints a readable list of all game objects
void Project::PrintGameObjects() {
  for (auto const& object : objects) {
    std::cout << *object.second << std::endl;
  }
}

// Deconstructor
// Cleans up glfw/openGL
Project::~Project() {
  // Delete all objects
  for (auto const& obj : objects) {
    trashcan.push_back(obj.first);
  }
  TrashCollector();
  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
}

}  // namespace engine
