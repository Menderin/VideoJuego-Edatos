#include "IA/IaHex.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <random>
#include <iostream>

// Implementación de Posicion
Posicion::Posicion(int f, int c) : fila(f), columna(c) {}

bool Posicion::operator==(const Posicion& other) const {
    return fila == other.fila && columna == other.columna;
}

// Implementación de NodoDecision
IAHex::NodoDecision::NodoDecision() : umbral(0), estrategia(-1), izquierdo(nullptr), derecho(nullptr) {}

IAHex::NodoDecision::~NodoDecision() {
    delete izquierdo;
    delete derecho;
}

// Implementación de IAHex
IAHex::IAHex(int profundidad, int dificultad)
    : profundidadMaxima(profundidad), nivelDificultad(dificultad), arbolDecision(nullptr) {
    generador.seed(std::random_device{}());
    inicializarIA();
}

IAHex::~IAHex() {
    limpiarRecursos();
}

void IAHex::inicializarIA() {
    construirArbolDecision();
    std::cout << "[IA] Inicializada con profundidad " << profundidadMaxima
              << " y dificultad " << nivelDificultad << std::endl;
}

void IAHex::limpiarRecursos() {
    delete arbolDecision;
    arbolDecision = nullptr;
}

void IAHex::construirArbolDecision() {
    arbolDecision = new NodoDecision();
    
    // Ajustar umbrales según la dificultad
    double factorDificultad = nivelDificultad / 4.0; // Normalizar a 0.0-1.0
    
    arbolDecision->caracteristica = "fase_juego";
    arbolDecision->umbral = 0.15 + (factorDificultad * 0.3); // 0.15-0.45 según dificultad

    arbolDecision->izquierdo = new NodoDecision();
    arbolDecision->izquierdo->caracteristica = "centro_disponible";
    arbolDecision->izquierdo->umbral = 0.3 + (factorDificultad * 0.4); // 0.3-0.7

    arbolDecision->izquierdo->izquierdo = new NodoDecision();
    arbolDecision->izquierdo->izquierdo->estrategia = ESTRATEGIA_CENTRO;

    arbolDecision->izquierdo->derecho = new NodoDecision();
    arbolDecision->izquierdo->derecho->estrategia = ESTRATEGIA_CONEXION;

    arbolDecision->derecho = new NodoDecision();
    arbolDecision->derecho->caracteristica = "amenaza_oponente";
    arbolDecision->derecho->umbral = 0.8 - (factorDificultad * 0.3); // 0.8-0.5 (más sensible = menor umbral)

    arbolDecision->derecho->derecho = new NodoDecision();
    arbolDecision->derecho->derecho->estrategia = ESTRATEGIA_DEFENSIVA;

    arbolDecision->derecho->izquierdo = new NodoDecision();
    arbolDecision->derecho->izquierdo->estrategia = ESTRATEGIA_OFENSIVA;
}

std::vector<double> IAHex::extraerCaracteristicas(const Hex& juego) {
    std::vector<double> caracteristicas;
    int tamaño = TAMAÑO_TABLERO;
    int centro = tamaño / 2;
    int totalCasillas = tamaño * tamaño;

    int casillasOcupadas = 0;
    int piezasJugador1 = 0;
    int piezasJugador2 = 0;

    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            EstadoCasilla estado = juego.getCasilla(i, j);
            if (estado != EstadoCasilla::VACIA) {
                casillasOcupadas++;
                if (estado == EstadoCasilla::JUGADOR1) piezasJugador1++;
                else if (estado == EstadoCasilla::JUGADOR2) piezasJugador2++;
            }
        }
    }

    // Característica 0: Fase del juego (0.0 = inicio, 1.0 = final)
    caracteristicas.push_back(static_cast<double>(casillasOcupadas) / totalCasillas);
    
    // Característica 1: Centro disponible (1.0 = disponible, 0.0 = ocupado)
    caracteristicas.push_back((juego.getCasilla(centro, centro) == EstadoCasilla::VACIA) ? 1.0 : 0.0);

    // Característica 2: Amenaza del oponente (0.0-1.0+)
    int conectividadOponente = calcularConectividad(juego, EstadoCasilla::JUGADOR1);
    double amenazaOponente = static_cast<double>(conectividadOponente) / std::max(piezasJugador1, 1);
    caracteristicas.push_back(std::min(amenazaOponente / 10.0, 1.0)); // Normalizar

    // Característica 3: Ventaja propia
    int conectividadPropia = calcularConectividad(juego, EstadoCasilla::JUGADOR2);
    double ventajaPropia = static_cast<double>(conectividadPropia) / std::max(piezasJugador2, 1);
    caracteristicas.push_back(std::min(ventajaPropia / 10.0, 1.0));

    return caracteristicas;
}

int IAHex::determinarEstrategia(const Hex& juego) {
    // En dificultades bajas, introducir aleatoriedad
    if (nivelDificultad <= 2) {
        std::uniform_int_distribution<int> dist(0, 99);
        int random = dist(generador);
        
        // Probabilidad de jugar aleatoriamente según dificultad
        int probabilidadAleatoria = (3 - nivelDificultad) * 15; // 45%, 30%, 15% para dif 1,2,3
        
        if (random < probabilidadAleatoria) {
            std::uniform_int_distribution<int> estrategiaDist(0, 3);
            int estrategiaAleatoria = estrategiaDist(generador);
            std::cout << "[IA] Usando estrategia aleatoria: " << estrategiaAleatoria 
                      << " (dificultad " << nivelDificultad << ")" << std::endl;
            return estrategiaAleatoria;
        }
    }
    
    std::vector<double> caracteristicas = extraerCaracteristicas(juego);
    
    std::cout << "[IA] Características: ";
    for (size_t i = 0; i < caracteristicas.size(); i++) {
        std::cout << "C" << i << "=" << caracteristicas[i] << " ";
    }
    std::cout << std::endl;
    
    return evaluarNodoDecision(arbolDecision, caracteristicas);
}

int IAHex::evaluarNodoDecision(NodoDecision* nodo, const std::vector<double>& caracteristicas) {
    if (!nodo) return ESTRATEGIA_OFENSIVA;

    if (nodo->estrategia >= 0) {
        return nodo->estrategia;
    }

    double valor = 0;
    if (nodo->caracteristica == "fase_juego") {
        valor = caracteristicas[0];
    } else if (nodo->caracteristica == "centro_disponible") {
        valor = caracteristicas[1];
    } else if (nodo->caracteristica == "amenaza_oponente") {
        valor = caracteristicas[2];
    }

    std::cout << "[IA] Evaluando " << nodo->caracteristica << ": valor=" << valor 
              << " vs umbral=" << nodo->umbral << std::endl;

    if (valor < nodo->umbral) {
        return evaluarNodoDecision(nodo->izquierdo, caracteristicas);
    } else {
        return evaluarNodoDecision(nodo->derecho, caracteristicas);
    }
}

Posicion IAHex::calcularMejorMovimiento(Hex& juego) {
    std::vector<Posicion> movimientos = getMovimientosValidos(juego);
    if (movimientos.empty()) {
        return Posicion(-1, -1);
    }

    // Verificar si hay un movimiento ganador inmediato
    for (const Posicion& mov : movimientos) {
        Hex juegoTemp = juego;
        juegoTemp.hacerMovimiento(mov.fila, mov.columna);
        if (juegoTemp.estaTerminado() && juegoTemp.getGanador() == 2) {
            std::cout << "[IA] Movimiento ganador encontrado!" << std::endl;
            return mov;
        }
    }

    // Verificar amenazas inmediatas del oponente
    if (nivelDificultad >= 2) {
        std::vector<Posicion> movimientosCriticos = encontrarMovimientosCriticos(juego);
        if (!movimientosCriticos.empty()) {
            std::cout << "[IA] Detectada amenaza inmediata, jugando defensivamente" << std::endl;
            return movimientosCriticos[0];
        }
    }

    int estrategia = determinarEstrategia(juego);
    std::cout << "[IA] Usando estrategia: " << estrategia << " (";
    switch(estrategia) {
        case 0: std::cout << "CENTRO"; break;
        case 1: std::cout << "OFENSIVA"; break;
        case 2: std::cout << "DEFENSIVA"; break;
        case 3: std::cout << "CONEXION"; break;
        default: std::cout << "DESCONOCIDA"; break;
    }
    std::cout << ")" << std::endl;

    Posicion movimientoEstrategico;
    switch (estrategia) {
        case ESTRATEGIA_CENTRO:
            movimientoEstrategico = aplicarEstrategiaCentro(juego);
            break;
        case ESTRATEGIA_OFENSIVA:
            movimientoEstrategico = aplicarEstrategiaOfensiva(juego);
            break;
        case ESTRATEGIA_DEFENSIVA:
            movimientoEstrategico = aplicarEstrategiaDefensiva(juego);
            break;
        case ESTRATEGIA_CONEXION:
            movimientoEstrategico = aplicarEstrategiaConexion(juego);
            break;
        default:
            movimientoEstrategico = Posicion(-1, -1);
    }

    // Si la estrategia no encontró un movimiento válido, usar minimax
    if (movimientoEstrategico.fila == -1 || movimientoEstrategico.columna == -1) {
        std::cout << "[IA] Estrategia falló, usando minimax" << std::endl;
        
        // Ajustar profundidad según dificultad
        int profundidadAjustada = std::max(1, std::min(profundidadMaxima, nivelDificultad + 1));
        
        Posicion mejorMovimiento;
        int mejorPuntuacion = INT_MIN;
        std::vector<Posicion> movimientosOrdenados = ordenarMovimientos(juego, movimientos);

        // Limitar búsqueda en dificultades bajas
        int maxMovimientosAEvaluar = (nivelDificultad <= 2) ? std::min(15, (int)movimientosOrdenados.size()) 
                                                           : movimientosOrdenados.size();

        for (int i = 0; i < maxMovimientosAEvaluar; i++) {
            const Posicion& mov = movimientosOrdenados[i];
            Hex juegoTemp = juego;
            juegoTemp.hacerMovimiento(mov.fila, mov.columna);
            int puntuacion = minimax(juegoTemp, profundidadAjustada - 1, false);
            
            if (puntuacion > mejorPuntuacion) {
                mejorPuntuacion = puntuacion;
                mejorMovimiento = mov;
            }
        }

        return mejorMovimiento;
    }

    return movimientoEstrategico;
}

int IAHex::minimax(Hex& juego, int profundidad, bool esMaximizador, int alfa, int beta) {
    if (profundidad == 0 || juego.estaTerminado()) {
        return evaluarPosicionHex(juego);
    }

    std::vector<Posicion> movimientos = getMovimientosValidos(juego);
    
    // En dificultades bajas, limitar el número de movimientos evaluados
    if (nivelDificultad <= 2) {
        std::shuffle(movimientos.begin(), movimientos.end(), generador);
        int maxMovimientos = std::min(10 + (nivelDificultad * 5), (int)movimientos.size());
        movimientos.resize(maxMovimientos);
    }

    if (esMaximizador) {
        int maxEval = INT_MIN;
        for (const Posicion& mov : movimientos) {
            Hex juegoTemp = juego;
            juegoTemp.hacerMovimiento(mov.fila, mov.columna);
            int eval = minimax(juegoTemp, profundidad - 1, false, alfa, beta);

            maxEval = std::max(maxEval, eval);
            alfa = std::max(alfa, eval);
            if (beta <= alfa) break; // Poda alfa-beta
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const Posicion& mov : movimientos) {
            Hex juegoTemp = juego;
            juegoTemp.hacerMovimiento(mov.fila, mov.columna);
            int eval = minimax(juegoTemp, profundidad - 1, true, alfa, beta);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alfa) break; // Poda alfa-beta
        }
        return minEval;
    }
}



int IAHex::evaluarPosicionHex(const Hex& juego) {
    if (juego.estaTerminado()) {
        if (juego.getGanador() == 2) return 10000;  // IA gana
        if (juego.getGanador() == 1) return -10000; // Humano gana
    }

    int puntuacion = 0;
    
    // Ajustar pesos según dificultad
    int pesoConectividad = 5 + (nivelDificultad * 2); // 7-15
    int pesoBordes = 3 + nivelDificultad; // 4-7
    int pesoEstrategico = 2 + nivelDificultad; // 3-6
    
    puntuacion += calcularConectividad(juego, EstadoCasilla::JUGADOR2) * pesoConectividad;
    puntuacion -= calcularConectividad(juego, EstadoCasilla::JUGADOR1) * pesoConectividad;
    puntuacion += evaluarControlBordes(juego) * pesoBordes;
    puntuacion += evaluarPosicionesEstrategicas(juego) * pesoEstrategico;

    return puntuacion;
}

int IAHex::calcularConectividad(const Hex& juego, EstadoCasilla jugador) {
    int tamaño = TAMAÑO_TABLERO;
    int conectividad = 0;
    
    // Las 6 direcciones hexagonales
    int dx[] = {-1, -1, 0, 0, 1, 1};
    int dy[] = {0, 1, -1, 1, -1, 0};

    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            if (juego.getCasilla(i, j) == jugador) {
                int conexiones = 0;
                for (int k = 0; k < 6; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 0 && ni < tamaño && nj >= 0 && nj < tamaño && 
                        juego.getCasilla(ni, nj) == jugador) {
                        conexiones++;
                    }
                }
                conectividad += conexiones;
            }
        }
    }

    return conectividad;
}

int IAHex::evaluarControlBordes(const Hex& juego) {
    int tamaño = TAMAÑO_TABLERO;
    int control = 0;

    for (int i = 0; i < tamaño; i++) {
        // Borde izquierdo y derecho (importante para JUGADOR1)
        if (juego.getCasilla(i, 0) == EstadoCasilla::JUGADOR2) control += 5;
        else if (juego.getCasilla(i, 0) == EstadoCasilla::JUGADOR1) control -= 15;

        if (juego.getCasilla(i, tamaño-1) == EstadoCasilla::JUGADOR2) control += 5;
        else if (juego.getCasilla(i, tamaño-1) == EstadoCasilla::JUGADOR1) control -= 15;

        // Borde superior e inferior (importante para JUGADOR2)
        if (juego.getCasilla(0, i) == EstadoCasilla::JUGADOR2) control += 15;
        else if (juego.getCasilla(0, i) == EstadoCasilla::JUGADOR1) control -= 5;

        if (juego.getCasilla(tamaño-1, i) == EstadoCasilla::JUGADOR2) control += 15;
        else if (juego.getCasilla(tamaño-1, i) == EstadoCasilla::JUGADOR1) control -= 5;
    }

    return control;
}

int IAHex::evaluarPosicionesEstrategicas(const Hex& juego) {
    int tamaño = TAMAÑO_TABLERO;
    int puntuacion = 0;

    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            EstadoCasilla estado = juego.getCasilla(i, j);
            if (estado == EstadoCasilla::JUGADOR2) {
                // JUGADOR2 quiere conectar arriba-abajo
                puntuacion += (tamaño - i) * 2;
                int distanciaCentro = abs(i - tamaño/2) + abs(j - tamaño/2);
                puntuacion += std::max(0, 10 - distanciaCentro);
            } else if (estado == EstadoCasilla::JUGADOR1) {
                // JUGADOR1 quiere conectar izquierda-derecha
                puntuacion -= (tamaño - j) * 2;
            }
        }
    }

    return puntuacion;
}

Posicion IAHex::aplicarEstrategiaCentro(const Hex& juego) {
    int tamaño = TAMAÑO_TABLERO;
    int centro = tamaño / 2;

    std::vector<Posicion> candidatos;
    
    // Buscar primero el centro exacto
    if (juego.getCasilla(centro, centro) == EstadoCasilla::VACIA) {
        std::cout << "[IA] Jugando en el centro exacto" << std::endl;
        return Posicion(centro, centro);
    }
    
    // Luego buscar en círculos concéntricos
    for (int radio = 1; radio <= 3; radio++) {
        for (int i = std::max(0, centro - radio); i <= std::min(tamaño - 1, centro + radio); i++) {
            for (int j = std::max(0, centro - radio); j <= std::min(tamaño - 1, centro + radio); j++) {
                if (abs(i - centro) + abs(j - centro) <= radio && 
                    juego.getCasilla(i, j) == EstadoCasilla::VACIA) {
                    candidatos.push_back(Posicion(i, j));
                }
            }
        }
        if (!candidatos.empty()) {
            std::cout << "[IA] Jugando cerca del centro, radio " << radio << std::endl;
            break;
        }
    }

    if (!candidatos.empty()) {
        return candidatos[0];
    }

    std::cout << "[IA] No se encontraron posiciones centrales disponibles" << std::endl;
    return Posicion(-1, -1);
}

Posicion IAHex::aplicarEstrategiaOfensiva(const Hex& juego) {
    std::vector<Posicion> movimientos = getMovimientosValidos(juego);
    Posicion mejorMovimiento(-1, -1);
    int mejorPuntuacion = INT_MIN;

    std::cout << "[IA] Evaluando " << movimientos.size() << " movimientos para estrategia ofensiva" << std::endl;

    for (const Posicion& mov : movimientos) {
        int puntuacion = evaluarPotencialConexion(juego, mov);
        
        // Bonus por proximidad a bordes objetivo (arriba-abajo para IA)
        if (mov.fila == 0 || mov.fila == TAMAÑO_TABLERO - 1) {
            puntuacion += 20;
        }
        
        if (puntuacion > mejorPuntuacion) {
            mejorPuntuacion = puntuacion;
            mejorMovimiento = mov;
        }
    }

    if (mejorMovimiento.fila != -1) {
        std::cout << "[IA] Movimiento ofensivo seleccionado: (" << mejorMovimiento.fila 
                  << "," << mejorMovimiento.columna << ") con puntuación " << mejorPuntuacion << std::endl;
    }

    return mejorMovimiento;
}

Posicion IAHex::aplicarEstrategiaDefensiva(const Hex& juego) {
    std::cout << "[IA] Aplicando estrategia defensiva" << std::endl;
    
    // Buscar movimientos críticos del oponente
    std::vector<Posicion> movimientosCriticos = encontrarMovimientosCriticos(juego);
    if (!movimientosCriticos.empty()) {
        std::cout << "[IA] Bloqueando movimiento crítico del oponente" << std::endl;
        return movimientosCriticos[0];
    }

    // Si no hay amenazas inmediatas, buscar bloquear conexiones del oponente
    std::vector<Posicion> movimientos = getMovimientosValidos(juego);
    Posicion mejorDefensivo(-1, -1);
    int mejorBloqueo = INT_MIN;

    for (const Posicion& mov : movimientos) {
        // Simular el movimiento del oponente en esta posición
        Hex juegoTemp = juego;
        juegoTemp.hacerMovimiento(mov.fila, mov.columna); // Simular que el oponente juega aquí
        
        int conectividadOponente = calcularConectividad(juegoTemp, EstadoCasilla::JUGADOR1);
        int valorBloqueo = -conectividadOponente; // Queremos minimizar la conectividad del oponente
        
        if (valorBloqueo > mejorBloqueo) {
            mejorBloqueo = valorBloqueo;
            mejorDefensivo = mov;
        }
    }

    if (mejorDefensivo.fila != -1) {
        std::cout << "[IA] Movimiento defensivo seleccionado para bloquear conectividad" << std::endl;
    }

    return mejorDefensivo;
}

Posicion IAHex::aplicarEstrategiaConexion(const Hex& juego) {
    std::cout << "[IA] Aplicando estrategia de conexión" << std::endl;
    
    std::vector<Posicion> movimientos = getMovimientosValidos(juego);
    Posicion mejorMovimiento(-1, -1);
    int mejorConectividad = -1;

    for (const Posicion& mov : movimientos) {
        Hex juegoTemp = juego;
        juegoTemp.hacerMovimiento(mov.fila, mov.columna);
        int conectividad = calcularConectividad(juegoTemp, EstadoCasilla::JUGADOR2);

        // Bonus adicional por crear puentes hacia los bordes objetivo
        int bonusBorde = 0;
        if (mov.fila <= 2 || mov.fila >= TAMAÑO_TABLERO - 3) {
            bonusBorde = 5;
        }

        int puntuacionTotal = conectividad + bonusBorde;
        
        if (puntuacionTotal > mejorConectividad) {
            mejorConectividad = puntuacionTotal;
            mejorMovimiento = mov;
        }
    }

    if (mejorMovimiento.fila != -1) {
        std::cout << "[IA] Movimiento de conexión seleccionado con conectividad " << mejorConectividad << std::endl;
    }

    return mejorMovimiento;
}

std::vector<Posicion> IAHex::ordenarMovimientos(const Hex& juego, const std::vector<Posicion>& movimientos) {
    std::vector<std::pair<Posicion, int>> movimientosConPuntuacion;

    for (const Posicion& mov : movimientos) {
        int puntuacion = evaluarPotencialConexion(juego, mov);
        movimientosConPuntuacion.push_back({mov, puntuacion});
    }

    std::sort(movimientosConPuntuacion.begin(), movimientosConPuntuacion.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::vector<Posicion> resultado;
    for (const auto& par : movimientosConPuntuacion) {
        resultado.push_back(par.first);
    }

    return resultado;
}

bool IAHex::amenazaInmediata(const Hex& juego) {
    std::vector<Posicion> movimientos = getMovimientosValidos(juego);

    for (const Posicion& mov : movimientos) {
        Hex juegoTemp = juego;
        juegoTemp.hacerMovimiento(mov.fila, mov.columna);
        if (juegoTemp.estaTerminado() && juegoTemp.getGanador() == 1) {
            return true;
        }
    }

    return false;
}

std::vector<Posicion> IAHex::encontrarMovimientosCriticos(const Hex& juego) {
    std::vector<Posicion> criticos;
    std::vector<Posicion> movimientos = getMovimientosValidos(juego);

    for (const Posicion& mov : movimientos) {
        Hex juegoTemp = juego;
        juegoTemp.hacerMovimiento(mov.fila, mov.columna);
        if (juegoTemp.estaTerminado() && juegoTemp.getGanador() == 1) {
            criticos.push_back(mov);
        }
    }

    return criticos;
}

int IAHex::evaluarPotencialConexion(const Hex& juego, const Posicion& pos) {
    int puntuacion = 0;
    int tamaño = TAMAÑO_TABLERO;

    puntuacion += calcularDistanciaAlObjetivo(pos, tamaño);

    // Las 6 direcciones hexagonales
    int dx[] = {-1, -1, 0, 0, 1, 1};
    int dy[] = {0, 1, -1, 1, -1, 0};

    for (int i = 0; i < 6; i++) {
        int nx = pos.fila + dx[i];
        int ny = pos.columna + dy[i];

        if (nx >= 0 && nx < tamaño && ny >= 0 && ny < tamaño) {
            if (juego.getCasilla(nx, ny) == EstadoCasilla::JUGADOR2) {
                puntuacion += 5;
            } else if (juego.getCasilla(nx, ny) == EstadoCasilla::VACIA) {
                puntuacion += 1;
            }
        }
    }

    return puntuacion;
}

int IAHex::calcularDistanciaAlObjetivo(const Posicion& pos, int tamaño) {
    // Para JUGADOR2 que conecta arriba-abajo
    return (tamaño - pos.fila) + std::max(0, 5 - abs(pos.columna - tamaño/2));
}

std::vector<Posicion> IAHex::getMovimientosValidos(const Hex& juego) {
    std::vector<Posicion> movimientos;
    int tamaño = TAMAÑO_TABLERO;

    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            if (juego.getCasilla(i, j) == EstadoCasilla::VACIA) {
                movimientos.push_back(Posicion(i, j));
            }
        }
    }

    return movimientos;
}