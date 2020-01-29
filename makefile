OS=$(shell uname)
ifeq ($(OS),Darwin)
CXXFLAGS=-framework OpenGL -lglfw
else
CXXFLAGS=-lglfw -lGL
endif

src=src
test=test

all: gears orbiting_star

gears: gears.o model.o
	g++ gears.o model.o -o gears $(CXXFLAGS)

gears.o: $(test)/gears.cpp $(src)/model.h
	g++ -c $(test)/gears.cpp

orbiting_star: orbiting_star.o model.o
	g++ orbiting_star.o model.o -o orbiting_star $(CXXFLAGS)

orbiting_star.o: $(test)/orbiting_star.cpp $(src)/model.h
	g++ -c $(test)/orbiting_star.cpp

model.o: $(src)/model.cpp $(src)/model.h $(src)/constants.h
	g++ -c $(src)/model.cpp

clean:
	rm -fr *.o orbiting_star gears
