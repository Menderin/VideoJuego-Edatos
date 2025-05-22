#include "Tablero.h"
#include <iostream>
using namespace std;

#include <string>

void mostrarMenu() {
    cout << "\n=== JUEGO DEL GATO - MENDEWING ===" << std::endl;

    cout << "===================================" << std::endl;
}

void mostrarInstrucciones() {
    cout << "\nInstrucciones del juego:" << endl;
    cout << "- El tablero es de 3x3, numerado de 0 a 2 (filas y columnas)" << endl;
    cout << "- Jugador 1 usa 'X', Jugador 2 usa 'O'" << endl;
    cout << "- Gana quien haga 3 en linea (horizontal, vertical o diagonal)" << endl;
    cout << "- Si el tablero se llena sin 3 en linea, gana quien tenga mas fichas" << endl;
    cout << "- Los minijuegos no pueden empatar, siempre hay un ganador" << endl;
}

int main() {
    Tablero tablero;
    std::string comando;
    
    std::cout << "¡Bienvenido al Gato de Mendewing!" << std::endl;
    mostrarInstrucciones();
    mostrarMenu();
    
    
    
    while (true) {
        tablero.mostrarTablero();
        
        int fila, columna;
        cout<<"Ingrese la fila: ";cin>>fila;
        cout<<"Ingrese la columna: ";cin>>columna;
            
        if (tablero.esMovimientoValido(fila, columna)) {
            bool movimientoExitoso = tablero.jugarNodo(fila, columna);
                
            if (movimientoExitoso) {
                cout << "¡Movimiento exitoso en [" << fila << "][" << columna << "]!" << std::endl;
                tablero.mostrarTablero();
                    
                // Verificar si el juego terminó
                EstadoJuego estado = tablero.getEstadoJuego();
                if (estado != EstadoJuego::EN_CURSO) {
                    cout << "\nJUEGO TERMINADO" << std::endl;
                    switch(estado) {
                        case EstadoJuego::GANADOR_J1:
                            cout << " JUGADOR 1 (X) GANA! " << std::endl;
                            break;
                        case EstadoJuego::GANADOR_J2:
                            cout << " JUGADOR 2 (O) GANA! " << std::endl;
                            break;
                            
                    }
                    cout << "Resultado final - J1: " << tablero.getFichasJ1() << " fichas | J2: " << tablero.getFichasJ2() << " fichas" << std::endl;
                    break;
                    
                    }
                } else {
                    std::cout << " Error: No se pudo realizar el movimiento." << std::endl;
                   
                }
            } else {
                std::cout << " Movimiento invalido. Verifica que:" << std::endl;
                std::cout << "   - Las coordenadas esten entre 0 y 2" << std::endl;
                std::cout << "   - La casilla este vacia" << std::endl;
                std::cout << "   - El juego este en curso" << std::endl;
            }
    }
    
    return 0;
}