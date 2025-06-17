#include "IA/IaHex.h"
#include <algorithm>
#include <climits>
#include <random>

IAHex::IAHex(int profundidad, int dificultadIA) 
    : profundidadMaxima(profundidad), dificultad(dificultadIA) {}

void IAHex::setDificultad(int nuevaDificultad) {
    dificultad = std::clamp(nuevaDificultad, 1, 100);
}

void IAHex::setProfundidadMaxima(int nuevaProfundidad) {
    profundidadMaxima = std::max(1, nuevaProfundidad);
}

std::vector<Posicion> IAHex::obtenerMovimientosDisponibles(const Hex& estadoJuego) {
    std::vector<Posicion> movimientos;
    const int TABLERO_SIZE = 7;
    
    for(int i = 0; i < TABLERO_SIZE; i++) {
        for(int j = 0; j < TABLERO_SIZE; j++) {
            if(estadoJuego.getCasilla(i, j) == EstadoCasilla::VACIA) {
                movimientos.push_back(Posicion(i, j));
            }
        }
    }
    return movimientos;
}



// Modificar la función evaluarDefensa para ser más agresiva
int IAHex::evaluarDefensa(const Hex& estadoJuego) {
    int puntuacion = 0;
    const int TABLERO_SIZE = 7;

    // Aumentar la detección y valoración de amenazas
    for(int i = 0; i < TABLERO_SIZE; i++) {
        for(int j = 0; j < TABLERO_SIZE - 1; j++) {
            if(estadoJuego.getCasilla(i, j) == EstadoCasilla::JUGADOR1) {
                // Aumentar penalización por fichas consecutivas
                if(estadoJuego.getCasilla(i, j + 1) == EstadoCasilla::JUGADOR1) {
                    puntuacion -= 300;
                }
                
                // Mayor penalización por puentes
                if(j < TABLERO_SIZE - 2 && 
                   estadoJuego.getCasilla(i, j + 1) == EstadoCasilla::VACIA &&
                   estadoJuego.getCasilla(i, j + 2) == EstadoCasilla::JUGADOR1) {
                    puntuacion -= 250;
                }

                // Nueva detección de patrones de victoria potencial
                if(j < TABLERO_SIZE - 3 &&
                   estadoJuego.getCasilla(i, j + 1) == EstadoCasilla::VACIA &&
                   estadoJuego.getCasilla(i, j + 2) == EstadoCasilla::VACIA &&
                   estadoJuego.getCasilla(i, j + 3) == EstadoCasilla::JUGADOR1) {
                    puntuacion -= 200;
                }
            }
        }
    }

    // Aumentar importancia de los bordes
    for(int i = 0; i < TABLERO_SIZE; i++) {
        if(estadoJuego.getCasilla(i, 0) == EstadoCasilla::JUGADOR1) 
            puntuacion -= 250;
        if(estadoJuego.getCasilla(i, TABLERO_SIZE-1) == EstadoCasilla::JUGADOR1) 
            puntuacion -= 250;
    }

    return puntuacion;
}

// Mejorar la función de evaluación de ataque
int IAHex::evaluarAtaque(const Hex& estadoJuego) {
    int puntuacion = 0;
    const int TABLERO_SIZE = 7;

    // Aumentar valor de conexiones verticales
    for(int j = 0; j < TABLERO_SIZE; j++) {
        for(int i = 0; i < TABLERO_SIZE - 1; i++) {
            if(estadoJuego.getCasilla(i, j) == EstadoCasilla::JUGADOR2) {
                // Mayor valor a conexiones directas
                if(estadoJuego.getCasilla(i + 1, j) == EstadoCasilla::JUGADOR2) {
                    puntuacion += 400;
                }
                
                // Mayor valor a puentes potenciales
                if(i < TABLERO_SIZE - 2 && 
                   estadoJuego.getCasilla(i + 1, j) == EstadoCasilla::VACIA &&
                   estadoJuego.getCasilla(i + 2, j) == EstadoCasilla::JUGADOR2) {
                    puntuacion += 300;
                }

                // Valorar patrones de victoria potencial
                if(i < TABLERO_SIZE - 3 &&
                   estadoJuego.getCasilla(i + 1, j) == EstadoCasilla::VACIA &&
                   estadoJuego.getCasilla(i + 2, j) == EstadoCasilla::VACIA &&
                   estadoJuego.getCasilla(i + 3, j) == EstadoCasilla::JUGADOR2) {
                    puntuacion += 250;
                }
            }
        }
    }

    // Aumentar valor del control de los bordes
    for(int j = 0; j < TABLERO_SIZE; j++) {
        if(estadoJuego.getCasilla(0, j) == EstadoCasilla::JUGADOR2) 
            puntuacion += 250;
        if(estadoJuego.getCasilla(TABLERO_SIZE-1, j) == EstadoCasilla::JUGADOR2) 
            puntuacion += 250;
    }

    // Control del centro más agresivo
    for(int i = 2; i < TABLERO_SIZE - 2; i++) {
        for(int j = 2; j < TABLERO_SIZE - 2; j++) {
            if(estadoJuego.getCasilla(i, j) == EstadoCasilla::JUGADOR2) {
                puntuacion += 100;
            }
        }
    }

    return puntuacion;
}

// Función principal de evaluación modificada
int IAHex::evaluarTablero(const Hex& estadoJuego) {
    if(estadoJuego.estaTerminado()) {
        return estadoJuego.getGanador() == 2 ? 7000 : -7000;
    }

    int puntuacionDefensa = evaluarDefensa(estadoJuego);
    int puntuacionAtaque = evaluarAtaque(estadoJuego);

    double factorAtaque = dificultad / 100.0;
    double factorDefensa = 1.0 - factorAtaque;

    return static_cast<int>(puntuacionDefensa * factorDefensa + puntuacionAtaque * factorAtaque);
}


Posicion IAHex::calcularMejorMovimiento(Hex& estadoJuego) {
    try {
        std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
        
        if(movimientos.empty()) {
            return Posicion(-1, -1);
        }

        // Buscar amenazas horizontales inmediatas primero
        for(const auto& mov : movimientos) {
            const int i = mov.fila;
            const int j = mov.columna;
            
            if(estadoJuego.getCasilla(i, j) != EstadoCasilla::VACIA) {
                continue;
            }

            // Detectar puentes horizontales potenciales
            bool amenazaHorizontal = false;
            if(j > 0 && j < 6) {
                // Verificar si hay fichas del jugador a los lados
                bool fichaIzquierda = (j > 0 && estadoJuego.getCasilla(i, j-1) == EstadoCasilla::JUGADOR1);
                bool fichaDerecha = (j < 10 && estadoJuego.getCasilla(i, j+1) == EstadoCasilla::JUGADOR1);
                
                if(fichaIzquierda || fichaDerecha) {
                    amenazaHorizontal = true;
                }
                
                // Verificar puentes potenciales
                if(j > 1 && estadoJuego.getCasilla(i, j-2) == EstadoCasilla::JUGADOR1 && 
                   estadoJuego.getCasilla(i, j-1) == EstadoCasilla::VACIA) {
                    amenazaHorizontal = true;
                }
                if(j < 5 && estadoJuego.getCasilla(i, j+2) == EstadoCasilla::JUGADOR1 && 
                   estadoJuego.getCasilla(i, j+1) == EstadoCasilla::VACIA) {
                    amenazaHorizontal = true;
                }
            }

            if(amenazaHorizontal) {
                return mov;
            }
        }

        // Si no hay amenazas horizontales inmediatas, usar minimax
        int mejorValor = INT_MAX;
        Posicion mejorMovimiento = movimientos[0];

        for(const auto& mov : movimientos) {
            if(estadoJuego.getCasilla(mov.fila, mov.columna) == EstadoCasilla::VACIA) {
                Hex copiaEstado = estadoJuego;
                if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                    int valor = minimax(copiaEstado, 1, INT_MIN, INT_MAX, true);
                    if(valor < mejorValor) {
                        mejorValor = valor;
                        mejorMovimiento = mov;
                    }
                }
            }
        }

        return mejorMovimiento;

    } catch(const std::exception& e) {
        std::cerr << "Error en calcularMejorMovimiento: " << e.what() << std::endl;
        return Posicion(-1, -1);
    }
}



int IAHex::minimax(Hex& estadoJuego, int profundidad, int alfa, int beta, bool esMaximizador) {
    if(profundidad == 0 || estadoJuego.estaTerminado()) {
        return evaluarTablero(estadoJuego);
    }

    std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
    
    if(esMaximizador) {
        int mejorValor = INT_MIN;
        for(const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                int valor = minimax(copiaEstado, profundidad - 1, alfa, beta, false);
                mejorValor = std::max(mejorValor, valor);
                alfa = std::max(alfa, mejorValor);
                if(beta <= alfa) break;
            }
        }
        return mejorValor;
    } 
    else {
        int mejorValor = INT_MAX;
        for(const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                int valor = minimax(copiaEstado, profundidad - 1, alfa, beta, true);
                mejorValor = std::min(mejorValor, valor);
                beta = std::min(beta, mejorValor);
                if(beta <= alfa) break;
            }
        }
        return mejorValor;
    }
}
