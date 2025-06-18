#ifndef IA_BATALLA_CARTAS_H
#define IA_BATALLA_CARTAS_H

#include <vector>
#include "MiniJuegos/BatallaDeCartas/BatallaDeCartas.h"

class IaBatallaCartas {
private:
    std::vector<int> cartasDisponibles;
    int puntosIA;
    int puntosJugador;
    int dificultad;

    struct Estado {
        std::vector<int> cartasIA;
        int puntosIA;
        int puntosJugador;
        int ultimaCartaJugador;
    };

    // Declaración de métodos privados
    int minimax(Estado estado, int profundidad, int alpha, int beta, bool esMaximizando);
    int evaluarEstado(const Estado& estado);

public:
    IaBatallaCartas(int nivelDificultad = 75);
    int elegirCarta(int cartaJugador = -1);
    void actualizarCartas(const std::vector<int>& nuevasCartas);
    void actualizarPuntos(int puntosIA, int puntosJugador);
    void reiniciar();
};

#endif // IA_BATALLA_CARTAS_H
