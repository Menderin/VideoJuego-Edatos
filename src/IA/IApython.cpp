#include "IA/IaHex.h"
#include <algorithm>
#include <climits>
#include <random>
#include <iostream>
#include <chrono>
#include <queue>
#include <set>

IAHex::IAHex(int profundidad, int dificultadIA) 
    : profundidadMaxima(profundidad), dificultad(dificultadIA), nodosExploradosTemp(0) {
    profundidadMaxima = std::min(profundidad, 4);
    std::cout << "[DEBUG] IAHex inicializada - Profundidad: " << profundidadMaxima 
              << ", Dificultad: " << dificultadIA << std::endl;
    std::cout << "[DEBUG] IA (Jugador 2) debe conectar NORTE-SUR (vertical)" << std::endl;
    std::cout << "[DEBUG] Humano (Jugador 1) debe conectar ESTE-OESTE (horizontal)" << std::endl;
}

void IAHex::setDificultad(int nuevaDificultad) {
    int anterior = dificultad;
    dificultad = std::clamp(nuevaDificultad, 1, 100);
    std::cout << "[DEBUG] Dificultad cambiada de " << anterior << " a " << dificultad << std::endl;
}

void IAHex::setProfundidadMaxima(int nuevaProfundidad) {
    int anterior = profundidadMaxima;
    profundidadMaxima = std::clamp(nuevaProfundidad, 1, 4);
    std::cout << "[DEBUG] Profundidad cambiada de " << anterior << " a " << profundidadMaxima << std::endl;
}

// Función auxiliar para obtener vecinos en tablero hexagonal
std::vector<Posicion> IAHex::obtenerVecinosHex(int fila, int columna) {
    std::vector<Posicion> vecinos;
    const int TABLERO_SIZE = 11;
    
    // En un tablero hexagonal, cada casilla tiene hasta 6 vecinos
    int direcciones[6][2] = {
        {-1, 0}, {-1, 1},  // Arriba-izquierda, Arriba-derecha
        {0, -1}, {0, 1},   // Izquierda, Derecha
        {1, -1}, {1, 0}    // Abajo-izquierda, Abajo-derecha
    };
    
    for(int i = 0; i < 6; i++) {
        int nuevaFila = fila + direcciones[i][0];
        int nuevaCol = columna + direcciones[i][1];
        
        if(nuevaFila >= 0 && nuevaFila < TABLERO_SIZE && 
           nuevaCol >= 0 && nuevaCol < TABLERO_SIZE) {
            vecinos.push_back(Posicion(nuevaFila, nuevaCol));
        }
    }
    
    return vecinos;
}

// Algoritmo de Dijkstra para calcular distancia mínima de conexión
int IAHex::dijkstra(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical) {
    const int TABLERO_SIZE = 11;
    const int INF = 999999;
    
    // Matriz de distancias mínimas
    std::vector<std::vector<int>> minDist(TABLERO_SIZE, std::vector<int>(TABLERO_SIZE, INF));
    
    // Cola de prioridad: (distancia, fila, columna)
    std::priority_queue<std::tuple<int, int, int>, 
                       std::vector<std::tuple<int, int, int>>, 
                       std::greater<std::tuple<int, int, int>>> heap;
    
    // Inicializar heap con todas las celdas de la primera fila/columna
    if (esVertical) { // Norte-Sur (primera fila)
        for (int c = 0; c < TABLERO_SIZE; c++) {
            EstadoCasilla celda = estadoJuego.getCasilla(0, c);
            int nuevaDist = INF;
            
            if (celda == EstadoCasilla::VACIA) {
                nuevaDist = 1; // Celda vacía cuesta 1
            } else if (celda == jugador) {
                nuevaDist = 0; // Nuestro color es gratis
            } else {
                continue; // Color enemigo no es alcanzable
            }
            
            minDist[0][c] = nuevaDist;
            heap.push(std::make_tuple(nuevaDist, 0, c));
        }
    } else { // Este-Oeste (primera columna)
        for (int r = 0; r < TABLERO_SIZE; r++) {
            EstadoCasilla celda = estadoJuego.getCasilla(r, 0);
            int nuevaDist = INF;
            
            if (celda == EstadoCasilla::VACIA) {
                nuevaDist = 1; // Celda vacía cuesta 1
            } else if (celda == jugador) {
                nuevaDist = 0; // Nuestro color es gratis
            } else {
                continue; // Color enemigo no es alcanzable
            }
            
            minDist[r][0] = nuevaDist;
            heap.push(std::make_tuple(nuevaDist, r, 0));
        }
    }
    
    // Procesar heap
    while (!heap.empty()) {
        auto [dist, r, c] = heap.top();
        heap.pop();
        
        // Explorar vecinos
        for (const auto& vecino : obtenerVecinosHex(r, c)) {
            int nr = vecino.fila;
            int nc = vecino.columna;
            
            EstadoCasilla celda = estadoJuego.getCasilla(nr, nc);
            int nuevaDist = INF;
            
            if (celda == EstadoCasilla::VACIA) {
                nuevaDist = dist + 1;
            } else if (celda == jugador) {
                nuevaDist = dist;
            } else {
                continue; // Color enemigo no es alcanzable
            }
            
            if (minDist[nr][nc] > nuevaDist) {
                minDist[nr][nc] = nuevaDist;
                
                // No agregar al heap si ya llegamos al borde objetivo
                bool enBordeObjetivo = false;
                if (esVertical && nr == TABLERO_SIZE - 1) {
                    enBordeObjetivo = true;
                } else if (!esVertical && nc == TABLERO_SIZE - 1) {
                    enBordeObjetivo = true;
                }
                
                if (!enBordeObjetivo) {
                    heap.push(std::make_tuple(nuevaDist, nr, nc));
                }
            }
        }
    }
    
    // Encontrar distancia mínima al borde objetivo
    int resultado = INF;
    if (esVertical) { // Buscar en última fila
        for (int c = 0; c < TABLERO_SIZE; c++) {
            resultado = std::min(resultado, minDist[TABLERO_SIZE - 1][c]);
        }
    } else { // Buscar en última columna
        for (int r = 0; r < TABLERO_SIZE; r++) {
            resultado = std::min(resultado, minDist[r][TABLERO_SIZE - 1]);
        }
    }
    
    return resultado == INF ? 999 : resultado;
}

std::vector<Posicion> IAHex::obtenerMovimientosDisponibles(const Hex& estadoJuego) {
    std::vector<Posicion> movimientos;
    const int TABLERO_SIZE = 11;
    
    for(int i = 0; i < TABLERO_SIZE; i++) {
        for(int j = 0; j < TABLERO_SIZE; j++) {
            if(estadoJuego.getCasilla(i, j) == EstadoCasilla::VACIA) {
                movimientos.push_back(Posicion(i, j));
            }
        }
    }
    
    // Barajear movimientos para diversidad
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(movimientos.begin(), movimientos.end(), g);
    
    return movimientos;
}

// Función de evaluación usando diferencia de distancias Dijkstra
int IAHex::evaluarTablero(const Hex& estadoJuego) {
    if(estadoJuego.estaTerminado()) {
        int ganador = estadoJuego.getGanador();
        std::cout << "[DEBUG] JUEGO TERMINADO - Ganador: " << ganador << std::endl;
        return ganador == 2 ? 10000 : -10000;
    }

    // Calcular distancias usando Dijkstra
    int miDistancia = dijkstra(estadoJuego, EstadoCasilla::JUGADOR2, true); // IA: Norte-Sur
    int distanciaOponente = dijkstra(estadoJuego, EstadoCasilla::JUGADOR1, false); // Humano: Este-Oeste
    
    // La evaluación es la diferencia: mejor si el oponente está más lejos
    int evaluacion = distanciaOponente - miDistancia;
    
    std::cout << "[DEBUG] Evaluación - Mi distancia: " << miDistancia 
              << ", Distancia oponente: " << distanciaOponente 
              << ", Evaluación: " << evaluacion << std::endl;
    
    return evaluacion * 100; // Escalar para mayor precisión
}

// Implementación de Negamax (equivalente a Minimax pero más eficiente)
std::pair<int, Posicion> IAHex::negamax(Hex& estadoJuego, int alpha, int beta, int profundidad, bool esJugadorIA) {
    nodosExploradosTemp++;
    
    // Condiciones de parada
    if (profundidad <= 0 || estadoJuego.estaTerminado() || nodosExploradosTemp > 3000) {
        int valor = evaluarTablero(estadoJuego);
        // En negamax, siempre evaluamos desde la perspectiva del jugador actual
        return std::make_pair(esJugadorIA ? valor : -valor, Posicion(-1, -1));
    }
    
    std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
    
    // Limitar movimientos para mejor rendimiento
    if (movimientos.size() > 12) {
        movimientos.resize(12);
    }
    
    if (movimientos.empty()) {
        return std::make_pair(evaluarTablero(estadoJuego), Posicion(-1, -1));
    }
    
    Posicion mejorMovimiento = movimientos[0];
    int mejorValor = INT_MIN;
    
    for (const auto& mov : movimientos) {
        Hex copiaEstado = estadoJuego;
        if (copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
            // Llamada recursiva con jugadores intercambiados
            auto [valor, _] = negamax(copiaEstado, -beta, -alpha, profundidad - 1, !esJugadorIA);
            valor = -valor; // Negar el valor porque cambiamos de perspectiva
            
            if (valor > mejorValor) {
                mejorValor = valor;
                mejorMovimiento = mov;
            }
            
            alpha = std::max(alpha, valor);
            if (alpha >= beta) {
                break; // Poda alpha-beta
            }
        }
    }
    
    return std::make_pair(mejorValor, mejorMovimiento);
}

Posicion IAHex::calcularMejorMovimiento(Hex& estadoJuego) {
    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << "[DEBUG] ========== CALCULANDO MEJOR MOVIMIENTO (NEGAMAX) ==========" << std::endl;
    
    try {
        std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
        
        if(movimientos.empty()) {
            return Posicion(-1, -1);
        }

        nodosExploradosTemp = 0;
        
        // Usar negamax para encontrar el mejor movimiento
        auto [valor, mejorMovimiento] = negamax(estadoJuego, INT_MIN, INT_MAX, profundidadMaxima, true);

        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
        
        std::cout << "[DEBUG] ========== RESUMEN NEGAMAX ==========" << std::endl;
        std::cout << "[DEBUG] Mejor movimiento: (" << mejorMovimiento.fila << "," << mejorMovimiento.columna << ")" << std::endl;
        std::cout << "[DEBUG] Valor: " << valor << std::endl;
        std::cout << "[DEBUG] Nodos explorados: " << nodosExploradosTemp << std::endl;
        std::cout << "[DEBUG] Tiempo: " << duracion.count() << "ms" << std::endl;
        std::cout << "[DEBUG] ===============================" << std::endl;

        // Si no se encontró un movimiento válido, usar el primero disponible
        if (mejorMovimiento.fila == -1 && !movimientos.empty()) {
            mejorMovimiento = movimientos[0];
            std::cout << "[DEBUG] Usando movimiento por defecto: (" << mejorMovimiento.fila << "," << mejorMovimiento.columna << ")" << std::endl;
        }

        return mejorMovimiento;

    } catch(const std::exception& e) {
        std::cerr << "[ERROR] Error en calcularMejorMovimiento: " << e.what() << std::endl;
        std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
        return movimientos.empty() ? Posicion(-1, -1) : movimientos[0];
    }
}

// Métodos legacy mantenidos para compatibilidad pero simplificados
int IAHex::calcularDistanciaCamino(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical) {
    return dijkstra(estadoJuego, jugador, esVertical);
}

int IAHex::evaluarDefensa(const Hex& estadoJuego) {
    int distanciaOponente = dijkstra(estadoJuego, EstadoCasilla::JUGADOR1, false);
    return -(distanciaOponente * 100); // Negativo porque menor distancia es peor para nosotros
}

int IAHex::evaluarAtaque(const Hex& estadoJuego) {
    int miDistancia = dijkstra(estadoJuego, EstadoCasilla::JUGADOR2, true);
    return -(miDistancia * 100); // Negativo porque menor distancia es mejor para nosotros
}

int IAHex::evaluarPrioridadMovimiento(const Hex& estadoJuego, const Posicion& mov) {
    // Simplificado - usar evaluación de tablero después de hacer el movimiento
    Hex copia = estadoJuego;
    if (copia.hacerMovimiento(mov.fila, mov.columna)) {
        return evaluarTablero(copia);
    }
    return 0;
}

Posicion IAHex::buscarMovimientoOfensivo(const Hex& estadoJuego, const std::vector<Posicion>& movimientos) {
    // Simplificado - el negamax ya maneja esto
    return Posicion(-1, -1);
}

bool IAHex::esAmenazaCritica(const Hex& estadoJuego, const Posicion& mov) {
    // Simplificado - el negamax ya maneja esto
    return false;
}

// Método legacy mantenido para compatibilidad
int IAHex::minimax(Hex& estadoJuego, int profundidad, int alfa, int beta, bool esMaximizador) {
    // Redirigir a negamax
    auto [valor, _] = negamax(estadoJuego, alfa, beta, profundidad, esMaximizador);
    return valor;
}

int IAHex::evaluarProgresoVictoria(const Hex& estadoJuego) {
    int distanciaNuestra = dijkstra(estadoJuego, EstadoCasilla::JUGADOR2, true);
    int distanciaOponente = dijkstra(estadoJuego, EstadoCasilla::JUGADOR1, false);
    
    std::cout << "[DEBUG] === Progreso Victoria ===" << std::endl;
    std::cout << "[DEBUG] Nuestra distancia (NORTE-SUR): " << distanciaNuestra << std::endl;
    std::cout << "[DEBUG] Distancia oponente (ESTE-OESTE): " << distanciaOponente << std::endl;
    
    return (distanciaOponente - distanciaNuestra) * 100;
}