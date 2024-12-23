OBJS = main.cpp
OBJ_NAME = raycast
CC = g++
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)