#pragma once
#include "Nodo.h"
#include <vector>

enum class EstadoJuego {
    EN_CURSO,
    GANADOR_J1,
    GANADOR_J2,
    EMPATE
};

class Tablero {
private:
    std::vector<std::vector<Nodo>> matrizNodos;
    int turnoActual;
    EstadoJuego estadoJuego;
    int fichasJ1, fichasJ2;
   
    
public:
    // Constructor
    Tablero();
    
    bool verificarFilas();
    bool verificarColumnas();
    bool verificarDiagonales();
    bool tableroLleno();
    void determinarGanadorPorFichas();

    // Métodos de inicialización
    void inicializarMatriz();
    void reiniciar();
    void reiniciarEstadoJuego();
    
    // Métodos de juego
    bool jugarNodo(int fila, int columna);
    void verificarVictoria();
    void cambiarTurno();
    
    // Getters
    const Nodo& getNodo(int fila, int columna) const;
    Nodo& getNodo(int fila, int columna);
    int getTurnoActual() const;
    EstadoJuego getEstadoJuego() const;
    int getFichasJ1() const;
    int getFichasJ2() const;
    
    // Utilidades
    void mostrarTablero() const;
    void mostrarEstadoJuego() const;
    bool esMovimientoValido(int fila, int columna) const;
};