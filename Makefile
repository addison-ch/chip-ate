INCLUDES= -I ./inc
FLAGS= -g

OBJECTS=./build/chip8_memory.o ./build/stack.o ./build/keyboard.o .build/chip8.o

all: ${OBJECTS}
	gcc  ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip8_memory.o:src/chip8_memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.o

./build/stack.o:src/stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/stack.c -c -o ./build/stack.o

./build/stack.o:src/keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/keyboard.c -c -o ./build/keyboard.o

./build/stack.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

clean:
	del build\*