#include "BatallaDeCartas.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

// Constructor
BatallaDeCartas::BatallaDeCartas() : rondasGanadasJ1(0), rondasGanadasJ2(0), totalRondas(5), rng(std::random_device{}()) {
    inicializarMazos();
}

void BatallaDeCartas::inicializarMazos() {
    // Crear mazos con cartas del 1 al 5 (múltiples copias para más variedad)
    std::vector<int> cartasBase = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    
    mazoJugador1 = cartasBase;
    mazoJugador2 = cartasBase;
    
    // Mezclar los mazos
    std::shuffle(mazoJugador1.begin(), mazoJugador1.end(), rng);
    std::shuffle(mazoJugador2.begin(), mazoJugador2.end(), rng);
}

void BatallaDeCartas::mostrarTitulo() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║          BATALLA DE CARTAS          ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "REGLAS:\n";
    std::cout << "• Cada jugador tiene cartas de ataque (1-5)\n";
    std::cout << "• Por turnos, juegan una carta\n";
    std::cout << "• La carta de mayor valor gana la ronda\n";
    std::cout << "• Gana quien consiga más rondas de " << totalRondas << "\n";
    std::cout << "\n" << std::string(40, '─') << "\n\n";
}

void BatallaDeCartas::mostrarMano(const std::vector<int>& mano, const std::string& jugador) {
    std::cout << "Mano de " << jugador << ": ";
    for (size_t i = 0; i < mano.size() && i < 5; ++i) {
        std::cout << "[" << mano[i] << "] ";
    }
    std::cout << "\n";
}

int BatallaDeCartas::seleccionarCarta(std::vector<int>& mano, const std::string& jugador) {
    if (jugador == "Jugador 1") {
        mostrarMano(mano, jugador);
        int indice;
        
        do {
            std::cout << "Elige una carta (1-" << std::min(5, (int)mano.size()) << "): ";
            std::cin >> indice;
            indice--; // Convertir a índice 0-based
            
            if (indice < 0 || indice >= (int)mano.size() || indice >= 5) {
                std::cout << "Selección inválida. Intenta de nuevo.\n";
            }
        } while (indice < 0 || indice >= (int)mano.size() || indice >= 5);
        
        int carta = mano[indice];
        mano.erase(mano.begin() + indice);
        return carta;
    } else {
        // IA simple: selecciona carta aleatoria de las primeras 5
        int indice = rng() % std::min(5, (int)mano.size());
        int carta = mano[indice];
        mano.erase(mano.begin() + indice);
        return carta;
    }
}

void BatallaDeCartas::mostrarResultadoRonda(int cartaJ1, int cartaJ2, int ronda) {
    std::cout << "\nRONDA " << ronda << ":\n";
    std::cout << "Jugador 1: [" << cartaJ1 << "] vs Jugador 2: [" << cartaJ2 << "]\n";
    
    if (cartaJ1 > cartaJ2) {
        std::cout << "¡Jugador 1 gana la ronda!\n";
        rondasGanadasJ1++;
    } else {
        std::cout << "¡Jugador 2 gana la ronda!\n";
        rondasGanadasJ2++;
    }
    
    std::cout << "Puntuación: Jugador 1: " << rondasGanadasJ1 
              << " | Jugador 2: " << rondasGanadasJ2 << "\n";
    std::cout << std::string(40, '─') << "\n";
}

void BatallaDeCartas::mostrarResultadoFinal() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║            RESULTADO FINAL          ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "Puntuación Final:\n";
    std::cout << "Jugador 1: " << rondasGanadasJ1 << " rondas\n";
    std::cout << "Jugador 2: " << rondasGanadasJ2 << " rondas\n";
    std::cout << "\n";
    
    if (rondasGanadasJ1 > rondasGanadasJ2) {
        std::cout << "FELICIDADES JUGADOR 1 HAS GANADO LA BATALLA\n";
    } else {
        std::cout << "JUGADOR 2 GANA LA BATALLA Mejor suerte la proxima vez\n";
    }
    std::cout << "\n" << std::string(40, '=') << "\n";
}

void BatallaDeCartas::jugar() {
    mostrarTitulo();
    
    // Esperar a que el jugador esté listo
    std::cout << "Presiona ENTER para comenzar...";
    std::cin.ignore();
    std::cin.get();
    
    // Jugar todas las rondas
    for (int ronda = 1; ronda <= totalRondas; ++ronda) {
        std::cout << "\nRONDA " << ronda << " DE " << totalRondas << "\n";
        std::cout << std::string(30, '─') << "\n";
        
        // Jugador 1 selecciona carta
        int cartaJ1 = seleccionarCarta(mazoJugador1, "Jugador 1");
        
        // Jugador 2 (IA) selecciona carta
        int cartaJ2 = seleccionarCarta(mazoJugador2, "Jugador 2");
        std::cout << "Jugador 2 selecciona: [" << cartaJ2 << "]\n";
        
        // Mostrar resultado de la ronda
        mostrarResultadoRonda(cartaJ1, cartaJ2, ronda);
        
        // Pausa entre rondas (excepto la última)
        if (ronda < totalRondas) {
            std::cout << "Presiona ENTER para continuar...";
            std::cin.get();
        }
    }
    
    // Mostrar resultado final
    mostrarResultadoFinal();
}

void BatallaDeCartas::reiniciarJuego() {
    rondasGanadasJ1 = 0;
    rondasGanadasJ2 = 0;
    inicializarMazos();
}


int main() {
    BatallaDeCartas juego;
    char opcion;
    
    do {
        juego.jugar();
        
        std::cout << "\n¿Quieres jugar otra partida? (s/n): ";
        std::cin >> opcion;
        
        if (opcion == 's' || opcion == 'S') {
            juego.reiniciarJuego();
            std::cout << "\n" << std::string(50, '=') << "\n";
        }
        
    } while (opcion == 's' || opcion == 'S');
    
    std::cout << "\nGracias por jugar Batalla de Cartas\n";
    return 0;
}
