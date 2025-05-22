#include "Tablero.h"
#include <iostream>

// Constructor
Tablero::Tablero() : turnoActual(1), estadoJuego(EstadoJuego::EN_CURSO), fichasJ1(0), fichasJ2(0) {
    inicializarMatriz();
}

// Métodos de inicialización
void Tablero::inicializarMatriz() {
    matrizNodos.resize(3);
    for (int i = 0; i < 3; i++) {
        matrizNodos[i].resize(3);
        for (int j = 0; j < 3; j++) {
            matrizNodos[i][j] = Nodo(i, j);
        }
    }
}

void Tablero::reiniciar() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrizNodos[i][j].reiniciar();
        }
    }
    turnoActual = 1;
    estadoJuego = EstadoJuego::EN_CURSO;
    fichasJ1 = fichasJ2 = 0;
}

// Métodos de juego
bool Tablero::jugarNodo(int fila, int columna) {
    if (!esMovimientoValido(fila, columna)) {
        return false;
    }
    
    Nodo& nodo = matrizNodos[fila][columna];
    EstadoNodo estadoJugador = (turnoActual == 1) ? EstadoNodo::JUGADOR1 : EstadoNodo::JUGADOR2;
    
    // Si el nodo tiene minijuego, ocuparNodo manejará automáticamente el resultado
    // Si no tiene minijuego, se asignará al jugador actual
    if (nodo.ocuparNodo(estadoJugador)) {
        // Actualizar contadores basándose en el estado final del nodo
        EstadoNodo estadoFinal = nodo.getEstado();
        if (estadoFinal == EstadoNodo::JUGADOR1) {
            fichasJ1++;
        } else if (estadoFinal == EstadoNodo::JUGADOR2) {
            fichasJ2++;
        }
        
        verificarVictoria();
        
        if (estadoJuego == EstadoJuego::EN_CURSO) {
            cambiarTurno();
        }
        
        return true;
    }
    return false;
}

void Tablero::verificarVictoria() {
    // Verificar condiciones de victoria por líneas
    if (verificarFilas() || verificarColumnas() || verificarDiagonales()) {
        return; // Ya se estableció el ganador
    }
    
    // Si el tablero está lleno y no hay línea, determinar ganador por fichas
    if (tableroLleno()) {
        determinarGanadorPorFichas();
    }
}

void Tablero::cambiarTurno() {
    turnoActual = (turnoActual == 1) ? 2 : 1;
}

// Métodos privados para verificación de victoria
bool Tablero::verificarFilas() {
    for (int i = 0; i < 3; i++) {
        if (!matrizNodos[i][0].estaVacio() && 
            matrizNodos[i][0].getEstado() == matrizNodos[i][1].getEstado() && 
            matrizNodos[i][1].getEstado() == matrizNodos[i][2].getEstado()) {
            
            estadoJuego = (matrizNodos[i][0].getEstado() == EstadoNodo::JUGADOR1) ? 
                          EstadoJuego::GANADOR_J1 : EstadoJuego::GANADOR_J2;
            return true;
        }
    }
    return false;
}

bool Tablero::verificarColumnas() {
    for (int j = 0; j < 3; j++) {
        if (!matrizNodos[0][j].estaVacio() && 
            matrizNodos[0][j].getEstado() == matrizNodos[1][j].getEstado() && 
            matrizNodos[1][j].getEstado() == matrizNodos[2][j].getEstado()) {
            
            estadoJuego = (matrizNodos[0][j].getEstado() == EstadoNodo::JUGADOR1) ? 
                          EstadoJuego::GANADOR_J1 : EstadoJuego::GANADOR_J2;
            return true;
        }
    }
    return false;
}

bool Tablero::verificarDiagonales() {
    // Diagonal principal (0,0) -> (1,1) -> (2,2)
    if (!matrizNodos[0][0].estaVacio() && 
        matrizNodos[0][0].getEstado() == matrizNodos[1][1].getEstado() && 
        matrizNodos[1][1].getEstado() == matrizNodos[2][2].getEstado()) {
        
        estadoJuego = (matrizNodos[0][0].getEstado() == EstadoNodo::JUGADOR1) ? 
                      EstadoJuego::GANADOR_J1 : EstadoJuego::GANADOR_J2;
        return true;
    }
    
    // Diagonal secundaria (0,2) -> (1,1) -> (2,0)
    if (!matrizNodos[0][2].estaVacio() && 
        matrizNodos[0][2].getEstado() == matrizNodos[1][1].getEstado() && 
        matrizNodos[1][1].getEstado() == matrizNodos[2][0].getEstado()) {
        
        estadoJuego = (matrizNodos[0][2].getEstado() == EstadoNodo::JUGADOR1) ? 
                      EstadoJuego::GANADOR_J1 : EstadoJuego::GANADOR_J2;
        return true;
    }
    
    return false;
}

bool Tablero::tableroLleno() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrizNodos[i][j].estaVacio()) {
                return false;
            }
        }
    }
    return true;
}

void Tablero::determinarGanadorPorFichas() {
    if (fichasJ1 > fichasJ2) {
        estadoJuego = EstadoJuego::GANADOR_J1;
    } else if (fichasJ2 > fichasJ1) {
        estadoJuego = EstadoJuego::GANADOR_J2;
    } else {
        estadoJuego = EstadoJuego::EMPATE;
    }
}

// Getters
const Nodo& Tablero::getNodo(int fila, int columna) const {
    static Nodo nodoVacio;
    if (fila < 0 || fila >= 3 || columna < 0 || columna >= 3) {
        return nodoVacio;
    }
    return matrizNodos[fila][columna];
}

Nodo& Tablero::getNodo(int fila, int columna) {
    static Nodo nodoVacio;
    if (fila < 0 || fila >= 3 || columna < 0 || columna >= 3) {
        return nodoVacio;
    }
    return matrizNodos[fila][columna];
}

int Tablero::getTurnoActual() const {
    return turnoActual;
}

EstadoJuego Tablero::getEstadoJuego() const {
    return estadoJuego;
}

int Tablero::getFichasJ1() const {
    return fichasJ1;
}

int Tablero::getFichasJ2() const {
    return fichasJ2;
}

// Utilidades
void Tablero::mostrarTablero() const {
    std::cout << "\n=== Estado del Tablero ===" << std::endl;
    
    // Mostrar tablero visual
    std::cout << "Tablero visual:" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "| ";
        for (int j = 0; j < 3; j++) {
            switch(matrizNodos[i][j].getEstado()) {
                case EstadoNodo::VACIO:
                    std::cout << " ";
                    break;
                case EstadoNodo::JUGADOR1:
                    std::cout << "X";
                    break;
                case EstadoNodo::JUGADOR2:
                    std::cout << "O";
                    break;
            }
            std::cout << " | ";
        }
        std::cout << std::endl;
        if (i < 2) std::cout << "-------------" << std::endl;
    }
    
    /**std::cout << "\nDetalle de nodos:" << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrizNodos[i][j].mostrarInfo();
        }
    }**/
    
    std::cout << "\nEstadisticas del juego:" << std::endl;
    std::cout << "Fichas J1: " << fichasJ1 << " | Fichas J2: " << fichasJ2 << std::endl;
    std::cout << "Turno actual: Jugador " << turnoActual << std::endl;
    
    mostrarEstadoJuego();
    std::cout << "========================\n" << std::endl;
}

void Tablero::mostrarEstadoJuego() const {
    std::cout << "Estado del juego: ";
    switch(estadoJuego) {
        case EstadoJuego::EN_CURSO:
            std::cout << "EN CURSO";
            break;
        case EstadoJuego::GANADOR_J1:
            std::cout << "GANADOR JUGADOR 1";
            break;
        case EstadoJuego::GANADOR_J2:
            std::cout << "GANADOR JUGADOR 2";
            break;
        case EstadoJuego::EMPATE:
            std::cout << "EMPATE";
            break;
    }
    std::cout << std::endl;
}

bool Tablero::esMovimientoValido(int fila, int columna) const {
    if (fila < 0 || fila >= 3 || columna < 0 || columna >= 3) {
        return false;
    }
    if (estadoJuego != EstadoJuego::EN_CURSO) {
        return false;
    }
    if (!matrizNodos[fila][columna].estaActivo() || !matrizNodos[fila][columna].estaVacio()) {
        return false;
    }
    return true;
}