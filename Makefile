LIBS=-framework OpenGL -lGLEW -framework GLUT
CC=g++

SRCS=main_file.cpp tga.cpp
HDRS=tga.h cube.h wall.h

SOURCES=$(addprefix ./src/, $(SRCS))
HEADERS=$(addprefix ./src/, $(HDRS))

OBJECTS=$(SOURCES:.cpp=.o)

TARGET=sokoban

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@

clean:
	-rm -rf *.o sokoban
