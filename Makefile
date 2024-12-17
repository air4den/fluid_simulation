TARGET  = fluid_simulation
CC      = g++
CFLAGS  = -std=c++17 -Wall -g 
LIBS    = -lsfml-graphics -lsfml-window -lsfml-system

build:./src/*.cpp
	$(CC) $(CFLAGS) -o  $(TARGET) ./src/*.cpp $(LIBS)

run: ./src/*.cpp
	$(CC) $(CFLAGS) -o  $(TARGET) ./src/*.cpp $(LIBS)
	./$(TARGET)

clean:
	@rm -rf $(TARGET) *.o $(wildcard *.dSYM)
