OS=$(shell uname)
ifeq ($(OS),Darwin)
CXXFLAGS=-framework OpenGL -lglfw
else
CXXFLAGS=-lglfw -lGL
endif

src=../src

gears: Gears.o Model.o
	g++ Gears.o Model.o -o gears $(CXXFLAGS)

Gears.o: Gears.cpp $(src)/Model.h
	g++ -c Gears.cpp

orbiting_star: OrbitingStar.o Model.o
	g++ OrbitingStar.o Model.o -o orbiting_star $(CXXFLAGS)

OrbitingStar.o: OrbitingStar.cpp $(src)/Model.h
	g++ -c OrbitingStar.cpp

Model.o: $(src)/Model.cpp $(src)/Model.h $(src)/Constants.h
	g++ -c $(src)/Model.cpp

clean:
	rm -fr *.o orbiting_star gears
