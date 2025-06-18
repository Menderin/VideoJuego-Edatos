#pragma once
#include "Minijuegos/Minijuego.h"
#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

enum class EstadoCasilla {
    VACIA = 0,
    JUGADOR1 = 1,  // Rojo (conecta izquierda-derecha)
    JUGADOR2 = 2   // Azul (conecta arriba-abajo)
};

// Estructura para representar un nodo del grafo
struct NodoHex {
    int id;
    int fila, columna;
    EstadoCasilla estado;
    std::vector<int> vecinos;  // IDs de nodos vecinos
    
    NodoHex() : id(-1), fila(-1), columna(-1), estado(EstadoCasilla::VACIA) {}
    
    NodoHex(int _id, int _fila, int _col) 
        : id(_id), fila(_fila), columna(_col), estado(EstadoCasilla::VACIA) {}
};

// Clase para manejar el grafo del tablero Hex
class GrafoHex {
private:
    int tamañoTablero; // Nuevo atributo dinámico
    std::unordered_map<int, NodoHex> nodos;
    bool modoIA;

    // Nodos virtuales para los bordes
    int nodoVirtualIzquierda;
    int nodoVirtualDerecha;
    int nodoVirtualArriba;
    int nodoVirtualAbajo;

    int obtenerID(int fila, int col) const;
    void conectarVecinos();
    bool sonVecinos(int fila1, int col1, int fila2, int col2) const;

public:
    GrafoHex(int _tamaño = 11, bool _modoIA = false); // Cambiado: recibe tamaño

    void inicializar();
    void reiniciar();

    // Operaciones básicas
    bool colocarFicha(int fila, int col, EstadoCasilla jugador);
    EstadoCasilla obtenerEstado(int fila, int col) const;
    bool esPosicionValida(int fila, int col) const;
    bool esCasillaVacia(int fila, int col) const;

    // Detección de victoria usando DFS
    bool verificarVictoria(EstadoCasilla jugador);
    bool dfs(int nodoActual, int nodoDestino, EstadoCasilla jugador, std::unordered_set<int>& visitados);

    // Getters
    const std::unordered_map<int, NodoHex>& getNodos() const { return nodos; }
    int getTamañoTablero() const { return tamañoTablero; } // Cambiado

    // Para debugging
    void mostrarGrafo() const;
    void mostrarConexiones(int fila, int col) const;
};

class Hex : public MiniJuego {
private:
    GrafoHex grafo;
    int jugadorActual;
    bool juegoTerminado;
    int ganador;
    bool primerMovimiento;
    bool puedeRobar;
    int ultimaFila, ultimaColumna;
    bool modoIA;

    void mostrarTablero() const;
    void cambiarTurno();
    char obtenerSimbolo(EstadoCasilla estado) const;

public:
    // Constructor
    Hex(int tamaño = 11, bool _modoIA = false); // Cambiado: recibe tamaño

    // Métodos virtuales puros de MiniJuego
    bool procesarMovimiento(int jugador, int input) override;
    void mostrarEstado() const override;
    void reiniciar() override;

    // Métodos específicos de Hex
    void jugar();
    bool hacerMovimiento(int fila, int col);
    bool aplicarReglaRobo();
    int getGanador() const;
    bool estaTerminado() const;
    void mostrarInstrucciones() const;

    // Getters para integración con Nodo.cpp
    EstadoCasilla getCasilla(int fila, int col) const;
    int getJugadorActual() const;
    bool getPuedeRobar() const;
    const GrafoHex& getGrafo() const { return grafo; }

    int getTamañoTablero() const { return grafo.getTamañoTablero(); }

    bool esCasillaVacia(int fila, int col) const {
        return grafo.esCasillaVacia(fila, col);
    }
};