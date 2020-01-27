OS=$(shell uname)
ifeq ($(OS),Darwin)
CXXFLAGS=-framework OpenGL -lglfw
else
CXXFLAGS=-lglfw -lGL
endif

engine: Main.o Model.o
	g++ Main.o Model.o -o engine $(CXXFLAGS)

Main.o: Main.cpp Model.h
	g++ -c Main.cpp

Model.o: Model.cpp Model.h Constants.h
	g++ -c Model.cpp

clean:
	rm -fr *.o engine
