//versión funcional del tablero con sistema de 3 en raya principal
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <random>
#include "MiniJuegos/Hex/Hex.h" // Añade esta línea

//minijuegos
// ...resto del código...

//minijuegos

enum class GameState {
    MENU,
    GAME, //para que no de errores
};

// Estados para el juego de adivinar número
enum class AdivinaState {
    JUGADOR1_INGRESA,
    JUGADOR2_INGRESA,
    JUGANDO_TURNO_J1,
    JUGANDO_TURNO_J2
};

bool verificarVictoriaTablero(char tablero[9]) {
    // Combinaciones ganadoras
    int combinaciones[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Filas
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columnas
        {0, 4, 8}, {2, 4, 6}             // Diagonales
    };

    for (int i = 0; i < 8; i++) {
        if (tablero[combinaciones[i][0]] != ' ' &&
            tablero[combinaciones[i][0]] == tablero[combinaciones[i][1]] &&
            tablero[combinaciones[i][1]] == tablero[combinaciones[i][2]]) {
            return true;
        }
    }
    return false;
}


// Función para mostrar la ventana del ganador
void mostrarVentanaGanador(int puntosJ1, int puntosJ2) {
    sf::RenderWindow ventanaGanador(sf::VideoMode({600, 300}), "Resultado Final");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Título
    sf::Text titulo(fuente, "Fin del Juego", 36);
    titulo.setPosition({200, 30});
    titulo.setFillColor(sf::Color::Black);
    
    // Puntuaciones
    sf::Text txtPuntosJ1(fuente, "Jugador 1: " + std::to_string(puntosJ1) + " pts", 24);
    txtPuntosJ1.setPosition({50, 100});
    txtPuntosJ1.setFillColor(sf::Color::Blue);
    
    sf::Text txtPuntosJ2(fuente, "Jugador 2: " + std::to_string(puntosJ2) + " pts", 24);
    txtPuntosJ2.setPosition({50, 140});
    txtPuntosJ2.setFillColor(sf::Color::Red);
    
    // Mensaje ganador
    sf::Text mensajeGanador(fuente, "", 50);
    mensajeGanador.setPosition({50, 180});
    
    if (puntosJ1 > puntosJ2) {
        mensajeGanador.setString("Jugador 1 gana");
        mensajeGanador.setFillColor(sf::Color::Blue);
    } else if (puntosJ2 > puntosJ1) {
        mensajeGanador.setString("Jugador 2 gana");
        mensajeGanador.setFillColor(sf::Color::Red);
    } else {
        mensajeGanador.setString("Empate");
        mensajeGanador.setFillColor(sf::Color::Green);
    }

    
    while (ventanaGanador.isOpen()) {
        while (const std::optional event = ventanaGanador.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaGanador.close();
            }
        }
        
        ventanaGanador.clear(sf::Color::White);
        ventanaGanador.draw(titulo);
        ventanaGanador.draw(txtPuntosJ1);
        ventanaGanador.draw(txtPuntosJ2);
        ventanaGanador.draw(mensajeGanador);
        ventanaGanador.display();
    }
}



// Variables globales para el tablero principal
char tableroPrincipal[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
int casillaMiniJuego = -1; // Para saber en qué casilla se jugó el minijuego

// Función para mostrar ventana de victoria del tablero principal
void mostrarVentanaVictoriaTablero(char simboloGanador) {
    sf::RenderWindow ventanaVictoria(sf::VideoMode({500, 300}), "Victoria del Tablero Principal!");
    
    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Título de victoria
    sf::Text tituloVictoria(fuente, "VICTORIA TOTAL!", 48);
    tituloVictoria.setPosition({80, 50});
    tituloVictoria.setFillColor(sf::Color::Yellow);
    
    // Mensaje del ganador
    std::string jugadorGanador = (simboloGanador == 'X') ? "Jugador 1 (X)" : "Jugador 2 (O)";
    std::string mensajeGanador = jugadorGanador + " ha ganado el 3 en raya!";
    sf::Text textoGanador(fuente, mensajeGanador, 24);
    textoGanador.setPosition({60, 120});
    textoGanador.setFillColor(sf::Color::Blue);
    
    // Mensaje adicional
    sf::Text textoAdicional(fuente, "Consiguio 3 en linea en el tablero principal", 18);
    textoAdicional.setPosition({80, 160});
    textoAdicional.setFillColor(sf::Color::Black);
    
    // Botón para cerrar
    sf::RectangleShape btnCerrar({120, 40});
    btnCerrar.setPosition({190, 220});
    btnCerrar.setFillColor(sf::Color::Red);
    
    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({225, 230});
    txtCerrar.setFillColor(sf::Color::White);
    
    // Loop de la ventana de victoria
    while (ventanaVictoria.isOpen()) {
        while (const std::optional event = ventanaVictoria.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaVictoria.close();
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaVictoria);
                
                // Verificar click en botón cerrar
                if (mousePos.x >= 190 && mousePos.x <= 310 &&
                    mousePos.y >= 220 && mousePos.y <= 260) {
                    ventanaVictoria.close();
                }
            }
            
            // También permitir cerrar con cualquier tecla
            if (event->is<sf::Event::KeyPressed>()) {
                ventanaVictoria.close();
            }
        }
        
        ventanaVictoria.clear(sf::Color::White);
        ventanaVictoria.draw(tituloVictoria);
        ventanaVictoria.draw(textoGanador);
        ventanaVictoria.draw(textoAdicional);
        ventanaVictoria.draw(btnCerrar);
        ventanaVictoria.draw(txtCerrar);
        ventanaVictoria.display();
    }
}

// Función para mostrar ventana de victoria del minijuego (modificada)
// Función para mostrar ventana de victoria del minijuego (simplificada)
void mostrarVentanaVictoria(int jugadorGanador, int numeroSecreto) {
    sf::RenderWindow ventanaVictoria(sf::VideoMode({500, 300}), "Victoria!");

    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Título de victoria
    sf::Text tituloVictoria(fuente, "VICTORIA!", 48);
    tituloVictoria.setPosition({120, 50});
    tituloVictoria.setFillColor(sf::Color::Green);

    // Mensaje del ganador
    std::string mensajeGanador = "Jugador " + std::to_string(jugadorGanador) + " ha ganado!";
    sf::Text textoGanador(fuente, mensajeGanador, 28);
    textoGanador.setPosition({80, 120});
    textoGanador.setFillColor(jugadorGanador == 1 ? sf::Color::Blue : sf::Color::Red);

    // Mensaje del número secreto (solo para Adivina el Número)
    if (numeroSecreto != -1) {
        std::string mensajeNumero = "El numero secreto era: " + std::to_string(numeroSecreto);
        sf::Text textoNumero(fuente, mensajeNumero, 22);
        textoNumero.setPosition({100, 160});
        textoNumero.setFillColor(sf::Color::Black);
        ventanaVictoria.draw(textoNumero);
    }

    // Botón para cerrar
    sf::RectangleShape btnCerrar({120, 40});
    btnCerrar.setPosition({190, 230});
    btnCerrar.setFillColor(sf::Color::Red);

    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({225, 240});
    txtCerrar.setFillColor(sf::Color::White);

    // Loop de la ventana de victoria
    while (ventanaVictoria.isOpen()) {
        while (const std::optional event = ventanaVictoria.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaVictoria.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaVictoria);

                // Verificar click en botón cerrar
                if (mousePos.x >= 190 && mousePos.x <= 310 &&
                    mousePos.y >= 230 && mousePos.y <= 270) {
                    ventanaVictoria.close();
                }
            }

            // También permitir cerrar con cualquier tecla
            if (event->is<sf::Event::KeyPressed>()) {
                ventanaVictoria.close();
            }
        }

        ventanaVictoria.clear(sf::Color::White);
        ventanaVictoria.draw(tituloVictoria);
        ventanaVictoria.draw(textoGanador);
        ventanaVictoria.draw(btnCerrar);
        ventanaVictoria.draw(txtCerrar);
        ventanaVictoria.display();
    }

    // Verificar victoria en el tablero principal
    if (verificarVictoriaTablero(tableroPrincipal)) {
        char simboloGanador = (jugadorGanador == 1) ? 'X' : 'O';
        mostrarVentanaVictoriaTablero(simboloGanador);
    }
}



// Función para abrir ventana de Hex
sf::CircleShape crearHexagono(float radio, sf::Vector2f posicion) {
    sf::CircleShape hexagono(radio, 6); // 6 lados para hacer un hexágono
    hexagono.setPosition(posicion);
    hexagono.setFillColor(sf::Color::White);
    hexagono.setOutlineThickness(2);
    hexagono.setOutlineColor(sf::Color::Black);
    return hexagono;
}


// Función para abrir ventana de Hex
// Función para abrir ventana de Hex
void abrirHex(int casilla) {
    casillaMiniJuego = casilla;

    sf::RenderWindow ventanaHex(sf::VideoMode({800, 700}), "Juego Hex");

    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Crear título
    sf::Text titulo(fuente, "Juego Hex", 36);
    titulo.setPosition({350, 20});
    titulo.setFillColor(sf::Color::Black);

    // Instrucciones
    sf::Text instrucciones(fuente, "Jugador 1 (Rojo) vs Jugador 2 (Azul)", 20);
    instrucciones.setPosition({300, 60});
    instrucciones.setFillColor(sf::Color::Blue);

    // Botón de cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({350, 630});
    btnCerrar.setFillColor(sf::Color::Red);

    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({375, 640});
    txtCerrar.setFillColor(sf::Color::White);

    // Crear el tablero hexagonal (11x11)
    const int TABLERO_SIZE = 11;
    const float HEX_RADIO = 25.0f; // Ajustar el radio del hexágono
    const float HEX_SPACING = 40.0f; // Ajustar el espaciado entre hexágonos

    std::vector<std::vector<sf::CircleShape>> tableroHex(TABLERO_SIZE, std::vector<sf::CircleShape>(TABLERO_SIZE));

    // Crear los hexágonos del tablero
    for(int fila = 0; fila < TABLERO_SIZE; fila++) {
        for(int col = 0; col < TABLERO_SIZE; col++) {
            // Calcular posición con offset para crear forma de diamante
            float x = 150 + col * HEX_SPACING + (fila * HEX_SPACING * 0.5f);
            float y = 200 + fila * (HEX_SPACING * 0.866f);

            sf::CircleShape hex = crearHexagono(HEX_RADIO, {x, y});
            tableroHex[fila][col] = hex;
        }
    }

    // Colorear bordes
    for(int i = 0; i < TABLERO_SIZE; i++) {
        // Bordes superior e inferior (azul)
        tableroHex[0][i].setOutlineColor(sf::Color::Blue);
        tableroHex[0][i].setOutlineThickness(4);
        tableroHex[TABLERO_SIZE-1][i].setOutlineColor(sf::Color::Blue);
        tableroHex[TABLERO_SIZE-1][i].setOutlineThickness(4);

        // Bordes izquierdo y derecho (rojo)
        tableroHex[i][0].setOutlineColor(sf::Color::Red);
        tableroHex[i][0].setOutlineThickness(4);
        tableroHex[i][TABLERO_SIZE-1].setOutlineColor(sf::Color::Red);
        tableroHex[i][TABLERO_SIZE-1].setOutlineThickness(4);
    }

    // Crear una instancia del juego Hex
    Hex juegoHex;

    // Loop de la ventana
    while (ventanaHex.isOpen()) {
        while (const std::optional event = ventanaHex.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaHex.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaHex);

                // Verificar click en botón cerrar
                if (mousePos.x >= 350 && mousePos.x <= 450 &&
                    mousePos.y >= 630 && mousePos.y <= 670) {
                    ventanaHex.close();
                }

                // Detectar clicks en hexágonos del tablero
                bool movimientoRealizado = false;
                for(int fila = 0; fila < TABLERO_SIZE && !movimientoRealizado; fila++) {
                    for(int col = 0; col < TABLERO_SIZE && !movimientoRealizado; col++) {
                        sf::Vector2f hexCenter = tableroHex[fila][col].getPosition() + sf::Vector2f(HEX_RADIO, HEX_RADIO);
                        float distance = std::sqrt(std::pow(mousePos.x - hexCenter.x, 2) + std::pow(mousePos.y - hexCenter.y, 2));

                        if (distance <= HEX_RADIO) {
                            // Lógica para manejar el clic en el hexágono
                            if (juegoHex.hacerMovimiento(fila, col)) {
                                // Actualizar el tablero gráficamente
                                EstadoCasilla estado = juegoHex.getCasilla(fila, col);
                                if (estado == EstadoCasilla::JUGADOR1) {
                                    tableroHex[fila][col].setFillColor(sf::Color::Red);
                                } else if (estado == EstadoCasilla::JUGADOR2) {
                                    tableroHex[fila][col].setFillColor(sf::Color::Blue);
                                }

                                // Verificar si hay un ganador
                                if (juegoHex.estaTerminado()) {
                                    int ganador = juegoHex.getGanador();
                                    std::cout << "¡Jugador " << ganador << " ha ganado!" << std::endl;

                                    // Marcar en el tablero principal
                                    if (casillaMiniJuego >= 0 && casillaMiniJuego < 9) {
                                        tableroPrincipal[casillaMiniJuego] = (ganador == 1) ? 'X' : 'O';
                                        std::cout << "Marcando " << ((ganador == 1) ? 'X' : 'O')
                                                  << " en casilla " << casillaMiniJuego << std::endl;
                                    }

                                    // Mostrar ventana de victoria
                                    mostrarVentanaVictoria(ganador, -1);
                                    ventanaHex.close();
                                }
                                movimientoRealizado = true;
                            }
                        }
                    }
                }
            }
        }

        ventanaHex.clear(sf::Color::White);
        ventanaHex.draw(titulo);
        ventanaHex.draw(instrucciones);

        // Dibujar todos los hexágonos del tablero
        for(int fila = 0; fila < TABLERO_SIZE; fila++) {
            for(int col = 0; col < TABLERO_SIZE; col++) {
                ventanaHex.draw(tableroHex[fila][col]);
            }
        }

        ventanaHex.draw(btnCerrar);
        ventanaHex.draw(txtCerrar);
        ventanaHex.display();
    }
}






// Función para abrir ventana de "Adivina el número" (modificada para recibir la casilla)
void abrirAdivinaNumero(int casilla) {
    casillaMiniJuego = casilla; // Guardar en qué casilla se jugó
    
    sf::RenderWindow ventanaAdivina(sf::VideoMode({600, 450}), "Adivina el Numero");
    
    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Estado del juego de adivinar
    AdivinaState estadoAdivina = AdivinaState::JUGADOR1_INGRESA;
    int numeroJugador1 = 0;
    int numeroJugador2 = 0;
    
    // Crear título
    sf::Text titulo(fuente, "Adivina el Numero", 36);
    titulo.setPosition({150, 30});
    titulo.setFillColor(sf::Color::Black);
    
    // Mensaje para jugador (se actualizará según el estado)
    sf::Text mensajeJugador(fuente, "Jugador 1: Elige tu numero", 24);
    mensajeJugador.setPosition({150, 90});
    mensajeJugador.setFillColor(sf::Color::Blue);
    
    // Campo de entrada de número
    sf::RectangleShape campoNumero({200, 40});
    campoNumero.setPosition({200, 140});
    campoNumero.setFillColor(sf::Color::White);
    campoNumero.setOutlineThickness(2);
    campoNumero.setOutlineColor(sf::Color::Black);
    
    // Texto del número ingresado
    std::string numeroIngresado = "";
    sf::Text textoNumero(fuente, numeroIngresado, 24);
    textoNumero.setPosition({210, 148});
    textoNumero.setFillColor(sf::Color::Black);
    
    // Instrucciones
    sf::Text instrucciones(fuente, "Ingresa un numero entre 1 y 100", 18);
    instrucciones.setPosition({170, 200});
    instrucciones.setFillColor(sf::Color::Green);
    
    // Mensaje de resultado
    sf::Text mensajeResultado(fuente, "", 18);
    mensajeResultado.setPosition({150, 240});
    mensajeResultado.setFillColor(sf::Color::Magenta);
    
    // Botón de confirmar
    sf::RectangleShape btnConfirmar({120, 40});
    btnConfirmar.setPosition({240, 280});
    btnConfirmar.setFillColor(sf::Color::Blue);
    
    sf::Text txtConfirmar(fuente, "Confirmar", 18);
    txtConfirmar.setPosition({255, 290});
    txtConfirmar.setFillColor(sf::Color::White);
    
    // Botón de cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({250, 350});
    btnCerrar.setFillColor(sf::Color::Red);
    
    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({275, 360});
    txtCerrar.setFillColor(sf::Color::White);
    
    // Loop de la ventana
    while (ventanaAdivina.isOpen()) {
        while (const std::optional event = ventanaAdivina.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaAdivina.close();
            }
            
            // Manejar entrada de texto
            if (auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                char c = static_cast<char>(textEvent->unicode);
                
                // Solo permitir números y limitar a 3 dígitos
                if (c >= '0' && c <= '9' && numeroIngresado.length() < 3) {
                    numeroIngresado += c;
                    textoNumero.setString(numeroIngresado);
                }
                // Permitir borrar con backspace
                else if (c == 8 && !numeroIngresado.empty()) { // 8 es backspace
                    numeroIngresado.pop_back();
                    textoNumero.setString(numeroIngresado);
                }
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaAdivina);
                
                // Verificar click en botón confirmar
                if (mousePos.x >= 240 && mousePos.x <= 360 &&
                    mousePos.y >= 280 && mousePos.y <= 320) {
                    
                    if (!numeroIngresado.empty()) {
                        int numero = std::stoi(numeroIngresado);
                        
                        if (numero >= 1 && numero <= 100) {
                            if (estadoAdivina == AdivinaState::JUGADOR1_INGRESA) {
                                // Guardar número del jugador 1 y cambiar al jugador 2
                                numeroJugador1 = numero;
                                estadoAdivina = AdivinaState::JUGADOR2_INGRESA;
                                
                                // Actualizar textos para jugador 2
                                mensajeJugador.setString("Jugador 2: Elige tu numero");
                                mensajeJugador.setFillColor(sf::Color::Red);
                                instrucciones.setString("Ingresa un numero entre 1 y 100");
                                
                                // Limpiar campo de entrada
                                numeroIngresado = "";
                                textoNumero.setString(numeroIngresado);
                                
                                std::cout << "Jugador 1 eligio: " << numeroJugador1 << std::endl;
                            }
                            else if (estadoAdivina == AdivinaState::JUGADOR2_INGRESA) {
                                // Guardar número del jugador 2 e iniciar el juego
                                numeroJugador2 = numero;
                                estadoAdivina = AdivinaState::JUGANDO_TURNO_J1;
                                
                                // Actualizar textos para comenzar el juego
                                mensajeJugador.setString("Jugador 1: Adivina el numero del J2");
                                mensajeJugador.setFillColor(sf::Color::Blue);
                                instrucciones.setString("Intenta adivinar el numero secreto");
                                mensajeResultado.setString("");
                                
                                // Limpiar campo de entrada
                                numeroIngresado = "";
                                textoNumero.setString(numeroIngresado);
                                
                                std::cout << "Jugador 2 eligio: " << numeroJugador2 << std::endl;
                                std::cout << "Comienza el juego!" << std::endl;
                            }
                            else if (estadoAdivina == AdivinaState::JUGANDO_TURNO_J1) {
                                // Jugador 1 intenta adivinar el número del jugador 2
                                if (numero == numeroJugador2) {
                                    std::cout << "Jugador 1 acerto! El numero era: " << numeroJugador2 << std::endl;
                                    
                                    // Mostrar ventana de victoria y cerrar juego
                                    mostrarVentanaVictoria(1, numeroJugador2);
                                    ventanaAdivina.close();
                                } else {
                                    // Dar pista y cambiar turno
                                    if (numero < numeroJugador2) {
                                        mensajeResultado.setString("El numero del jugador 2 es MAYOR. Turno del J2");
                                    } else {
                                        mensajeResultado.setString("El numero del jugador 2 es MENOR. Turno del J2");
                                    }
                                    estadoAdivina = AdivinaState::JUGANDO_TURNO_J2;
                                    mensajeJugador.setString("Jugador 2: Adivina el numero del J1");
                                    mensajeJugador.setFillColor(sf::Color::Red);
                                }
                                // Limpiar campo
                                numeroIngresado = "";
                                textoNumero.setString(numeroIngresado);
                            }
                            else if (estadoAdivina == AdivinaState::JUGANDO_TURNO_J2) {
                                // Jugador 2 intenta adivinar el número del jugador 1
                                if (numero == numeroJugador1) {
                                    std::cout << "Jugador 2 acerto! El numero era: " << numeroJugador1 << std::endl;
                                    
                                    // Mostrar ventana de victoria y cerrar juego
                                    mostrarVentanaVictoria(2, numeroJugador1);
                                    ventanaAdivina.close();
                                } else {
                                    // Dar pista y cambiar turno
                                    if (numero < numeroJugador1) {
                                        mensajeResultado.setString("El numero del jugador 1 es MAYOR. Turno del J1");
                                    } else {
                                        mensajeResultado.setString("El numero del jugador 1 es MENOR. Turno del J1");
                                    }
                                    estadoAdivina = AdivinaState::JUGANDO_TURNO_J1;
                                    mensajeJugador.setString("Jugador 1: Adivina el numero del J2");
                                    mensajeJugador.setFillColor(sf::Color::Blue);
                                }
                                // Limpiar campo
                                numeroIngresado = "";
                                textoNumero.setString(numeroIngresado);
                            }
                        }
                    }
                }
                
                // Verificar click en botón cerrar
                if (mousePos.x >= 250 && mousePos.x <= 350 &&
                    mousePos.y >= 350 && mousePos.y <= 390) {
                    ventanaAdivina.close();
                }
            }
        }
        
        ventanaAdivina.clear(sf::Color::White);
        ventanaAdivina.draw(titulo);
        ventanaAdivina.draw(mensajeJugador);
        ventanaAdivina.draw(campoNumero);
        ventanaAdivina.draw(textoNumero);
        ventanaAdivina.draw(instrucciones);
        ventanaAdivina.draw(btnConfirmar);
        ventanaAdivina.draw(txtConfirmar);
        ventanaAdivina.draw(btnCerrar);
        ventanaAdivina.draw(txtCerrar);
        ventanaAdivina.draw(mensajeResultado);
        ventanaAdivina.display();
    }
}

// Función para la ventana de cada jugador
bool ventanaJugador(bool esJugador1, std::vector<int>& valoresCartas, int& cartaSeleccionada, int puntosJ1, int puntosJ2) {
    std::string titulo = esJugador1 ? "Turno Jugador 1" : "Turno Jugador 2";
    sf::Color colorJugador = esJugador1 ? sf::Color::Blue : sf::Color::Red;
    
    sf::RenderWindow ventana(sf::VideoMode({600, 400}), titulo);
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Título
    sf::Text tituloTexto(fuente, titulo, 24);
    tituloTexto.setPosition({50, 30});
    tituloTexto.setFillColor(colorJugador);
    
    // Mostrar cartas
    std::vector<sf::Text> cartasTexto;
    for(int i = 0; i < valoresCartas.size(); i++) {
        sf::Text carta(fuente, std::to_string(valoresCartas[i]), 30);
        carta.setPosition({80.f + (i * 60), 100});
        carta.setFillColor(sf::Color::Black);
        cartasTexto.push_back(carta);
    }
    
    // Campo de selección
    sf::RectangleShape campoSeleccion({60, 40});
    campoSeleccion.setPosition({50, 200});
    campoSeleccion.setFillColor(sf::Color::White);
    campoSeleccion.setOutlineThickness(2);
    campoSeleccion.setOutlineColor(sf::Color::Black);
    
    std::string numeroSeleccionado = "";
    sf::Text textoSeleccion(fuente, numeroSeleccionado, 24);
    textoSeleccion.setPosition({60, 205});
    textoSeleccion.setFillColor(sf::Color::Black);
    
    // Botón confirmar
    sf::RectangleShape btnConfirmar({120, 40});
    btnConfirmar.setPosition({150, 200});
    btnConfirmar.setFillColor(colorJugador);
    
    sf::Text txtConfirmar(fuente, "Confirmar", 18);
    txtConfirmar.setPosition({165, 210});
    txtConfirmar.setFillColor(sf::Color::White);
    
    // Instrucciones
    sf::Text instrucciones(fuente, "Selecciona una carta (1-" + 
                          std::to_string(valoresCartas.size()) + "):", 18);
    instrucciones.setPosition({50, 160});
    instrucciones.setFillColor(sf::Color::Black);
    
    // Textos de puntuación
    sf::Text txtPuntosJ1(fuente, "Puntos J1: " + std::to_string(puntosJ1), 20);
    txtPuntosJ1.setPosition({350, 30}); // Arriba a la derecha
    txtPuntosJ1.setFillColor(sf::Color::Blue);
    
    sf::Text txtPuntosJ2(fuente, "Puntos J2: " + std::to_string(puntosJ2), 20);
    txtPuntosJ2.setPosition({350, 60}); // Debajo del puntaje J1
    txtPuntosJ2.setFillColor(sf::Color::Red);
    
    while (ventana.isOpen()) {
        while (const std::optional event = ventana.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventana.close();
                return false;
            }
            
            if (auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                char c = static_cast<char>(textEvent->unicode);
                if (c >= '1' && c <= '5' && numeroSeleccionado.empty()) {
                    int indice = c - '1';
                    if (indice >= 0 && indice < valoresCartas.size()) {
                        numeroSeleccionado += c;
                        textoSeleccion.setString(numeroSeleccionado);
                    }
                }
                else if (c == 8 && !numeroSeleccionado.empty()) {
                    numeroSeleccionado.pop_back();
                    textoSeleccion.setString(numeroSeleccionado);
                }
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
                if (mousePos.x >= 150 && mousePos.x <= 270 &&
                    mousePos.y >= 200 && mousePos.y <= 240) {
                    if (!numeroSeleccionado.empty()) {
                        int indice = std::stoi(numeroSeleccionado) - 1;
                        if (indice >= 0 && indice < valoresCartas.size()) {
                            cartaSeleccionada = valoresCartas[indice];
                            valoresCartas.erase(valoresCartas.begin() + indice);
                            ventana.close();
                            return true;
                        }
                    }
                }
            }
        }
        
        ventana.clear(sf::Color::White);
        ventana.draw(tituloTexto);
        ventana.draw(txtPuntosJ1); // Dibujar puntuación J1
        ventana.draw(txtPuntosJ2); // Dibujar puntuación J2
        for(const auto& carta : cartasTexto) {
            ventana.draw(carta);
        }
        ventana.draw(campoSeleccion);
        ventana.draw(textoSeleccion);
        ventana.draw(btnConfirmar);
        ventana.draw(txtConfirmar);
        ventana.draw(instrucciones);
        ventana.display();
    }
    
    return false;
}

void abrirBatallaCartas(int casilla) {
    // Verificar si la casilla ya está ocupada

    casillaMiniJuego = casilla;
    
    // Crear vector con todos los números posibles
    std::vector<int> numerosDisponibles;
    for(int i = 1; i <= 15; i++) {
        numerosDisponibles.push_back(i);
    }
    
    // Generar mazos para ambos jugadores
    std::vector<int> mazoJ1, mazoJ2;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Repartir cartas J1 sin repetición
    for(int i = 0; i < 5; i++) {
        std::uniform_int_distribution<> dis(0, numerosDisponibles.size() - 1);
        int indiceAleatorio = dis(gen);
        int valorCarta = numerosDisponibles[indiceAleatorio];
        numerosDisponibles.erase(numerosDisponibles.begin() + indiceAleatorio);
        mazoJ1.push_back(valorCarta);
    }
    
    // Repartir cartas J2 sin repetición
    for(int i = 0; i < 5; i++) {
        std::uniform_int_distribution<> dis(0, numerosDisponibles.size() - 1);
        int indiceAleatorio = dis(gen);
        int valorCarta = numerosDisponibles[indiceAleatorio];
        numerosDisponibles.erase(numerosDisponibles.begin() + indiceAleatorio);
        mazoJ2.push_back(valorCarta);
    }
    
    int puntosJ1 = 0, puntosJ2 = 0;
    
    // Loop principal del juego
    while (!mazoJ1.empty() && !mazoJ2.empty()) {
        int cartaJ1 = -1, cartaJ2 = -1;
        
        // Turno J1
        if (!ventanaJugador(true, mazoJ1, cartaJ1, puntosJ1, puntosJ2)) break;
        
        // Turno J2
        if (!ventanaJugador(false, mazoJ2, cartaJ2, puntosJ1, puntosJ2)) break;
        
        // Comparar cartas y actualizar puntos
        if (cartaJ1 > cartaJ2) {
            puntosJ1++;
            std::cout << "J1 gana la ronda! (" << cartaJ1 << " vs " << cartaJ2 << ")" << std::endl;
        } else if (cartaJ2 > cartaJ1) {
            puntosJ2++;
            std::cout << "J2 gana la ronda! (" << cartaJ1 << " vs " << cartaJ2 << ")" << std::endl;
        } else {
            std::cout << "Empate! (" << cartaJ1 << " vs " << cartaJ2 << ")" << std::endl;
        }
    }

    // Determinar ganador y colocar ficha
    if (puntosJ1 > puntosJ2) {
        tableroPrincipal[casilla] = 'X';  // Marcar X para Jugador 1
        std::cout << "Jugador 1 gana - Marcando X en casilla " << casilla << std::endl;
    } else if (puntosJ2 > puntosJ1) {
        tableroPrincipal[casilla] = 'O';  // Marcar O para Jugador 2
        std::cout << "Jugador 2 gana - Marcando O en casilla " << casilla << std::endl;
    } else {
        std::cout << "Empate - No se marca la casilla" << std::endl;
    }
    
    // Mostrar resultado final
    mostrarVentanaGanador(puntosJ1, puntosJ2);
    
    // Verificar victoria en el tablero principal
    if (verificarVictoriaTablero(tableroPrincipal)) {
        char simboloGanador = (puntosJ1 > puntosJ2) ? 'X' : 'O';
        mostrarVentanaVictoriaTablero(simboloGanador);
    }
}

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode({800, 800}), "MendeWing");
    
    // Estado actual del juego
    GameState currentState = GameState::MENU;
    
    // Crear botones del menú
    sf::RectangleShape titulo({300, 50});
    sf::RectangleShape btnIniciar({200, 50});
    sf::RectangleShape btnSalir({200, 50});
    
    titulo.setPosition({250,100});
    btnIniciar.setPosition({300, 400});
    btnSalir.setPosition({300, 500});
    
    titulo.setFillColor(sf::Color::Black);
    btnIniciar.setFillColor(sf::Color::Green);
    btnSalir.setFillColor(sf::Color::Red);
    
    // Crear fuente para el texto
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    sf::Text texto(fuente, "Bienvenidos al videojuego kuliao mas bomba", 50);

    sf::Text titulo1(fuente, "MendenWing",30);
    sf::Text txtIniciar(fuente, "iniciar", 30);
    sf::Text txtSalir(fuente, "salir", 30);
    
    titulo1.setPosition({310,110});
    txtIniciar.setPosition({350, 410});
    txtSalir.setPosition({370, 510});
    
    // Crear las líneas del tablero
    sf::RectangleShape lineas[4];
    
    // Crear símbolos para el tablero
    sf::Text simbolos[9] = {
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, "")
    };
    std::string symbols[9] = {"?", "H", "C", "H", "?", "C", "C", "H", "?"};
    
    // Crear símbolos para las fichas del tablero principal (X y O)
    sf::Text fichasTablero[9] = {
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, "")
    };
    for(int i = 0; i < 9; i++) {
        fichasTablero[i].setCharacterSize(80); // Más grande que los símbolos de minijuegos
        fichasTablero[i].setFillColor(sf::Color::Red); // Color diferente para destacar
        
        // Calcular posición en el tablero (un poco más centrada)
        int x = (i % 3) * 200 + 85;
        int y = (i / 3) * 200 + 60;
        fichasTablero[i].setPosition({x, y});
    }
    
    for(int i = 0; i < 9; i++) {
        simbolos[i].setFont(fuente);
        simbolos[i].setString(symbols[i]);
        simbolos[i].setCharacterSize(50);
        simbolos[i].setFillColor(sf::Color::Black);
        
        // Calcular posición en el tablero (ligeramente arriba para dejar espacio a las fichas)
        int x = (i % 3) * 200 + 85;
        int y = (i / 3) * 200 + 60; // Movido un poco arriba
        simbolos[i].setPosition({x, y});
    }
    
    for (auto& linea : lineas) {
        linea.setFillColor(sf::Color::Black);
    }
    
    // Configuración de líneas verticales (reducidas a 500px de altura)
    lineas[0].setSize({2, 500});  // Vertical izquierda
    lineas[1].setSize({2, 500});  // Vertical derecha

    // Configuración de líneas horizontales (reducidas a 500px de ancho)
    lineas[2].setSize({500, 2});  // Horizontal superior
    lineas[3].setSize({500, 2});  // Horizontal inferior

    // Nuevo posicionamiento (movido hacia esquina superior izquierda)
    lineas[0].setPosition({200, 50});  // Vertical izquierda
    lineas[1].setPosition({400, 50});  // Vertical derecha
    lineas[2].setPosition({50, 200});  // Horizontal superior
    lineas[3].setPosition({50, 400});  // Horizontal inferior

    while (window.isOpen())
    {
        // Actualizar las fichas del tablero principal
        for(int i = 0; i < 9; i++) {
            if (tableroPrincipal[i] != ' ') {
                fichasTablero[i].setString(std::string(1, tableroPrincipal[i]));
            } else {
                fichasTablero[i].setString("");
            }
        }
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (currentState == GameState::MENU) {
                    // Verificar click en botón Iniciar
                    if (mousePos.x >= 300 && mousePos.x <= 500 &&
                        mousePos.y >= 400 && mousePos.y <= 450) {
                        currentState = GameState::GAME;
                        std::cout<<"iniciando"<<std::endl;
                    }
                    // Verificar click en botón Salir
                    else if (mousePos.x >= 300 && mousePos.x <= 500 &&
                             mousePos.y >= 500 && mousePos.y <= 550) {
                                std::cout<<"salir"<<std::endl;
                        window.close();
                    }
                }
                else if (currentState == GameState::GAME) {
                    // Verificar clicks en el tablero
                    int columna = mousePos.x / 200;
                    int fila = mousePos.y / 200;
                    if (columna < 3 && fila < 3) {
                        int index = fila * 3 + columna;
                        
                        // Solo permitir jugar en casillas que no tengan ficha
                        if (tableroPrincipal[index] == ' ') {
                            std::string symbol = symbols[index];
                            if (symbol == "H") {
                                std::cout<<"H en casilla " << index << std::endl;
                                // Iniciar juego Hex
                                abrirHex(index);
                            }
                            else if (symbol == "C") {
                                // Iniciar Batalla de Cartas
                                // TODO: Implementar redirección a Batalla de Cartas
                                std::cout<<"C en casilla " << index << std::endl;
                                // abrirBatallaCartas(index);
                                abrirBatallaCartas(index);
                            }
                            else if (symbol == "?") {
                                // Abrir ventana de Adivina el Número
                                std::cout<<"Abriendo Adivina el Numero en casilla " << index << std::endl;
                                abrirAdivinaNumero(index);
                            }
                        } else {
                            std::cout << "Casilla " << index << " ya ocupada por ficha '" 
                                     << tableroPrincipal[index] << "'" << std::endl;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        
        if (currentState == GameState::MENU) {
            // Dibujar menú
            window.draw(titulo);
            window.draw(btnIniciar);
            window.draw(btnSalir);
            window.draw(titulo1);
            window.draw(txtIniciar);
            window.draw(txtSalir);
        }
        else {
            // Dibujar tablero
            for (int i = 0; i < 4; i++){
                window.draw(lineas[i]);
            }
            // Dibujar símbolos de minijuegos
            for(int i = 0; i < 9; i++) {
                // Solo dibujar el símbolo si no hay ficha en esa casilla
                if (tableroPrincipal[i] == ' ') {
                    window.draw(simbolos[i]);
                }
            }
            // Dibujar fichas del tablero principal (X y O)
            for(int i = 0; i < 9; i++) {
                if (tableroPrincipal[i] != ' ') {
                    window.draw(fichasTablero[i]);
                }
            }
        }
        
        window.display();
    }





    
    return 0;
}









