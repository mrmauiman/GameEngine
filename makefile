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

tests := $(patsubst test/%.cc,bin/%,$(wildcard test/lighting.cc))

all: test

build:
	$(MKDIR) build

bin:
	$(MKDIR) bin

test: $(tests)

bin/%: build/%.o build/model.o build/game_object.o build/camera.o build/rigid_body.o build/helper.o build/light.o build/material.o | bin
	g++ $< build/model.o build/game_object.o build/camera.o build/rigid_body.o build/helper.o build/light.o build/material.o -o $@ $(CXXFLAGS)

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

build/material.o: src/engine/material.cc src/engine/material.h | build
	g++ -c src/engine/material.cc -o build/material.o $(CFLAGS)

clean:
	$(RM) build bin
