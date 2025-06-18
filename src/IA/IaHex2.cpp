#include "IA/IaHexVnegamex.h"
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
    const int TABLERO_SIZE = 7;
    
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

// Calcular distancia mínima entre dos bordes usando BFS
int IAHex::calcularDistanciaCamino(const Hex& estadoJuego, EstadoCasilla jugador, bool esVertical) {
    const int TABLERO_SIZE = 7;
    std::queue<std::pair<Posicion, int>> cola;
    std::set<std::pair<int, int>> visitados;
    
    // Agregar todas las casillas del borde inicial
    if(esVertical) { // Norte a Sur (IA)
        for(int j = 0; j < TABLERO_SIZE; j++) {
            if(estadoJuego.getCasilla(0, j) == jugador) {
                cola.push({Posicion(0, j), 0});
                visitados.insert({0, j});
            }
        }
    } else { // Este a Oeste (Humano)
        for(int i = 0; i < TABLERO_SIZE; i++) {
            if(estadoJuego.getCasilla(i, 0) == jugador) {
                cola.push({Posicion(i, 0), 0});
                visitados.insert({i, 0});
            }
        }
    }
    
    while(!cola.empty()) {
        auto actual = cola.front();
        cola.pop();
        
        Posicion pos = actual.first;
        int distancia = actual.second;
        
        // Verificar si llegamos al borde opuesto
        bool llegadaObjetivo = false;
        if(esVertical && pos.fila == TABLERO_SIZE - 1) { // Llegó al sur
            llegadaObjetivo = true;
        } else if(!esVertical && pos.columna == TABLERO_SIZE - 1) { // Llegó al este
            llegadaObjetivo = true;
        }
        
        if(llegadaObjetivo) {
            return distancia;
        }
        
        // Explorar vecinos
        for(const auto& vecino : obtenerVecinosHex(pos.fila, pos.columna)) {
            if(visitados.find({vecino.fila, vecino.columna}) == visitados.end()) {
                EstadoCasilla casillaVecino = estadoJuego.getCasilla(vecino.fila, vecino.columna);
                
                if(casillaVecino == jugador) {
                    // Mismo jugador, distancia 0
                    cola.push({vecino, distancia});
                    visitados.insert({vecino.fila, vecino.columna});
                } else if(casillaVecino == EstadoCasilla::VACIA) {
                    // Casilla vacía, distancia +1
                    cola.push({vecino, distancia + 1});
                    visitados.insert({vecino.fila, vecino.columna});
                }
                // No explorar casillas del oponente
            }
        }
    }
    
    return 999; // No hay camino posible
}

std::vector<Posicion> IAHex::obtenerMovimientosDisponibles(const Hex& estadoJuego) {
    std::vector<Posicion> movimientos;
    const int TABLERO_SIZE = estadoJuego.getTamañoTablero();
    
    for(int i = 0; i < TABLERO_SIZE; i++) {
        for(int j = 0; j < TABLERO_SIZE; j++) {
            if(estadoJuego.getCasilla(i, j) == EstadoCasilla::VACIA) {
                movimientos.push_back(Posicion(i, j));
            }
        }
    }
    
    // Optimización: ordenar por prioridad estratégica
    std::sort(movimientos.begin(), movimientos.end(), [&](const Posicion& a, const Posicion& b) {
        return evaluarPrioridadMovimiento(estadoJuego, a) > evaluarPrioridadMovimiento(estadoJuego, b);
    });
    
    // Limitar a los mejores movimientos para optimización
    if(movimientos.size() > 15) {
        movimientos.resize(15);
    }
    
    return movimientos;
}

int IAHex::evaluarDefensa(const Hex& estadoJuego) {
    int puntuacion = 0;
    
    // Calcular qué tan cerca está el oponente de ganar (ESTE-OESTE)
    int distanciaOponente = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR1, false);
    
    if(distanciaOponente <= 2) {
        puntuacion -= 2000; // Amenaza crítica
        std::cout << "[DEBUG] AMENAZA CRÍTICA: Oponente a distancia " << distanciaOponente << std::endl;
    } else if(distanciaOponente <= 4) {
        puntuacion -= 1000; // Amenaza seria
        std::cout << "[DEBUG] AMENAZA SERIA: Oponente a distancia " << distanciaOponente << std::endl;
    } else if(distanciaOponente <= 6) {
        puntuacion -= 300; // Amenaza moderada
    }
    
    return puntuacion;
}

int IAHex::evaluarAtaque(const Hex& estadoJuego) {
    int puntuacion = 0;
    
    // Calcular qué tan cerca estamos de ganar (NORTE-SUR)
    int nuestraDistancia = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR2, true);
    
    if(nuestraDistancia <= 2) {
        puntuacion += 2000; // Estamos muy cerca de ganar
        std::cout << "[DEBUG] OPORTUNIDAD DE VICTORIA: Distancia " << nuestraDistancia << std::endl;
    } else if(nuestraDistancia <= 4) {
        puntuacion += 1000; // Buena posición
        std::cout << "[DEBUG] BUENA POSICIÓN: Distancia " << nuestraDistancia << std::endl;
    } else if(nuestraDistancia <= 6) {
        puntuacion += 400; // Posición aceptable
    } else if(nuestraDistancia < 999) {
        puntuacion += 100; // Al menos hay camino
    }
    
    // Bonificación por controlar el centro (estratégicamente importante)
    const int centro = 5;
    for(int i = centro - 1; i <= centro + 1; i++) {
        for(int j = centro - 1; j <= centro + 1; j++) {
            if(estadoJuego.getCasilla(i, j) == EstadoCasilla::JUGADOR2) {
                puntuacion += 50;
            }
        }
    }
    
    return puntuacion;
}

int IAHex::evaluarPrioridadMovimiento(const Hex& estadoJuego, const Posicion& mov) {
    int prioridad = 0;
    const int i = mov.fila;
    const int j = mov.columna;
    
    // Simular el movimiento y evaluar mejora
    Hex copia = estadoJuego;
    copia.hacerMovimiento(i, j); // Fixed: removed third parameter
    
    int distanciaAntes = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR2, true);
    int distanciaDespues = calcularDistanciaCamino(copia, EstadoCasilla::JUGADOR2, true);
    
    // Alta prioridad si reduce nuestra distancia al objetivo
    if(distanciaDespues < distanciaAntes) {
        prioridad += (distanciaAntes - distanciaDespues) * 300;
        std::cout << "[DEBUG] Movimiento (" << i << "," << j << ") reduce distancia de " 
                  << distanciaAntes << " a " << distanciaDespues << std::endl;
    }
    
    // Evaluar si bloquea al oponente
    copia = estadoJuego;
    copia.hacerMovimiento(i, j); // Fixed: removed third parameter
    int distanciaOponenteAntes = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR1, false);
    int distanciaOponenteDespues = calcularDistanciaCamino(copia, EstadoCasilla::JUGADOR1, false);
    
    if(distanciaOponenteDespues > distanciaOponenteAntes) {
        prioridad += (distanciaOponenteDespues - distanciaOponenteAntes) * 200;
        std::cout << "[DEBUG] Movimiento (" << i << "," << j << ") aumenta distancia oponente de " 
                  << distanciaOponenteAntes << " a " << distanciaOponenteDespues << std::endl;
    }
    
    // Prioridad por posición estratégica
    // Centro del tablero
    int distanciaCentro = abs(i - 5) + abs(j - 5);
    prioridad += (10 - distanciaCentro) * 10;
    
    // Conectividad con fichas propias
    int conexiones = 0;
    for(const auto& vecino : obtenerVecinosHex(i, j)) {
        if(estadoJuego.getCasilla(vecino.fila, vecino.columna) == EstadoCasilla::JUGADOR2) {
            conexiones++;
        }
    }
    prioridad += conexiones * 100;
    
    return prioridad;
}

int IAHex::evaluarTablero(const Hex& estadoJuego) {
    if(estadoJuego.estaTerminado()) {
        int ganador = estadoJuego.getGanador();
        std::cout << "[DEBUG] JUEGO TERMINADO - Ganador: " << ganador << std::endl;
        return ganador == 2 ? 8000 : -8000;
    }

    int puntuacionDefensa = evaluarDefensa(estadoJuego);
    int puntuacionAtaque = evaluarAtaque(estadoJuego);
    
    // Balance entre ataque y defensa
    double factorAtaque = 0.6;
    double factorDefensa = 0.4;
    
    int puntuacionFinal = static_cast<int>(
        puntuacionDefensa * factorDefensa + 
        puntuacionAtaque * factorAtaque
    );
    
    std::cout << "[DEBUG] Evaluación - Defensa: " << puntuacionDefensa 
              << ", Ataque: " << puntuacionAtaque << ", Total: " << puntuacionFinal << std::endl;
    
    return puntuacionFinal;
}

Posicion IAHex::buscarMovimientoOfensivo(const Hex& estadoJuego, const std::vector<Posicion>& movimientos) {
    std::cout << "[DEBUG] Buscando movimiento ofensivo..." << std::endl;
    
    int mejorReduccion = 0;
    Posicion mejorMovimiento(-1, -1);
    int distanciaActual = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR2, true);
    
    for(const auto& mov : movimientos) {
        Hex copia = estadoJuego;
        if(copia.hacerMovimiento(mov.fila, mov.columna)) { // Fixed: removed third parameter
            int nuevaDistancia = calcularDistanciaCamino(copia, EstadoCasilla::JUGADOR2, true);
            int reduccion = distanciaActual - nuevaDistancia;
            
            if(reduccion > mejorReduccion) {
                mejorReduccion = reduccion;
                mejorMovimiento = mov;
            }
        }
    }
    
    if(mejorReduccion > 0) {
        std::cout << "[DEBUG] OPORTUNIDAD OFENSIVA: (" << mejorMovimiento.fila 
                  << "," << mejorMovimiento.columna << ") reduce distancia en " 
                  << mejorReduccion << std::endl;
    }
    
    return mejorMovimiento;
}

bool IAHex::esAmenazaCritica(const Hex& estadoJuego, const Posicion& mov) {
    // Simular movimiento del oponente
    Hex copia = estadoJuego;
    if(copia.hacerMovimiento(mov.fila, mov.columna)) { // Fixed: removed third parameter
        int distanciaOponente = calcularDistanciaCamino(copia, EstadoCasilla::JUGADOR1, false);
        return distanciaOponente <= 1; // Amenaza crítica si puede ganar en 1 movimiento
    }
    return false;
}

Posicion IAHex::calcularMejorMovimiento(Hex& estadoJuego) {
    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << "[DEBUG] ========== CALCULANDO MEJOR MOVIMIENTO ==========" << std::endl;
    
    try {
        std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
        
        if(movimientos.empty()) {
            return Posicion(-1, -1);
        }

        // PASO 1: Verificar amenazas críticas del oponente
        std::cout << "[DEBUG] PASO 1: Verificando amenazas críticas..." << std::endl;
        for(const auto& mov : movimientos) {
            if(esAmenazaCritica(estadoJuego, mov)) {
                std::cout << "[DEBUG] *** DEFENSA CRÍTICA: (" << mov.fila << "," << mov.columna << ") ***" << std::endl;
                return mov;
            }
        }

        // PASO 2: Buscar oportunidades ofensivas
        std::cout << "[DEBUG] PASO 2: Buscando oportunidades ofensivas..." << std::endl;
        Posicion movOfensivo = buscarMovimientoOfensivo(estadoJuego, movimientos);
        if(movOfensivo.fila != -1) {
            int distanciaActual = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR2, true);
            if(distanciaActual <= 3) { // Solo tomar oportunidad ofensiva si estamos cerca
                std::cout << "[DEBUG] *** ATAQUE: (" << movOfensivo.fila << "," << movOfensivo.columna << ") ***" << std::endl;
                return movOfensivo;
            }
        }

        // PASO 3: Usar minimax para decisión estratégica
        std::cout << "[DEBUG] PASO 3: Usando minimax..." << std::endl;
        
        int mejorValor = INT_MIN;
        Posicion mejorMovimiento = movimientos[0];
        
        auto tiempoLimite = inicio + std::chrono::milliseconds(1500);

        for(const auto& mov : movimientos) {
            if(std::chrono::high_resolution_clock::now() > tiempoLimite) {
                std::cout << "[DEBUG] LÍMITE DE TIEMPO ALCANZADO" << std::endl;
                break;
            }

            Hex copiaEstado = estadoJuego;
            if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                nodosExploradosTemp = 0;
                int valor = minimax(copiaEstado, profundidadMaxima - 1, INT_MIN, INT_MAX, false);
                
                std::cout << "[DEBUG] Movimiento (" << mov.fila << "," << mov.columna 
                          << ") valor: " << valor << std::endl;
                
                if(valor > mejorValor) {
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
        std::cout << "[DEBUG] Tiempo: " << duracion.count() << "ms" << std::endl;
        std::cout << "[DEBUG] ===============================" << std::endl;

        return mejorMovimiento;

    } catch(const std::exception& e) {
        std::cerr << "[ERROR] Error en calcularMejorMovimiento: " << e.what() << std::endl;
        std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
        return movimientos.empty() ? Posicion(-1, -1) : movimientos[0];
    }
}

int IAHex::minimax(Hex& estadoJuego, int profundidad, int alfa, int beta, bool esMaximizador) {
    nodosExploradosTemp++;
    
    if(nodosExploradosTemp > 5000 || profundidad <= 0 || estadoJuego.estaTerminado()) {
        return evaluarTablero(estadoJuego);
    }

    std::vector<Posicion> movimientos = obtenerMovimientosDisponibles(estadoJuego);
    
    // Limitar movimientos para mejor rendimiento
    if(movimientos.size() > 8) {
        movimientos.resize(8);
    }
    
    if(esMaximizador) {
        int mejorValor = INT_MIN;
        for(const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                int valor = minimax(copiaEstado, profundidad - 1, alfa, beta, false);
                mejorValor = std::max(mejorValor, valor);
                alfa = std::max(alfa, mejorValor);
                if(beta <= alfa) break;
            }
        }
        return mejorValor;
    } else {
        int mejorValor = INT_MAX;
        for(const auto& mov : movimientos) {
            Hex copiaEstado = estadoJuego;
            if(copiaEstado.hacerMovimiento(mov.fila, mov.columna)) {
                int valor = minimax(copiaEstado, profundidad - 1, alfa, beta, true);
                mejorValor = std::min(mejorValor, valor);
                beta = std::min(beta, mejorValor);
                if(beta <= alfa) break;
            }
        }
        return mejorValor;
    }
}

int IAHex::evaluarProgresoVictoria(const Hex& estadoJuego) {
    int distanciaNuestra = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR2, true);
    int distanciaOponente = calcularDistanciaCamino(estadoJuego, EstadoCasilla::JUGADOR1, false);
    
    std::cout << "[DEBUG] === Progreso Victoria ===" << std::endl;
    std::cout << "[DEBUG] Nuestra distancia (NORTE-SUR): " << distanciaNuestra << std::endl;
    std::cout << "[DEBUG] Distancia oponente (ESTE-OESTE): " << distanciaOponente << std::endl;
    
    // Retornar diferencia: positivo si estamos mejor, negativo si el oponente está mejor
    return (distanciaOponente - distanciaNuestra) * 100;
}