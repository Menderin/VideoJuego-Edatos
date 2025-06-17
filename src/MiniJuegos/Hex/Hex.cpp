#include "Minijuegos/Hex/Hex.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>

// ===================== IMPLEMENTACIÓN DE GrafoHex =====================

GrafoHex::GrafoHex(bool _modoIA) : modoIA(_modoIA) {
    inicializar();
}

void GrafoHex::inicializar() {
    nodos.clear();
    int tamaño = modoIA ? TAMAÑO_TABLERO_IA : TAMAÑO_TABLERO;
    
    // Crear nodos para cada casilla del tablero
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            int id = obtenerID(i, j);
            nodos[id] = NodoHex(id, i, j);
        }
    }
    
    // Crear nodos virtuales para los bordes
    nodoVirtualIzquierda = tamaño * tamaño;
    nodoVirtualDerecha = nodoVirtualIzquierda + 1;
    nodoVirtualArriba = nodoVirtualDerecha + 1;
    nodoVirtualAbajo = nodoVirtualArriba + 1;
    
    nodos[nodoVirtualIzquierda] = NodoHex(nodoVirtualIzquierda, -1, -1);
    nodos[nodoVirtualDerecha] = NodoHex(nodoVirtualDerecha, -1, -1);
    nodos[nodoVirtualArriba] = NodoHex(nodoVirtualArriba, -1, -1);
    nodos[nodoVirtualAbajo] = NodoHex(nodoVirtualAbajo, -1, -1);
    
    // Conectar nodos vecinos
    conectarVecinos();
}

void GrafoHex::reiniciar() {
    for (auto& par : nodos) {
        par.second.estado = EstadoCasilla::VACIA;
    }
}

int GrafoHex::obtenerID(int fila, int col) const {
    return fila * TAMAÑO_TABLERO + col;
}


// Modificar la función conectarVecinos para asegurar conexiones correctas
void GrafoHex::conectarVecinos() {
    // Conectar casillas adyacentes
    for (int i = 0; i < TAMAÑO_TABLERO; i++) {
        for (int j = 0; j < TAMAÑO_TABLERO; j++) {
            int idActual = obtenerID(i, j);
            
            // Direcciones para las 6 casillas adyacentes en un hexágono
            const int direcciones[6][2] = {
                {-1, 0},  // Arriba
                {-1, 1},  // Arriba-derecha
                {0, 1},   // Derecha
                {1, 0},   // Abajo
                {1, -1},  // Abajo-izquierda
                {0, -1}   // Izquierda
            };
            
            for (const auto& dir : direcciones) {
                int nuevaFila = i + dir[0];
                int nuevaCol = j + dir[1];
                
                if (esPosicionValida(nuevaFila, nuevaCol)) {
                    int idVecino = obtenerID(nuevaFila, nuevaCol);
                    nodos[idActual].vecinos.push_back(idVecino);
                }
            }
        }
    }
    
    // Conectar nodos virtuales con los bordes
    for (int i = 0; i < TAMAÑO_TABLERO; i++) {
        // Borde izquierdo
        nodos[nodoVirtualIzquierda].vecinos.push_back(obtenerID(i, 0));
        nodos[obtenerID(i, 0)].vecinos.push_back(nodoVirtualIzquierda);
        
        // Borde derecho
        nodos[nodoVirtualDerecha].vecinos.push_back(obtenerID(i, TAMAÑO_TABLERO-1));
        nodos[obtenerID(i, TAMAÑO_TABLERO-1)].vecinos.push_back(nodoVirtualDerecha);
        
        // Borde superior
        nodos[nodoVirtualArriba].vecinos.push_back(obtenerID(0, i));
        nodos[obtenerID(0, i)].vecinos.push_back(nodoVirtualArriba);
        
        // Borde inferior
        nodos[nodoVirtualAbajo].vecinos.push_back(obtenerID(TAMAÑO_TABLERO-1, i));
        nodos[obtenerID(TAMAÑO_TABLERO-1, i)].vecinos.push_back(nodoVirtualAbajo);
    }
}


bool GrafoHex::colocarFicha(int fila, int col, EstadoCasilla jugador) {
    if (!esPosicionValida(fila, col) || !esCasillaVacia(fila, col)) {
        return false;
    }
    
    int id = obtenerID(fila, col);
    nodos[id].estado = jugador;
    return true;
}

EstadoCasilla GrafoHex::obtenerEstado(int fila, int col) const {
    if (!esPosicionValida(fila, col)) {
        return EstadoCasilla::VACIA;
    }
    
    int id = obtenerID(fila, col);
    auto it = nodos.find(id);
    return (it != nodos.end()) ? it->second.estado : EstadoCasilla::VACIA;
}

bool GrafoHex::esPosicionValida(int fila, int col) const {
    return fila >= 0 && fila < TAMAÑO_TABLERO && col >= 0 && col < TAMAÑO_TABLERO;
}

bool GrafoHex::esCasillaVacia(int fila, int col) const {
    return obtenerEstado(fila, col) == EstadoCasilla::VACIA;
}

// Modificar la función verificarVictoria en GrafoHex
bool GrafoHex::verificarVictoria(EstadoCasilla jugador) {
    std::unordered_set<int> visitados;
    
    // Para Jugador 1 (Rojo) - Conexión Izquierda-Derecha
    if (jugador == EstadoCasilla::JUGADOR1) {
        // Primero verificar si hay fichas en los bordes izquierdo y derecho
        bool hayFichaIzquierda = false;
        bool hayFichaDerecha = false;
        
        for (int i = 0; i < TAMAÑO_TABLERO; i++) {
            if (obtenerEstado(i, 0) == EstadoCasilla::JUGADOR1) {
                hayFichaIzquierda = true;
            }
            if (obtenerEstado(i, TAMAÑO_TABLERO-1) == EstadoCasilla::JUGADOR1) {
                hayFichaDerecha = true;
            }
        }
        
        // Solo verificar la conexión si hay fichas en ambos bordes
        if (hayFichaIzquierda && hayFichaDerecha) {
            return dfs(nodoVirtualIzquierda, nodoVirtualDerecha, jugador, visitados);
        }
    }
    // Para Jugador 2 (Azul) - Conexión Arriba-Abajo
    else if (jugador == EstadoCasilla::JUGADOR2) {
        // Primero verificar si hay fichas en los bordes superior e inferior
        bool hayFichaArriba = false;
        bool hayFichaAbajo = false;
        
        for (int j = 0; j < TAMAÑO_TABLERO; j++) {
            if (obtenerEstado(0, j) == EstadoCasilla::JUGADOR2) {
                hayFichaArriba = true;
            }
            if (obtenerEstado(TAMAÑO_TABLERO-1, j) == EstadoCasilla::JUGADOR2) {
                hayFichaAbajo = true;
            }
        }
        
        // Solo verificar la conexión si hay fichas en ambos bordes
        if (hayFichaArriba && hayFichaAbajo) {
            return dfs(nodoVirtualArriba, nodoVirtualAbajo, jugador, visitados);
        }
    }
    
    return false;
}

// Modificar la función dfs para mejorar la detección de conexiones
bool GrafoHex::dfs(int nodoActual, int nodoDestino, EstadoCasilla jugador, std::unordered_set<int>& visitados) {
    // Si llegamos al nodo destino, hay victoria
    if (nodoActual == nodoDestino) {
        return true;
    }
    
    // Marcar el nodo actual como visitado
    visitados.insert(nodoActual);
    
    // Recorrer todos los vecinos del nodo actual
    for (int vecino : nodos[nodoActual].vecinos) {
        // Si el vecino no ha sido visitado y es del jugador actual o es un nodo virtual
        if (visitados.find(vecino) == visitados.end() && 
            (nodos[vecino].estado == jugador || 
             vecino == nodoDestino || 
             vecino == nodoVirtualIzquierda || 
             vecino == nodoVirtualDerecha || 
             vecino == nodoVirtualArriba || 
             vecino == nodoVirtualAbajo)) {
            
            // Llamada recursiva para el vecino
            if (dfs(vecino, nodoDestino, jugador, visitados)) {
                return true;
            }
        }
    }
    
    return false;
}


void GrafoHex::mostrarGrafo() const {
    std::cout << "\n=== ESTRUCTURA DEL GRAFO ===" << std::endl;
    for (const auto& par : nodos) {
        const NodoHex& nodo = par.second;
        if (nodo.fila >= 0 && nodo.columna >= 0) {  // Solo mostrar nodos reales
            std::cout << "Nodo [" << nodo.fila << "][" << nodo.columna << "] (ID:" << nodo.id 
                      << ") - Vecinos: " << nodo.vecinos.size() << std::endl;
        }
    }
}

void GrafoHex::mostrarConexiones(int fila, int col) const {
    if (!esPosicionValida(fila, col)) return;
    
    int id = obtenerID(fila, col);
    auto it = nodos.find(id);
    if (it == nodos.end()) return;
    
    std::cout << "Conexiones de [" << fila << "][" << col << "]: ";
    for (int vecinoId : it->second.vecinos) {
        auto itVecino = nodos.find(vecinoId);
        if (itVecino != nodos.end() && itVecino->second.fila >= 0) {
            std::cout << "[" << itVecino->second.fila << "][" << itVecino->second.columna << "] ";
        }
    }
    std::cout << std::endl;
}

// ===================== IMPLEMENTACIÓN DE Hex =====================

Hex::Hex(bool _modoIA) : modoIA(_modoIA), jugadorActual(1), 
    juegoTerminado(false), ganador(0), primerMovimiento(true), 
    puedeRobar(false), ultimaFila(-1), ultimaColumna(-1) {
    grafo = GrafoHex(modoIA);
}

bool Hex::procesarMovimiento(int jugador, int input) {
    int fila = input / grafo.getTamañoTablero();
    int col = input % grafo.getTamañoTablero();
    
    return hacerMovimiento(fila, col);
}

void Hex::mostrarEstado() const {
    mostrarTablero();
    std::cout << "\nJugador actual: " << jugadorActual << std::endl;
    std::cout << "Estado: " << (juegoTerminado ? "TERMINADO" : "EN CURSO") << std::endl;
    if (juegoTerminado) {
        std::cout << "Ganador: Jugador " << ganador << std::endl;
    }
    if (puedeRobar) {
        std::cout << "¡El Jugador 2 puede robar la ficha del Jugador 1!" << std::endl;
    }
}

void Hex::reiniciar() {
    grafo.reiniciar();
    jugadorActual = 1;
    juegoTerminado = false;
    ganador = 0;
    primerMovimiento = true;
    puedeRobar = false;
    ultimaFila = -1;
    ultimaColumna = -1;
}

bool Hex::hacerMovimiento(int fila, int col) {
    // Validación de límites del tablero
    if (fila < 0 || fila >= grafo.getTamañoTablero() || 
        col < 0 || col >= grafo.getTamañoTablero()) {
        return false;
    }

    // Validación de casilla vacía y juego no terminado
    if (!grafo.esCasillaVacia(fila, col) || juegoTerminado) {
        return false;
    }
    
    // Determinar el estado según el jugador actual
    EstadoCasilla jugadorEstado = (jugadorActual == 1) ? 
                                 EstadoCasilla::JUGADOR1 : 
                                 EstadoCasilla::JUGADOR2;
    
    // Intentar colocar la ficha
    if (!grafo.colocarFicha(fila, col, jugadorEstado)) {
        return false;
    }
    
    // Manejar el primer movimiento y la regla del robo
    if (primerMovimiento) {
        ultimaFila = fila;
        ultimaColumna = col;
        puedeRobar = true;
        primerMovimiento = false;
    } else {
        puedeRobar = false;
    }
    
    // Verificar si hay victoria
    if (grafo.verificarVictoria(jugadorEstado)) {
        juegoTerminado = true;
        ganador = jugadorActual;
        return true;
    }
    
    // Cambiar el turno si no hay victoria
    cambiarTurno();
    return true;
}

bool Hex::aplicarReglaRobo() {
    if (!puedeRobar || primerMovimiento || ultimaFila == -1 || ultimaColumna == -1) {
        return false;
    }
    
    // El jugador 2 roba la ficha del jugador 1
    if (!grafo.colocarFicha(ultimaFila, ultimaColumna, EstadoCasilla::JUGADOR2)) {
        return false;
    }
    
    puedeRobar = false;
    jugadorActual = 1;  // El jugador 1 vuelve a jugar
    
    std::cout << "\n¡Jugador 2 ha robado la ficha en [" << ultimaFila << "][" << ultimaColumna << "]!" << std::endl;
    return true;
}

void Hex::mostrarTablero() const {
    std::cout << "\n=== TABLERO HEX (Implementado con Grafos) ===\n";
    std::cout << "JUGADOR 1 (R): Conectar IZQUIERDA a DERECHA\n";
    std::cout << "JUGADOR 2 (A): Conectar ARRIBA a ABAJO\n\n";

    int tamaño = grafo.getTamañoTablero();

    // Encabezado: columnas numeradas
    std::cout << "     ";
    for (int col = 0; col < tamaño; ++col) {
        std::cout << col << "   ";
    }
    std::cout << "\n";

    for (int fila = 0; fila < tamaño; ++fila) {
        // Espacios al inicio para simular desplazamiento en hexágono
        std::cout << std::string(fila * 2, ' ');

        // Mostrar fila con casillas y conexiones horizontales
        std::cout << fila << "  ";
        for (int col = 0; col < tamaño; ++col) {
            char simbolo = obtenerSimbolo(grafo.obtenerEstado(fila, col));
            std::cout << simbolo;
            if (col != tamaño - 1) std::cout << " - ";
        }
        std::cout << "  " << fila << "\n";

        // Mostrar conexiones diagonales inferiores (excepto en la última fila)
        if (fila != tamaño - 1) {
            std::cout << std::string(fila * 2 + 3, ' ');
            for (int col = 0; col < tamaño; ++col) {
                std::cout << "\\ /";
                if (col != tamaño - 1) std::cout << " ";
            }
            std::cout << "\n";
        }
    }

    // Encabezado inferior (igual que el superior)
    std::cout << std::string(tamaño * 2 + 3, ' ');
    for (int col = 0; col < tamaño; ++col) {
        std::cout << col << "   ";
    }
    std::cout << "\n";

    std::cout << "\nLEYENDA:\n";
    std::cout << "  . = Casilla vacía\n";
    std::cout << "  R = Ficha del Jugador 1 (Rojo) - Conecta IZQUIERDA-DERECHA\n";
    std::cout << "  A = Ficha del Jugador 2 (Azul) - Conecta ARRIBA-ABAJO\n";
    std::cout << "=============================================\n";
}

char Hex::obtenerSimbolo(EstadoCasilla estado) const {
    switch (estado) {
        case EstadoCasilla::VACIA: return '.';
        case EstadoCasilla::JUGADOR1: return 'R';  // Rojo
        case EstadoCasilla::JUGADOR2: return 'A';  // Azul
        default: return '?';
    }
}

void Hex::cambiarTurno() {
    jugadorActual = (jugadorActual == 1) ? 2 : 1;
}

void Hex::mostrarInstrucciones() const {
    std::cout << "\n INSTRUCCIONES DEL HEX " << std::endl;
    std::cout << "• Hex es un juego de conexión para 2 jugadores" << std::endl;
    std::cout << "• Objetivo:" << std::endl;
    std::cout << "  - Jugador 1 (R): Conectar el lado IZQUIERDO con el DERECHO" << std::endl;
    std::cout << "  - Jugador 2 (A): Conectar el lado SUPERIOR con el INFERIOR" << std::endl;
    std::cout << " El tablero usa grafos para detectar conexiones automáticamente" << std::endl;
    std::cout << " Regla especial: Después del primer movimiento del Jugador 1," << std::endl;
    std::cout << "  el Jugador 2 puede 'robar' esa ficha (swap rule)" << std::endl;
    std::cout << "• Para jugar: introduce fila (0-10) y columna (0-10)" << std::endl;
    std::cout << std::endl;
}

void Hex::jugar() {
    mostrarInstrucciones();
    
    while (!juegoTerminado) {
        mostrarEstado();
        
        // Preguntar por regla del robo
        if (puedeRobar && jugadorActual == 2) {
            char respuesta;
            std::cout << "\nJugador 2, ¿quieres robar la ficha del Jugador 1? (s/n): ";
            std::cin >> respuesta;
            
            if (respuesta == 's' || respuesta == 'S') {
                aplicarReglaRobo();
                continue;
            } else {
                puedeRobar = false;
            }
        }
        
        int fila, col;
        std::cout << "\nJugador " << jugadorActual << ", ingresa tu movimiento:" << std::endl;
        std::cout << "Fila (0-" << (grafo.getTamañoTablero()-1) << "): ";
        std::cin >> fila;
        std::cout << "Columna (0-" << (grafo.getTamañoTablero()-1) << "): ";
        std::cin >> col;
        
        if (hacerMovimiento(fila, col)) {
            std::cout << "Movimiento realizado en [" << fila << "][" << col << "]" << std::endl;
        } else {
            std::cout << "Movimiento inválido. Intenta de nuevo." << std::endl;
        }
    }
    
    std::cout << "\n¡JUEGO TERMINADO!" << std::endl;
    std::cout << "Ganador: Jugador " << ganador << std::endl;
    mostrarTablero();
}

// Getters
int Hex::getGanador() const { return ganador; }
bool Hex::estaTerminado() const { return juegoTerminado; }
EstadoCasilla Hex::getCasilla(int fila, int col) const { return grafo.obtenerEstado(fila, col); }
int Hex::getJugadorActual() const { return jugadorActual; }
bool Hex::getPuedeRobar() const { return puedeRobar; }