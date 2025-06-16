#ifndef IAHEX_H
#define IAHEX_H

#include "Minijuegos/Hex/Hex.h"  // Incluir el archivo correcto
#include <vector>
#include <string>
#include <random>
#include <climits>

// Constante para el tamaño del tablero Hex
const int TAMAÑO_TABLERO = 11;

// Estructura para representar una posición en el tablero
struct Posicion {
    int fila, columna;
    
    Posicion(int f = 0, int c = 0);
    bool operator==(const Posicion& other) const;
};

class IAHex {
private:
    // Constantes para las estrategias
    static const int ESTRATEGIA_CENTRO = 0;
    static const int ESTRATEGIA_OFENSIVA = 1;
    static const int ESTRATEGIA_DEFENSIVA = 2;
    static const int ESTRATEGIA_CONEXION = 3;

    // Estructura del árbol de decisión
    struct NodoDecision {
        std::string caracteristica;
        double umbral;
        int estrategia;  // -1 si no es hoja
        NodoDecision* izquierdo;
        NodoDecision* derecho;
        
        NodoDecision();
        ~NodoDecision();
    };

    // Miembros privados
    int profundidadMaxima;
    int nivelDificultad;
    NodoDecision* arbolDecision;
    std::mt19937 generador;

    // Métodos privados para la IA
    void inicializarIA();
    void limpiarRecursos();
    void construirArbolDecision();
    
    // Extracción de características del juego
    std::vector<double> extraerCaracteristicas(const Hex& juego);
    int determinarEstrategia(const Hex& juego);
    int evaluarNodoDecision(NodoDecision* nodo, const std::vector<double>& caracteristicas);
    
    // Algoritmo minimax
    int minimax(Hex& juego, int profundidad, bool esMaximizador, int alfa = INT_MIN, int beta = INT_MAX);
    int evaluarPosicionHex(const Hex& juego);
    
    // Funciones de evaluación
    int calcularConectividad(const Hex& juego, EstadoCasilla jugador);
    int evaluarControlBordes(const Hex& juego);
    int evaluarPosicionesEstrategicas(const Hex& juego);
    
    // Estrategias específicas
    Posicion aplicarEstrategiaCentro(const Hex& juego);
    Posicion aplicarEstrategiaOfensiva(const Hex& juego);
    Posicion aplicarEstrategiaDefensiva(const Hex& juego);
    Posicion aplicarEstrategiaConexion(const Hex& juego);
    
    // Funciones auxiliares
    std::vector<Posicion> ordenarMovimientos(const Hex& juego, const std::vector<Posicion>& movimientos);
    bool amenazaInmediata(const Hex& juego);
    std::vector<Posicion> encontrarMovimientosCriticos(const Hex& juego);
    int evaluarPotencialConexion(const Hex& juego, const Posicion& pos);
    int calcularDistanciaAlObjetivo(const Posicion& pos, int tamaño);
    std::vector<Posicion> getMovimientosValidos(const Hex& juego);

public:
    // Constructor y destructor
    IAHex(int profundidad = 4, int dificultad = 1);
    ~IAHex();
    
    // Función principal para calcular el mejor movimiento
    Posicion calcularMejorMovimiento(Hex& juego);
};

#endif // IAHEX_H