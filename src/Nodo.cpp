#include "Nodo.h"

// Constructores
Nodo::Nodo() : estado(EstadoNodo::VACIO), fila(-1), columna(-1), activo(true) {
}

Nodo::Nodo(int f, int c) : estado(EstadoNodo::VACIO), fila(f), columna(c), activo(true) {
}

// Getters
EstadoNodo Nodo::getEstado() const {
    return estado;
}

int Nodo::getFila() const {
    return fila;
}

int Nodo::getColumna() const {
    return columna;
}

bool Nodo::estaActivo() const {
    return activo;
}

bool Nodo::estaVacio() const {
    return estado == EstadoNodo::VACIO;
}

// Setters
void Nodo::setEstado(EstadoNodo nuevoEstado) {
    estado = nuevoEstado;
}

void Nodo::setPosicion(int f, int c) {
    fila = f;
    columna = c;
}

void Nodo::setActivo(bool nuevoActivo) {
    activo = nuevoActivo;
}

// Métodos de juego
bool Nodo::ocuparNodo(EstadoNodo jugador) {
    if (!activo || !estaVacio()) {
        return false;
    }
    estado = jugador;
    return true;
}

void Nodo::reiniciar() {
    estado = EstadoNodo::VACIO;
    activo = true;
}

// Utilidades
void Nodo::mostrarInfo() const {
    std::cout << "Nodo[" << fila << "][" << columna << "] - Estado: ";
    
    switch(estado) {
        case EstadoNodo::VACIO:
            std::cout << "VACIO";
            break;
        case EstadoNodo::JUGADOR1:
            std::cout << "JUGADOR1(X)";
            break;
        case EstadoNodo::JUGADOR2:
            std::cout << "JUGADOR2(O)";
            break;
    }
    
    std::cout << " - Activo: " << (activo ? "SI" : "NO") << std::endl;
}