# Compilador de C++
CXX = g++
CXXFLAGS = -Wall -g

# Ficheros fuente
SOURCES = main.cc customFunctions.cc schedulersFunctions.cc

# Ficheros objeto (se generan a partir de los .cc)
OBJECTS = $(SOURCES:.cc=.o)

# Cabeceras
HEADERS = customFunctions.h schedulersFunctions.h

# Nombre del ejecutable
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm output.txt
	touch output.txt
