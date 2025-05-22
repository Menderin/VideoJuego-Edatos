#pragma once
#include <iostream>

enum class EstadoNodo {
    VACIO = 0,
    JUGADOR1 = 1,
    JUGADOR2 = 2
};

class Nodo {
private:
    EstadoNodo estado;
    int fila, columna;
    bool activo;
    
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
    
    // Utilidades
    void mostrarInfo() const;
};