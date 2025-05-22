#include "Tablero.h"
#include <iostream>
#include <random>
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

void inicializarMinijuegosAleatorios(Tablero& tablero) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1); // Por ahora solo tenemos AdivinaNumero (tipo 1)
    
    cout << "\nInicializando minijuegos en cada casilla..." << endl;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Nodo& nodo = tablero.getNodo(i, j);
            
            // Por ahora solo asignamos AdivinaNumero a todas las casillas
            nodo.asignarMiniJuego(TipoMiniJuego::ADIVINA_NUMERO);
            
            //cout << "Casilla [" << i << "][" << j << "]: Adivina el Número" << endl;
        }
    }
    cout << "Minijuegos inicializados!" << endl;
}

void mostrarTableroConMinijuegos(const Tablero& tablero) {
    cout << "\n=== Estado del Tablero con Minijuegos ===" << endl;
    
    // Mostrar tablero visual
    cout << "Tablero:" << endl;
    cout << "   0   1   2" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i << " ";
        for (int j = 0; j < 3; j++) {
            cout << "| ";
            const Nodo& nodo = tablero.getNodo(i, j);
            
            if (nodo.estaVacio()) {
                // Mostrar tipo de minijuego si está vacío
                if (nodo.tieneMiniJuegoAsignado()) {
                    switch(nodo.getTipoMiniJuego()) {
                        case TipoMiniJuego::ADIVINA_NUMERO:
                            cout << "?";
                            break;
                        case TipoMiniJuego::HEX:
                            cout << "H";
                            break;
                        case TipoMiniJuego::BATALLA_CARTAS:
                            cout << "C";
                            break;
                        default:
                            cout << " ";
                            break;
                    }
                } else {
                    cout << " ";
                }
            } else {
                // Mostrar el ganador del minijuego
                switch(nodo.getEstado()) {
                    case EstadoNodo::JUGADOR1:
                        cout << "X";
                        break;
                    case EstadoNodo::JUGADOR2:
                        cout << "O";
                        break;
                    default:
                        cout << " ";
                        break;
                }
            }
            cout << " ";
        }
        cout << "|" << endl;
        if (i < 2) cout << "  -----------" << endl;
    }
    
    cout << "\nLeyenda: ? = Adivina Numero, H = Hex, C = Cartas" << endl;
    cout << "Fichas J1(X): " << tablero.getFichasJ1() << " | Fichas J2(O): " << tablero.getFichasJ2() << endl;
    cout << "Turno actual: Jugador " << tablero.getTurnoActual() << endl;
    
    switch(tablero.getEstadoJuego()) {
        case EstadoJuego::EN_CURSO:
            cout << "Estado: EN CURSO" << endl;
            break;
        case EstadoJuego::GANADOR_J1:
            cout << "Estado: GANADOR JUGADOR 1 (X)" << endl;
            break;
        case EstadoJuego::GANADOR_J2:
            cout << "Estado: GANADOR JUGADOR 2 (O)" << endl;
            break;
        case EstadoJuego::EMPATE:
            cout << "Estado: EMPATE" << endl;
            break;
    }
    cout << "========================================\n" << endl;
}

int main() {
    Tablero tablero;
    std::string comando;
    
    std::cout << "¡Bienvenido al Gato de Mendewing!" << std::endl;
    mostrarInstrucciones();
    mostrarMenu();
    inicializarMinijuegosAleatorios(tablero);
    
    
 /**    while (true) {
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
    } **/
    
    while (true) {
        mostrarTableroConMinijuegos(tablero);
        
        if (tablero.getEstadoJuego() != EstadoJuego::EN_CURSO) {
            cout << "\nJUEGO TERMINADO!" << endl;
            switch(tablero.getEstadoJuego()) {
                case EstadoJuego::GANADOR_J1:
                    cout << "JUGADOR 1 (X) GANA! " << endl;
                    break;
                case EstadoJuego::GANADOR_J2:
                    cout << "JUGADOR 2 (O) GANA! " << endl;
                    break;
                case EstadoJuego::EMPATE:
                    cout << " EMPATE! " << endl;
                    break;
            }
            cout << "Resultado final - J1: " << tablero.getFichasJ1() << " fichas | J2: " << tablero.getFichasJ2() << " fichas" << endl;
            break;
        }
        
        int fila, columna;
        cout << "Jugador " << tablero.getTurnoActual() << ", elige la casilla a jugar:" << endl;
        cout << "Ingrese la fila (0-2): ";
        cin >> fila;
        cout << "Ingrese la columna (0-2): ";
        cin >> columna;
        
        if (tablero.esMovimientoValido(fila, columna)) {
            const Nodo& nodo = tablero.getNodo(fila, columna);
            
            cout << "\nPreparandose para jugar en casilla [" << fila << "][" << columna << "]!" << endl;
            
            if (nodo.tieneMiniJuegoAsignado()) {
                cout << "Minijuego: ";
                switch(nodo.getTipoMiniJuego()) {
                    case TipoMiniJuego::ADIVINA_NUMERO:
                        cout << "Adivina el Numero" << endl;
                        break;
                    case TipoMiniJuego::HEX:
                        cout << "Hex" << endl;
                        break;
                    case TipoMiniJuego::BATALLA_CARTAS:
                        cout << "Batalla de Cartas" << endl;
                        break;
                    default:
                        cout << "Desconocido" << endl;
                        break;
                }
                cout << "Que comience el minijuego!" << endl;
            }
            
            bool movimientoExitoso = tablero.jugarNodo(fila, columna);
            
            if (movimientoExitoso) {
                cout << "\n Movimiento completado en [" << fila << "][" << columna << "]!" << endl;
                
                // Mostrar quién ganó el minijuego
                const Nodo& nodoActualizado = tablero.getNodo(fila, columna);
                if (!nodoActualizado.estaVacio()) {
                    cout << " Ganador del minijuego: Jugador " 
                         << (nodoActualizado.getEstado() == EstadoNodo::JUGADOR1 ? "1 (X)" : "2 (O)") 
                         << endl;
                }
            } else {
                cout << " Error: No se pudo realizar el movimiento." << endl;
            }
        } else {
            cout << " Movimiento invalido. Verifica que:" << endl;
            cout << "   - Las coordenadas esten entre 0 y 2" << endl;
            cout << "   - La casilla esté vacía" << endl;
            cout << "   - El juego esté en curso" << endl;
        }
        
        cout << "\nPresiona Enter para continuar...";
        cin.ignore();
        cin.get();
    }
    
    cout << "\n¡Gracias por jugar al Gato de Mendewing!" << endl;



    return 0;
}