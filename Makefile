#OBJS specifies which files to compile
OBJS = maze.cpp main.cpp

#CC specifies which compiler to use
CC = g++

#COMPILER_FLAGS which compiler options to use
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies which libraries need to be linked
LINKER_FLAGS = -lSDL2 -lSDL2_ttf

#OBJ_NAME specifies the name of the executable
OBJ_NAME = maze

#Target that compiles the executable
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
clean:
	rm maze
