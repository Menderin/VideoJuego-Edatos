#include "Nodo.h"
#include "MiniJuegos/AdivinaNumero/AdivinaNumero.h"
#include "MiniJuegos/BatallaDeCartas/BatallaDeCartas.h"

// Constructores
Nodo::Nodo() : estado(EstadoNodo::VACIO), fila(-1), columna(-1), activo(true), 
               miniJuego(nullptr), tipoMiniJuego(TipoMiniJuego::NINGUNO), tieneMiniJuego(false) {
}

Nodo::Nodo(int f, int c) : estado(EstadoNodo::VACIO), fila(f), columna(c), activo(true),
                           miniJuego(nullptr), tipoMiniJuego(TipoMiniJuego::NINGUNO), tieneMiniJuego(false) {
}

// Getters originales
EstadoNodo Nodo::getEstado() const {
    return estado;
}

int Nodo::getFila() const {
    return fila;
}

int Nodo::getColumna() const {
    return columna;
}

bool Nodo::estaActivo() const {
    return activo;
}

bool Nodo::estaVacio() const {
    return estado == EstadoNodo::VACIO;
}

// Setters originales
void Nodo::setEstado(EstadoNodo nuevoEstado) {
    estado = nuevoEstado;
}

void Nodo::setPosicion(int f, int c) {
    fila = f;
    columna = c;
}

void Nodo::setActivo(bool nuevoActivo) {
    activo = nuevoActivo;
}

// Métodos de juego originales
bool Nodo::ocuparNodo(EstadoNodo jugador) {
    if (!activo || !estaVacio()) {
        return false;
    }
    
    // Si tiene minijuego, debe jugarlo primero
    if (tieneMiniJuego) {
        EstadoNodo ganador = jugarMiniJuego();
        estado = ganador;
    } else {
        estado = jugador;
    }
    
    return true;
}

void Nodo::reiniciar() {
    estado = EstadoNodo::VACIO;
    activo = true;
    
    // Reiniciar minijuego si existe
    if (miniJuego) {
        miniJuego->reiniciar();
    }
}

// Nuevos métodos para minijuegos
void Nodo::asignarMiniJuego(TipoMiniJuego tipo) {
    tipoMiniJuego = tipo;
    tieneMiniJuego = true;
    
    switch(tipo) {
        case TipoMiniJuego::ADIVINA_NUMERO:
            miniJuego = std::make_unique<AdivinaNumero>();
            break;
        case TipoMiniJuego::HEX:
            // TODO: Implementar cuando esté listo
            std::cout << "HEX aun no implementado" << std::endl;
            break;
        case TipoMiniJuego::BATALLA_CARTAS:
            miniJuego = std::make_unique<BatallaDeCartas>();
            break;
        default:
            tieneMiniJuego = false;
            miniJuego.reset();
            break;
    }
}

bool Nodo::tieneMiniJuegoAsignado() const {
    return tieneMiniJuego && miniJuego != nullptr;
}

TipoMiniJuego Nodo::getTipoMiniJuego() const {
    return tipoMiniJuego;
}

MiniJuego* Nodo::getMiniJuego() const {
    return miniJuego.get();
}

// Modificar el método jugarMiniJuego en Nodo.cpp para agregar la lógica de Batalla de Cartas
EstadoNodo Nodo::jugarMiniJuego() {
    if (!tieneMiniJuegoAsignado()) {
        return EstadoNodo::VACIO;
    }
    
    std::cout << "\nIniciando minijuego en posicion [" << fila << "][" << columna << "]!" << std::endl;
    
    // Para AdivinaNumero, implementamos la lógica jugador vs jugador
    if (tipoMiniJuego == TipoMiniJuego::ADIVINA_NUMERO) {
        AdivinaNumero* juego = static_cast<AdivinaNumero*>(miniJuego.get());
        
        // Los jugadores eligen sus números secretos
        int numeroSecreto1, numeroSecreto2;
        
        std::cout << "Jugador 1, elige tu numero secreto (1-100): ";
        std::cin >> numeroSecreto1;
        while (!juego->establecerNumeroSecreto(1, numeroSecreto1)) {
            std::cout << "Numero invalido. Elige otro numero (1-100): ";
            std::cin >> numeroSecreto1;
        }
        
        std::cout << "Jugador 2, elige tu numero secreto (1-100): ";
        std::cin >> numeroSecreto2;
        while (!juego->establecerNumeroSecreto(2, numeroSecreto2)) {
            std::cout << "Numero invalido. Elige otro numero (1-100): ";
            std::cin >> numeroSecreto2;
        }
        
        // Juego por turnos hasta que alguien gane
        while (!juego->estaTerminado()) {
            juego->mostrarEstado();
            
            int intento;
            int jugadorActual = juego->getTurnoActual();
            
            std::cout << "Jugador " << jugadorActual << ", ¿cual es el numero secreto del otro jugador? (1-100): ";
            std::cin >> intento;
            
            if (!juego->procesarMovimiento(jugadorActual, intento)) {
                std::cout << "Movimiento invalido. Intenta de nuevo." << std::endl;
            }
        }
        
        juego->mostrarEstado();
        
        // Convertir resultado a EstadoNodo
        switch(juego->getEstado()) {
            case EstadoMiniJuego::GANADOR_JUGADOR1:
                return EstadoNodo::JUGADOR1;
            case EstadoMiniJuego::GANADOR_JUGADOR2:
                return EstadoNodo::JUGADOR2;
            default:
                return EstadoNodo::VACIO; // No debería pasar
        }
    }
    // NUEVA LÓGICA PARA BATALLA DE CARTAS
    else if (tipoMiniJuego == TipoMiniJuego::BATALLA_CARTAS) {
        BatallaDeCartas* juego = static_cast<BatallaDeCartas*>(miniJuego.get());
        
        // Crear una versión modificada de BatallaDeCartas que funcione sin IA
        // y devuelva el ganador para integrarlo en el tablero
        
        juego->mostrarTitulo();
        
        std::cout << "Presiona ENTER para comenzar la batalla...";
        std::cin.get();
        
        // Reiniciar el juego para asegurar estado limpio
        juego->reiniciarJuego();
        
        // Simular el juego completo
        for (int ronda = 1; ronda <= 5; ++ronda) {
            std::cout << "\nRONDA " << ronda << " DE 5\n";
            std::cout << std::string(30, '─') << "\n";
            
            // Obtener mazos actuales (necesitarás agregar getters públicos a BatallaDeCartas)
            std::vector<int> mazoJ1 = juego->getMazoJugador1();
            std::vector<int> mazoJ2 = juego->getMazoJugador2();
            
            // Jugador 1 selecciona carta
            juego->mostrarMano(mazoJ1, "Jugador 1");
            int indiceJ1;
            do {
                std::cout << "Jugador 1, elige una carta (1-" << std::min(5, (int)mazoJ1.size()) << "): ";
                std::cin >> indiceJ1;
                indiceJ1--; // Convertir a índice 0-based
            } while (indiceJ1 < 0 || indiceJ1 >= (int)mazoJ1.size() || indiceJ1 >= 5);
            
            int cartaJ1 = mazoJ1[indiceJ1];
            
            // Jugador 2 selecciona carta
            juego->mostrarMano(mazoJ2, "Jugador 2");
            int indiceJ2;
            do {
                std::cout << "Jugador 2, elige una carta (1-" << std::min(5, (int)mazoJ2.size()) << "): ";
                std::cin >> indiceJ2;
                indiceJ2--; // Convertir a índice 0-based
            } while (indiceJ2 < 0 || indiceJ2 >= (int)mazoJ2.size() || indiceJ2 >= 5);
            
            int cartaJ2 = mazoJ2[indiceJ2];
            
            // Procesar la ronda (necesitarás un método público para esto)
            juego->procesarRonda(cartaJ1, cartaJ2, ronda);
            
            if (ronda < 5) {
                std::cout << "Presiona ENTER para continuar...";
                std::cin.get();
            }
        }
        
        // Obtener el resultado final
        int rondasJ1 = juego->getRondasGanadasJ1();
        int rondasJ2 = juego->getRondasGanadasJ2();
        
        juego->mostrarResultadoFinal();
        
        // Retornar el ganador
        if (rondasJ1 > rondasJ2) {
            return EstadoNodo::JUGADOR1;
        } else {
            return EstadoNodo::JUGADOR2;
        }
    }
    
    return EstadoNodo::VACIO;
}

// Utilidades
void Nodo::mostrarInfo() const {
    std::cout << "Nodo[" << fila << "][" << columna << "] - Estado: ";
    
    switch(estado) {
        case EstadoNodo::VACIO:
            std::cout << "VACIO";
            break;
        case EstadoNodo::JUGADOR1:
            std::cout << "JUGADOR1(X)";
            break;
        case EstadoNodo::JUGADOR2:
            std::cout << "JUGADOR2(O)";
            break;
    }
    
    std::cout << " - Activo: " << (activo ? "SI" : "NO");
    
    if (tieneMiniJuego) {
        std::cout << " - MiniJuego: ";
        switch(tipoMiniJuego) {
            case TipoMiniJuego::ADIVINA_NUMERO:
                std::cout << "ADIVINA_EL_NUMERO";
                break;
            case TipoMiniJuego::HEX:
                std::cout << "HEX";
                break;
            case TipoMiniJuego::BATALLA_CARTAS:
                std::cout << "BATALLA_CARTAS";
                break;
            default:
                std::cout << "NINGUNO";
                break;
        }
    }
    
    std::cout << std::endl;
}