#pragma once
#include "Minijuegos/MiniJuego.h"
#include <random>
#include <vector>
#include <iostream>

class AdivinaNumero : public MiniJuego {
private:
    int numeroJugador1;      // Número secreto del jugador 1
    int numeroJugador2;      // Número secreto del jugador 2
    bool numeroJ1Elegido;
    bool numeroJ2Elegido;
    
    std::vector<int> intentosJugador1;  // Intentos del J1 para adivinar número de J2
    std::vector<int> intentosJugador2;  // Intentos del J2 para adivinar número de J1
    
    int turnoActual; // 1 o 2
    
    std::mt19937 generador;
    
public:
    AdivinaNumero();
    
    // Implementación de métodos virtuales
    bool procesarMovimiento(int jugador, int numeroAdivinado) override;
    void mostrarEstado() const override;
    void reiniciar() override;
    int getNumeroJugador(int jugador) const;
    void setNumeroJugador(int jugador, int numero);
    // Métodos específicos del juego
    bool establecerNumeroSecreto(int jugador, int numero);
    bool validarNumero(int numero) const;
    bool ambosNumerosElegidos() const;
    
    // Getters para información del juego
    int getTurnoActual() const { return turnoActual; }
    const std::vector<int>& getIntentosJugador(int jugador) const;
};