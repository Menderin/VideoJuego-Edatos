# Makefile para el proyecto Gato con Minijuegos
# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
INCLUDES = -Iinclude

# Directorios
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Archivos fuente
SOURCES = $(SRC_DIR)/test_gato.cpp \
          $(SRC_DIR)/Nodo.cpp \
          $(SRC_DIR)/Tablero.cpp \
          $(SRC_DIR)/MiniJuegos/AdivinaNumero/AdivinaNumero.cpp

# Archivos objeto (en directorio build)
OBJECTS = $(BUILD_DIR)/test_gato.o \
          $(BUILD_DIR)/Nodo.o \
          $(BUILD_DIR)/Tablero.o \
          $(BUILD_DIR)/AdivinaNumero.o

# Nombre del ejecutable
TARGET = $(BIN_DIR)/gato

# Colores para output
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
NC = \033[0m # No Color

# Regla principal
all: directories $(TARGET)
	@echo "$(GREEN)✅ Compilación exitosa!$(NC)"
	@echo "$(YELLOW)Ejecuta con: ./$(TARGET)$(NC)"

# Crear directorios necesarios
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Regla para crear el ejecutable
$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)🔗 Enlazando ejecutable...$(NC)"
	$(CXX) $(OBJECTS) -o $@

# Reglas para compilar archivos objeto
$(BUILD_DIR)/test_gato.o: $(SRC_DIR)/test_gato.cpp
	@echo "$(YELLOW)🔨 Compilando test_gato.cpp...$(NC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/Nodo.o: $(SRC_DIR)/Nodo.cpp
	@echo "$(YELLOW)🔨 Compilando Nodo.cpp...$(NC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/Tablero.o: $(SRC_DIR)/Tablero.cpp
	@echo "$(YELLOW)🔨 Compilando Tablero.cpp...$(NC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/AdivinaNumero.o: $(SRC_DIR)/MiniJuegos/AdivinaNumero/AdivinaNumero.cpp
	@echo "$(YELLOW)🔨 Compilando AdivinaNumero.cpp...$(NC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compilación rápida (sin directorios build/bin)
quick: 
	@echo "$(YELLOW)⚡ Compilación rápida...$(NC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o gato
	@echo "$(GREEN)✅ Listo! Ejecuta con: ./gato$(NC)"

# Limpiar archivos generados
clean:
	@echo "$(RED)🧹 Limpiando archivos generados...$(NC)"
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)
	rm -f gato gato.exe

# Limpiar y recompilar
rebuild: clean all

# Ejecutar el programa
run: $(TARGET)
	@echo "$(GREEN)🎮 Ejecutando el juego...$(NC)"
	./$(TARGET)

# Ejecutar compilación rápida y luego el programa
quickrun: quick
	@echo "$(GREEN)🎮 Ejecutando el juego...$(NC)"
	./gato

# Mostrar ayuda
help:
	@echo "$(YELLOW)📖 Comandos disponibles:$(NC)"
	@echo "  make          - Compilar el proyecto (completo)"
	@echo "  make quick    - Compilación rápida (sin organizar archivos)"
	@echo "  make run      - Compilar y ejecutar"
	@echo "  make quickrun - Compilación rápida y ejecutar"
	@echo "  make clean    - Limpiar archivos generados"
	@echo "  make rebuild  - Limpiar y recompilar"
	@echo "  make help     - Mostrar esta ayuda"

# Debug: mostrar variables
debug:
	@echo "$(YELLOW)🔍 Variables de debug:$(NC)"
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "TARGET: $(TARGET)"

# Indicar que estos targets no son archivos
.PHONY: all clean rebuild run quick quickrun help debug directories