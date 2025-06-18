#pragma once
#include "MiniJuegos/Hex/Hex.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <unordered_set>

// Estructura para representar una posición en el tablero
struct Posicion {
    int fila;
    int columna;
    
    Posicion(int f = -1, int c = -1) : fila(f), columna(c) {}
};

/*// Hash function para EstadoHash (declarado en el .cpp)
struct EstadoHash {
    size_t operator()(const std::vector<std::vector<EstadoCasilla>>& estado) const;
};*/

class IAHex {
private:
    int profundidadMaxima;
    int dificultad; // 1-100, afecta qué tan "inteligente" juega la IA
    
    // VARIABLES PARA OPTIMIZACIONES
    static int tablaPosicional[11][11];
    static bool tablaInicializada;
    
    // Variable para contar nodos explorados
    int nodosExploradosTemp;
    
    // Métodos privados para el algoritmo minimax original
    int minimax(Hex& estadoJuego, int profundidad, int alfa, int beta, bool esMaximizador);
    int evaluarDefensa(const Hex& estadoJuego);
    int evaluarAtaque(const Hex& estadoJuego);
    int evaluarTablero(const Hex& estadoJuego);
    std::vector<Posicion> obtenerMovimientosDisponibles(const Hex& estadoJuego);
    bool evaluarConexion(const Hex& estadoJuego, bool esJugador1);
    int evaluarCentro(const Hex& estadoJuego);
    int evaluarConectividad(const Hex& estadoJuego, bool esJugador1);
    
    // Funciones auxiliares para análisis del juego original
    Posicion buscarMovimientoOfensivo(const Hex& estadoJuego, const std::vector<Posicion>& movimientos);
    bool esAmenazaCritica(const Hex& estadoJuego, const Posicion& mov);
    int evaluarPrioridadMovimiento(const Hex& estadoJuego, const Posicion& mov);
    int evaluarProgresoVictoria(const Hex& estadoJuego);
    std::vector<Posicion> obtenerVecinosHex(int fila, int columna);
    int calcularDistanciaCamino(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical);
    
    // MÉTODOS ORIGINALES PARA OPTIMIZACIONES
    int evaluarPrioridadMovimientoRapido(const Hex& estadoJuego, const Posicion& mov);
    size_t calcularHashTablero(const Hex& estadoJuego);
    Posicion calcularMejorMovimientoOptimizado(Hex &estadoJuego);

    // NUEVOS MÉTODOS OPTIMIZADOS (de la implementación actual)
    
    /**
     * Versión optimizada del cálculo de distancia con cache y early termination
     * @param estadoJuego Estado actual del juego
     * @param jugador Jugador para el cual calcular la distancia
     * @param esVertical true para conexión vertical (Norte-Sur), false para horizontal (Este-Oeste)
     * @return Distancia mínima al objetivo
     */
    int calcularDistanciaCaminoOptimizada(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical);
    
    /**
     * Obtiene movimientos candidatos inteligentemente, limitando el espacio de búsqueda
     * @param estadoJuego Estado actual del juego
     * @return Vector de posiciones candidatas ordenadas por prioridad
     */
    std::vector<Posicion> obtenerMovimientosCandidatos(const Hex& estadoJuego);
    
    /**
     * Función de evaluación simplificada y rápida para priorizar movimientos
     * @param estadoJuego Estado actual del juego
     * @param mov Movimiento a evaluar
     * @return Puntuación de prioridad del movimiento
     */
    int evaluarPrioridadSimple(const Hex& estadoJuego, const Posicion& mov);
    
    /**
     * Evaluación rápida del tablero optimizada para velocidad
     * @param estadoJuego Estado actual del juego
     * @return Puntuación del tablero desde la perspectiva de la IA
     */
    int evaluarTableroRapido(const Hex& estadoJuego);
    
    /**
     * Minimax optimizado con límite de tiempo y mejor poda alfa-beta
     * @param estadoJuego Estado actual del juego (referencia no const para permitir modificaciones temporales)
     * @param profundidad Profundidad restante de búsqueda
     * @param alfa Valor alfa para poda alfa-beta
     * @param beta Valor beta para poda alfa-beta
     * @param esMaximizador true si es turno de maximizar, false si es de minimizar
     * @param tiempoLimite Tiempo límite para la búsqueda
     * @return Mejor puntuación encontrada
     */
    int minimaxOptimizado(Hex& estadoJuego, int profundidad, int alfa, int beta, 
                         bool esMaximizador, std::chrono::steady_clock::time_point tiempoLimite);
    
    /**
     * Limpia el cache de distancias para evitar uso excesivo de memoria
     */
    void limpiarCache();

public:
    /**
     * Constructor de la IA de Hex
     * @param profundidad Profundidad máxima de búsqueda (limitada a 3 para rendimiento)
     * @param dificultadIA Nivel de dificultad de la IA (1-100)
     */
    IAHex(int profundidad = 4, int dificultadIA = 75);
    
    /**
     * Método principal para calcular el mejor movimiento usando algoritmos optimizados
     * Incluye detección de amenazas críticas, búsqueda de victoria inmediata y minimax con tiempo limitado
     * @param estadoJuego Estado actual del juego
     * @return Mejor posición calculada para jugar
     */
    Posicion calcularMejorMovimiento(Hex& estadoJuego);
    
    // Métodos auxiliares para configuración
    void setDificultad(int nuevaDificultad);
    void setProfundidadMaxima(int nuevaProfundidad);
};