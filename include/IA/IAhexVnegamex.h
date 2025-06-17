#pragma once
#include "MiniJuegos/Hex/Hex.h"
#include <vector>
#include <iostream>
#include <utility>

// Estructura para representar una posición en el tablero
struct Posicion {
    int fila;
    int columna;
    
    Posicion(int f = -1, int c = -1) : fila(f), columna(c) {}
};

class IAHex {
private:
    int profundidadMaxima;
    int dificultad; // 1-100, afecta qué tan "inteligente" juega la IA
    
    // Variable de clase para contar nodos explorados
    int nodosExploradosTemp;
    
    // MÉTODOS PRINCIPALES DE LA NUEVA LÓGICA
    
    // Algoritmo de Dijkstra para calcular distancia mínima de conexión
    int dijkstra(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical);
    
    // Algoritmo Negamax (más eficiente que minimax)
    std::pair<int, Posicion> negamax(Hex& estadoJuego, int alpha, int beta, int profundidad, bool esJugadorIA);
    
    // MÉTODOS DE EVALUACIÓN
    int evaluarTablero(const Hex& estadoJuego);
    
    // MÉTODOS AUXILIARES
    std::vector<Posicion> obtenerMovimientosDisponibles(const Hex& estadoJuego);
    std::vector<Posicion> obtenerVecinosHex(int fila, int columna);
    
    // MÉTODOS LEGACY (mantenidos para compatibilidad)
    int minimax(Hex& estadoJuego, int profundidad, int alfa, int beta, bool esMaximizador);
    int evaluarDefensa(const Hex& estadoJuego);
    int evaluarAtaque(const Hex& estadoJuego);
    int calcularDistanciaCamino(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical);
    
    // Funciones auxiliares legacy
    Posicion buscarMovimientoOfensivo(const Hex& estadoJuego, const std::vector<Posicion>& movimientos);
    bool esAmenazaCritica(const Hex& estadoJuego, const Posicion& mov);
    int evaluarPrioridadMovimiento(const Hex& estadoJuego, const Posicion& mov);
    int evaluarProgresoVictoria(const Hex& estadoJuego);

public:
    IAHex(int profundidad = 4, int dificultadIA = 75);
    
    // Método principal para calcular el mejor movimiento
    Posicion calcularMejorMovimiento(Hex& estadoJuego);
    
    // Métodos auxiliares públicos
    void setDificultad(int nuevaDificultad);
    void setProfundidadMaxima(int nuevaProfundidad);
};