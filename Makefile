# Variables
CXX = g++
TARGET = test.exe
SRC = src/GatoGUI.cpp
SFML_DIR = lib/SFML-3.0.0
INCLUDE_DIR = $(SFML_DIR)/include
LIB_DIR = $(SFML_DIR)/lib

# Flags del compilador
CXXFLAGS = -I"$(INCLUDE_DIR)" -DSFML_STATIC
LDFLAGS = -L"$(LIB_DIR)" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32

# Regla principal
all: $(TARGET)

# Regla para compilar y enlazar en un solo paso
$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Regla para limpiar archivos generados
clean:
	del $(TARGET)

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Indicar que estas reglas no son archivos
.PHONY: all clean run