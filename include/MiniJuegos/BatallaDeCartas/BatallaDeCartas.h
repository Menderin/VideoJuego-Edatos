#pragma once
#include <vector>
#include <random>
#include <string>
#include "Minijuegos/MiniJuego.h"

class BatallaDeCartas : public MiniJuego{
    
public:
    // Constructor
    std::vector<int> mazoJugador1;
    std::vector<int> mazoJugador2;
    int rondasGanadasJ1;
    int rondasGanadasJ2;
    int totalRondas;
    std::mt19937 rng;
    
    BatallaDeCartas();
    
    // Métodos virtuales puros de MiniJuego
    bool procesarMovimiento(int jugador, int input) override;
    void mostrarEstado() const override;
    void reiniciar() override;

    // Métodos públicos
    void inicializarMazos();
    void mostrarTitulo();
    void mostrarMano(const std::vector<int>& mano, const std::string& jugador);
    int seleccionarCarta(std::vector<int>& mano, const std::string& jugador);
    void mostrarResultadoRonda(int cartaJ1, int cartaJ2, int ronda);
    void mostrarResultadoFinal();
    void jugar();
    void reiniciarJuego();

    // NUEVOS MÉTODOS PÚBLICOS NECESARIOS:
    std::vector<int> getMazoJugador1() const { return mazoJugador1; }
    std::vector<int> getMazoJugador2() const { return mazoJugador2; }
    int getRondasGanadasJ1() const { return rondasGanadasJ1; }
    int getRondasGanadasJ2() const { return rondasGanadasJ2; }
    
    void procesarRonda(int cartaJ1, int cartaJ2, int numeroRonda);
    void mostrarMano(const std::vector<int>& mano, const std::string& jugador) const;

};
