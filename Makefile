CXX = g++
CXXFLAGS = -Wall -Wextra
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = program

all: $(TARGET)

$(TARGET): main.o
	$(CXX) main.o -o $(TARGET) $(LIBS)

main.o: main.cpp
	$(CXX) -c main.cpp $(CXXFLAGS)

clean:
	rm -f *.o $(TARGET)