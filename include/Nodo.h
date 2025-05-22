#pragma once
#include <iostream>
#include "MiniJuegos/Minijuego.h"
#include <memory>

enum class EstadoNodo {
    VACIO = 0,
    JUGADOR1 = 1,
    JUGADOR2 = 2
};

enum class TipoMiniJuego {
    NINGUNO,
    ADIVINA_NUMERO,
    HEX,
    BATALLA_CARTAS
};

class Nodo {
private:
    EstadoNodo estado;
    int fila, columna;
    bool activo;
    //Sistema de minijuegos
    std::unique_ptr<MiniJuego> miniJuego;
    TipoMiniJuego tipoMiniJuego;
    bool tieneMiniJuego;
    
public:
    // Constructores
    Nodo();
    Nodo(int f, int c);
    
    // Getters
    EstadoNodo getEstado() const;
    int getFila() const;
    int getColumna() const;
    bool estaActivo() const;
    bool estaVacio() const;
    
    // Setters
    void setEstado(EstadoNodo nuevoEstado);
    void setPosicion(int f, int c);
    void setActivo(bool activo);
    
    // Métodos de juego
    bool ocuparNodo(EstadoNodo jugador);
    void reiniciar();

    // Nuevos métodos para minijuegos
    void asignarMiniJuego(TipoMiniJuego tipo);
    bool tieneMiniJuegoAsignado() const;
    TipoMiniJuego getTipoMiniJuego() const;
    MiniJuego* getMiniJuego() const;
    
    // Método para jugar el minijuego y determinar ganador
    EstadoNodo jugarMiniJuego();
    
    // Utilidades
    void mostrarInfo() const;
};