#include "IA/IaBatallaCartas.h"
#include <algorithm>
#include <random>
#include <limits>

IaBatallaCartas::IaBatallaCartas(int nivelDificultad) : 
    puntosIA(0), puntosJugador(0), dificultad(nivelDificultad) {
    reiniciar();
}

int IaBatallaCartas::evaluarEstado(const Estado& estado) {
    int valorEstado = 0;
    
    // Evaluar diferencia de puntos
    valorEstado += (estado.puntosIA - estado.puntosJugador) * 100;
    
    // Evaluar cartas restantes
    if (!estado.cartasIA.empty()) {
        int sumaCartas = 0;
        for (int carta : estado.cartasIA) {
            sumaCartas += carta;
        }
        valorEstado += sumaCartas / estado.cartasIA.size();
    }
    
    return valorEstado;
}

int IaBatallaCartas::minimax(Estado estado, int profundidad, int alpha, int beta, bool esMaximizando) {
    if (profundidad == 0 || estado.cartasIA.empty()) {
        return evaluarEstado(estado);
    }

    if (esMaximizando) {
        int mejorValor = std::numeric_limits<int>::min();
        for (int carta : estado.cartasIA) {
            Estado nuevoEstado = estado;
            auto it = std::find(nuevoEstado.cartasIA.begin(), nuevoEstado.cartasIA.end(), carta);
            if (it != nuevoEstado.cartasIA.end()) {
                nuevoEstado.cartasIA.erase(it);
            }
            
            if (carta > estado.ultimaCartaJugador) {
                nuevoEstado.puntosIA++;
            }

            int valor = minimax(nuevoEstado, profundidad - 1, alpha, beta, false);
            mejorValor = std::max(mejorValor, valor);
            alpha = std::max(alpha, mejorValor);

            if (beta <= alpha) break; // Poda alfa-beta
        }
        return mejorValor;
    } else {
        int mejorValor = std::numeric_limits<int>::max();
        for (int posibleCarta = 1; posibleCarta <= 15; posibleCarta++) {
            Estado nuevoEstado = estado;
            nuevoEstado.ultimaCartaJugador = posibleCarta;
            
            if (posibleCarta > estado.ultimaCartaJugador) {
                nuevoEstado.puntosJugador++;
            }

            int valor = minimax(nuevoEstado, profundidad - 1, alpha, beta, true);
            mejorValor = std::min(mejorValor, valor);
            beta = std::min(beta, mejorValor);

            if (beta <= alpha) break; // Poda alfa-beta
        }
        return mejorValor;
    }
}

int IaBatallaCartas::elegirCarta(int cartaJugador) {
    if (cartasDisponibles.empty()) {
        return -1;
    }

    Estado estadoInicial;
    estadoInicial.cartasIA = cartasDisponibles;
    estadoInicial.puntosIA = puntosIA;
    estadoInicial.puntosJugador = puntosJugador;
    estadoInicial.ultimaCartaJugador = cartaJugador;

    int mejorValor = std::numeric_limits<int>::min();
    int mejorCarta = cartasDisponibles[0];

    for (int carta : cartasDisponibles) {
        Estado nuevoEstado = estadoInicial;
        auto it = std::find(nuevoEstado.cartasIA.begin(), nuevoEstado.cartasIA.end(), carta);
        if (it != nuevoEstado.cartasIA.end()) {
            nuevoEstado.cartasIA.erase(it);
        }

        int valor = minimax(nuevoEstado, 3, std::numeric_limits<int>::min(), 
                          std::numeric_limits<int>::max(), false);
        
        if (valor > mejorValor) {
            mejorValor = valor;
            mejorCarta = carta;
        }
    }

    auto it = std::find(cartasDisponibles.begin(), cartasDisponibles.end(), mejorCarta);
    if (it != cartasDisponibles.end()) {
        cartasDisponibles.erase(it);
    }

    return mejorCarta;
}

void IaBatallaCartas::actualizarCartas(const std::vector<int>& nuevasCartas) {
    cartasDisponibles = nuevasCartas;
}

void IaBatallaCartas::actualizarPuntos(int pIA, int pJugador) {
    puntosIA = pIA;
    puntosJugador = pJugador;
}

void IaBatallaCartas::reiniciar() {
    cartasDisponibles.clear();
    puntosIA = 0;
    puntosJugador = 0;
}
