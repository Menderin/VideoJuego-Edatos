#include "IA/IaHexVnegamex.h"
#include <algorithm>
#include <climits>
#include <random>
#include <iostream>
#include <chrono>
#include <queue>
#include <set>
#include <unordered_set>

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

// Algoritmo de Dijkstra mejorado para calcular distancia mínima de conexión
int IAHex::dijkstra(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical) {
    const int TABLERO_SIZE = 11;
    const int INF = 999999;
    
    // Matriz de distancias mínimas
    std::vector<std::vector<int>> minDist(TABLERO_SIZE, std::vector<int>(TABLERO_SIZE, INF));
    std::vector<std::vector<bool>> visitado(TABLERO_SIZE, std::vector<bool>(TABLERO_SIZE, false));
    
    // Cola de prioridad: (distancia, fila, columna)
    std::priority_queue<std::tuple<int, int, int>, 
                       std::vector<std::tuple<int, int, int>>, 
                       std::greater<std::tuple<int, int, int>>> pq;
    
    // Inicializar con el borde de inicio
    if (esVertical) { // Norte-Sur (conectar fila 0 con fila 10)
        for (int c = 0; c < TABLERO_SIZE; c++) {
            EstadoCasilla celda = estadoJuego.getCasilla(0, c);
            int dist = 0;
            
            if (celda == jugador) {
                dist = 0; // Ya tenemos una ficha aquí
            } else if (celda == EstadoCasilla::VACIA) {
                dist = 1; // Necesitamos colocar una ficha
            } else {
                continue; // Bloqueado por el oponente
            }
            
            minDist[0][c] = dist;
            pq.push(std::make_tuple(dist, 0, c));
        }
    } else { // Este-Oeste (conectar columna 0 con columna 10)
        for (int r = 0; r < TABLERO_SIZE; r++) {
            EstadoCasilla celda = estadoJuego.getCasilla(r, 0);
            int dist = 0;
            
            if (celda == jugador) {
                dist = 0; // Ya tenemos una ficha aquí
            } else if (celda == EstadoCasilla::VACIA) {
                dist = 1; // Necesitamos colocar una ficha
            } else {
                continue; // Bloqueado por el oponente
            }
            
            minDist[r][0] = dist;
            pq.push(std::make_tuple(dist, r, 0));
        }
    }
    
    // Algoritmo de Dijkstra
    while (!pq.empty()) {
        auto [dist, r, c] = pq.top();
        pq.pop();
        
        if (visitado[r][c]) continue;
        visitado[r][c] = true;
        
        // Explorar vecinos
        for (const auto& vecino : obtenerVecinosHex(r, c)) {
            int nr = vecino.fila;
            int nc = vecino.columna;
            
            if (visitado[nr][nc]) continue;
            
            EstadoCasilla celda = estadoJuego.getCasilla(nr, nc);
            int nuevaDist = INF;
            
            if (celda == jugador) {
                nuevaDist = dist; // Movimiento gratis a nuestra ficha
            } else if (celda == EstadoCasilla::VACIA) {
                nuevaDist = dist + 1; // Cuesta 1 colocar una ficha
            } else {
                continue; // Bloqueado por el oponente
            }
            
            if (nuevaDist < minDist[nr][nc]) {
                minDist[nr][nc] = nuevaDist;
                pq.push(std::make_tuple(nuevaDist, nr, nc));
            }
        }
    }
    
    // Encontrar la distancia mínima al borde objetivo
    int resultado = INF;
    if (esVertical) { // Buscar en la última fila
        for (int c = 0; c < TABLERO_SIZE; c++) {
            resultado = std::min(resultado, minDist[TABLERO_SIZE - 1][c]);
        }
    } else { // Buscar en la última columna
        for (int r = 0; r < TABLERO_SIZE; r++) {
            resultado = std::min(resultado, minDist[r][TABLERO_SIZE - 1]);
        }
    }
    
    return resultado >= INF ? 999 : resultado;
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
    
    return movimientos;
}

// Función de evaluación mejorada
int IAHex::evaluarTablero(const Hex& estadoJuego) {
    // Verificar si el juego ha terminado
    if(estadoJuego.estaTerminado()) {
        int ganador = estadoJuego.getGanador();
        std::cout << "[DEBUG] JUEGO TERMINADO - Ganador: " << ganador << std::endl;
        if (ganador == 2) return 100000;  // IA gana
        if (ganador == 1) return -100000; // Humano gana
        return 0; // Empate (aunque no debería pasar en Hex)
    }

    // Calcular distancias usando Dijkstra
    int miDistancia = dijkstra(estadoJuego, EstadoCasilla::JUGADOR2, true); // IA: Norte-Sur
    int distanciaOponente = dijkstra(estadoJuego, EstadoCasilla::JUGADOR1, false); // Humano: Este-Oeste
    
    // Evaluar amenazas críticas
    int evaluacion = 0;
    
    // Si el oponente está muy cerca de ganar, penalizar fuertemente
    if (distanciaOponente <= 1) {
        evaluacion -= 50000;
    } else if (distanciaOponente <= 2) {
        evaluacion -= 10000;
    } else if (distanciaOponente <= 3) {
        evaluacion -= 2000;
    }
    
    // Si nosotros estamos cerca de ganar, bonificar
    if (miDistancia <= 1) {
        evaluacion += 50000;
    } else if (miDistancia <= 2) {
        evaluacion += 10000;
    } else if (miDistancia <= 3) {
        evaluacion += 2000;
    }
    
    // Evaluación base: diferencia de distancias
    evaluacion += (distanciaOponente - miDistancia) * 1000;
    
    // Bonificación por control del centro
    int centroControl = 0;
    for (int i = 4; i <= 6; i++) {
        for (int j = 4; j <= 6; j++) {
            EstadoCasilla casilla = estadoJuego.getCasilla(i, j);
            if (casilla == EstadoCasilla::JUGADOR2) {
                centroControl += 100;
            } else if (casilla == EstadoCasilla::JUGADOR1) {
                centroControl -= 100;
            }
        }
    }
    evaluacion += centroControl;
    
    std::cout << "[DEBUG] Evaluación - Mi dist: " << miDistancia 
              << ", Oponente dist: " << distanciaOponente 
              << ", Eval: " << evaluacion << std::endl;
    
    return evaluacion;
}

// Minimax con poda alpha-beta
int IAHex::minimax(Hex& estadoJuego, int profundidad, int alfa, int beta, bool esMaximizador) {
    nodosExploradosTemp++;
    
    // Condiciones de parada
    if (profundidad <= 0 || estadoJuego.estaTerminado() || nodosExploradosTemp > 2000) {
        return evaluarTablero(estadoJuego);
    }
    
    std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
    
    // Ordenar movimientos por prioridad heurística
    std::sort(movimientos.begin(), movimientos.end(), [&](const Posicion& a, const Posicion& b) {
        return evaluarPrioridadMovimiento(estadoJuego, a) > evaluarPrioridadMovimiento(estadoJuego, b);
    });
    
    // Limitar movimientos para mejor rendimiento
    if (movimientos.size() > 10) {
        movimientos.resize(10);
    }
    
    if (esMaximizador) { // Turno de la IA (JUGADOR2)
        int mejorValor = INT_MIN;
        for (const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if (copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                // Cambiar al turno del oponente
                int valor = minimax(copiaEstado, profundidad - 1, alfa, beta, false);
                mejorValor = std::max(mejorValor, valor);
                alfa = std::max(alfa, mejorValor);
                if (beta <= alfa) break; // Poda
            }
        }
        return mejorValor;
    } else { // Turno del humano (JUGADOR1)
        int mejorValor = INT_MAX;
        for (const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if (copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                // Cambiar al turno de la IA
                int valor = minimax(copiaEstado, profundidad - 1, alfa, beta, true);
                mejorValor = std::min(mejorValor, valor);
                beta = std::min(beta, mejorValor);
                if (beta <= alfa) break; // Poda
            }
        }
        return mejorValor;
    }
}

Posicion IAHex::calcularMejorMovimiento(Hex& estadoJuego) {
    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << "[DEBUG] ========== CALCULANDO MEJOR MOVIMIENTO ==========" << std::endl;
    
    try {
        std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
        
        if(movimientos.empty()) {
            std::cout << "[DEBUG] No hay movimientos disponibles" << std::endl;
            return Posicion(-1, -1);
        }

        // Verificar si hay movimientos de victoria inmediata
        for (const auto& mov : movimientos) {
            Hex copia = estadoJuego;
            if (copia.hacerMovimiento(mov.fila, mov.columna)) {
                if (copia.estaTerminado() && copia.getGanador() == 2) {
                    std::cout << "[DEBUG] *** MOVIMIENTO DE VICTORIA: (" << mov.fila << "," << mov.columna << ") ***" << std::endl;
                    return mov;
                }
            }
        }

        // Verificar amenazas críticas del oponente
        for (const auto& mov : movimientos) {
            Hex copia = estadoJuego;
            if (copia.hacerMovimiento(mov.fila, mov.columna)) {
                // Simular movimiento del oponente después
                std::vector<Posicion> movimientosOponente = obtenerMovimientosDisponibles(copia);
                for (const auto& movOp : movimientosOponente) {
                    Hex copia2 = copia;
                    if (copia2.hacerMovimiento(movOp.fila, movOp.columna)) {
                        if (copia2.estaTerminado() && copia2.getGanador() == 1) {
                            std::cout << "[DEBUG] *** BLOQUEANDO AMENAZA CRÍTICA: (" << movOp.fila << "," << movOp.columna << ") ***" << std::endl;
                            return movOp; // Bloquear la amenaza
                        }
                    }
                }
            }
        }

        // Usar minimax para encontrar el mejor movimiento
        int mejorValor = INT_MIN;
        Posicion mejorMovimiento = movimientos[0];
        nodosExploradosTemp = 0;
        
        // Ordenar movimientos por prioridad
        std::sort(movimientos.begin(), movimientos.end(), [&](const Posicion& a, const Posicion& b) {
            return evaluarPrioridadMovimiento(estadoJuego, a) > evaluarPrioridadMovimiento(estadoJuego, b);
        });
        
        // Limitar movimientos para mejor rendimiento
        if (movimientos.size() > 12) {
            movimientos.resize(12);
        }

        for (const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if (copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                int valor = minimax(copiaEstado, profundidadMaxima - 1, INT_MIN, INT_MAX, false);
                
                std::cout << "[DEBUG] Movimiento (" << mov.fila << "," << mov.columna 
                          << ") valor: " << valor << std::endl;
                
                if (valor > mejorValor) {
                    mejorValor = valor;
                    mejorMovimiento = mov;
                }
            }
        }

        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
        
        std::cout << "[DEBUG] ========== RESUMEN ==========" << std::endl;
        std::cout << "[DEBUG] Mejor movimiento: (" << mejorMovimiento.fila << "," << mejorMovimiento.columna << ")" << std::endl;
        std::cout << "[DEBUG] Valor: " << mejorValor << std::endl;
        std::cout << "[DEBUG] Nodos explorados: " << nodosExploradosTemp << std::endl;
        std::cout << "[DEBUG] Tiempo: " << duracion.count() << "ms" << std::endl;
        std::cout << "[DEBUG] ===============================" << std::endl;

        return mejorMovimiento;

    } catch(const std::exception& e) {
        std::cerr << "[ERROR] Error en calcularMejorMovimiento: " << e.what() << std::endl;
        std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
        return movimientos.empty() ? Posicion(-1, -1) : movimientos[0];
    }
}

// Métodos auxiliares mantenidos para compatibilidad
int IAHex::calcularDistanciaCamino(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical) {
    return dijkstra(estadoJuego, jugador, esVertical);
}

int IAHex::evaluarDefensa(const Hex& estadoJuego) {
    int distanciaOponente = dijkstra(estadoJuego, EstadoCasilla::JUGADOR1, false);
    if (distanciaOponente <= 1) return -50000;
    if (distanciaOponente <= 2) return -10000;
    return -(distanciaOponente * 1000);
}

int IAHex::evaluarAtaque(const Hex& estadoJuego) {
    int miDistancia = dijkstra(estadoJuego, EstadoCasilla::JUGADOR2, true);
    if (miDistancia <= 1) return 50000;
    if (miDistancia <= 2) return 10000;
    return -(miDistancia * 1000);
}

int IAHex::evaluarPrioridadMovimiento(const Hex& estadoJuego, const Posicion& mov) {
    Hex copia = estadoJuego;
    if (copia.hacerMovimiento(mov.fila, mov.columna)) {
        int evaluacionAntes = evaluarTablero(estadoJuego);
        int evaluacionDespues = evaluarTablero(copia);
        return evaluacionDespues - evaluacionAntes;
    }
    return 0;
}

Posicion IAHex::buscarMovimientoOfensivo(const Hex& estadoJuego, const std::vector<Posicion>& movimientos) {
    return Posicion(-1, -1); // Simplificado
}

bool IAHex::esAmenazaCritica(const Hex& estadoJuego, const Posicion& mov) {
    return false; // Simplificado
}

int IAHex::evaluarProgresoVictoria(const Hex& estadoJuego) {
    int distanciaNuestra = dijkstra(estadoJuego, EstadoCasilla::JUGADOR2, true);
    int distanciaOponente = dijkstra(estadoJuego, EstadoCasilla::JUGADOR1, false);
    
    std::cout << "[DEBUG] === Progreso Victoria ===" << std::endl;
    std::cout << "[DEBUG] Nuestra distancia (NORTE-SUR): " << distanciaNuestra << std::endl;
    std::cout << "[DEBUG] Distancia oponente (ESTE-OESTE): " << distanciaOponente << std::endl;
    
    return (distanciaOponente - distanciaNuestra) * 1000;
}