# Compiler
CXX := g++

# Compiler Flags
CXXFLAGS := -std=c++17

# so that it's compatible for both mac and windows
ifeq ($(OS),Windows_NT)
	EXEEXT := .exe
else
	EXEEXT :=
endif

# Target executable. append if windows
TARGET1 := run$(EXEEXT)

# Compile the correct classes
SRCS1 := main.cpp

# Object files
OBJS1 = ${SRCS1:.cpp=.o}

# default rule to build and run
all: $(TARGET1)

# Rule to link objects into the target executable
$(TARGET1) : $(OBJS1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(OBJS1)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean rule to remove generated files
clean:
	cmd /C "for %f in (\"$(TARGET1)\" $(OBJS1:%=\"%\")) do @if exist %f del /Q %f"