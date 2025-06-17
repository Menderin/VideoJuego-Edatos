#include "IA/IaHex.h"
#include <algorithm>
#include <climits>
#include <random>
#include <iostream>
#include <chrono>
#include <queue>
#include <unordered_set>
#include <unordered_map>

// Cache para distancias calculadas
struct EstadoHash {
    size_t operator()(const std::vector<std::vector<EstadoCasilla>>& estado) const {
        size_t hash = 0;
        for(const auto& fila : estado) {
            for(const auto& casilla : fila) {
                hash ^= std::hash<int>{}(static_cast<int>(casilla)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
        }
        return hash;
    }
};

// Cache global para distancias
std::unordered_map<size_t, int> cacheDistancias;

IAHex::IAHex(int profundidad, int dificultadIA) 
    : profundidadMaxima(profundidad), dificultad(dificultadIA), nodosExploradosTemp(0) {
    profundidadMaxima = std::min(profundidad, 3); // Reducir profundidad máxima
    std::cout << "[DEBUG] IAHex inicializada - Profundidad: " << profundidadMaxima 
              << ", Dificultad: " << dificultadIA << std::endl;
}

// Versión optimizada con cache y early termination
int IAHex::calcularDistanciaCaminoOptimizada(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical) {
    const int TABLERO_SIZE = 11;
    
    // Crear hash del estado para cache
    size_t hashEstado = 0;
    for(int i = 0; i < TABLERO_SIZE; i++) {
        for(int j = 0; j < TABLERO_SIZE; j++) {
            hashEstado ^= std::hash<int>{}(static_cast<int>(estadoJuego.getCasilla(i, j))) + 
                         std::hash<int>{}(i * TABLERO_SIZE + j) + 0x9e3779b9;
        }
    }
    hashEstado ^= std::hash<bool>{}(esVertical) + std::hash<int>{}(static_cast<int>(jugador));
    
    // Verificar cache
    auto it = cacheDistancias.find(hashEstado);
    if(it != cacheDistancias.end()) {
        return it->second;
    }
    
    // Usar vector<bool> más eficiente para visitados
    std::vector<std::vector<bool>> visitados(TABLERO_SIZE, std::vector<bool>(TABLERO_SIZE, false));
    std::queue<std::pair<std::pair<int, int>, int>> cola;
    
    // Agregar casillas del borde inicial
    if(esVertical) { // Norte a Sur (IA)
        for(int j = 0; j < TABLERO_SIZE; j++) {
            if(estadoJuego.getCasilla(0, j) == jugador) {
                cola.push({{0, j}, 0});
                visitados[0][j] = true;
            }
        }
    } else { // Este a Oeste (Humano)
        for(int i = 0; i < TABLERO_SIZE; i++) {
            if(estadoJuego.getCasilla(i, 0) == jugador) {
                cola.push({{i, 0}, 0});
                visitados[i][0] = true;
            }
        }
    }
    
    int resultado = 999;
    
    while(!cola.empty()) {
        auto actual = cola.front();
        cola.pop();
        
        int fila = actual.first.first;
        int col = actual.first.second;
        int distancia = actual.second;
        
        // Early termination si encontramos una distancia muy buena
        if(distancia >= 8) break;
        
        // Verificar si llegamos al borde opuesto
        if((esVertical && fila == TABLERO_SIZE - 1) || (!esVertical && col == TABLERO_SIZE - 1)) {
            resultado = distancia;
            break;
        }
        
        // Explorar vecinos (versión optimizada)
        static const int direcciones[6][2] = {
            {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}
        };
        
        for(int i = 0; i < 6; i++) {
            int nuevaFila = fila + direcciones[i][0];
            int nuevaCol = col + direcciones[i][1];
            
            if(nuevaFila >= 0 && nuevaFila < TABLERO_SIZE && 
               nuevaCol >= 0 && nuevaCol < TABLERO_SIZE && 
               !visitados[nuevaFila][nuevaCol]) {
                
                EstadoCasilla casillaVecino = estadoJuego.getCasilla(nuevaFila, nuevaCol);
                
                if(casillaVecino == jugador) {
                    cola.push({{nuevaFila, nuevaCol}, distancia});
                    visitados[nuevaFila][nuevaCol] = true;
                } else if(casillaVecino == EstadoCasilla::VACIA) {
                    cola.push({{nuevaFila, nuevaCol}, distancia + 1});
                    visitados[nuevaFila][nuevaCol] = true;
                }
            }
        }
    }
    
    // Guardar en cache
    cacheDistancias[hashEstado] = resultado;
    
    return resultado;
}

// Selección inteligente de movimientos candidatos
std::vector<Posicion> IAHex::obtenerMovimientosCandidatos(const Hex& estadoJuego) {
    std::vector<Posicion> movimientos;
    std::vector<std::pair<Posicion, int>> movimientosConPrioridad;
    const int TABLERO_SIZE = 11;
    
    // Primera pasada: encontrar casillas vacías adyacentes a fichas existentes
    std::unordered_set<std::pair<int, int>, std::hash<std::pair<int, int>>> candidatos;
    
    for(int i = 0; i < TABLERO_SIZE; i++) {
        for(int j = 0; j < TABLERO_SIZE; j++) {
            if(estadoJuego.getCasilla(i, j) != EstadoCasilla::VACIA) {
                // Agregar vecinos vacíos como candidatos
                static const int direcciones[6][2] = {
                    {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}
                };
                
                for(int d = 0; d < 6; d++) {
                    int ni = i + direcciones[d][0];
                    int nj = j + direcciones[d][1];
                    
                    if(ni >= 0 && ni < TABLERO_SIZE && nj >= 0 && nj < TABLERO_SIZE &&
                       estadoJuego.getCasilla(ni, nj) == EstadoCasilla::VACIA) {
                        candidatos.insert({ni, nj});
                    }
                }
            }
        }
    }
    
    // Si no hay fichas en el tablero, empezar en el centro
    if(candidatos.empty()) {
        candidatos.insert({5, 5});
        candidatos.insert({4, 5});
        candidatos.insert({6, 5});
        candidatos.insert({5, 4});
        candidatos.insert({5, 6});
    }
    
    // Evaluar candidatos con función de prioridad simplificada
    for(const auto& pos : candidatos) {
        int prioridad = evaluarPrioridadSimple(estadoJuego, Posicion(pos.first, pos.second));
        movimientosConPrioridad.push_back({Posicion(pos.first, pos.second), prioridad});
    }
    
    // Ordenar por prioridad
    std::sort(movimientosConPrioridad.begin(), movimientosConPrioridad.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Extraer solo las posiciones, limitando a los mejores
    int limite = std::min(8, static_cast<int>(movimientosConPrioridad.size()));
    for(int i = 0; i < limite; i++) {
        movimientos.push_back(movimientosConPrioridad[i].first);
    }
    
    return movimientos;
}

// Función de evaluación simplificada y rápida
int IAHex::evaluarPrioridadSimple(const Hex& estadoJuego, const Posicion& mov) {
    int prioridad = 0;
    const int i = mov.fila;
    const int j = mov.columna;
    
    // Conectividad con fichas propias (más rápido)
    int conexionesIA = 0;
    int conexionesOponente = 0;
    
    static const int direcciones[6][2] = {
        {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}
    };
    
    for(int d = 0; d < 6; d++) {
        int ni = i + direcciones[d][0];
        int nj = j + direcciones[d][1];
        
        if(ni >= 0 && ni < 11 && nj >= 0 && nj < 11) {
            EstadoCasilla casilla = estadoJuego.getCasilla(ni, nj);
            if(casilla == EstadoCasilla::JUGADOR2) {
                conexionesIA++;
            } else if(casilla == EstadoCasilla::JUGADOR1) {
                conexionesOponente++;
            }
        }
    }
    
    prioridad += conexionesIA * 100;
    prioridad += conexionesOponente * 80; // Bloquear también es importante
    
    // Bonificación por cercanía al centro
    int distanciaCentro = abs(i - 5) + abs(j - 5);
    prioridad += (10 - distanciaCentro) * 5;
    
    // Bonificación por estar en el camino vertical (objetivo de la IA)
    if(i <= 2 || i >= 8) prioridad += 30; // Cerca de los bordes norte/sur
    
    return prioridad;
}

// Evaluación de tablero optimizada
int IAHex::evaluarTableroRapido(const Hex& estadoJuego) {
    if(estadoJuego.estaTerminado()) {
        int ganador = estadoJuego.getGanador();
        return ganador == 2 ? 5000 : -5000;
    }

    int distanciaIA = calcularDistanciaCaminoOptimizada(estadoJuego, EstadoCasilla::JUGADOR2, true);
    int distanciaOponente = calcularDistanciaCaminoOptimizada(estadoJuego, EstadoCasilla::JUGADOR1, false);
    
    int puntuacion = (distanciaOponente - distanciaIA) * 100;
    
    // Bonificaciones simples
    if(distanciaIA <= 2) puntuacion += 1000;
    else if(distanciaIA <= 4) puntuacion += 300;
    
    if(distanciaOponente <= 2) puntuacion -= 1200;
    else if(distanciaOponente <= 4) puntuacion -= 400;
    
    return puntuacion;
}

// Minimax optimizado con time limit y mejor poda
int IAHex::minimaxOptimizado(Hex& estadoJuego, int profundidad, int alfa, int beta, 
                            bool esMaximizador, std::chrono::steady_clock::time_point tiempoLimite) {
    nodosExploradosTemp++;
    
    // Verificar límite de tiempo
    if(std::chrono::steady_clock::now() > tiempoLimite) {
        return evaluarTableroRapido(estadoJuego);
    }
    
    if(profundidad <= 0 || estadoJuego.estaTerminado() || nodosExploradosTemp > 2000) {
        return evaluarTableroRapido(estadoJuego);
    }

    std::vector<Posicion> movimientos = obtenerMovimientosCandidatos(estadoJuego);
    
    if(esMaximizador) {
        int mejorValor = INT_MIN;
        for(const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                int valor = minimaxOptimizado(copiaEstado, profundidad - 1, alfa, beta, false, tiempoLimite);
                mejorValor = std::max(mejorValor, valor);
                alfa = std::max(alfa, mejorValor);
                if(beta <= alfa) break; // Poda alfa-beta
            }
        }
        return mejorValor;
    } else {
        int mejorValor = INT_MAX;
        for(const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                int valor = minimaxOptimizado(copiaEstado, profundidad - 1, alfa, beta, true, tiempoLimite);
                mejorValor = std::min(mejorValor, valor);
                beta = std::min(beta, mejorValor);
                if(beta <= alfa) break; // Poda alfa-beta
            }
        }
        return mejorValor;
    }
}

// Función principal optimizada
Posicion IAHex::calcularMejorMovimiento(Hex& estadoJuego) {
    auto inicio = std::chrono::steady_clock::now();
    auto tiempoLimite = inicio + std::chrono::milliseconds(800); // Reducir tiempo límite
    
    std::cout << "[DEBUG] Calculando movimiento optimizado..." << std::endl;
    
    std::vector<Posicion> movimientos = obtenerMovimientosCandidatos(estadoJuego);
    
    if(movimientos.empty()) {
        return Posicion(5, 5); // Centro por defecto
    }

    // Verificar amenazas críticas rápidamente
    for(const auto& mov : movimientos) {
        Hex copia = estadoJuego;
        if(copia.hacerMovimiento(mov.fila, mov.columna)) {
            int distOponente = calcularDistanciaCaminoOptimizada(copia, EstadoCasilla::JUGADOR1, false);
            if(distOponente == 0) { // El oponente ganaría
                // Verificar si nosotros podemos bloquear
                Hex copia2 = estadoJuego;
                copia2.hacerMovimiento(mov.fila, mov.columna); // Nuestro movimiento
                int distOponenteBloqueada = calcularDistanciaCaminoOptimizada(copia2, EstadoCasilla::JUGADOR1, false);
                if(distOponenteBloqueada > 0) {
                    std::cout << "[DEBUG] DEFENSA CRÍTICA: (" << mov.fila << "," << mov.columna << ")" << std::endl;
                    return mov;
                }
            }
        }
    }

    // Buscar victoria inmediata
    for(const auto& mov : movimientos) {
        Hex copia = estadoJuego;
        if(copia.hacerMovimiento(mov.fila, mov.columna)) {
            int nuestraDistancia = calcularDistanciaCaminoOptimizada(copia, EstadoCasilla::JUGADOR2, true);
            if(nuestraDistancia == 0) {
                std::cout << "[DEBUG] VICTORIA INMEDIATA: (" << mov.fila << "," << mov.columna << ")" << std::endl;
                return mov;
            }
        }
    }

    // Minimax con tiempo limitado
    int mejorValor = INT_MIN;
    Posicion mejorMovimiento = movimientos[0];
    
    for(const auto& mov : movimientos) {
        if(std::chrono::steady_clock::now() > tiempoLimite) break;
        
        Hex copiaEstado = estadoJuego;
        if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
            nodosExploradosTemp = 0;
            int valor = minimaxOptimizado(copiaEstado, profundidadMaxima - 1, INT_MIN, INT_MAX, false, tiempoLimite);
            
            if(valor > mejorValor) {
                mejorValor = valor;
                mejorMovimiento = mov;
            }
        }
    }

    auto fin = std::chrono::steady_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
    
    std::cout << "[DEBUG] Movimiento: (" << mejorMovimiento.fila << "," << mejorMovimiento.columna 
              << ") Tiempo: " << duracion.count() << "ms" << std::endl;

    return mejorMovimiento;
}

// Limpiar cache periódicamente para evitar uso excesivo de memoria
void IAHex::limpiarCache() {
    if(cacheDistancias.size() > 1000) {
        cacheDistancias.clear();
        std::cout << "[DEBUG] Cache limpiado" << std::endl;
    }
}