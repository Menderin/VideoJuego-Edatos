#include "IA/IaBatallaCartas.h"
#include <algorithm>
#include <random>

IaBatallaCartas::IaBatallaCartas(int nivelDificultad) : 
    puntosIA(0), puntosJugador(0), dificultad(nivelDificultad) {
    reiniciar();
}

int IaBatallaCartas::elegirCarta(int cartaJugador) {
    if (cartasDisponibles.empty()) {
        return -1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    // Si la dificultad supera el número aleatorio, la IA juega de manera inteligente
    if (dis(gen) <= dificultad) {
        // Si conocemos la carta del jugador, intentamos ganar con la carta más baja posible
        if (cartaJugador != -1) {
            for (int carta : cartasDisponibles) {
                if (carta > cartaJugador) {
                    auto it = std::find(cartasDisponibles.begin(), cartasDisponibles.end(), carta);
                    cartasDisponibles.erase(it);
                    return carta;
                }
            }
        }
        
        // Si no conocemos la carta del jugador o no podemos ganar,
        // jugamos la carta más alta si vamos perdiendo, o la más baja si vamos ganando
        if (puntosIA <= puntosJugador) {
            int cartaMasAlta = *std::max_element(cartasDisponibles.begin(), cartasDisponibles.end());
            auto it = std::find(cartasDisponibles.begin(), cartasDisponibles.end(), cartaMasAlta);
            cartasDisponibles.erase(it);
            return cartaMasAlta;
        } else {
            int cartaMasBaja = *std::min_element(cartasDisponibles.begin(), cartasDisponibles.end());
            auto it = std::find(cartasDisponibles.begin(), cartasDisponibles.end(), cartaMasBaja);
            cartasDisponibles.erase(it);
            return cartaMasBaja;
        }
    }

    // Jugar de manera aleatoria
    std::uniform_int_distribution<> cartaDis(0, cartasDisponibles.size() - 1);
    int index = cartaDis(gen);
    int cartaElegida = cartasDisponibles[index];
    cartasDisponibles.erase(cartasDisponibles.begin() + index);
    return cartaElegida;
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
