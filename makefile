OS=$(shell uname)
ifeq ($(OS),Darwin)
CXXFLAGS=-framework OpenGL -lglfw
else
CXXFLAGS=-lglfw -lGL
endif

engine=src/engine
test=test

all: gears orbiting_star moons_and_planets wasd_movement

wasd_movement: wasd_movement.o model.o gameobject.o helper.o camera.o rigidbody.o
	g++ -I ./ wasd_movement.o model.o gameobject.o helper.o camera.o rigidbody.o -o wasd_movement $(CXXFLAGS)

wasd_movement.o: $(test)/wasd_movement.cc $(engine)/model.h $(engine)/gameobject.h $(engine)/helper.h $(engine)/constants.h $(engine)/camera.h $(engine)/rigidbody.h
	g++ -I ./ -c $(test)/wasd_movement.cc

moons_and_planets: moons_and_planets.o model.o
	g++ -I ./ moons_and_planets.o model.o -o moons_and_planets $(CXXFLAGS)

moons_and_planets.o: $(test)/moons_and_planets.cc $(engine)/model.h
	g++ -I ./ -c $(test)/moons_and_planets.cc

gears: gears.o model.o
	g++ -I ./ gears.o model.o -o gears $(CXXFLAGS)

gears.o: $(test)/gears.cc $(engine)/model.h
	g++ -I ./ -c $(test)/gears.cc

orbiting_star: orbiting_star.o model.o
	g++ -I ./ orbiting_star.o model.o -o orbiting_star $(CXXFLAGS)

orbiting_star.o: $(test)/orbiting_star.cc $(engine)/model.h
	g++ -I ./ -c $(test)/orbiting_star.cc

model.o: $(engine)/model.cc $(engine)/model.h $(engine)/constants.h
	g++ -I ./ -c $(engine)/model.cc

gameobject.o: $(engine)/gameobject.cc $(engine)/gameobject.h $(engine)/constants.h $(engine)/helper.h
	g++ -I ./ -c $(engine)/gameobject.cc

camera.o: $(engine)/camera.cc $(engine)/camera.h $(engine)/gameobject.h $(engine)/constants.h $(engine)/helper.h
	g++ -I ./ -c $(engine)/camera.cc

rigidbody.o: $(engine)/rigidbody.cc $(engine)/rigidbody.h $(engine)/gameobject.h $(engine)/model.h $(engine)/helper.h
	g++ -I ./ -c $(engine)/rigidbody.cc

helper.o: $(engine)/helper.cc $(engine)/helper.h $(engine)/constants.h
	g++ -I ./ -c $(engine)/helper.cc

clean:
	rm -fr *.o orbiting_star gears moons_and_planets wasd_movement
