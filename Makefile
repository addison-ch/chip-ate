INCLUDES= -I ./inc
FLAGS= -g

OBJECTS=./build/chip8_memory.o ./build/stack.o ./build/keyboard.o ./build/chip8.o ./build/screen.o

all: ${OBJECTS}
	gcc  ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip8_memory.o:src/chip8_memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.o

./build/stack.o:src/stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/stack.c -c -o ./build/stack.o ./build/screen.o

./build/keyboard.o:src/keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/keyboard.c -c -o ./build/keyboard.o

./build/chip8.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

./build/screen.o:src/screen.c
	gcc ${FLAGS} ${INCLUDES} ./src/screen.c -c -o ./build/screen.o

clean:
	del build\*