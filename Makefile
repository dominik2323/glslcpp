CXX = clang++
CC = clang
CXXFLAGS = -std=c++17 -Wall -Wextra -I/opt/homebrew/include -I. -Iinclude -DGL_SILENCE_DEPRECATION
CFLAGS = -I/opt/homebrew/include -Iinclude -DGL_SILENCE_DEPRECATION
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
TARGET = main
CPP_SOURCES = main.cpp src/opengl_renderer.cpp src/window_manager.cpp
C_SOURCES = glad.c
SHADERS = shaders/vertex.glsl shaders/fragment.glsl

$(TARGET): $(CPP_SOURCES) $(C_SOURCES) $(SHADERS)
	$(CC) $(CFLAGS) -c $(C_SOURCES)
	$(CXX) $(CXXFLAGS) $(CPP_SOURCES) glad.o $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

watch:
	find . -name "*.cpp" -o -name "*.h" -o -name "*.glsl" | entr -c make run

watch-glsl:
	find . -name "*.glsl" | entr -cr make run

watch-build:
	find . -name "*.cpp" -o -name "*.h" -o -name "*.glsl" | entr -c make

clean:
	rm -f $(TARGET) glad.o

.PHONY: run watch clean
