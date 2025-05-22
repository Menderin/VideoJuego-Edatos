#include "Minijuegos/Hex/Hex.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>

// ===================== IMPLEMENTACIÓN DE GrafoHex =====================

GrafoHex::GrafoHex() {
    inicializar();
}

void GrafoHex::inicializar() {
    nodos.clear();
    
    // Crear nodos para cada casilla del tablero
    for (int i = 0; i < TAMAÑO_TABLERO; i++) {
        for (int j = 0; j < TAMAÑO_TABLERO; j++) {
            int id = obtenerID(i, j);
            nodos[id] = NodoHex(id, i, j);
        }
    }
    
    // Crear nodos virtuales para los bordes
    nodoVirtualIzquierda = TAMAÑO_TABLERO * TAMAÑO_TABLERO;
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

void GrafoHex::conectarVecinos() {
    // Conectar casillas del tablero entre sí
    for (int i = 0; i < TAMAÑO_TABLERO; i++) {
        for (int j = 0; j < TAMAÑO_TABLERO; j++) {
            int idActual = obtenerID(i, j);
            
            // Las 6 direcciones hexagonales
            int direcciones[6][2] = {
                {-1, 0}, {-1, 1},  // arriba-izquierda, arriba-derecha
                {0, -1}, {0, 1},   // izquierda, derecha
                {1, -1}, {1, 0}    // abajo-izquierda, abajo-derecha
            };
            
            for (int d = 0; d < 6; d++) {
                int nuevaFila = i + direcciones[d][0];
                int nuevaCol = j + direcciones[d][1];
                
                if (esPosicionValida(nuevaFila, nuevaCol)) {
                    int idVecino = obtenerID(nuevaFila, nuevaCol);
                    nodos[idActual].vecinos.push_back(idVecino);
                }
            }
        }
    }
    
    // Conectar nodos virtuales con los bordes del tablero
    for (int i = 0; i < TAMAÑO_TABLERO; i++) {
        // Borde izquierdo conecta con nodo virtual izquierdo
        int idIzquierdo = obtenerID(i, 0);
        nodos[nodoVirtualIzquierda].vecinos.push_back(idIzquierdo);
        nodos[idIzquierdo].vecinos.push_back(nodoVirtualIzquierda);
        
        // Borde derecho conecta con nodo virtual derecho
        int idDerecho = obtenerID(i, TAMAÑO_TABLERO - 1);
        nodos[nodoVirtualDerecha].vecinos.push_back(idDerecho);
        nodos[idDerecho].vecinos.push_back(nodoVirtualDerecha);
        
        // Borde superior conecta con nodo virtual superior
        int idSuperior = obtenerID(0, i);
        nodos[nodoVirtualArriba].vecinos.push_back(idSuperior);
        nodos[idSuperior].vecinos.push_back(nodoVirtualArriba);
        
        // Borde inferior conecta con nodo virtual inferior
        int idInferior = obtenerID(TAMAÑO_TABLERO - 1, i);
        nodos[nodoVirtualAbajo].vecinos.push_back(idInferior);
        nodos[idInferior].vecinos.push_back(nodoVirtualAbajo);
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

bool GrafoHex::verificarVictoria(EstadoCasilla jugador) {
    std::unordered_set<int> visitados;
    
    if (jugador == EstadoCasilla::JUGADOR1) {
        // Jugador 1 conecta izquierda con derecha
        return dfs(nodoVirtualIzquierda, nodoVirtualDerecha, jugador, visitados);
    } else if (jugador == EstadoCasilla::JUGADOR2) {
        // Jugador 2 conecta arriba con abajo
        return dfs(nodoVirtualArriba, nodoVirtualAbajo, jugador, visitados);
    }
    
    return false;
}

bool GrafoHex::dfs(int nodoActual, int nodoDestino, EstadoCasilla jugador, std::unordered_set<int>& visitados) {
    if (nodoActual == nodoDestino) {
        return true;
    }
    
    visitados.insert(nodoActual);
    
    auto it = nodos.find(nodoActual);
    if (it == nodos.end()) {
        return false;
    }
    
    for (int vecino : it->second.vecinos) {
        if (visitados.find(vecino) != visitados.end()) {
            continue;  // Ya visitado
        }
        
        auto itVecino = nodos.find(vecino);
        if (itVecino == nodos.end()) {
            continue;
        }
        
        // Solo seguir el camino si es del mismo jugador o es un nodo virtual
        bool esNodoVirtual = (vecino == nodoVirtualIzquierda || vecino == nodoVirtualDerecha || 
                             vecino == nodoVirtualArriba || vecino == nodoVirtualAbajo);
        
        if (esNodoVirtual || itVecino->second.estado == jugador) {
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

Hex::Hex() : jugadorActual(1), juegoTerminado(false), ganador(0), 
             primerMovimiento(true), puedeRobar(false), ultimaFila(-1), ultimaColumna(-1) {
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
    if (!grafo.esPosicionValida(fila, col) || !grafo.esCasillaVacia(fila, col) || juegoTerminado) {
        return false;
    }
    
    EstadoCasilla jugadorEstado = (jugadorActual == 1) ? EstadoCasilla::JUGADOR1 : EstadoCasilla::JUGADOR2;
    
    if (!grafo.colocarFicha(fila, col, jugadorEstado)) {
        return false;
    }
    
    // Guardar la posición para la regla del robo
    if (primerMovimiento) {
        ultimaFila = fila;
        ultimaColumna = col;
        puedeRobar = true;
        primerMovimiento = false;
    } else {
        puedeRobar = false;
    }
    
    // Verificar victoria usando el algoritmo de grafos
    if (grafo.verificarVictoria(jugadorEstado)) {
        juegoTerminado = true;
        ganador = jugadorActual;
        return true;
    }
    
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
    std::cout << "\n=== TABLERO HEX (Implementado con Grafos) ===" << std::endl;
    std::cout << "JUGADOR 1 (R): Conectar IZQUIERDA ↔ DERECHA" << std::endl;
    std::cout << "JUGADOR 2 (A): Conectar ARRIBA ↕ ABAJO" << std::endl;
    std::cout << std::endl;
    
    int tamaño = grafo.getTamañoTablero();
    
    // Mostrar números de columna
    std::cout << "   ";
    for (int j = 0; j < tamaño; j++) {
        std::cout << std::setw(3) << j;
    }
    std::cout << std::endl;
    
    // Mostrar el tablero con indentación hexagonal
    for (int i = 0; i < tamaño; i++) {
        std::cout << std::setw(2) << i << " ";
        
        // Indentación para simular forma hexagonal
        for (int k = 0; k < i; k++) {
            std::cout << " ";
        }
        
        // Mostrar las casillas
        for (int j = 0; j < tamaño; j++) {
            char simbolo = obtenerSimbolo(grafo.obtenerEstado(i, j));
            std::cout << simbolo << " ";
            
            // Separador entre casillas (excepto la última)
            if (j < tamaño - 1) {
                std::cout << "- ";
            }
        }
        
        std::cout << " " << i << std::endl;
        
        // Líneas de conexión vertical (excepto en la última fila)
        if (i < tamaño - 1) {
            std::cout << "   ";
            for (int k = 0; k <= i; k++) {
                std::cout << " ";
            }
            for (int j = 0; j < tamaño - 1; j++) {
                std::cout << "\\ / ";
            }
            std::cout << "\\" << std::endl;
        }
    }
    
    // Mostrar números de columna abajo
    std::cout << "   ";
    for (int k = 0; k < tamaño; k++) {
        std::cout << " ";
    }
    for (int j = 0; j < tamaño; j++) {
        std::cout << std::setw(3) << j;
    }
    std::cout << std::endl;
    std::cout << "=============================================" << std::endl;
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
    std::cout << "\n=== INSTRUCCIONES DEL HEX ===" << std::endl;
    std::cout << "• Hex es un juego de conexión para 2 jugadores" << std::endl;
    std::cout << "• Objetivo:" << std::endl;
    std::cout << "  - Jugador 1 (R): Conectar el lado IZQUIERDO con el DERECHO" << std::endl;
    std::cout << "  - Jugador 2 (A): Conectar el lado SUPERIOR con el INFERIOR" << std::endl;
    std::cout << "• El tablero usa grafos para detectar conexiones automáticamente" << std::endl;
    std::cout << "• Regla especial: Después del primer movimiento del Jugador 1," << std::endl;
    std::cout << "  el Jugador 2 puede 'robar' esa ficha (swap rule)" << std::endl;
    std::cout << "• Para jugar: introduce fila (0-6) y columna (0-6)" << std::endl;
    std::cout << "==============================" << std::endl;
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