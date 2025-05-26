//versión funcional del tablero con sistema de 3 en raya principal
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <SFML/Audio.hpp>


#include "MiniJuegos/AdivinaNumero/AdivinaNumero.h" 
#include "MiniJuegos/BatallaDeCartas/BatallaDeCartas.h"
#include "MiniJuegos/Hex/Hex.h"
#include "Nodo.h"
#include "Minijuegos/Minijuego.h"
#include "Tablero.h"

using namespace std;

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

enum class OpcionJuego {
    VOLVER_A_JUGAR,
    SALIR,
    NINGUNO
};

// Función para mostrar ventana de opciones después de la victoria
OpcionJuego mostrarVentanaOpciones() {
    // Crear ventana de opciones
    sf::RenderWindow ventanaOpciones(sf::VideoMode({400, 300}), "Que deseas hacer?");
    
    // Crear fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Elementos de la ventana
    sf::Text textoPregunta(fuente, "Que deseas hacer?", 25);
    sf::RectangleShape btnVolverJugar({180, 50});
    sf::RectangleShape btnSalir({120, 50});
    sf::Text txtVolverJugar(fuente, "Volver a Jugar", 20);
    sf::Text txtSalir(fuente, "Salir", 25);
    
    // Configurar posiciones y colores
    // Alternative way using getSize()
    textoPregunta.setPosition({110,60});
    textoPregunta.setFillColor(sf::Color::Black);

    btnVolverJugar.setPosition({60, 150});
    btnVolverJugar.setFillColor(sf::Color::Green);
    txtVolverJugar.setPosition({70, 160});
    txtVolverJugar.setFillColor(sf::Color::White);
    
    btnSalir.setPosition({260, 150});
    btnSalir.setFillColor(sf::Color::Red);
    txtSalir.setPosition({290, 160});
    txtSalir.setFillColor(sf::Color::White);
    
    OpcionJuego resultado = OpcionJuego::NINGUNO;
    
    while (ventanaOpciones.isOpen()) {
        while (const std::optional event = ventanaOpciones.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaOpciones.close();
                resultado = OpcionJuego::SALIR;
                exit(0); // Salir del programa si se cierra la ventana 
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaOpciones);
                
                // Verificar click en Volver a Jugar
                if (mousePos.x >= 60 && mousePos.x <= 240 &&
                    mousePos.y >= 150 && mousePos.y <= 200) {
                    resultado = OpcionJuego::VOLVER_A_JUGAR;
                    ventanaOpciones.close();
                }
                // Verificar click en Salir
                else if (mousePos.x >= 260 && mousePos.x <= 380 &&
                         mousePos.y >= 150 && mousePos.y <= 200) {
                    resultado = OpcionJuego::SALIR;
                    ventanaOpciones.close();
                    exit(0); // Salir del programa si se cierra la ventana
                    
                }
            }
            
            // Controles de teclado opcionales
            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter ||
                    event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
                    resultado = OpcionJuego::VOLVER_A_JUGAR;
                    ventanaOpciones.close();
                } else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) {
                    resultado = OpcionJuego::SALIR;
                    ventanaOpciones.close();
                    
                    
                }
            }
        }
        
        ventanaOpciones.clear(sf::Color::White);
        
        // Dibujar elementos
        ventanaOpciones.draw(textoPregunta);
        ventanaOpciones.draw(btnVolverJugar);
        ventanaOpciones.draw(btnSalir);
        ventanaOpciones.draw(txtVolverJugar);
        ventanaOpciones.draw(txtSalir);
        
        ventanaOpciones.display();
    }
    
    return resultado;
}

// Función para mostrar la ventana del ganador - batlla de cartas
void mostrarVentanaGanador(int puntosJ1, int puntosJ2) {
    sf::RenderWindow ventanaGanador(sf::VideoMode({600, 300}), "Resultado Final");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    sf::Texture Fondo;
    if (!Fondo.loadFromFile("assets/Fondos/FondoVictoria.jpg")) {
        // Manejar el error si la imagen no se puede cargar
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    // Crear un sprite para el fondo
    sf::Sprite spriteFondo(Fondo);
    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scale(
    static_cast<float>(ventanaGanador.getSize().x) / Fondo.getSize().x,
    static_cast<float>(ventanaGanador.getSize().y) / Fondo.getSize().y
    );
    spriteFondo.setScale(scale);
    
    // Título
    sf::Text titulo(fuente, "Fin del Juego", 36);
    titulo.setPosition({200, 30});
    titulo.setFillColor(sf::Color::White);
    
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
        ventanaGanador.draw(spriteFondo);
        ventanaGanador.draw(titulo);
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

    // Cargar la imagen de fondo
    sf::Texture fondo;
    if (!fondo.loadFromFile("assets/Fondos/Victoria absoluta.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }

    // Crear un sprite para el fondo
    sf::Sprite spriteFondo(fondo);

    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scale(
        static_cast<float>(ventanaVictoria.getSize().x) / fondo.getSize().x,
        static_cast<float>(ventanaVictoria.getSize().y) / fondo.getSize().y
    );
    spriteFondo.setScale(scale);
    
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
        ventanaVictoria.draw(spriteFondo);
        ventanaVictoria.draw(tituloVictoria);
        ventanaVictoria.draw(textoGanador);
        ventanaVictoria.draw(textoAdicional);
        ventanaVictoria.draw(btnCerrar);
        ventanaVictoria.draw(txtCerrar);
        ventanaVictoria.display();
    }
}

void mostrarVentanaVictoriaFichas(char simboloGanador) {
    sf::RenderWindow ventanaVictoria(sf::VideoMode({500, 300}), "Victoria por Fichas!");
    
    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Cargar la imagen de fondo
    sf::Texture fondo;
    if (!fondo.loadFromFile("assets/Fondos/Victoria absoluta.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }

    // Crear un sprite para el fondo
    sf::Sprite spriteFondo(fondo);

    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scale(
        static_cast<float>(ventanaVictoria.getSize().x) / fondo.getSize().x,
        static_cast<float>(ventanaVictoria.getSize().y) / fondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Título de victoria
    sf::Text tituloVictoria(fuente, "VICTORIA TOTAL!", 48);
    tituloVictoria.setPosition({80, 50});
    tituloVictoria.setFillColor(sf::Color::Green);
    
    // Mensaje del ganador
    std::string jugadorGanador = (simboloGanador == 'X') ? "Jugador 1 (X)" : "Jugador 2 (O)";
    std::string mensajeGanador = jugadorGanador + " ha ganado!";
    sf::Text textoGanador(fuente, mensajeGanador, 24);
    textoGanador.setPosition({100, 120});
    textoGanador.setFillColor(sf::Color::Blue);
    
    // Mensaje adicional específico para victoria por fichas
    sf::Text textoAdicional(fuente, "Ganaste por tener mas fichas en el tablero", 18);
    textoAdicional.setPosition({70, 160});
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
        ventanaVictoria.draw(spriteFondo);
        ventanaVictoria.draw(tituloVictoria);
        ventanaVictoria.draw(textoGanador);
        ventanaVictoria.draw(textoAdicional);
        ventanaVictoria.draw(btnCerrar);
        ventanaVictoria.draw(txtCerrar);
        ventanaVictoria.display();
    }
}

// Función para mostrar ventana de victoria del minijuego (modificada)
void mostrarVentanaVictoria(int jugadorGanador, int numeroSecreto, Tablero& tablero) {
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
// Agregar esta nueva función para mostrar la ayuda específica del Hex
void mostrarVentanaAyudaHex() {
    sf::RenderWindow ventanaAyuda(sf::VideoMode({500, 300}), "Ayuda - Juego Hex");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Título
    sf::Text titulo(fuente, "Como Jugar Hex", 24);
    titulo.setPosition({150, 30});
    titulo.setFillColor(sf::Color::Black);
    
    // Instrucciones específicas
    std::vector<std::string> textos = {
        "1. Jugador 1 (Rojo) conecta izquierda-derecha",
        "2. Jugador 2 (Azul) conecta arriba-abajo",
        "3. Haz click en un hexagono para marcarlo",
        "4. Gana quien conecte primero sus lados"
    };
    
    std::vector<sf::Text> instrucciones;
    for(size_t i = 0; i < textos.size(); i++) {
        sf::Text texto(fuente, textos[i], 18);
        texto.setPosition({50, 90.f + (i * 35)});
        texto.setFillColor(sf::Color::Black);
        instrucciones.push_back(texto);
    }
    
    // Botón cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({200, 240});
    btnCerrar.setFillColor(sf::Color::Red);
    
    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({220, 250});
    txtCerrar.setFillColor(sf::Color::White);
    
    while (ventanaAyuda.isOpen()) {
        while (const std::optional event = ventanaAyuda.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaAyuda.close();
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaAyuda);
                if (mousePos.x >= 200 && mousePos.x <= 300 &&
                    mousePos.y >= 240 && mousePos.y <= 280) {
                    ventanaAyuda.close();
                }
            }
        }
        
        ventanaAyuda.clear(sf::Color::White);
        ventanaAyuda.draw(titulo);
        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }
        ventanaAyuda.draw(btnCerrar);
        ventanaAyuda.draw(txtCerrar);
        ventanaAyuda.display();
    }
}

// Función para abrir ventana de Hex
void abrirHex(int casilla, Tablero& tablero,sf::Music& musicaFondo) {
    casillaMiniJuego = casilla;

    sf::RenderWindow ventanaHex(sf::VideoMode({800, 700}), "Juego Hex");

    // Detener música de fondo actual
    musicaFondo.stop();
    
    // Cargar y reproducir música de Batalla de Cartas
    sf::Music musicHex;
    if (!musicHex.openFromFile("assets/Audios/Backgrounds/hex.ogg")) {
        std::cerr << "Error al cargar la música de Adivinar el numero" << std::endl;
    } else {
        musicHex.setLooping(true);
        musicHex.setVolume(30.0f);
        musicHex.play();
    }

    // Cargar la imagen de fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/Fondo espacio exterior.jpg")) {
        // Manejar el error si la imagen no se puede cargar
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    // Crear un sprite para el fondo
    sf::Sprite spriteFondo(textureFondo);
    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scale(
    static_cast<float>(ventanaHex.getSize().x) / textureFondo.getSize().x,
    static_cast<float>(ventanaHex.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Crear título
    sf::Text titulo(fuente, "Juego Hex", 36);
    titulo.setPosition({300, 20});
    titulo.setFillColor(sf::Color::White);

    // Instrucciones
    sf::Text instrucciones(fuente, "Jugador 1 (Rojo) vs Jugador 2 (Azul)", 20);
    instrucciones.setPosition({220, 60});
    instrucciones.setFillColor(sf::Color::White);

    // Botón de cerrar
    /*sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({350, 630});
    btnCerrar.setFillColor(sf::Color::Red);*/

    sf::Text txtCerrar(fuente, "Cerrar", 25);
    txtCerrar.setPosition({360, 640});
    txtCerrar.setFillColor(sf::Color::White);

    
    // Agregar botón de ayuda circular
    sf::CircleShape btnAyuda(20);
    btnAyuda.setPosition({20, 650}); // Posición en esquina inferior izquierda
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(2);
    btnAyuda.setOutlineColor(sf::Color::Black);

    // Texto del signo de interrogación para el botón de ayuda
    sf::Text txtAyuda(fuente, "?", 20);
    txtAyuda.setPosition({33, 658});
    txtAyuda.setFillColor(sf::Color::White);

    // Crear el tablero hexagonal (11x11)
    const int TABLERO_SIZE = 11;
    const float HEX_RADIO = 25.0f; // Ajustar el radio del hexágono
    const float HEX_SPACING = 40.0f; // Ajustar el espaciado entre hexágonos

    std::vector<std::vector<sf::CircleShape>> tableroHex(TABLERO_SIZE, std::vector<sf::CircleShape>(TABLERO_SIZE));

    // Crear los hexágonos del tablero
    for(int fila = 0; fila < TABLERO_SIZE; fila++) {
        for(int col = 0; col < TABLERO_SIZE; col++) {
            // Calcular posición con offset para crear forma de diamante
            float x = 100 + col * HEX_SPACING + (fila * HEX_SPACING * 0.5f);
            float y = 200 + fila * (HEX_SPACING * 0.866f);

            sf::CircleShape hex = crearHexagono(HEX_RADIO, {x, y});
             hex.setFillColor(sf::Color(255, 255, 255, 128)); // 128 es el valor de transparencia (alfa)
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

                sf::Vector2f posAyuda = btnAyuda.getPosition();
                float radio = btnAyuda.getRadius();
                if (mousePos.x >= posAyuda.x && mousePos.x <= posAyuda.x + (radio * 2) &&
                    mousePos.y >= posAyuda.y && mousePos.y <= posAyuda.y + (radio * 2)) {
                    mostrarVentanaAyudaHex();
                    continue;
                }

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
                                        tablero.getNodo(casillaMiniJuego / 3, casillaMiniJuego % 3).setEstado(ganador == 1 ? EstadoNodo::JUGADOR1 : EstadoNodo::JUGADOR2);
                                        std::cout << "Marcando " << ((ganador == 1) ? 'X' : 'O')
                                                  << " en casilla " << casillaMiniJuego << std::endl;
                                    }

                                    // Verificar victoria en el tablero principal
                                    tablero.verificarVictoria();

                                    // Mostrar ventana de victoria
                                    mostrarVentanaVictoria(ganador, -1,tablero);
                                    musicHex.stop();
                                    musicaFondo.play();
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
        ventanaHex.draw(spriteFondo); // Dibujar el fondo
        ventanaHex.draw(titulo);
        ventanaHex.draw(instrucciones);

        // Dibujar todos los hexágonos del tablero
        for(int fila = 0; fila < TABLERO_SIZE; fila++) {
            for(int col = 0; col < TABLERO_SIZE; col++) {
                ventanaHex.draw(tableroHex[fila][col]);
            }
        }
        //ventanaHex.draw(btnAyuda);
        ventanaHex.draw(txtAyuda);
        //ventanaHex.draw(btnCerrar);
        ventanaHex.draw(txtCerrar);
        ventanaHex.display();
    }
}
// Nueva función para mostrar ayuda específica del juego Adivina el Número
void mostrarVentanaAyudaAdivinaNumero() {
    sf::RenderWindow ventanaAyuda(sf::VideoMode({500, 300}), "Ayuda - Adivina el Numero");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Título
    sf::Text titulo(fuente, "Como Jugar  Adivina el Numero", 24);
    titulo.setPosition({100, 30});
    titulo.setFillColor(sf::Color::Black);
    
    // Instrucciones específicas
    std::vector<std::string> textos = {
        "1 Cada jugador elige un numero secreto (1-100)",
        "2 Por turnos intentan adivinar el numero",
        "3 Recibiran pistas MAYOR o MENOR",
        "4 Gana quien adivine primero"
    };
    
    std::vector<sf::Text> instrucciones;
    for(size_t i = 0; i < textos.size(); i++) {
        sf::Text texto(fuente, textos[i], 18);
        texto.setPosition({50, 90.f + (i * 35)});
        texto.setFillColor(sf::Color::Black);
        instrucciones.push_back(texto);
    }
    
    // Botón cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({200, 240});
    btnCerrar.setFillColor(sf::Color::Red);
    
    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({220, 250});
    txtCerrar.setFillColor(sf::Color::White);
    
    while (ventanaAyuda.isOpen()) {
        while (const std::optional event = ventanaAyuda.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaAyuda.close();
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaAyuda);
                if (mousePos.x >= 200 && mousePos.x <= 300 &&
                    mousePos.y >= 240 && mousePos.y <= 280) {
                    ventanaAyuda.close();
                }
            }
        }
        
        ventanaAyuda.clear(sf::Color::White);
        ventanaAyuda.draw(titulo);
        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }
        ventanaAyuda.draw(btnCerrar);
        ventanaAyuda.draw(txtCerrar);
        ventanaAyuda.display();
    }
}

// Función para abrir ventana de "Adivina el número" (modificada para recibir la casilla)
void abrirAdivinaNumero(int casilla,Tablero& tablero,sf::Music& musicaFondo) {
    casillaMiniJuego = casilla; // Guardar en qué casilla se jugó

    sf::RenderWindow ventanaAdivina(sf::VideoMode({600, 450}), "Adivina el Numero");

    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Detener música de fondo actual
    musicaFondo.stop();
    
    // Cargar y reproducir música de Batalla de Cartas
    sf::Music musicaAdivinarNumero;
    if (!musicaAdivinarNumero.openFromFile("assets/Audios/Backgrounds/AdivinaNumero.ogg")) {
        std::cerr << "Error al cargar la música de Adivinar el numero" << std::endl;
    } else {
        musicaAdivinarNumero.setLooping(true);
        musicaAdivinarNumero.setVolume(25.0f);
        musicaAdivinarNumero.play();
    }

     // Cargar la imagen de fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/Fondo adivina numero.png")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }

    // Crear un sprite para el fondo
    sf::Sprite spriteFondo(textureFondo);

    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scale(
        static_cast<float>(ventanaAdivina.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaAdivina.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Crear título
    sf::Text titulo(fuente, "Adivina el Numero", 36);
    titulo.setPosition({150, 30});
    titulo.setFillColor(sf::Color::Black);

    sf::RectangleShape RectanguloTitulo({320, 50});
    RectanguloTitulo.setPosition({140, 25});
    RectanguloTitulo.setFillColor(sf::Color::White);
    RectanguloTitulo.setFillColor(sf::Color(255, 255, 255, 128)); // 128 es el valor de transparencia (alfa)
    // Mensaje para jugador (se actualizará según el estado)
    sf::Text mensajeJugador(fuente, "Jugador 1: Elige tu numero", 24);
    mensajeJugador.setPosition({150, 90});
    mensajeJugador.setFillColor(sf::Color::White);

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
    instrucciones.setFillColor(sf::Color::White);

    // Mensaje de resultado
    sf::Text mensajeResultado(fuente, "", 18);
    mensajeResultado.setPosition({125, 240});
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


    // Agregar botón de ayuda circular
    sf::CircleShape btnAyuda(20);
    btnAyuda.setPosition({20, 390}); // Posición en esquina inferior izquierda
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(2);
    btnAyuda.setOutlineColor(sf::Color::Black);

    // Texto del signo de interrogación para el botón de ayuda
    sf::Text txtAyuda(fuente, "?", 20);
    txtAyuda.setPosition({33, 398});
    txtAyuda.setFillColor(sf::Color::Black);

    // Crear una instancia del juego AdivinaNumero
    AdivinaNumero juegoAdivina;

    // Variable para manejar el turno actual
    int turnoActual = 1;

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

                sf::Vector2f posAyuda = btnAyuda.getPosition();
                float radio = btnAyuda.getRadius();
                if (mousePos.x >= posAyuda.x && mousePos.x <= posAyuda.x + (radio * 2) &&
                    mousePos.y >= posAyuda.y && mousePos.y <= posAyuda.y + (radio * 2)) {
                    // Mostrar ventana de ayuda específica para Adivina el Número
                    mostrarVentanaAyudaAdivinaNumero();
                    continue;
                }

                // Verificar click en botón confirmar
                if (mousePos.x >= 240 && mousePos.x <= 360 &&
                    mousePos.y >= 280 && mousePos.y <= 320) {

                    if (!numeroIngresado.empty()) {
                        int numero = std::stoi(numeroIngresado);

                        if (numero >= 1 && numero <= 100) {
                            if (!juegoAdivina.ambosNumerosElegidos()) {
                                // Guardar número del jugador 1 o 2
                                if (!juegoAdivina.establecerNumeroSecreto(1, numero)) {
                                    juegoAdivina.establecerNumeroSecreto(2, numero);
                                }

                                // Actualizar textos para el siguiente jugador
                                if (juegoAdivina.ambosNumerosElegidos()) {
                                    mensajeJugador.setString("Jugador 1: Adivina el numero del J2");
                                    mensajeJugador.setFillColor(sf::Color::White);
                                    instrucciones.setString("Intenta adivinar el numero secreto");
                                    mensajeResultado.setString("");
                                } else {
                                    mensajeJugador.setString("Jugador 2: Elige tu numero");
                                    mensajeJugador.setFillColor(sf::Color::White);
                                    instrucciones.setString("Ingresa un numero entre 1 y 100");
                                }

                                // Limpiar campo de entrada
                                numeroIngresado = "";
                                textoNumero.setString(numeroIngresado);
                            } else {
                                // Jugador intenta adivinar el número del otro jugador
                                if (juegoAdivina.procesarMovimiento(turnoActual, numero)) {
                                    // Marcar en el tablero principal
                                    tablero.getNodo(casillaMiniJuego / 3, casillaMiniJuego % 3).setEstado(turnoActual == 1 ? EstadoNodo::JUGADOR1 : EstadoNodo::JUGADOR2);
                                    // Verificar victoria en el tablero principal
                                    tablero.verificarVictoria();
                                    // Mostrar ventana de victoria y cerrar juego
                                    mostrarVentanaVictoria(turnoActual, numero,tablero);
                                    musicaAdivinarNumero.stop();
                                    musicaFondo.play();
                                    ventanaAdivina.close();
                                } else {
                                    // Dar pista y cambiar turno
                                    int numeroSecreto = juegoAdivina.getNumeroJugador(turnoActual == 1 ? 2 : 1);

                                    if (numero < numeroSecreto) {
                                        mensajeResultado.setString("El numero del jugador " + std::to_string(turnoActual == 1 ? 2 : 1) + " es MAYOR. Turno del J" + std::to_string(turnoActual == 1 ? 2 : 1));
                                    } else {
                                        mensajeResultado.setString("El numero del jugador " + std::to_string(turnoActual == 1 ? 2 : 1) + " es MENOR. Turno del J" + std::to_string(turnoActual == 1 ? 2 : 1));
                                    }

                                    // Cambiar turno manualmente
                                    turnoActual = (turnoActual == 1) ? 2 : 1;
                                    mensajeJugador.setString("Jugador " + std::to_string(turnoActual) + ": Adivina el numero del J" + std::to_string(turnoActual == 1 ? 2 : 1));
                                    mensajeJugador.setFillColor(turnoActual == 1 ? sf::Color::Blue : sf::Color::White);
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
        ventanaAdivina.draw(spriteFondo); // Dibujar el fondo
        ventanaAdivina.draw(RectanguloTitulo);
        ventanaAdivina.draw(titulo);
        ventanaAdivina.draw(mensajeJugador);
        ventanaAdivina.draw(campoNumero);
        ventanaAdivina.draw(textoNumero);
        ventanaAdivina.draw(instrucciones);
        //ventanaAdivina.draw(btnConfirmar);
        ventanaAdivina.draw(txtConfirmar);
       // ventanaAdivina.draw(btnCerrar);
        ventanaAdivina.draw(txtCerrar);
        ventanaAdivina.draw(mensajeResultado);
        //ventanaAdivina.draw(btnAyuda);
        ventanaAdivina.draw(txtAyuda);
        ventanaAdivina.display();
    }
}
// Agregar esta nueva función para mostrar la ayuda específica de Batalla de Cartas
void mostrarVentanaAyudaBatallaCartas() {
    sf::RenderWindow ventanaAyuda(sf::VideoMode({500, 300}), "Ayuda - Batalla de Cartas");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Título
    sf::Text titulo(fuente, "Como Jugar Batalla de Cartas", 24);
    titulo.setPosition({100, 30});
    titulo.setFillColor(sf::Color::Black);
    
    // Instrucciones específicas
    std::vector<std::string> textos = {
        "1. Cada jugador recibe 5 cartas (1-15)",
        "2. Por turnos eligen una carta",
        "3. La carta mas alta gana el punto",
        "4. Gana quien tenga mas puntos al final"
    };
    
    std::vector<sf::Text> instrucciones;
    for(size_t i = 0; i < textos.size(); i++) {
        sf::Text texto(fuente, textos[i], 18);
        texto.setPosition({50, 90.f + (i * 35)});
        texto.setFillColor(sf::Color::Black);
        instrucciones.push_back(texto);
    }
    
    // Botón cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({200, 240});
    btnCerrar.setFillColor(sf::Color::Red);
    
    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({220, 250});
    txtCerrar.setFillColor(sf::Color::White);
    
    while (ventanaAyuda.isOpen()) {
        while (const std::optional event = ventanaAyuda.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaAyuda.close();
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaAyuda);
                if (mousePos.x >= 200 && mousePos.x <= 300 &&
                    mousePos.y >= 240 && mousePos.y <= 280) {
                    ventanaAyuda.close();
                }
            }
        }
        
        ventanaAyuda.clear(sf::Color::White);
        ventanaAyuda.draw(titulo);
        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }
        ventanaAyuda.draw(btnCerrar);
        ventanaAyuda.draw(txtCerrar);
        ventanaAyuda.display();
    }
}

// Función para la ventana de cada jugador
bool ventanaJugador(bool esJugador1, std::vector<int>& valoresCartas, int& cartaSeleccionada, int puntosJ1, int puntosJ2) {
    std::string titulo = esJugador1 ? "Turno Jugador 1" : "Turno Jugador 2";
    sf::Color colorJugador = esJugador1 ? sf::Color::Blue : sf::Color::Red;
    
    sf::RenderWindow ventana(sf::VideoMode({800, 600}), titulo);
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

     // Cargar la imagen de fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/Fondo batalla de cartas.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }

    // Crear un sprite para el fondo
    sf::Sprite spriteFondo(textureFondo);

    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scale(
        static_cast<float>(ventana.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventana.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);
    
    // Título más grande y mejor posicionado
    sf::Text tituloTexto(fuente, titulo, 32);
    tituloTexto.setPosition({80, 40});
    tituloTexto.setFillColor(colorJugador);

    // Crear las cartas con diseño visual más grandes
    std::vector<sf::RectangleShape> cartasVisuales;
    std::vector<sf::Text> numerosCartas;
    
    // Dimensiones de la carta aumentadas
    const float CARTA_ANCHO = 120.f;
    const float CARTA_ALTO = 180.f;
    const float ESPACIO_ENTRE_CARTAS = 30.f;
    
    // Crear las cartas centradas horizontalmente
    float inicioCartas = (800 - (valoresCartas.size() * CARTA_ANCHO + (valoresCartas.size() - 1) * ESPACIO_ENTRE_CARTAS)) / 2;
    
    for(int i = 0; i < valoresCartas.size(); i++) {
        // Crear el rectángulo de la carta
        sf::RectangleShape carta(sf::Vector2f(CARTA_ANCHO, CARTA_ALTO));
        carta.setPosition({inicioCartas + (i * (CARTA_ANCHO + ESPACIO_ENTRE_CARTAS)), 150.f});
        carta.setFillColor(sf::Color::White);
        carta.setOutlineThickness(3);
        carta.setOutlineColor(sf::Color::Black);
        cartasVisuales.push_back(carta);
        
        // Crear el número de la carta (grande en el centro)
        sf::Text numero(fuente, std::to_string(valoresCartas[i]), 45);
        // Centrar el número en la carta
        auto bounds = numero.getLocalBounds();
        float xPos = carta.getPosition().x + (CARTA_ANCHO/2) - (bounds.position.x/2);
        float yPos = carta.getPosition().y + (CARTA_ALTO/2) - (bounds.position.y/2);
        
        numero.setPosition({xPos, yPos});
        numero.setFillColor(sf::Color::Black);
        numerosCartas.push_back(numero);
        
        // Número pequeño en la esquina superior izquierda
        sf::Text numeroEsquina(fuente, std::to_string(valoresCartas[i]), 18);
        numeroEsquina.setPosition({carta.getPosition().x + 8, carta.getPosition().y + 8});
        numeroEsquina.setFillColor(sf::Color::Black);
        numerosCartas.push_back(numeroEsquina);
        
        // Número de posición debajo de cada carta
        sf::Text numeroPosicion(fuente, std::to_string(i + 1), 20);
        numeroPosicion.setPosition({carta.getPosition().x + (CARTA_ANCHO/2) - 10, carta.getPosition().y + CARTA_ALTO + 10});
        numeroPosicion.setFillColor(colorJugador);
        numerosCartas.push_back(numeroPosicion);
    }
    
    std::string numeroSeleccionado = "";
    sf::Text textoSeleccion(fuente, numeroSeleccionado, 28);
    textoSeleccion.setPosition({90, 420});
    textoSeleccion.setFillColor(sf::Color::Black);
    
    // Campo de selección más grande
    sf::RectangleShape campoSeleccion({80, 50});
    campoSeleccion.setPosition({80, 410});
    campoSeleccion.setFillColor(sf::Color::White);
    campoSeleccion.setOutlineThickness(3);
    campoSeleccion.setOutlineColor(sf::Color::Black);

    // Botón confirmar más grande
    sf::RectangleShape btnConfirmar({150, 50});
    btnConfirmar.setPosition({200, 410});
    btnConfirmar.setFillColor(colorJugador);

    // Botón de ayuda circular más grande
    sf::CircleShape btnAyuda(25);
    btnAyuda.setPosition({30, 520});
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(3);
    btnAyuda.setOutlineColor(sf::Color::Black);

    sf::Text txtAyuda(fuente, "?", 24);
    txtAyuda.setPosition({47, 532});
    txtAyuda.setFillColor(sf::Color::Black);
    
    sf::Text txtConfirmar(fuente, "Confirmar", 22);
    txtConfirmar.setPosition({225, 425});
    txtConfirmar.setFillColor(sf::Color::White);
    
    // Instrucciones más grandes y mejor posicionadas
    sf::Text instrucciones(fuente, "Selecciona una carta (1-" + 
                        std::to_string(valoresCartas.size()) + "):", 22);
    instrucciones.setPosition({80, 370});

    instrucciones.setFillColor(sf::Color::White);

    //instrucciones.setFillColor(sf::Color::Cyan);

    
    // Textos de puntuación más grandes y mejor posicionados
    sf::Text txtPuntosJ1(fuente, "Puntos J1: " + std::to_string(puntosJ1), 24);
    txtPuntosJ1.setPosition({500, 40});
    txtPuntosJ1.setFillColor(sf::Color::Blue);
    
    sf::Text txtPuntosJ2(fuente, "Puntos J2: " + std::to_string(puntosJ2), 24);
    txtPuntosJ2.setPosition({500, 75});
    txtPuntosJ2.setFillColor(sf::Color::Red);
    
    // Texto adicional de ayuda
    sf::Text ayudaExtra(fuente, "Haz clic en '?' para ver las reglas del juego", 18);
    ayudaExtra.setPosition({400, 520});
    ayudaExtra.setFillColor(sf::Color::Cyan);
    
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

                // Verificar click en botón de ayuda (coordenadas actualizadas)
                sf::Vector2f posAyuda = btnAyuda.getPosition();
                float radio = btnAyuda.getRadius();
                if (mousePos.x >= posAyuda.x && mousePos.x <= posAyuda.x + (radio * 2) &&
                    mousePos.y >= posAyuda.y && mousePos.y <= posAyuda.y + (radio * 2)) {
                    mostrarVentanaAyudaBatallaCartas();
                    continue;
                }

                // Click en botón confirmar (coordenadas actualizadas)
                if (mousePos.x >= 200 && mousePos.x <= 350 &&
                    mousePos.y >= 410 && mousePos.y <= 460) {
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
                
                // Click directo en las cartas
                for(int i = 0; i < cartasVisuales.size(); i++) {
                    sf::Vector2f posCart = cartasVisuales[i].getPosition();
                    if (mousePos.x >= posCart.x && mousePos.x <= posCart.x + CARTA_ANCHO &&
                        mousePos.y >= posCart.y && mousePos.y <= posCart.y + CARTA_ALTO) {
                        numeroSeleccionado = std::to_string(i + 1);
                        textoSeleccion.setString(numeroSeleccionado);
                        break;
                    }
                }
            }
        }
        
        ventana.clear(sf::Color::White);
        ventana.draw(spriteFondo); // Dibujar el fondo
        ventana.draw(tituloTexto);
        ventana.draw(txtPuntosJ1);
        ventana.draw(txtPuntosJ2);
        
        // Dibujar las cartas y números
        for(size_t i = 0; i < cartasVisuales.size(); i++) {
            ventana.draw(cartasVisuales[i]);
        }
        for(const auto& numero : numerosCartas) {
            ventana.draw(numero);
        }
        
        ventana.draw(campoSeleccion);
        ventana.draw(textoSeleccion);
        ventana.draw(btnConfirmar);
        ventana.draw(txtConfirmar);
        ventana.draw(instrucciones);
        ventana.draw(btnAyuda);
        ventana.draw(txtAyuda);
        ventana.draw(ayudaExtra);
        ventana.display();
    }
    
    return false;
}

// Agregar esta función junto con las otras funciones de ventanas
void mostrarVentanaAyuda() {
    sf::RenderWindow ventanaAyuda(sf::VideoMode({600, 400}), "Ayuda del Juego");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Título
    sf::Text titulo(fuente, "Ayuda Como Jugar", 30);
    titulo.setPosition({200, 30});
    titulo.setFillColor(sf::Color::Black);
    
    // Instrucciones
    std::vector<sf::Text> instrucciones;
    
    std::vector<std::string> textos = {
        "Juego de 3 en raya con minijuegos",
        "Simbolos en el tablero:",
        "   ? Adivina el Numero",
        "   H Juego Hex",
        "   C Batalla de Cartas",
        "Gana quien consiga 3 en linea O quien tenga mas fichas"
    };
    
    for(size_t i = 0; i < textos.size(); i++) {
        sf::Text texto(fuente, textos[i], 20);
        texto.setPosition({50, 100.f + (i * 40)});
        texto.setFillColor(sf::Color::Black);
        instrucciones.push_back(texto);
    }
    
    // Botón cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({250, 340});
    btnCerrar.setFillColor(sf::Color::Red);
    
    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({270, 350});
    txtCerrar.setFillColor(sf::Color::White);
    
    while (ventanaAyuda.isOpen()) {
        while (const std::optional event = ventanaAyuda.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaAyuda.close();
            }
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaAyuda);
                
                // Click en botón cerrar
                if (mousePos.x >= 250 && mousePos.x <= 350 &&
                    mousePos.y >= 340 && mousePos.y <= 380) {
                    ventanaAyuda.close();
                }
            }
        }
        
        ventanaAyuda.clear(sf::Color::White);
        ventanaAyuda.draw(titulo);
        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }
        ventanaAyuda.draw(btnCerrar);
        ventanaAyuda.draw(txtCerrar);
        ventanaAyuda.display();
    }
}

void abrirBatallaCartas(int casilla, Tablero& tablero,sf::Music& musicaFondo) {
    casillaMiniJuego = casilla;
    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Detener música de fondo actual
    musicaFondo.stop();
    
    // Cargar y reproducir música de Batalla de Cartas
    sf::Music musicaBatallaCartas;
    if (!musicaBatallaCartas.openFromFile("assets/Audios/Backgrounds/BatallaCartas.ogg")) {
        std::cerr << "Error al cargar la música de Batalla de Cartas" << std::endl;
    } else {
        musicaBatallaCartas.setLooping(true);
        musicaBatallaCartas.setVolume(10.0f);
        musicaBatallaCartas.play();
    }

    // Crear título
    sf::Text titulo(fuente, "Batalla de Cartas", 36);
    titulo.setPosition({150, 30});
    titulo.setFillColor(sf::Color::Black);
    
    

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
        tablero.getNodo(casilla / 3, casilla % 3).setEstado(EstadoNodo::JUGADOR1);
        std::cout << "Jugador 1 gana - Marcando X en casilla " << casilla << std::endl;
    } else if (puntosJ2 > puntosJ1) {
        tablero.getNodo(casilla / 3, casilla % 3).setEstado(EstadoNodo::JUGADOR2);
        std::cout << "Jugador 2 gana - Marcando O en casilla " << casilla << std::endl;
    } else {
        std::cout << "Empate - No se marca la casilla" << std::endl;
    }

    // Mostrar resultado final
    mostrarVentanaGanador(puntosJ1, puntosJ2);
    musicaBatallaCartas.stop();
    musicaFondo.play();
    // Verificar victoria en el tablero principal
    tablero.verificarVictoria();
}

void inicializarMinijuegosAleatorios(Tablero& tablero) {
    random_device rd;
    mt19937 gen(rd());

    cout << "\nInicializando minijuegos en cada casilla..." << endl;

    // Crear un vector con exactamente 3 de cada tipo de minijuego
    vector<TipoMiniJuego> minijuegos = {
        TipoMiniJuego::ADIVINA_NUMERO, TipoMiniJuego::ADIVINA_NUMERO, TipoMiniJuego::ADIVINA_NUMERO,
        TipoMiniJuego::HEX, TipoMiniJuego::HEX, TipoMiniJuego::HEX,
        TipoMiniJuego::BATALLA_CARTAS, TipoMiniJuego::BATALLA_CARTAS, TipoMiniJuego::BATALLA_CARTAS
    };

    // Mezclar aleatoriamente el vector
    shuffle(minijuegos.begin(), minijuegos.end(), gen);

    // Asignar los minijuegos mezclados al tablero
    int indice = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Nodo& nodo = tablero.getNodo(i, j);

            // Asignar el minijuego desde el vector mezclado
            nodo.asignarMiniJuego(minijuegos[indice]);

            // Mostrar qué se asignó
            cout << "Casilla [" << i << "][" << j << "]: ";
            switch(minijuegos[indice]) {
                case TipoMiniJuego::ADIVINA_NUMERO:
                    cout << "Adivina el Numero" << endl;
                    break;
                case TipoMiniJuego::HEX:
                    cout << "Hex" << endl;
                    break;
                case TipoMiniJuego::BATALLA_CARTAS:
                    cout << "Batalla de Cartas" << endl;
                    break;
            }

            indice++;
        }
    }
    cout << "Minijuegos inicializados! (3 de cada tipo)" << endl;
}

// Función helper para obtener el símbolo basado en el tipo de minijuego
std::string obtenerSimboloMinijuego(TipoMiniJuego tipo) {
    switch(tipo) {
        case TipoMiniJuego::ADIVINA_NUMERO:
            return "?";
        case TipoMiniJuego::HEX:
            return "H";
        case TipoMiniJuego::BATALLA_CARTAS:
            return "C";
        default:
            return "?";
    }
}


int main() {

    // Crear ventana
    sf::RenderWindow window(sf::VideoMode({800, 800}), "MendeWing");

    // Cargar música de fondo
    sf::Music musicaFondo;
    if (!musicaFondo.openFromFile("assets/Audios/backgrounds/musica1.ogg")) {
        std::cerr << "Error al cargar la música de fondo" << std::endl;
    } else {
        musicaFondo.setLooping(true); // Configurar la música para que se repita en bucle
        musicaFondo.setVolume(10.0f); // Ajustar el volumen
        musicaFondo.play(); // Reproducir la música
    }

    // Cargar la imagen de fondo para el tablero principal
    sf::Texture textureFondoTablero;
    if (!textureFondoTablero.loadFromFile("assets/Fondos/hot dog fondo tablero.png")) {
        std::cerr << "Error al cargar la imagen de fondo del tablero" << std::endl;
    }

    // Crear un sprite para el fondo del tablero
    sf::Sprite spriteFondoTablero(textureFondoTablero);

    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scale(
        static_cast<float>(window.getSize().x) / textureFondoTablero.getSize().x,
        static_cast<float>(window.getSize().y) / textureFondoTablero.getSize().y
    );
    spriteFondoTablero.setScale(scale);

    // Estado actual del juego
    GameState currentState = GameState::MENU;

    // Crear botones del menú
    sf::RectangleShape titulo({300, 50});
    sf::RectangleShape btnIniciar({200, 50});
    sf::RectangleShape btnSalir({200, 50});

    titulo.setPosition({250, 100});
    btnIniciar.setPosition({300, 400});
    btnSalir.setPosition({300, 500});

    titulo.setFillColor(sf::Color::Black);
    btnIniciar.setFillColor(sf::Color::Green);
    btnSalir.setFillColor(sf::Color::Red);

    // Crear fuente para el texto
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    sf::Text texto(fuente, "Bienvenidos al videojuego kuliao mas bomba", 50);

    sf::Text titulo1(fuente, "MendenWing", 30);
    sf::Text txtIniciar(fuente, "iniciar", 30);
    sf::Text txtSalir(fuente, "salir", 30);

    titulo1.setPosition({310, 110});
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

    // Crear símbolos para las fichas del tablero principal (X y O)
    sf::Text fichasTablero[9] = {
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, "")
    };
    for(int i = 0; i < 9; i++) {
        fichasTablero[i].setCharacterSize(80);
        fichasTablero[i].setFillColor(sf::Color::Red);

        int x = (i % 3) * 200 + 85;
        int y = (i / 3) * 200 + 60;
        fichasTablero[i].setPosition({x, y});
    }

    for(int i = 0; i < 9; i++) {
        simbolos[i].setFont(fuente);
        simbolos[i].setCharacterSize(50);
        simbolos[i].setFillColor(sf::Color::White);

        int x = (i % 3) * 200 + 85;
        int y = (i / 3) * 200 + 60;
        simbolos[i].setPosition({x, y});
    }

    for (auto& linea : lineas) {
        linea.setFillColor(sf::Color::White);
    }

    // Configuración de líneas verticales
    lineas[0].setSize({2, 500});  // Vertical izquierda
    lineas[1].setSize({2, 500});  // Vertical derecha

    // Configuración de líneas horizontales
    lineas[2].setSize({500, 2});  // Horizontal superior
    lineas[3].setSize({500, 2});  // Horizontal inferior

    // Posicionamiento de líneas
    lineas[0].setPosition({200, 50});  // Vertical izquierda
    lineas[1].setPosition({400, 50});  // Vertical derecha
    lineas[2].setPosition({50, 200});  // Horizontal superior
    lineas[3].setPosition({50, 400});  // Horizontal inferior

    // Crear botón de ayuda circular
    sf::CircleShape btnAyuda(30); // Radio de 20 pixels
    btnAyuda.setPosition({20, 740}); // Posición en esquina inferior izquierda
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(2);
    btnAyuda.setOutlineColor(sf::Color::Black);

    // Texto del signo de interrogación
    sf::Text txtAyuda(fuente, "?", 24);
    txtAyuda.setPosition({40, 755}); // Ajustar posición para centrar en el círculo
    txtAyuda.setFillColor(sf::Color::Black);
    // Crear instancia del tablero

    Tablero tablero;

    // Asignar minijuegos a los nodos
    inicializarMinijuegosAleatorios(tablero);

    // AQUÍ ESTÁ LA CORRECCIÓN: Actualizar símbolos basándose en los minijuegos asignados
    for(int i = 0; i < 9; i++) {
        int fila = i / 3;
        int columna = i % 3;
        TipoMiniJuego tipoMinijuego = tablero.getNodo(fila, columna).getTipoMiniJuego();
        std::string simbolo = obtenerSimboloMinijuego(tipoMinijuego);
        simbolos[i].setString(simbolo);
    }

    bool victoriaMostrada = false;
    

    while (window.isOpen()) {
       
        while (const std::optional event = window.pollEvent()) {
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
                        std::cout << "iniciando" << std::endl;
                    }
                    // Verificar click en botón Salir
                    else if (mousePos.x >= 300 && mousePos.x <= 500 &&
                             mousePos.y >= 500 && mousePos.y <= 550) {
                        std::cout << "salir" << std::endl;
                        window.close();
                    }
                } else if (currentState == GameState::GAME) {
                    // Verificar click en botón de ayuda primero
                    sf::Vector2f posAyuda = btnAyuda.getPosition();
                    float radio = btnAyuda.getRadius();
                    if (mousePos.x >= posAyuda.x && mousePos.x <= posAyuda.x + (radio * 2) &&
                        mousePos.y >= posAyuda.y && mousePos.y <= posAyuda.y + (radio * 2)) {
                        mostrarVentanaAyuda();
                        continue;
                    }
                    // Verificar clicks en el tablero
                    int columna = mousePos.x / 200;
                    int fila = mousePos.y / 200;
                    if (columna < 3 && fila < 3) {
                        int index = fila * 3 + columna;

                        // Solo permitir jugar en casillas que no tengan ficha
                        if (tablero.getNodo(fila, columna).estaVacio()) {
                            // CAMBIO: Obtener el tipo de minijuego directamente del tablero
                            TipoMiniJuego tipoMinijuego = tablero.getNodo(fila, columna).getTipoMiniJuego();
                            
                            if (tipoMinijuego == TipoMiniJuego::HEX) {
                                std::cout << "H en casilla " << index << std::endl;
                                // Iniciar juego Hex
                                abrirHex(index, tablero,musicaFondo);
                            } else if (tipoMinijuego == TipoMiniJuego::BATALLA_CARTAS) {
                                std::cout << "C en casilla " << index << std::endl;
                                // Iniciar Batalla de Cartas
                                abrirBatallaCartas(index, tablero,musicaFondo);
                            } else if (tipoMinijuego == TipoMiniJuego::ADIVINA_NUMERO) {
                                std::cout << "Abriendo Adivina el Numero en casilla " << index << std::endl;
                                // Abrir ventana de Adivina el Número
                                abrirAdivinaNumero(index, tablero,musicaFondo);
                            }
                        } else {
                            std::cout << "Casilla " << index << " ya ocupada" << std::endl;
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


        } else {
            window.draw(spriteFondoTablero); // Dibujar el fondo del tablero
            window.draw(btnAyuda);
            window.draw(txtAyuda);
            // Dibujar tablero
            for (int i = 0; i < 4; i++) {
                window.draw(lineas[i]);
            }
            // Dibujar símbolos de minijuegos
            for(int i = 0; i < 9; i++) {
                // Solo dibujar el símbolo si no hay ficha en esa casilla
                if (tablero.getNodo(i / 3, i % 3).estaVacio()) {
                    window.draw(simbolos[i]);
                }
            }
            // Dibujar fichas del tablero principal (X y O)
            for(int i = 0; i < 9; i++) {
                EstadoNodo estado = tablero.getNodo(i / 3, i % 3).getEstado();
                if (estado == EstadoNodo::JUGADOR1) {
                    fichasTablero[i].setString("X");
                    window.draw(fichasTablero[i]);
                } else if (estado == EstadoNodo::JUGADOR2) {
                    fichasTablero[i].setString("O");
                    window.draw(fichasTablero[i]);
                }
            }

            // Verificar victoria en el tablero principal
            if (tablero.getEstadoJuego() != EstadoJuego::EN_CURSO && !victoriaMostrada) {
                if (tablero.tableroLleno() && !tablero.verificarFilas() && !tablero.verificarColumnas() && !tablero.verificarDiagonales()) {
                    mostrarVentanaVictoriaFichas(tablero.getEstadoJuego() == EstadoJuego::GANADOR_J1 ? 'X' : 'O');
                } else {
                    mostrarVentanaVictoriaTablero(tablero.getEstadoJuego() == EstadoJuego::GANADOR_J1 ? 'X' : 'O');
                }

                 // Después de cerrar la ventana de victoria, mostrar opciones
                OpcionJuego opcion = mostrarVentanaOpciones();
                
                if (opcion == OpcionJuego::VOLVER_A_JUGAR) {
                    tablero = Tablero(); // Crear un nuevo tablero vacío
                    inicializarMinijuegosAleatorios(tablero); // Reiniciar minijuegos
                    // Actualizar símbolos de minijuegos
                    for(int i = 0; i < 9; i++) {
                        int fila = i / 3;
                        int columna = i % 3;
                        TipoMiniJuego tipoMinijuego = tablero.getNodo(fila, columna).getTipoMiniJuego();
                        std::string simbolo = obtenerSimboloMinijuego(tipoMinijuego);
                        simbolos[i].setString(simbolo);
                    }
                    
                    // Limpiar las fichas del tablero
                    for(int i = 0; i < 9; i++) {
                        fichasTablero[i].setString("");
                    }
        
                    victoriaMostrada = false; // Resetear el estado de victoria

                } else if (opcion == OpcionJuego::SALIR) {
                    // Volver al menú principal
                    currentState = GameState::MENU;
                    std::cout << "Regresando al menú principal" << std::endl;
                    exit(0);
                }

                victoriaMostrada = true;
            }
        }

        window.display();
    }

    return 0;
}