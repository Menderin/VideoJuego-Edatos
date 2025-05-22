#ifndef BATALLADECARTAS_H
#define BATALLADECARTAS_H


#include <vector>
#include <random>
#include <string>

class BatallaDeCartas {
private:
    std::vector<int> mazoJugador1;
    std::vector<int> mazoJugador2;
    int rondasGanadasJ1;
    int rondasGanadasJ2;
    int totalRondas;
    std::mt19937 rng;

public:
    // Constructor
    BatallaDeCartas();
    
    // Métodos públicos
    void inicializarMazos();
    void mostrarTitulo();
    void mostrarMano(const std::vector<int>& mano, const std::string& jugador);
    int seleccionarCarta(std::vector<int>& mano, const std::string& jugador);
    void mostrarResultadoRonda(int cartaJ1, int cartaJ2, int ronda);
    void mostrarResultadoFinal();
    void jugar();
    void reiniciarJuego();
};

#endif // BATALLADECARTAS_H