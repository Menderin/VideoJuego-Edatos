//versión funcional del tablero 
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

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

// Función para abrir ventana de "Adivina el número"
void abrirAdivinaNumero() {
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
    instrucciones.setPosition({150, 200});
    instrucciones.setFillColor(sf::Color::Green);
    
    // Mensaje de resultado
    sf::Text mensajeResultado(fuente, "", 18);
    mensajeResultado.setPosition({150, 240});
    mensajeResultado.setFillColor(sf::Color::Magenta);
    
    // Botón de confirmar
    sf::RectangleShape btnConfirmar({120, 40});
    btnConfirmar.setPosition({190, 280});
    btnConfirmar.setFillColor(sf::Color::Blue);
    
    sf::Text txtConfirmar(fuente, "Confirmar", 18);
    txtConfirmar.setPosition({205, 290});
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
                if (mousePos.x >= 190 && mousePos.x <= 310 &&
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
                                    mensajeResultado.setString("¡JUGADOR 1 GANA!");
                                    mensajeResultado.setFillColor(sf::Color::Green);
                                    std::cout << "¡Jugador 1 acerto! El numero era: " << numeroJugador2 << std::endl;
                                    // Cerrar ventana después de un momento
                                    sf::sleep(sf::seconds(2));
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
                                    mensajeResultado.setString("¡JUGADOR 2 GANA!");
                                    mensajeResultado.setFillColor(sf::Color::Green);
                                    std::cout << "¡Jugador 2 acerto! El numero era: " << numeroJugador1 << std::endl;
                                    // Cerrar ventana después de un momento
                                    sf::sleep(sf::seconds(2));
                                    ventanaAdivina.close();
                                } else {
                                    // Dar pista y cambiar turno
                                    if (numero < numeroJugador1) {
                                        mensajeResultado.setString("El numero del jugador 1 es numero es MAYOR. Turno del J1");
                                    } else {
                                        mensajeResultado.setString("El numero del jugador 1 es numero es MENOR. Turno del J1");
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
    
    // Crear fuente para el texeso'
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    sf::Text texto(fuente, "Bienvenidos al videojuego kuliao mas bomba", 50);

    sf::Text titulo1(fuente, "MendenWing",30);
    sf::Text txtIniciar(fuente, "iniciar", 30);
    sf::Text txtSalir(fuente, "salir", 30);
    
    titulo1.setPosition({310,110});
    txtIniciar.setPosition({350, 410});
    txtSalir.setPosition({370, 510});
    
    // Crear las líneas del tablero (tu código existente)
    sf::RectangleShape lineas[4];
    // ... resto de la configuración de líneas ...

    // Crear símbolos para el tablero
    sf::Text simbolos[9] = {
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, "")
    };
    std::string symbols[9] = {"?", "H", "C", "H", "?", "C", "C", "H", "?"};
    
    for(int i = 0; i < 9; i++) {
        
        simbolos[i].setFont(fuente);
        simbolos[i].setString(symbols[i]);
        simbolos[i].setCharacterSize(50);
        simbolos[i].setFillColor(sf::Color::Black);
        
        // Calcular posición en el tablero
        int x = (i % 3) * 200 + 85;
        int y = (i / 3) * 200 + 75;
        simbolos[i].setPosition({x, y});
    }
    for (auto& linea : lineas) {
    linea.setFillColor(sf::Color::Black);
    }
    // Configuración de líneas verticales (ajustadas a la posición de los símbolos)
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
                        std::string symbol = symbols[index];
                        if (symbol == "H") {
                            std::cout<<"H"<< std::endl;
                            // Iniciar juego Hex
                            // TODO: Implementar redirección a Hex
                        }
                        else if (symbol == "C") {
                            // Iniciar Batalla de Cartas
                            // TODO: Implementar redirección a Batalla de Cartas
                            std::cout<<"C"<< std::endl;
                        }
                        else if (symbol == "?") {
                            // Abrir ventana de Adivina el Número
                            std::cout<<"Abriendo Adivina el Numero"<< std::endl;
                            abrirAdivinaNumero();
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
            // Dibujar símbolos
            for(int i = 0; i < 9; i++) {
                window.draw(simbolos[i]);
            }
        }
        
        window.display();
    }

    return 0;
}