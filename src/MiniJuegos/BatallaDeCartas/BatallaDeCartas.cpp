#include "Minijuegos/BatallaDeCartas/BatallaDeCartas.h"
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
    std::cout << "\n";
    std::cout << "BATALLA DE CARTAS\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "REGLAS:\n";
    std::cout << "• Cada jugador tiene cartas de ataque (1-5)\n";
    std::cout << "• Por turnos, juegan una carta\n";
    std::cout << "• La carta de mayor valor gana la ronda\n";
    std::cout << "• Gana quien consiga más rondas de " << totalRondas << "\n";
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
                std::cout << "Seleccion invalida. Intenta de nuevo.\n";
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
        std::cout << "Jugador 1 gana la ronda!\n";
        rondasGanadasJ1++;
    } else {
        std::cout << "Jugador 2 gana la ronda!\n";
        rondasGanadasJ2++;
    }
    
    std::cout << "Puntuacion: Jugador 1: " << rondasGanadasJ1 
              << " | Jugador 2: " << rondasGanadasJ2 << "\n";
}

void BatallaDeCartas::mostrarResultadoFinal() {
    std::cout << "\n";
    std::cout << "           RESULTADO FINAL          \n";
    std::cout << "\n";
    std::cout << "Puntuacion Final:\n";
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

// Implementación de métodos virtuales puros de MiniJuego con las firmas correctas
bool BatallaDeCartas::procesarMovimiento(int jugador, int input) {
    // Lógica para procesar movimiento según el jugador y el input
    // Retorna true si el movimiento es válido, false si no
    
    if (jugador == 1) {
        // Validar que el input esté en rango válido para las cartas disponibles
        if (input >= 1 && input <= std::min(5, (int)mazoJugador1.size())) {
            return true;
        }
    } else if (jugador == 2) {
        // Para la IA, siempre es válido
        return true;
    }
    
    return false;
}

void BatallaDeCartas::mostrarEstado() const {
    std::cout << "Estado actual del juego:\n";
    std::cout << "Jugador 1: " << rondasGanadasJ1 << " rondas ganadas\n";
    std::cout << "Jugador 2: " << rondasGanadasJ2 << " rondas ganadas\n";
    std::cout << "Cartas restantes Jugador 1: " << mazoJugador1.size() << "\n";
    std::cout << "Cartas restantes Jugador 2: " << mazoJugador2.size() << "\n";
}

void BatallaDeCartas::reiniciar() {
    rondasGanadasJ1 = 0;
    rondasGanadasJ2 = 0;
    inicializarMazos();
}

void BatallaDeCartas::jugar() {
    mostrarTitulo();
    
    // Esperar a que el jugador esté listo
    std::cout << "Presiona ENTER para comenzar...";
    std::cin.get();
    
    // Jugar todas las rondas
    for (int ronda = 1; ronda <= totalRondas; ++ronda) {
        std::cout << "\nRONDA " << ronda << " DE " << totalRondas << "\n";
        std::cout << std::string(30, '-') << "\n"; // Cambiado el carácter problemático
        
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
    reiniciar(); // Usa el método virtual puro implementado
}

void BatallaDeCartas::procesarRonda(int cartaJ1, int cartaJ2, int numeroRonda) {
    // Remover las cartas de los mazos
    auto it1 = std::find(mazoJugador1.begin(), mazoJugador1.end(), cartaJ1);
    if (it1 != mazoJugador1.end()) {
        mazoJugador1.erase(it1);
    }
    
    auto it2 = std::find(mazoJugador2.begin(), mazoJugador2.end(), cartaJ2);
    if (it2 != mazoJugador2.end()) {
        mazoJugador2.erase(it2);
    }
    
    // Mostrar el resultado de la ronda
    mostrarResultadoRonda(cartaJ1, cartaJ2, numeroRonda);
}