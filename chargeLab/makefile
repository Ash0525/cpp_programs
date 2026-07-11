# Compiler
CXX := g++

# SFML path
SFML_PATH := C:/msys64/mingw64

# Compiler Flags
CXXFLAGS := -std=c++17 -I$(SFML_PATH)/include
LDFLAGS := -L$(SFML_PATH)/lib
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

# so that it's compatible for both mac and windows
ifeq ($(OS),Windows_NT)
	EXEEXT := .exe
else
	EXEEXT :=
endif

# Target executable
TARGET := run$(EXEEXT)

# Source files
SRCS := main.cpp \
	src/Vector2D.cpp \
	src/Particle.cpp \
	src/Simulation.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# default rule to build and run
all: $(TARGET)

# Rule to link objects into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean rule to remove generated files
clean:
	cmd /C "for %f in (\"$(TARGET)\" $(OBJS:%=\"%\")) do @if exist %f del /Q %f"