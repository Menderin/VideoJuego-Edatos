
#ifndef IA_BATALLA_CARTAS_H
#define IA_BATALLA_CARTAS_H

#include <vector>
#include "MiniJuegos/BatallaDeCartas/BatallaDeCartas.h"

class IaBatallaCartas {
private:
    std::vector<int> cartasDisponibles;  // Cartas que la IA aún puede jugar
    int puntosIA;                        // Puntos actuales de la IA
    int puntosJugador;                   // Puntos actuales del jugador
    int dificultad;                      // Nivel de dificultad de la IA (1-100)

public:
    // Constructor que inicializa la IA con un nivel de dificultad
    IaBatallaCartas(int nivelDificultad = 75);

    // Método para elegir la carta a jugar
    int elegirCarta(int cartaJugador = -1);

    // Método para actualizar las cartas disponibles
    void actualizarCartas(const std::vector<int>& nuevasCartas);

    // Método para actualizar los puntos
    void actualizarPuntos(int puntosIA, int puntosJugador);

    // Método para reiniciar la IA
    void reiniciar();
};

#endif // IA_BATALLA_CARTAS_H
