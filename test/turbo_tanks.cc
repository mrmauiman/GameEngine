/*
 * Copyright 2020 Maui Kelley
 */

#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

#include "engine/project.h"
#include "engine/model.h"
#include "engine/camera.h"
#include "engine/light.h"
#include "engine/constants.h"
#include "engine/helper.h"
#include "engine/ui.h"
#include "turbo_tanks/player.h"
#include "turbo_tanks/player_camera.h"
#include "turbo_tanks/player_cannon.h"
#include "turbo_tanks/energy_pickup.h"
#include "turbo_tanks/health_pickup.h"
#include "turbo_tanks/energy_ui.h"
#include "turbo_tanks/health_ui.h"
#include "turbo_tanks/enemy.h"
#include "turbo_tanks/enemy_cannon.h"
#include "glm/vec3.hpp"

#define RENDER_DISTANCE 34

// Sets the input map for turbo_tanks
void SetInputs(engine::Project* turbo_tanks) {
  // Set Button Inputs
  turbo_tanks->button_inputs = {
    {"pause", {
      {ENGINE_GAMEPAD, GLFW_GAMEPAD_BUTTON_START},
      {ENGINE_KEYBOARD, GLFW_KEY_ESCAPE}
    }},
    {"accept", {
      {ENGINE_GAMEPAD, GLFW_GAMEPAD_BUTTON_A},
      {ENGINE_KEYBOARD, GLFW_KEY_SPACE}
    }},
    {"cancel", {
      {ENGINE_GAMEPAD, GLFW_GAMEPAD_BUTTON_B},
      {ENGINE_KEYBOARD, GLFW_KEY_BACKSPACE}
    }},
    {"canon", {
      {ENGINE_GAMEPAD, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER},
      {ENGINE_MOUSE, GLFW_MOUSE_BUTTON_RIGHT}
    }},
    {"machinegun", {
      {ENGINE_AXIS, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER},
      {ENGINE_MOUSE, GLFW_MOUSE_BUTTON_LEFT}
    }},
    {"dev", {
      {ENGINE_KEYBOARD, GLFW_KEY_TAB}
    }}
  };
  // Set Vector Inputs
  turbo_tanks->vector_inputs = {
    {"move", {
      {ENGINE_AXIS, {
        GLFW_GAMEPAD_AXIS_LEFT_X,
        GLFW_GAMEPAD_AXIS_LEFT_Y
      }},
      {ENGINE_KEYBOARD, {
        GLFW_KEY_A,
        GLFW_KEY_D,
        GLFW_KEY_S,
        GLFW_KEY_W
      }}
    }},
    {"aim", {
      {ENGINE_AXIS, {
        GLFW_GAMEPAD_AXIS_RIGHT_X,
        GLFW_GAMEPAD_AXIS_RIGHT_Y
      }},
      {ENGINE_CURSOR, {
        ENGINE_CURSOR_X,
        ENGINE_CURSOR_Y
      }}
    }}
  };
}

int LoadLevel(std::string filename, engine::Project* turbo_tanks) {
  float w, h;
  // Load Models
  engine::Model* tank_md = new engine::Model("data/tank.obj");
  engine::Model* cannon_md = new engine::Model("data/cannon.obj");
  engine::Model* piller_md = new engine::Model("data/piller.obj");
  engine::Model* floor_md = new engine::Model("data/floor.obj");
  engine::Model* energyball_md = new engine::Model("data/energy_ball.obj");
  engine::Model* battery_md = new engine::Model("data/battery.obj");
  engine::Model* heart_md = new engine::Model("data/heart.obj");
  engine::Model* enemy_md = new engine::Model("data/enemytank.obj");
  turbotanks::Player* player = new turbotanks::Player(tank_md);
  turbo_tanks->AddRigidBody(player);
  // Load level file
  std::vector<GLubyte> level = engine::LoadPPM(filename, &w, &h);
  // Create a floor that spans entire level
  engine::RigidBody* floor = new engine::RigidBody(floor_md);
  floor->SetScale(glm::vec3(w, 1, h));
  floor->SetBoundingBox(glm::vec3(0, -1, 0), glm::vec3(w, 0, h));
  floor->tags.push_back("floor");
  turbo_tanks->AddRigidBody(floor);
  // Load level
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < h; y++) {
      int index = (y*w*COLOR_SIZE)+(x*COLOR_SIZE);
      glm::vec3 color(static_cast<float>(level[index]),
      static_cast<float>(level[index+1]), static_cast<float>(level[index+2]));
      if (color == glm::vec3(0, 0, RGB_MAX)) {  // player
        // RigidBodies
        turbotanks::PlayerCannon* player_cannon =
        new turbotanks::PlayerCannon(cannon_md, energyball_md);

        // Cameras
        turbotanks::PlayerCamera* camera =
        new turbotanks::PlayerCamera(45, 1.0f, 100);
        engine::Camera* dev_cam = new engine::Camera(45, 0.1f, 100);

        // Set Links
        player_cannon->player = player;
        player_cannon->camera = camera;
        camera->player = player;
        camera->dev_cam = dev_cam;

        // Add to project
        turbo_tanks->AddCamera(camera);
        turbo_tanks->AddCamera(dev_cam);
        turbo_tanks->AddRigidBody(player_cannon);

        // Place in world
        player->SetPosition(x, 0.7, y);
        player->SetOrientation(0, glm::vec3(0, 0, -1));
      } else if (color == glm::vec3(0, 0, 0)) {  // wall
        engine::RigidBody* wall = new engine::RigidBody(piller_md);
        wall->SetPosition(x, 0, y);
        wall->tags.push_back("wall");
        turbo_tanks->AddRigidBody(wall);
      } else if (color == glm::vec3(RGB_MAX, 0, RGB_MAX)) {
        turbotanks::EnergyPickup* b = new turbotanks::EnergyPickup(battery_md);
        b->SetPosition(x, 0, y);
        b->player = player;
        turbo_tanks->AddRigidBody(b);
      } else if (color == glm::vec3(RGB_MAX/2, 0, RGB_MAX)) {
        turbotanks::HealthPickup* h = new turbotanks::HealthPickup(heart_md);
        h->SetPosition(x, 0, y);
        h->player = player;
        turbo_tanks->AddRigidBody(h);
      } else if (color == glm::vec3(RGB_MAX, 0, 0)) {
        turbotanks::EnemyCannon* e_cannon =
        new turbotanks::EnemyCannon(cannon_md, energyball_md);
        turbotanks::Enemy* enemy = new turbotanks::Enemy(enemy_md);
        enemy->SetPosition(x, 0.7, y);
        enemy->player = player;
        enemy->cannon = e_cannon;
        e_cannon->player = player;
        e_cannon->enemy = enemy;
        turbo_tanks->AddRigidBody(enemy);
        turbo_tanks->AddRigidBody(e_cannon);
      }
    }
  }
  return player->id;
}

int main() {
  // Create Project
  engine::Project turbo_tanks("Turbo Tanks");
  turbo_tanks.Initialize();
  turbo_tanks.render_distance = RENDER_DISTANCE;
  turbo_tanks.collision_radius = 3;

  turbo_tanks.AddScene("menu");
  turbo_tanks.AddScene("level_1");
  turbo_tanks.SetCurrentScene("menu");
  // Add menu objects

  turbo_tanks.SetCurrentScene("level_1");

  // Set Input Map
  SetInputs(&turbo_tanks);

  // Load level 1
  int p_id = LoadLevel("data/level1.ppm", &turbo_tanks);
  turbotanks::Player* player =
  dynamic_cast<turbotanks::Player*>(turbo_tanks.GetObject(p_id));
  if (!player) {
    std::cout << "p_id is not a player" << std::endl;
    return 0;
  }
  // turbo_tanks.PrintGameObjects();

  // UI
  engine::UI* reticle = new engine::UI("data/reticle.pam");
  reticle->SetAttributes(1.0f/10.0f, 1, UI_FIX_WIDTH, UI_CENTER_CENTER);
  reticle->SetPosition(0.5f, 0.5f, -1.0f);
  turbo_tanks.AddUI(reticle);

  float sep = (1/32.0f);
  float width = (1/5.0f);
  float magic_offset = (47.0f/267.0f)*width;
  float magic_offset_y = (30.0f/86.0f)*(width*(86.0f/267.0f));
  float magic_num = (54.0f/267.0f)*width;
  float x = 1.0f - (width+(2*sep));

  turbotanks::HealthUI* hp = new turbotanks::HealthUI("data/health.ppm");
  hp->SetAttributes(width-(magic_num), 1, UI_FIX_WIDTH,
  UI_LEFT_BOTTOM);
  hp->SetPosition((x+magic_offset)-width, sep+magic_offset_y, -2);
  hp->player = player;
  turbo_tanks.AddUI(hp);

  engine::UI* hp_ui = new engine::UI("data/health_ui.pam");
  hp_ui->SetAttributes(width, 1, UI_FIX_WIDTH, UI_RIGHT_BOTTOM);
  hp_ui->SetPosition(x, sep, -1);
  turbo_tanks.AddUI(hp_ui);

  x = 1.0f - sep;
  turbotanks::EnergyUI* e = new turbotanks::EnergyUI("data/energy.ppm");
  e->SetAttributes(width-(magic_num), 1, UI_FIX_WIDTH,
  UI_LEFT_BOTTOM);
  e->SetPosition((x+magic_offset)-width, sep+magic_offset_y, -2);
  e->player = player;
  turbo_tanks.AddUI(e);

  engine::UI* e_ui = new engine::UI("data/energy_ui.pam");
  e_ui->SetAttributes(width, 1, UI_FIX_WIDTH, UI_RIGHT_BOTTOM);
  e_ui->SetPosition(x, sep, -1);
  turbo_tanks.AddUI(e_ui);

  // Light
  engine::Light light9;
  light9.LookAt(glm::vec3(0.0f, 10.0f, 10.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

  // turbo_tanks.SetCurrentScene("menu");

  turbo_tanks.GameLoop();
  return 0;
}
