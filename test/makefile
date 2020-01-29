OS=$(shell uname)
ifeq ($(OS),Darwin)
CXXFLAGS=-framework OpenGL -lglfw
else
CXXFLAGS=-lglfw -lGL
endif

src=../src

engine: Main.o Model.o
	g++ Main.o Model.o -o engine $(CXXFLAGS)

Main.o: Main.cpp $(src)/Model.h
	g++ -c Main.cpp

Model.o: $(src)/Model.cpp $(src)/Model.h $(src)/Constants.h
	g++ -c $(src)/Model.cpp

clean:
	rm -fr *.o engine
