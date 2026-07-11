# Compiler
CXX := g++

# SFML path
SFML_PATH := C:/msys64/mingw64
DLL_DIR := lib

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
TARGET := app$(EXEEXT)
LEGACY_TARGET := run$(EXEEXT)

# Runtime DLLs needed by the executable on Windows
RUNTIME_DLLS := \
	libsfml-graphics-3.dll \
	libsfml-window-3.dll \
	libsfml-system-3.dll \
	libgcc_s_seh-1.dll \
	libstdc++-6.dll \
	libwinpthread-1.dll

# Source files
SRCS := main.cpp \
	src/Vector2D.cpp \
	src/Particle.cpp \
	src/Simulation.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# default rule to build and run
all: $(TARGET) stage-dlls

# Rule to link objects into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Copy runtime DLLs into lib/ so the run scripts can load dependencies reliably.
stage-dlls:
ifeq ($(MSYSTEM),)
	cmd /C "if not exist $(DLL_DIR) mkdir $(DLL_DIR)"
	cmd /C "if not exist $(DLL_DIR)\libsfml-graphics-3.dll copy /Y C:\msys64\mingw64\bin\libsfml-graphics-3.dll $(DLL_DIR)\libsfml-graphics-3.dll >nul"
	cmd /C "if not exist $(DLL_DIR)\libsfml-window-3.dll copy /Y C:\msys64\mingw64\bin\libsfml-window-3.dll $(DLL_DIR)\libsfml-window-3.dll >nul"
	cmd /C "if not exist $(DLL_DIR)\libsfml-system-3.dll copy /Y C:\msys64\mingw64\bin\libsfml-system-3.dll $(DLL_DIR)\libsfml-system-3.dll >nul"
	cmd /C "if not exist $(DLL_DIR)\libgcc_s_seh-1.dll copy /Y C:\msys64\mingw64\bin\libgcc_s_seh-1.dll $(DLL_DIR)\libgcc_s_seh-1.dll >nul"
	cmd /C "if not exist $(DLL_DIR)\libstdc++-6.dll copy /Y C:\msys64\mingw64\bin\libstdc++-6.dll $(DLL_DIR)\libstdc++-6.dll >nul"
	cmd /C "if not exist $(DLL_DIR)\libwinpthread-1.dll copy /Y C:\msys64\mingw64\bin\libwinpthread-1.dll $(DLL_DIR)\libwinpthread-1.dll >nul"
else
	mkdir -p $(DLL_DIR)
	test -f $(DLL_DIR)/libsfml-graphics-3.dll || cp -f $(SFML_PATH)/bin/libsfml-graphics-3.dll $(DLL_DIR)/libsfml-graphics-3.dll
	test -f $(DLL_DIR)/libsfml-window-3.dll || cp -f $(SFML_PATH)/bin/libsfml-window-3.dll $(DLL_DIR)/libsfml-window-3.dll
	test -f $(DLL_DIR)/libsfml-system-3.dll || cp -f $(SFML_PATH)/bin/libsfml-system-3.dll $(DLL_DIR)/libsfml-system-3.dll
	test -f $(DLL_DIR)/libgcc_s_seh-1.dll || cp -f $(SFML_PATH)/bin/libgcc_s_seh-1.dll $(DLL_DIR)/libgcc_s_seh-1.dll
	test -f $(DLL_DIR)/libstdc++-6.dll || cp -f $(SFML_PATH)/bin/libstdc++-6.dll $(DLL_DIR)/libstdc++-6.dll
	test -f $(DLL_DIR)/libwinpthread-1.dll || cp -f $(SFML_PATH)/bin/libwinpthread-1.dll $(DLL_DIR)/libwinpthread-1.dll
endif

# clean rule to remove generated files
clean:
ifeq ($(MSYSTEM),)
	cmd /C "if exist $(TARGET) del /Q $(TARGET)"
	cmd /C "if exist $(LEGACY_TARGET) del /Q $(LEGACY_TARGET)"
	cmd /C "if exist main.o del /Q main.o"
	cmd /C "if exist src\Vector2D.o del /Q src\Vector2D.o"
	cmd /C "if exist src\Particle.o del /Q src\Particle.o"
	cmd /C "if exist src\Simulation.o del /Q src\Simulation.o"
else
	rm -f $(TARGET) $(LEGACY_TARGET) $(OBJS)
endif