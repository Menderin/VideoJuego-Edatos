#pragma once
#include "MiniJuegos/Hex/Hex.h"
#include <vector>
#include <iostream>

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

    // Métodos privados para el algoritmo minimax
    int minimax(Hex& estadoJuego, int profundidad, int alfa, int beta, bool esMaximizador);
    int evaluarDefensa(const Hex& estadoJuego);
    int evaluarAtaque(const Hex& estadoJuego);
    int evaluarTablero(const Hex& estadoJuego);
    std::vector<Posicion> obtenerMovimientosDisponibles(const Hex& estadoJuego);
    bool evaluarConexion(const Hex& estadoJuego, bool esJugador1);
    int evaluarCentro(const Hex& estadoJuego);
    int evaluarConectividad(const Hex& estadoJuego, bool esJugador1);

public:
    IAHex(int profundidad = 4, int dificultadIA = 75);
    
    // Método principal para calcular el mejor movimiento
    Posicion calcularMejorMovimiento(Hex& estadoJuego);
    
    // Métodos auxiliares
    void setDificultad(int nuevaDificultad);
    void setProfundidadMaxima(int nuevaProfundidad);
};












