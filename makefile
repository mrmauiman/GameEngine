OS=$(shell uname)
ifeq ($(OS),Darwin)
CXXFLAGS=-framework OpenGL -lglfw
else
CXXFLAGS=-lglfw -lGL
endif

src=src
test=test

all: gears orbiting_star moons_and_planets wasd_movement

wasd_movement: wasd_movement.o model.o
	g++ -I ./ wasd_movement.o model.o -o wasd_movement $(CXXFLAGS)

wasd_movement.o: $(test)/wasd_movement.cc $(src)/model.h
	g++ -I ./ -c $(test)/wasd_movement.cc

moons_and_planets: moons_and_planets.o model.o
	g++ -I ./ moons_and_planets.o model.o -o moons_and_planets $(CXXFLAGS)

moons_and_planets.o: $(test)/moons_and_planets.cc $(src)/model.h
	g++ -I ./ -c $(test)/moons_and_planets.cc

gears: gears.o model.o
	g++ -I ./ gears.o model.o -o gears $(CXXFLAGS)

gears.o: $(test)/gears.cc $(src)/model.h
	g++ -I ./ -c $(test)/gears.cc

orbiting_star: orbiting_star.o model.o
	g++ -I ./ orbiting_star.o model.o -o orbiting_star $(CXXFLAGS)

orbiting_star.o: $(test)/orbiting_star.cc $(src)/model.h
	g++ -I ./ -c $(test)/orbiting_star.cc

model.o: $(src)/model.cc $(src)/model.h $(src)/constants.h
	g++ -I ./ -c $(src)/model.cc

clean:
	rm -fr *.o orbiting_star gears moons_and_planets wasd_movement
