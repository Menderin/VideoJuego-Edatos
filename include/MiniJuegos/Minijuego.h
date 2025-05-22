#pragma once
#include <memory>

enum class EstadoMiniJuego {
    EN_PROGRESO,
    GANADOR_JUGADOR1,
    GANADOR_JUGADOR2
};

class MiniJuego {
protected:
    EstadoMiniJuego estado;
    bool juegoTerminado;
    
public:
    MiniJuego() : estado(EstadoMiniJuego::EN_PROGRESO), juegoTerminado(false) {}
    virtual ~MiniJuego() = default;
    
    // Métodos virtuales puros que deben implementar los minijuegos
    virtual bool procesarMovimiento(int jugador, int input) = 0;
    virtual void mostrarEstado() const = 0;
    virtual void reiniciar() = 0;
    
    // Métodos comunes
    EstadoMiniJuego getEstado() const { return estado; }
    bool estaTerminado() const { return juegoTerminado; }
    
protected:
    void terminarJuego(EstadoMiniJuego ganador) {
        estado = ganador;
        juegoTerminado = true;
    }
};