#include "Minijuegos/AdivinaNumero/AdivinaNumero.h"
#include <algorithm>
#include <chrono>

AdivinaNumero::AdivinaNumero() 
    : MiniJuego(), numeroJugador1(-1), numeroJugador2(-1), 
      numeroJ1Elegido(false), numeroJ2Elegido(false), turnoActual(1),
      generador(std::chrono::steady_clock::now().time_since_epoch().count()) {
}

bool AdivinaNumero::establecerNumeroSecreto(int jugador, int numero) {
    if (!validarNumero(numero)) {
        return false;
    }
    
    if (jugador == 1 && !numeroJ1Elegido) {
        numeroJugador1 = numero;
        numeroJ1Elegido = true;
        return true;
    } else if (jugador == 2 && !numeroJ2Elegido) {
        numeroJugador2 = numero;
        numeroJ2Elegido = true;
        return true;
    }
    
    return false;
}

bool AdivinaNumero::validarNumero(int numero) const {
    return numero >= 1 && numero <= 100;
}

bool AdivinaNumero::ambosNumerosElegidos() const {
    return numeroJ1Elegido && numeroJ2Elegido;
}

bool AdivinaNumero::procesarMovimiento(int jugador, int numeroAdivinado) {
    if (juegoTerminado || !ambosNumerosElegidos() || jugador != turnoActual) {
        return false;
    }
    
    if (!validarNumero(numeroAdivinado)) {
        return false;
    }
    
    bool acierto = false;
    
    if (jugador == 1) {
        intentosJugador1.push_back(numeroAdivinado);
        if (numeroAdivinado == numeroJugador2) {
            terminarJuego(EstadoMiniJuego::GANADOR_JUGADOR1);
            acierto = true;

        }
        else if (numeroAdivinado>numeroJugador2){
            std::cout<<"El numero del jugador 2 es menor"<<std::endl;
        }
        else{
            std::cout<<"El numero del jugador 2 es mayor"<<std::endl;
        }
    } else if (jugador == 2) {
        intentosJugador2.push_back(numeroAdivinado);
        if (numeroAdivinado == numeroJugador1) {
            terminarJuego(EstadoMiniJuego::GANADOR_JUGADOR2);
            acierto = true;
        }
        else if (numeroAdivinado>numeroJugador1){
            std::cout<<"El numero del jugador 1 es menor"<<std::endl;
        }
        else{
            std::cout<<"El numero del jugador 1 es mayor"<<std::endl;
        }
    }
    
    // Cambiar turno solo si no acertó
    if (!acierto) {
        turnoActual = (turnoActual == 1) ? 2 : 1;
    }
    
    return true;
}

const std::vector<int>& AdivinaNumero::getIntentosJugador(int jugador) const {
    if (jugador == 1) {
        return intentosJugador1;
    } else {
        return intentosJugador2;
    }
}

void AdivinaNumero::mostrarEstado() const {
    std::cout << "\n=== ADIVINA EL NUMERO ===" << std::endl;
    std::cout << "Estado: ";
    
    switch(estado) {
        case EstadoMiniJuego::EN_PROGRESO:
            std::cout << "En progreso - Turno del Jugador " << turnoActual;
            break;
        case EstadoMiniJuego::GANADOR_JUGADOR1:
            std::cout << "Gano el Jugador 1!";
            break;
        case EstadoMiniJuego::GANADOR_JUGADOR2:
            std::cout << "Gano el Jugador 2!";
            break;
    }
    std::cout << std::endl;
    
    if (numeroJ1Elegido && numeroJ2Elegido) {
        std::cout << "Ambos jugadores han elegido sus numeros secretos." << std::endl;
    } else {
        std::cout << "Esperando que ";
        if (!numeroJ1Elegido) std::cout << "Jugador 1 ";
        if (!numeroJ1Elegido && !numeroJ2Elegido) std::cout << "y ";
        if (!numeroJ2Elegido) std::cout << "Jugador 2 ";
        std::cout << "elija su numero secreto." << std::endl;
    }
    
    std::cout << std::endl;
    
    
    if (juegoTerminado) {
        std::cout << "Numero secreto del Jugador 1 era: " << numeroJugador1 << std::endl;
        std::cout << "Numero secreto del Jugador 2 era: " << numeroJugador2 << std::endl;
    }
}

void AdivinaNumero::reiniciar() {
    estado = EstadoMiniJuego::EN_PROGRESO;
    juegoTerminado = false;
    numeroJugador1 = -1;
    numeroJugador2 = -1;
    numeroJ1Elegido = false;
    numeroJ2Elegido = false;
    turnoActual = 1;
    intentosJugador1.clear();
    intentosJugador2.clear();
}