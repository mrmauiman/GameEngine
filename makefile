ifeq ($(OS),Windows_NT)
	CXXFLAGS=-lglfw3dll -lopengl32 -lgdi32 -static-libstdc++ -static-libgcc
	CFLAGS=-std=c++11 -Isrc -Ilib
	MKDIR=md
	RM=rd /s /q
else
	UNAME=$(shell uname)
	ifeq ($(UNAME),Darwin)
		CXXFLAGS=-framework OpenGL -lglfw
		CFLAGS=-Wno-deprecated-declarations -std=c++11 -Isrc -Ilib
	else
		CXXFLAGS=-lglfw -lGL
		CFLAGS=-std=c++11 -Isrc -Ilib
	endif
	MKDIR=mkdir -p
	RM=rm -fr
endif

tests := $(patsubst test/%.cc,bin/%,$(wildcard test/turbo_tanks.cc))

all: test

build:
	$(MKDIR) build

bin:
	$(MKDIR) bin

test: $(tests)

bin/%: build/%.o build/model.o build/game_object.o build/camera.o build/rigid_body.o build/helper.o build/light.o build/material.o build/project.o build/ui_model.o build/ui.o build/animation_controller.o build/animation.o build/player.o build/player_camera.o build/player_cannon.o build/energy_ball.o build/collectable.o | bin
	g++ $< build/model.o build/game_object.o build/camera.o build/rigid_body.o build/helper.o build/light.o build/material.o build/project.o build/ui_model.o build/ui.o build/animation_controller.o build/animation.o build/player.o build/player_camera.o build/player_cannon.o build/energy_ball.o build/collectable.o -o $@ $(CXXFLAGS)

build/%.o: test/%.cc | build
	g++ -c $< -o $@ $(CFLAGS)

build/model.o: src/engine/model.cc src/engine/model.h src/engine/material.h | build
	g++ -c src/engine/model.cc -o build/model.o $(CFLAGS)

build/game_object.o: src/engine/game_object.cc src/engine/game_object.h src/engine/helper.h | build
	g++ -c src/engine/game_object.cc -o build/game_object.o $(CFLAGS)

build/camera.o: src/engine/camera.cc src/engine/camera.h src/engine/helper.h build/game_object.o | build
	g++ -c src/engine/camera.cc -o build/camera.o $(CFLAGS)

build/rigid_body.o: src/engine/rigid_body.cc src/engine/rigid_body.h build/game_object.o build/model.o | build
	g++ -c src/engine/rigid_body.cc -o build/rigid_body.o $(CFLAGS)

build/helper.o: src/engine/helper.cc src/engine/helper.h | build
	g++ -c src/engine/helper.cc -o build/helper.o $(CFLAGS)

build/light.o: src/engine/light.cc src/engine/light.h build/game_object.o | build
	g++ -c src/engine/light.cc -o build/light.o $(CFLAGS)

build/material.o: src/engine/material.cc src/engine/material.h build/helper.o | build
	g++ -c src/engine/material.cc -o build/material.o $(CFLAGS)

build/project.o: src/engine/project.cc src/engine/project.h src/engine/constants.h | build
	g++ -c src/engine/project.cc -o build/project.o $(CFLAGS)

build/ui_model.o: src/engine/ui_model.cc src/engine/ui_model.h build/model.o | build
	g++ -c src/engine/ui_model.cc -o build/ui_model.o $(CFLAGS)

build/ui.o: src/engine/ui.cc src/engine/ui.h build/ui_model.o | build
	g++ -c src/engine/ui.cc -o build/ui.o $(CFLAGS)

build/animation_controller.o: src/engine/animation_controller.cc src/engine/animation_controller.h | build
	g++ -c src/engine/animation_controller.cc -o build/animation_controller.o $(CFLAGS)

build/animation.o: src/engine/animation.cc src/engine/animation.h | build
	g++ -c src/engine/animation.cc -o build/animation.o $(CFLAGS)

build/player.o: src/turbo_tanks/player.cc src/turbo_tanks/player.h build/rigid_body.o | build
	g++ -c src/turbo_tanks/player.cc -o build/player.o $(CFLAGS)

build/player_camera.o: src/turbo_tanks/player_camera.cc src/turbo_tanks/player_camera.h build/camera.o | build
	g++ -c src/turbo_tanks/player_camera.cc -o build/player_camera.o $(CFLAGS)

build/player_cannon.o: src/turbo_tanks/player_cannon.cc src/turbo_tanks/player_cannon.h | build
	g++ -c src/turbo_tanks/player_cannon.cc -o build/player_cannon.o $(CFLAGS)

build/energy_ball.o: src/turbo_tanks/energy_ball.cc src/turbo_tanks/energy_ball.h | build
	g++ -c src/turbo_tanks/energy_ball.cc -o build/energy_ball.o $(CFLAGS)

build/collectable.o: src/turbo_tanks/collectable.cc src/turbo_tanks/collectable.h | build
	g++ -c src/turbo_tanks/collectable.cc -o build/collectable.o $(CFLAGS)


clean:
	$(RM) build bin
