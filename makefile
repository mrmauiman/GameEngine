ifeq ($(OS), Windows_NT)
	CXXFLAGS=-lglfw3dll -lopengl32 -lgdi32 -static-libstdc++ -static-libgcc
	remove=del
else
	remove=rm
	UNAME=$(shell uname)
	ifeq ($(UNAME),Darwin)
		CXXFLAGS=-framework OpenGL -lglfw
	else
		CXXFLAGS=-lglfw -lGL
	endif
endif

engine=src/engine
test=test
here=.

wasd_movement: wasd_movement.o model.o gameobject.o helper.o camera.o rigidbody.o
	g++ -I $(here) wasd_movement.o model.o gameobject.o helper.o camera.o rigidbody.o -o wasd_movement $(CXXFLAGS)

wasd_movement.o: $(test)/wasd_movement.cc $(engine)/model.h $(engine)/gameobject.h $(engine)/helper.h $(engine)/constants.h $(engine)/camera.h $(engine)/rigidbody.h
	g++ -I $(here) -c $(test)/wasd_movement.cc

moons_and_planets: moons_and_planets.o model.o
	g++ -I $(here) moons_and_planets.o model.o -o moons_and_planets $(CXXFLAGS)

moons_and_planets.o: $(test)/moons_and_planets.cc $(engine)/model.h
	g++ -I $(here) -c $(test)/moons_and_planets.cc

gears: gears.o model.o
	g++ -I $(here) gears.o model.o -o gears $(CXXFLAGS)

gears.o: $(test)/gears.cc $(engine)/model.h
	g++ -I $(here) -c $(test)/gears.cc

orbiting_star: orbiting_star.o model.o
	g++ -I $(here) orbiting_star.o model.o -o orbiting_star $(CXXFLAGS)

orbiting_star.o: $(test)/orbiting_star.cc $(engine)/model.h
	g++ -I $(here) -c $(test)/orbiting_star.cc

model.o: $(engine)/model.cc $(engine)/model.h $(engine)/constants.h
	g++ -I $(here) -c $(engine)/model.cc

gameobject.o: $(engine)/gameobject.cc $(engine)/gameobject.h $(engine)/constants.h $(engine)/helper.h
	g++ -I $(here) -c $(engine)/gameobject.cc

camera.o: $(engine)/camera.cc $(engine)/camera.h $(engine)/gameobject.h $(engine)/constants.h $(engine)/helper.h
	g++ -I $(here) -c $(engine)/camera.cc

rigidbody.o: $(engine)/rigidbody.cc $(engine)/rigidbody.h $(engine)/gameobject.h $(engine)/model.h $(engine)/helper.h
	g++ -I $(here) -c $(engine)/rigidbody.cc

helper.o: $(engine)/helper.cc $(engine)/helper.h $(engine)/constants.h
	g++ -I $(here) -c $(engine)/helper.cc

clean:
	$(remove) -fr *.o orbiting_star* gears* moons_and_planets* wasd_movement*

run:
	wasd_movement
