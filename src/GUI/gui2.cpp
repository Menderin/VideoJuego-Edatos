//versión funcional del tablero con sistema de 3 en raya principal
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <chrono>


#include "MiniJuegos/AdivinaNumero/AdivinaNumero.h" 
#include "MiniJuegos/BatallaDeCartas/BatallaDeCartas.h"
#include "MiniJuegos/Hex/Hex.h"
#include "Nodo.h"
#include "Minijuegos/Minijuego.h"
#include "Tablero.h"
#include "IA/IAhexVnegamex.h"
#include "IA/IaAdivinaNumero.h"
#include "IA/IaBatallaCartas.h"

using namespace std;

enum class GameState {
    MENU,
    NUEVA_VENTANA,
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

OpcionJuego mostrarVentanaOpciones() {
    // Crear ventana de opciones
    sf::RenderWindow ventanaOpciones(sf::VideoMode({600, 600}), "Quieres volver a jugar?");

    
    // Crear fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Cargar la imagen de fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/Fondo espacio exterior.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo de opciones" << std::endl;
    }



     // Crear sprite para el fondo y ajustar escala
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventanaOpciones.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaOpciones.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);


    // Elementos de la ventana
    sf::Text textoPregunta(fuente, "Quieres volver a jugar?", 40);
    sf::RectangleShape btnVolverJugar({250, 80});
    sf::RectangleShape btnSalir({250, 80});
    sf::Text txtVolverJugar(fuente, "Volver a Jugar", 30);
    sf::Text txtSalir(fuente, "Salir", 30);
    
    // Configurar posiciones y colores para la nueva ventana
    textoPregunta.setPosition({150, 100});
    textoPregunta.setFillColor(sf::Color::White);

    btnVolverJugar.setPosition({175, 250});
    btnVolverJugar.setFillColor(sf::Color::Transparent);
    txtVolverJugar.setPosition({205, 270});
    txtVolverJugar.setFillColor(sf::Color::White);
    
    btnSalir.setPosition({175, 400});
    btnSalir.setFillColor(sf::Color::Transparent);
    txtSalir.setPosition({270, 420});
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
                
                // Actualizar áreas de click para los nuevos botones
                if (mousePos.x >= 175 && mousePos.x <= 425 &&
                    mousePos.y >= 250 && mousePos.y <= 330) {
                    resultado = OpcionJuego::VOLVER_A_JUGAR;
                    ventanaOpciones.close();
                }
                else if (mousePos.x >= 175 && mousePos.x <= 425 &&
                         mousePos.y >= 400 && mousePos.y <= 480) {
                    resultado = OpcionJuego::SALIR;
                    ventanaOpciones.close();
                    exit(0); // Salir del programa si se hace click en salir
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
        
        ventanaOpciones.draw(spriteFondo);
        ventanaOpciones.draw(textoPregunta);
        ventanaOpciones.draw(btnVolverJugar);
        ventanaOpciones.draw(btnSalir);
        ventanaOpciones.draw(txtVolverJugar);
        ventanaOpciones.draw(txtSalir);
        
        ventanaOpciones.display();
    }
    
    return resultado;
}

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
    } 
    else{
        ventanaGanador.close();
    }

    // Agregar botón cerrar
    sf::RectangleShape btnCerrar({120, 40});
    btnCerrar.setPosition({240, 250}); 
    btnCerrar.setFillColor(sf::Color::Transparent);

    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({270, 260}); 
    txtCerrar.setFillColor(sf::Color::Black);

    
    while (ventanaGanador.isOpen()) {
        while (const std::optional event = ventanaGanador.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaGanador.close();
            }

            // Agregar detección de click en botón cerrar
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaGanador);
                
                // Verificar click en botón cerrar
                if (mousePos.x >= 240 && mousePos.x <= 360 &&
                    mousePos.y >= 250 && mousePos.y <= 290) {
                    ventanaGanador.close();
                }
            }
        }
        
        ventanaGanador.clear(sf::Color::White);
        ventanaGanador.draw(spriteFondo);
        ventanaGanador.draw(titulo);

        ventanaGanador.draw(txtPuntosJ1);
        ventanaGanador.draw(txtPuntosJ2);

        ventanaGanador.draw(mensajeGanador);
        ventanaGanador.draw(btnCerrar);    // Dibujar el botón
        ventanaGanador.draw(txtCerrar);    // Dibujar el texto del botón
        ventanaGanador.display();
    }
}

// Variables globales para el tablero principal
char tableroPrincipal[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
int casillaMiniJuego = -1; // Para saber en qué casilla se jugó el minijuego

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
    sf::Text tituloVictoria(fuente, "VICTORIA TOTAL!", 40);
    tituloVictoria.setPosition({100, 10});
    tituloVictoria.setFillColor(sf::Color::Yellow);

    // Mensaje del ganador
    std::string jugadorGanador = (simboloGanador == 'X') ? "Jugador 1 (X)" : "Jugador 2 (O)";
    std::string mensajeGanador = jugadorGanador + " ha ganado el 3 en raya!";
    sf::Text textoGanador(fuente, mensajeGanador, 24);
    textoGanador.setPosition({80, 50});
    textoGanador.setFillColor(sf::Color::Blue);



    // Botón para cerrar - POSICIÓN AJUSTADA A LA ESQUINA INFERIOR DERECHA
    sf::RectangleShape btnCerrar({120, 40});
    btnCerrar.setPosition({ventanaVictoria.getSize().x - 130, ventanaVictoria.getSize().y - 50}); // Posición ajustada
    btnCerrar.setFillColor(sf::Color::Transparent);

    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({ventanaVictoria.getSize().x - 100, ventanaVictoria.getSize().y - 40}); // Posición ajustada
    txtCerrar.setFillColor(sf::Color::White);

    // Loop de la ventana de victoria
    while (ventanaVictoria.isOpen()) {
        while (const std::optional event = ventanaVictoria.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaVictoria.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaVictoria);

                // Verificar click en botón cerrar - COORDENADAS AJUSTADAS
                if (mousePos.x >= ventanaVictoria.getSize().x - 130 && mousePos.x <= ventanaVictoria.getSize().x - 10 &&
                    mousePos.y >= ventanaVictoria.getSize().y - 50 && mousePos.y <= ventanaVictoria.getSize().y - 10) {
                    ventanaVictoria.close();
                }
            }

        }

        ventanaVictoria.clear(sf::Color::White);
        ventanaVictoria.draw(spriteFondo);
        ventanaVictoria.draw(tituloVictoria);
        ventanaVictoria.draw(textoGanador);
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
    btnCerrar.setFillColor(sf::Color::Transparent);
    
    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({225, 230});
    txtCerrar.setFillColor(sf::Color::Black);
    
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
        ventanaVictoria.draw(txtCerrar);
        ventanaVictoria.display();
    }
}

void mostrarVentanaVictoria(int jugadorGanador, int numeroSecreto, Tablero& tablero) {
    sf::RenderWindow ventanaVictoria(sf::VideoMode({500, 300}), "Victoria!");

    // Cargar fuente
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
    static_cast<float>(ventanaVictoria.getSize().x) / Fondo.getSize().x,
    static_cast<float>(ventanaVictoria.getSize().y) / Fondo.getSize().y
    );
    spriteFondo.setScale(scale);
    // Título de victoria
    sf::Text tituloVictoria(fuente, "VICTORIA!", 40);
    tituloVictoria.setPosition({140, 170});
    tituloVictoria.setFillColor(sf::Color::Cyan);

    // Mensaje del ganador
    std::string mensajeGanador = "Jugador " + std::to_string(jugadorGanador) + " ha ganado!";
    sf::Text textoGanador(fuente, mensajeGanador, 28);
    textoGanador.setPosition({120, 10});
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
    btnCerrar.setFillColor(sf::Color::Transparent);

    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({225, 240});
    txtCerrar.setFillColor(sf::Color::Black);

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
        ventanaVictoria.draw(spriteFondo);
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
    


    // Cargar y configurar fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/mesa3.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo de ayuda Hex" << std::endl;
    }


    // Crear sprite para el fondo y ajustar escala
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventanaAyuda.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaAyuda.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);


    // Título con fondo semi-transparente
    sf::RectangleShape fondoTitulo({300, 40});
    fondoTitulo.setPosition({100, 25});
    fondoTitulo.setFillColor(sf::Color(0, 0, 0, 128));

    // Título
    sf::Text titulo(fuente, "Como Jugar Hex", 24);
    titulo.setPosition({150, 30});
    titulo.setFillColor(sf::Color::Cyan);
    

    // Panel semi-transparente para instrucciones
    sf::RectangleShape panelInstrucciones({400, 180});
    panelInstrucciones.setPosition({50, 80});
    panelInstrucciones.setFillColor(sf::Color(0, 0, 0, 128));



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
        texto.setFillColor(sf::Color::Cyan);
        instrucciones.push_back(texto);
    }
    
    // Botón cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({200, 240});
    btnCerrar.setFillColor(sf::Color::Transparent);
    
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
        ventanaAyuda.draw(spriteFondo);
        ventanaAyuda.draw(fondoTitulo);
        ventanaAyuda.draw(titulo);
        ventanaAyuda.draw(panelInstrucciones);

        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }

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
        musicHex.setVolume(100.0f);
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
        ventanaHex.draw(txtAyuda);
        ventanaHex.draw(txtCerrar);
        ventanaHex.display();
    }
    musicaFondo.play();
}
// Nueva función para mostrar ayuda específica del juego Adivina el Número
void mostrarVentanaAyudaAdivinaNumero() {
    sf::RenderWindow ventanaAyuda(sf::VideoMode({500, 300}), "Ayuda - Adivina el Numero");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Cargar y configurar fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/mesa3.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo de ayuda Adivina Numero" << std::endl;
    }

    // Crear sprite para el fondo y ajustar escala
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventanaAyuda.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaAyuda.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Título con fondo semi-transparente
    sf::RectangleShape fondoTitulo({400, 40});
    fondoTitulo.setPosition({50, 25});
    fondoTitulo.setFillColor(sf::Color(0, 0, 0, 128));
    // Título
    sf::Text titulo(fuente, "Como Jugar  Adivina el Numero", 24);
    titulo.setPosition({100, 30});
    titulo.setFillColor(sf::Color::Cyan);

    // Panel semi-transparente para instrucciones
    sf::RectangleShape panelInstrucciones({400, 180});
    panelInstrucciones.setPosition({50, 80});
    panelInstrucciones.setFillColor(sf::Color(0, 0, 0, 128));
    
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
        texto.setFillColor(sf::Color::Cyan);
        instrucciones.push_back(texto);
    }
    
    // Botón cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({200, 240});
    btnCerrar.setFillColor(sf::Color::Transparent);
    
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
        
        ventanaAyuda.clear();
        ventanaAyuda.draw(spriteFondo);
        ventanaAyuda.draw(fondoTitulo);
        ventanaAyuda.draw(titulo);
        ventanaAyuda.draw(panelInstrucciones);
        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }
        
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
        musicaAdivinarNumero.setVolume(100.0f);
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
    mensajeResultado.setFillColor(sf::Color::White);

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
                                    mensajeJugador.setString("J1 Adivina el numero del J2");
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
                                    mensajeJugador.setFillColor(turnoActual == 1 ? sf::Color::White : sf::Color::White);

                                    mensajeJugador.setString("J" + std::to_string(turnoActual) + ": Adivina el numero del J" + std::to_string(turnoActual == 1 ? 2 : 1));
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
        ventanaAdivina.draw(spriteFondo); 
        ventanaAdivina.draw(RectanguloTitulo);
        ventanaAdivina.draw(titulo);
        ventanaAdivina.draw(mensajeJugador);
        ventanaAdivina.draw(campoNumero);
        ventanaAdivina.draw(textoNumero);
        ventanaAdivina.draw(instrucciones);
        ventanaAdivina.draw(txtConfirmar);
        ventanaAdivina.draw(txtCerrar);
        ventanaAdivina.draw(mensajeResultado);
        ventanaAdivina.draw(txtAyuda);
        ventanaAdivina.display();
    }
    musicaFondo.play();
}
//mostrar la ayuda específica de Batalla de Cartas
void mostrarVentanaAyudaBatallaCartas() {
    sf::RenderWindow ventanaAyuda(sf::VideoMode({500, 300}), "Ayuda - Batalla de Cartas");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Cargar y configurar fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/mesa3.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo de ayuda Batalla de Cartas" << std::endl;
    }

    // Crear sprite para el fondo y ajustar escala
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventanaAyuda.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaAyuda.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Título con fondo semi-transparente
    sf::RectangleShape fondoTitulo({400, 40});
    fondoTitulo.setPosition({50, 25});
    fondoTitulo.setFillColor(sf::Color(0, 0, 0, 128));


    // Título
    sf::Text titulo(fuente, "Como Jugar Batalla de Cartas", 24);
    titulo.setPosition({100, 30});
    titulo.setFillColor(sf::Color::Cyan);

    // Panel semi-transparente para instrucciones
    sf::RectangleShape panelInstrucciones({400, 180});
    panelInstrucciones.setPosition({50, 80});
    panelInstrucciones.setFillColor(sf::Color(0, 0, 0, 128));
    
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
        texto.setFillColor(sf::Color::Cyan);
        instrucciones.push_back(texto);
    }
    
    // Botón cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({200, 240});
    btnCerrar.setFillColor(sf::Color::Transparent);
    
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
        
        ventanaAyuda.clear();
        ventanaAyuda.draw(spriteFondo);
        ventanaAyuda.draw(fondoTitulo);
        ventanaAyuda.draw(titulo);
        ventanaAyuda.draw(panelInstrucciones);
        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }

        ventanaAyuda.draw(txtCerrar);
        ventanaAyuda.display();
    }
}

bool ventanaJugador(bool esJugador1, std::vector<int>& valoresCartas, int& cartaSeleccionada, int puntosJ1, int puntosJ2, bool& iaPensando) {
    std::string titulo = esJugador1 ? "Turno Jugador 1" : "Turno Jugador 2";
    sf::Color colorJugador = esJugador1 ? sf::Color::Blue : sf::Color::Red;
    
    sf::RenderWindow ventana(sf::VideoMode({800, 600}), titulo);
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    // Configurar fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/Fondo batalla de cartas.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventana.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventana.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);
    
    // Título
    sf::Text tituloTexto(fuente, titulo, 32);
    tituloTexto.setPosition({80, 40});
    tituloTexto.setFillColor(colorJugador);

    // Configuración de cartas
    const float CARTA_ANCHO = 120.f;
    const float CARTA_ALTO = 180.f;
    const float ESPACIO_ENTRE_CARTAS = 30.f;
    
    std::vector<sf::RectangleShape> cartasVisuales;
    std::vector<sf::Text> numerosCartas;
    
    float inicioCartas = (800 - (valoresCartas.size() * CARTA_ANCHO + (valoresCartas.size() - 1) * ESPACIO_ENTRE_CARTAS)) / 2;
    
    // Crear visualización de cartas
    for(int i = 0; i < valoresCartas.size(); i++) {
        sf::RectangleShape carta(sf::Vector2f(CARTA_ANCHO, CARTA_ALTO));
        carta.setPosition({inicioCartas + (i * (CARTA_ANCHO + ESPACIO_ENTRE_CARTAS)), 150.f});
        carta.setFillColor(sf::Color::White);
        carta.setOutlineThickness(3);
        carta.setOutlineColor(sf::Color::Black);
        cartasVisuales.push_back(carta);
        
        sf::Text numero(fuente, std::to_string(valoresCartas[i]), 45);
        auto bounds = numero.getLocalBounds();
        float xPos = carta.getPosition().x + (CARTA_ANCHO/2) - (bounds.position.x/2);
        float yPos = carta.getPosition().y + (CARTA_ALTO/2) - (bounds.position.y/2);
        numero.setPosition({xPos, yPos});
        numero.setFillColor(sf::Color::Black);
        numerosCartas.push_back(numero);

        // Número pequeño en esquina
        sf::Text numeroEsquina(fuente, std::to_string(valoresCartas[i]), 18);
        numeroEsquina.setPosition({carta.getPosition().x + 8, carta.getPosition().y + 8});
        numeroEsquina.setFillColor(sf::Color::Black);
        numerosCartas.push_back(numeroEsquina);
        
        // Número de posición
        sf::Text numeroPosicion(fuente, std::to_string(i + 1), 20);
        numeroPosicion.setPosition({carta.getPosition().x + (CARTA_ANCHO/2) - 10, carta.getPosition().y + CARTA_ALTO + 10});
        numeroPosicion.setFillColor(colorJugador);
        numerosCartas.push_back(numeroPosicion);
    }

    // Elementos de la interfaz
    std::string numeroSeleccionado = "";
    sf::Text textoSeleccion(fuente, numeroSeleccionado, 28);
    textoSeleccion.setPosition({90, 420});
    textoSeleccion.setFillColor(sf::Color::Black);
    
    sf::RectangleShape campoSeleccion({80, 50});
    campoSeleccion.setPosition({80, 410});
    campoSeleccion.setFillColor(sf::Color::White);
    campoSeleccion.setOutlineThickness(3);
    campoSeleccion.setOutlineColor(sf::Color::Black);

    // Botones y otros elementos UI
    sf::RectangleShape btnConfirmar({150, 50});
    btnConfirmar.setPosition({200, 410});
    btnConfirmar.setFillColor(colorJugador);
    
    sf::Text txtConfirmar(fuente, "Confirmar", 22);
    txtConfirmar.setPosition({225, 425});
    txtConfirmar.setFillColor(sf::Color::White);

    // Botón de cerrar
    sf::RectangleShape btnCerrar({100, 40});
    btnCerrar.setPosition({350, 520});
    btnCerrar.setFillColor(sf::Color::Red);

    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({370, 530});
    txtCerrar.setFillColor(sf::Color::White);

    sf::CircleShape btnAyuda(25);
    btnAyuda.setPosition({30, 520});
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(3);
    btnAyuda.setOutlineColor(sf::Color::Black);

    sf::Text txtAyuda(fuente, "?", 24);
    txtAyuda.setPosition({47, 532});
    txtAyuda.setFillColor(sf::Color::Black);
    
    sf::Text instrucciones(fuente, "Selecciona una carta (1-" + 
                          std::to_string(valoresCartas.size()) + "):", 22);
    instrucciones.setPosition({80, 370});
    instrucciones.setFillColor(sf::Color::White);
    
    sf::Text txtPuntosJ1(fuente, "Puntos J1: " + std::to_string(puntosJ1), 24);
    txtPuntosJ1.setPosition({500, 40});
    txtPuntosJ1.setFillColor(sf::Color::Blue);
    
    sf::Text txtPuntosJ2(fuente, "Puntos J2: " + std::to_string(puntosJ2), 24);
    txtPuntosJ2.setPosition({500, 75});
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
                
                // Click en botón de ayuda
                sf::Vector2f posAyuda = btnAyuda.getPosition();
                float radio = btnAyuda.getRadius();
                if (mousePos.x >= posAyuda.x && mousePos.x <= posAyuda.x + (radio * 2) &&
                    mousePos.y >= posAyuda.y && mousePos.y <= posAyuda.y + (radio * 2)) {
                    mostrarVentanaAyudaBatallaCartas();
                    continue;
                }
                
                // Click en confirmar
                if (mousePos.x >= 200 && mousePos.x <= 350 &&
                    mousePos.y >= 410 && mousePos.y <= 460) {
                    if (!numeroSeleccionado.empty()) {
                        int indice = std::stoi(numeroSeleccionado) - 1;
                        if (indice >= 0 && indice < valoresCartas.size()) {
                            cartaSeleccionada = valoresCartas[indice];
                            valoresCartas.erase(valoresCartas.begin() + indice);
                            return true;
                        }
                    }
                }

                // Click en cerrar
                if (mousePos.x >= 350 && mousePos.x <= 450 &&
                    mousePos.y >= 520 && mousePos.y <= 560) {
                    ventana.close();
                    return false;
                }
                
                // Click directo en cartas
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
        ventana.draw(spriteFondo);
        ventana.draw(tituloTexto);
        ventana.draw(txtPuntosJ1);
        ventana.draw(txtPuntosJ2);
        
        for(const auto& carta : cartasVisuales) {
            ventana.draw(carta);
        }
        for(const auto& numero : numerosCartas) {
            ventana.draw(numero);
        }
        
        ventana.draw(campoSeleccion);
        ventana.draw(textoSeleccion);
        ventana.draw(btnConfirmar);
        ventana.draw(txtConfirmar);
        ventana.draw(btnCerrar);
        ventana.draw(txtCerrar);
        ventana.draw(instrucciones);
        ventana.draw(btnAyuda);
        ventana.draw(txtAyuda);
        
        ventana.display();
    }
    
    
    return false;
}

void mostrarVentanaAyuda() {
    sf::RenderWindow ventanaAyuda(sf::VideoMode({600, 400}), "Ayuda del Juego");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    

    // Cargar y configurar fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/mesa3.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo de ayuda" << std::endl;
    }

    // Crear sprite para el fondo y ajustar escala
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventanaAyuda.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaAyuda.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Título
    sf::Text titulo(fuente, "Ayuda Como Jugar", 30);
    titulo.setPosition({200, 30});
    titulo.setFillColor(sf::Color::Cyan);
    

    // Título con fondo semi-transparente
    sf::RectangleShape fondoTitulo({400, 40});
    fondoTitulo.setPosition({100, 25});
    fondoTitulo.setFillColor(sf::Color(0, 0, 0, 128));


    // Panel semi-transparente para instrucciones
    sf::RectangleShape panelInstrucciones({500, 250});
    panelInstrucciones.setPosition({50, 80});
    panelInstrucciones.setFillColor(sf::Color(0, 0, 0, 128));


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
        texto.setFillColor(sf::Color::Cyan);
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
        
        ventanaAyuda.clear();
        ventanaAyuda.draw(spriteFondo);
        ventanaAyuda.draw(fondoTitulo);
        ventanaAyuda.draw(titulo);
        ventanaAyuda.draw(panelInstrucciones);
        for(const auto& texto : instrucciones) {
            ventanaAyuda.draw(texto);
        }

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
        musicaBatallaCartas.setVolume(100.0f);
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

        bool iaPensando = false;
        // Turno J1
        if (!ventanaJugador(true, mazoJ1, cartaJ1, puntosJ1, puntosJ2, iaPensando)) break;

        // Turno J2
        if (!ventanaJugador(false, mazoJ2, cartaJ2, puntosJ1, puntosJ2, iaPensando)) break;

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

void abrirHexVsIA(int casilla, Tablero& tablero, sf::Music& musicaFondo) {
    
    casillaMiniJuego = casilla;

    sf::RenderWindow ventanaHex(sf::VideoMode({800, 700}), "Juego Hex vs IA");

    // Detener música de fondo actual
    musicaFondo.stop();
    
    // Cargar y reproducir música de Hex
    sf::Music musicHex;
    if (!musicHex.openFromFile("assets/Audios/Backgrounds/hex.ogg")) {
        std::cerr << "Error al cargar la música de Hex" << std::endl;
    } else {
        musicHex.setLooping(true);
        musicHex.setVolume(100.0f);
        musicHex.play();
    }

    // Configurar fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/Fondo espacio exterior.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventanaHex.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaHex.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Configurar elementos UI
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    
    sf::Text titulo(fuente, "Hex vs IA", 36);
    titulo.setPosition({300, 20});
    titulo.setFillColor(sf::Color::White);

    sf::Text instrucciones(fuente, "Jugador (Rojo) vs IA (Azul)", 20);
    instrucciones.setPosition({250, 60});
    instrucciones.setFillColor(sf::Color::White);

    sf::Text txtCerrar(fuente, "Cerrar", 25);
    txtCerrar.setPosition({360, 640});
    txtCerrar.setFillColor(sf::Color::White);

    sf::Text txtTurno(fuente, "Tu turno", 24);
    txtTurno.setPosition({350, 90});
    txtTurno.setFillColor(sf::Color::Yellow);

    // Botón de ayuda
    sf::CircleShape btnAyuda(20);
    btnAyuda.setPosition({20, 650});
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(2);
    btnAyuda.setOutlineColor(sf::Color::Black);

    sf::Text txtAyuda(fuente, "?", 20);
    txtAyuda.setPosition({33, 658});
    txtAyuda.setFillColor(sf::Color::White);

    // Configurar tablero hexagonal
    const int TABLERO_SIZE = 7;
    const float HEX_RADIO = 35.0f;
    const float HEX_SPACING = 60.0f;

    // Calcular dimensiones totales del tablero
    const float TABLERO_ANCHO = TABLERO_SIZE * HEX_SPACING;
    const float TABLERO_ALTO = TABLERO_SIZE * (HEX_SPACING * 0.866f);

    // Calcular offsets para centrar
    const float OFFSET_X = (800 - TABLERO_ANCHO - 150) / 2;  // 800 es el ancho de la ventana
    const float OFFSET_Y = (700 - TABLERO_ALTO) / 2 + 50;   // 700 es el alto de la ventana

    std::vector<std::vector<sf::CircleShape>> tableroHex(TABLERO_SIZE, std::vector<sf::CircleShape>(TABLERO_SIZE));

    // En el bucle donde se crean los hexágonos:
    for(int fila = 0; fila < TABLERO_SIZE; fila++) {
        for(int col = 0; col < TABLERO_SIZE; col++) {
            // Calcular posición con offset para centrado
            float x = OFFSET_X + col * HEX_SPACING + (fila * HEX_SPACING * 0.5f);
            float y = OFFSET_Y + fila * (HEX_SPACING * 0.866f);

            sf::CircleShape hex = crearHexagono(HEX_RADIO, {x, y});
            hex.setFillColor(sf::Color(255, 255, 255, 128));
            tableroHex[fila][col] = hex;
        }
    }

    // Colorear bordes del tablero
    for(int i = 0; i < TABLERO_SIZE; i++) {
        // Bordes superior e inferior (azul - IA)
        tableroHex[0][i].setOutlineColor(sf::Color::Blue);
        tableroHex[0][i].setOutlineThickness(4);
        tableroHex[TABLERO_SIZE-1][i].setOutlineColor(sf::Color::Blue);
        tableroHex[TABLERO_SIZE-1][i].setOutlineThickness(4);

        // Bordes izquierdo y derecho (rojo - Jugador)
        tableroHex[i][0].setOutlineColor(sf::Color::Red);
        tableroHex[i][0].setOutlineThickness(4);
        tableroHex[i][TABLERO_SIZE-1].setOutlineColor(sf::Color::Red);
        tableroHex[i][TABLERO_SIZE-1].setOutlineThickness(4);
    }

    // Crear instancias del juego y la IA
    Hex juegoHex(TABLERO_SIZE,true);
    
    IAHex ia(3, 500); // Profundidad 3, dificultad 500 mejor version por el momento
    bool turnoJugador = true;

    // Loop principal
    while (ventanaHex.isOpen()) {
        while (const std::optional event = ventanaHex.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaHex.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>() && turnoJugador) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaHex);

                // Verificar click en botón de ayuda
                sf::Vector2f posAyuda = btnAyuda.getPosition();
                float radio = btnAyuda.getRadius();
                if (mousePos.x >= posAyuda.x && mousePos.x <= posAyuda.x + (radio * 2) &&
                    mousePos.y >= posAyuda.y && mousePos.y <= posAyuda.y + (radio * 2)) {
                    mostrarVentanaAyudaHex();
                    continue;
                }

                // Verificar click en cerrar
                if (mousePos.x >= 350 && mousePos.x <= 450 &&
                    mousePos.y >= 630 && mousePos.y <= 670) {
                    ventanaHex.close();
                }

                // Procesar clicks en el tablero
                bool movimientoRealizado = false;
                for(int fila = 0; fila < TABLERO_SIZE&&!movimientoRealizado; fila++) {
                    for(int col = 0; col < TABLERO_SIZE&&!movimientoRealizado; col++) {
                        sf::Vector2f hexCenter = tableroHex[fila][col].getPosition() + sf::Vector2f(HEX_RADIO, HEX_RADIO);
                        float distance = std::sqrt(std::pow(mousePos.x - hexCenter.x, 2) + std::pow(mousePos.y - hexCenter.y, 2));

                        if (distance <= HEX_RADIO && juegoHex.getCasilla(fila, col) == EstadoCasilla::VACIA) {
                            if (juegoHex.hacerMovimiento(fila, col)) {
                                tableroHex[fila][col].setFillColor(sf::Color::Red);

                                if (juegoHex.estaTerminado()) {
                                    tablero.getNodo(casillaMiniJuego / 3, casillaMiniJuego % 3).setEstado(EstadoNodo::JUGADOR1);
                                    tablero.verificarVictoria();
                                    mostrarVentanaVictoria(1, -1, tablero);
                                    // Agregar estos cout justo antes del return mejorMovimiento
                                    std::cout << "\n=== TOTAL de estadisticas de la IA ===" << std::endl;
                                    std::cout << "Tiempo promedio por movimiento: " << ia.getTiempoPromedioMovimiento() << "ms" << std::endl;
                                    std::cout << "Tiempo total: " << ia.getTiempoTotalMovimientos() << "ms" << std::endl;
                                    std::cout << "Total movimientos: " << ia.getCantidadMovimientos() << std::endl;
                                    std::cout << "===============================" << std::endl;
                                    musicHex.stop();
                                    musicaFondo.play();
                                    ventanaHex.close();
                                    return;
                                }

                                turnoJugador = false;
                                txtTurno.setString("Turno de la IA");
                                movimientoRealizado = true;
                            }
                        }
                    }
                }
            }
        }

        // Turno de la IA
        if (!turnoJugador && !juegoHex.estaTerminado()) {
            sf::sleep(sf::milliseconds(500));
            
            Posicion movimientoIA = ia.calcularMejorMovimiento(juegoHex);
        
        
            
            if (movimientoIA.fila >= 0 && movimientoIA.columna >= 0 && 
                juegoHex.getCasilla(movimientoIA.fila, movimientoIA.columna) == EstadoCasilla::VACIA) {
                
                if (juegoHex.hacerMovimiento(movimientoIA.fila, movimientoIA.columna)) {
                    tableroHex[movimientoIA.fila][movimientoIA.columna].setFillColor(sf::Color::Blue);
                    
                    if (juegoHex.estaTerminado()) {
                        tablero.getNodo(casillaMiniJuego / 3, casillaMiniJuego % 3).setEstado(EstadoNodo::JUGADOR2);
                        tablero.verificarVictoria();
                        mostrarVentanaVictoria(2, -1, tablero);
                        musicHex.stop();
                        musicaFondo.play();
                        ventanaHex.close();
                        return;
                    }
                    
                    turnoJugador = true;
                    txtTurno.setString("Tu turno");
                }
            }
        }

        // Renderizado
        ventanaHex.clear(sf::Color::White);
        ventanaHex.draw(spriteFondo);
        ventanaHex.draw(titulo);
        ventanaHex.draw(instrucciones);
        ventanaHex.draw(txtTurno);

        // Dibujar tablero
        for(int fila = 0; fila < TABLERO_SIZE; fila++) {
            for(int col = 0; col < TABLERO_SIZE; col++) {
                ventanaHex.draw(tableroHex[fila][col]);
            }
        }

        ventanaHex.draw(txtAyuda);
        ventanaHex.draw(txtCerrar);
        ventanaHex.display();
    }
    
    musicaFondo.play();
}

void abrirAdivinaNumeroVsIA(int casilla, Tablero& tablero, sf::Music& musicaFondo) {
    casillaMiniJuego = casilla;
    
    sf::RenderWindow ventanaAdivina(sf::VideoMode({600, 450}), "Adivina el Numero vs IA");
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Configuración de música
    musicaFondo.stop();
    sf::Music musicaAdivinarNumero;
    if (!musicaAdivinarNumero.openFromFile("assets/Audios/Backgrounds/AdivinaNumero.ogg")) {
        std::cerr << "Error al cargar la música" << std::endl;
    } else {
        musicaAdivinarNumero.setLooping(true);
        musicaAdivinarNumero.setVolume(100.0f);
        musicaAdivinarNumero.play();
    }

    // Configuración del fondo
    sf::Texture textureFondo;
    if (!textureFondo.loadFromFile("assets/Fondos/Fondo adivina numero.png")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    sf::Sprite spriteFondo(textureFondo);
    sf::Vector2f scale(
        static_cast<float>(ventanaAdivina.getSize().x) / textureFondo.getSize().x,
        static_cast<float>(ventanaAdivina.getSize().y) / textureFondo.getSize().y
    );
    spriteFondo.setScale(scale);

    // Elementos de UI
    sf::RectangleShape RectanguloTitulo({330, 65});
    RectanguloTitulo.setPosition({140, 25});
    RectanguloTitulo.setFillColor(sf::Color(255, 255, 255, 128));

    sf::Text titulo(fuente, "Adivina el Numero vs IA", 30);
    titulo.setPosition({145, 30});
    titulo.setFillColor(sf::Color::Black);

    sf::Text mensajeJugador(fuente, "Elige tu numero secreto (1-100)", 22);
    mensajeJugador.setPosition({130, 90});
    mensajeJugador.setFillColor(sf::Color::White);

    // Campo de entrada y número
    sf::RectangleShape campoNumero({200, 40});
    campoNumero.setPosition({200, 140});
    campoNumero.setFillColor(sf::Color::White);
    campoNumero.setOutlineThickness(2);
    campoNumero.setOutlineColor(sf::Color::Black);

    std::string numeroIngresado = "";
    sf::Text textoNumero(fuente, numeroIngresado, 24);
    textoNumero.setPosition({210, 148});
    textoNumero.setFillColor(sf::Color::Black);

    // Instrucciones y mensajes
    sf::Text instrucciones(fuente, "La IA elegira un numero del 1 al 100", 18);
    instrucciones.setPosition({170, 200});
    instrucciones.setFillColor(sf::Color::White);

    sf::Text mensajeResultado(fuente, "", 24);
    mensajeResultado.setPosition({150, 240});
    mensajeResultado.setFillColor(sf::Color::White);

    // Botones
    sf::RectangleShape btnConfirmar({120, 40});
    btnConfirmar.setPosition({240, 280});
    btnConfirmar.setFillColor(sf::Color::Blue);

    sf::Text txtConfirmar(fuente, "Confirmar", 20);
    txtConfirmar.setPosition({255, 290});
    txtConfirmar.setFillColor(sf::Color::White);

    sf::RectangleShape btnCerrar({120, 40});
    btnCerrar.setPosition({240, 350});
    btnCerrar.setFillColor(sf::Color::Red);

    sf::Text txtCerrar(fuente, "Cerrar", 20);
    txtCerrar.setPosition({270, 360});
    txtCerrar.setFillColor(sf::Color::White);

    // Botón de ayuda
    sf::CircleShape btnAyuda(20);
    btnAyuda.setPosition({20, 390});
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(2);
    btnAyuda.setOutlineColor(sf::Color::Black);

    sf::Text txtAyuda(fuente, "?", 20);
    txtAyuda.setPosition({33, 398});
    txtAyuda.setFillColor(sf::Color::Black);

    // Inicialización del juego
    AdivinaNumero juegoAdivina;
    IaAdivinaNumero ia;

    // Generar número para la IA
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    int numeroIA = dis(gen);
    juegoAdivina.setNumeroJugador(2, numeroIA);

    bool numeroJugadorElegido = false;
    bool turnoJugador = true;
    int intentosJugador = 0;
    int intentosIA = 0;

    while (ventanaAdivina.isOpen()) {
        while (const std::optional event = ventanaAdivina.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                ventanaAdivina.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventanaAdivina);
                
                // Click en botón de ayuda
                sf::Vector2f posAyuda = btnAyuda.getPosition();
                if (mousePos.x >= posAyuda.x && mousePos.x <= posAyuda.x + 40 &&
                    mousePos.y >= posAyuda.y && mousePos.y <= posAyuda.y + 40) {
                    mostrarVentanaAyudaAdivinaNumero();
                    continue;
                }

                // Click en botón cerrar
                if (mousePos.x >= 240 && mousePos.x <= 360 &&
                    mousePos.y >= 350 && mousePos.y <= 390) {
                    ventanaAdivina.close();
                }

                // Click en botón confirmar
                if (mousePos.x >= 240 && mousePos.x <= 360 &&
                    mousePos.y >= 280 && mousePos.y <= 320) {
                    if (!numeroIngresado.empty()) {
                        int numero = std::stoi(numeroIngresado);
                        
                        if (numero < 1 || numero > 100) {
                            mensajeResultado.setString("El numero debe estar entre 1 y 100");
                            numeroIngresado = "";
                            textoNumero.setString(numeroIngresado);
                            continue;
                        }
                        
                        if (!numeroJugadorElegido) {
                            juegoAdivina.setNumeroJugador(1, numero);
                            numeroJugadorElegido = true;
                            mensajeJugador.setString("Tu turno Adivina el numero de la IA");
                            instrucciones.setString("Intenta adivinar el numero (1-100)");
                            mensajeResultado.setString("");
                            numeroIngresado = "";
                            textoNumero.setString(numeroIngresado);
                        } 
                        else if (turnoJugador) {
                            intentosJugador++;
                            if (numero == numeroIA) {
                                mensajeResultado.setString("Ganaste en " + std::to_string(intentosJugador) + " intentos");
                                tablero.getNodo(casilla / 3, casilla % 3).setEstado(EstadoNodo::JUGADOR1);
                                tablero.verificarVictoria();
                                mostrarVentanaVictoria(1, numeroIA, tablero);
                                musicaAdivinarNumero.stop();
                                musicaFondo.play();
                                ventanaAdivina.close();
                            } else {
                                std::cout << "Numero IA: " << numeroIA << ", Intento: " << numero << std::endl;
                                std::string pista = (numero > numeroIA) ? "MENOR" : "MAYOR";
                                mensajeResultado.setString(pista);
                                numeroIngresado = "";
                                textoNumero.setString(numeroIngresado);
                                turnoJugador = false;
                            }
                        }
                    }
                }
            }

            // Entrada de texto
            if (auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                char c = static_cast<char>(textEvent->unicode);
                if (c >= '0' && c <= '9' && numeroIngresado.length() < 3) {
                    numeroIngresado += c;
                    textoNumero.setString(numeroIngresado);
                } else if (c == 8 && !numeroIngresado.empty()) {
                    numeroIngresado.pop_back();
                    textoNumero.setString(numeroIngresado);
                }
            }
        }

        // Turno de la IA
        if (!turnoJugador && numeroJugadorElegido) {
            sf::sleep(sf::milliseconds(1000));
            int prediccionIA = ia.hacerPrediccion();
            intentosIA++;
            
            if (prediccionIA == juegoAdivina.getNumeroJugador(1)) {
                mensajeResultado.setString("La IA gana en " + std::to_string(intentosIA) + " intentos");
                tablero.getNodo(casilla / 3, casilla % 3).setEstado(EstadoNodo::JUGADOR2);
                tablero.verificarVictoria();
                mostrarVentanaVictoria(2, juegoAdivina.getNumeroJugador(1), tablero);
                musicaAdivinarNumero.stop();
                musicaFondo.play();
                ventanaAdivina.close();
            } else {
                mensajeResultado.setString(mensajeResultado.getString()  + " - IA intenta: " + std::to_string(prediccionIA));

                if (prediccionIA < juegoAdivina.getNumeroJugador(1)) {
                    mensajeResultado.setString(mensajeResultado.getString());
                    ia.actualizarLimites(true);
                } else if (prediccionIA > juegoAdivina.getNumeroJugador(1)) {
                    mensajeResultado.setString(mensajeResultado.getString());
                    ia.actualizarLimites(false);
                }
                turnoJugador = true;
            }
        }

        // Renderizado
        ventanaAdivina.clear(sf::Color::White);
        ventanaAdivina.draw(spriteFondo);
        ventanaAdivina.draw(RectanguloTitulo);
        ventanaAdivina.draw(titulo);
        ventanaAdivina.draw(mensajeJugador);
        ventanaAdivina.draw(campoNumero);
        ventanaAdivina.draw(textoNumero);
        ventanaAdivina.draw(instrucciones);
        ventanaAdivina.draw(btnConfirmar);
        ventanaAdivina.draw(txtConfirmar);
        ventanaAdivina.draw(btnCerrar);
        ventanaAdivina.draw(txtCerrar);
        ventanaAdivina.draw(btnAyuda);
        ventanaAdivina.draw(txtAyuda);
        ventanaAdivina.draw(mensajeResultado);
        ventanaAdivina.display();
    }
    
    musicaFondo.play();
}



void abrirBatallaCartasVsIA(int casilla, Tablero& tablero, sf::Music& musicaFondo) {
    casillaMiniJuego = casilla;
    
    // Cargar fuente
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");

    // Detener música de fondo actual
    musicaFondo.stop();
    
    // Cargar y reproducir música de Batalla de Cartas
    sf::Music musicaBatallaCartas;
    if (!musicaBatallaCartas.openFromFile("assets/Audios/Backgrounds/BatallaCartas.ogg")) {
        std::cerr << "Error al cargar la música" << std::endl;
    } else {
        musicaBatallaCartas.setLooping(true);
        musicaBatallaCartas.setVolume(100.0f);
        musicaBatallaCartas.play();
    }

    // Crear vector con todos los números posibles
    std::vector<int> numerosDisponibles;
    for(int i = 1; i <= 15; i++) {
        numerosDisponibles.push_back(i);
    }

    // Generar mazos para jugador y IA
    std::vector<int> mazoJugador, mazoIA;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Repartir cartas al jugador
    for(int i = 0; i < 5; i++) {
        std::uniform_int_distribution<> dis(0, numerosDisponibles.size() - 1);
        int indiceAleatorio = dis(gen);
        int valorCarta = numerosDisponibles[indiceAleatorio];
        numerosDisponibles.erase(numerosDisponibles.begin() + indiceAleatorio);
        mazoJugador.push_back(valorCarta);
    }

    // Repartir cartas a la IA
    for(int i = 0; i < 5; i++) {
        std::uniform_int_distribution<> dis(0, numerosDisponibles.size() - 1);
        int indiceAleatorio = dis(gen);
        int valorCarta = numerosDisponibles[indiceAleatorio];
        numerosDisponibles.erase(numerosDisponibles.begin() + indiceAleatorio);
        mazoIA.push_back(valorCarta);
    }

    // Crear instancia de la IA
    IaBatallaCartas ia(75);
    ia.actualizarCartas(mazoIA);

    int puntosJugador = 0, puntosIA = 0;

    // Loop principal del juego
    while (!mazoJugador.empty() && !mazoIA.empty()) {
        int cartaJugador = -1;
        bool iaPensando = false;

        // Turno del jugador
        if (!ventanaJugador(true, mazoJugador, cartaJugador, puntosJugador, puntosIA, iaPensando)) {
            break;
        }

        // La IA elige su carta
        int cartaIA = ia.elegirCarta(cartaJugador);
        
        auto it = std::find(mazoIA.begin(), mazoIA.end(), cartaIA);
        if (it != mazoIA.end()) {
            mazoIA.erase(it);
        }
        
        ia.actualizarCartas(mazoIA);

        // Comparar cartas y actualizar puntos
        if (cartaJugador > cartaIA) {
            puntosJugador++;
        } else if (cartaIA > cartaJugador) {
            puntosIA++;
        }
        
        ia.actualizarPuntos(puntosIA, puntosJugador);
    }

    // Determinar ganador y colocar ficha
    if (puntosJugador != puntosIA) {
        // Marcar casilla en el tablero
        tablero.getNodo(casilla / 3, casilla % 3).setEstado(
            puntosJugador > puntosIA ? EstadoNodo::JUGADOR1 : EstadoNodo::JUGADOR2
        );
        
        // Detener música y mostrar ventana de ganador
        musicaBatallaCartas.stop();
        musicaFondo.play();
        mostrarVentanaGanador(puntosJugador, puntosIA);
        
        // Verificar victoria en tablero principal
        tablero.verificarVictoria();
    } else {
        // En caso de empate
        musicaBatallaCartas.stop();
        musicaFondo.play();
    }
}




int main() {
    // CENTRADO DEL TABLERO - Calcular offset para centrar
    const int CELL_SIZE = 160; // Tamaño de cada celda
    const int BOARD_SIZE = CELL_SIZE * 3; // 480 píxeles
    const int OFFSET_X = (800 - BOARD_SIZE) / 2; 
    const int OFFSET_Y = (800 - BOARD_SIZE) / 2; 
    
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode({800, 800}), "MendeWing");

    sf::Texture textureFondoPortada;
    if (!textureFondoPortada.loadFromFile("assets/Fondos/Portada Juego.png")) {
        std::cerr << "Error al cargar la imagen de fondo del tablero" << std::endl;
    }

    // Crear un sprite para el fondo del tablero
    sf::Sprite spriteFondoPortada(textureFondoPortada);

    // Ajustar el sprite al tamaño de la ventana
    sf::Vector2f scalePortada(
        static_cast<float>(window.getSize().x) / textureFondoPortada.getSize().x,
        static_cast<float>(window.getSize().y) / textureFondoPortada.getSize().y
    );
    spriteFondoPortada.setScale(scalePortada);    

    // Cargar música de fondo
    sf::Music musicaFondo;
    if (!musicaFondo.openFromFile("assets/Audios/backgrounds/musica1.ogg")) {
        std::cerr << "Error al cargar la música de fondo" << std::endl;
    } else {
        musicaFondo.setLooping(true); // Configurar la música para que se repita en bucle
        musicaFondo.setVolume(100.0f); // Ajustar el volumen
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
    
    // Variable para el modo de juego
    enum ModoJuego { JUGADOR_VS_JUGADOR, JUGADOR_VS_IA };
    ModoJuego modoActual = JUGADOR_VS_JUGADOR;

    // Crear botones del menú
    sf::RectangleShape titulo({500, 60});
    sf::RectangleShape btnJvJ({300, 50});      // Botón Jugador vs Jugador
    sf::RectangleShape btnJvIA({300, 50});     // Botón Jugador vs IA
    sf::RectangleShape btnSalir({300, 50});    // Botón Salir

    titulo.setPosition({350, 100});
    btnJvJ.setPosition({250, 350});
    btnJvIA.setPosition({250, 425});
    btnSalir.setPosition({250, 500});

    titulo.setFillColor(sf::Color::Black);
    btnJvJ.setFillColor(sf::Color::Green);
    btnJvIA.setFillColor(sf::Color::Blue);
    btnSalir.setFillColor(sf::Color::Red);


    // Cargar textura para la imagen que va sobre las O
    sf::Texture texturaImagenY;
    if (!texturaImagenY.loadFromFile("assets/Fichas/Paolini ficha definitiva.png")) {
        std::cerr << "Error al cargar la imagen para X" << std::endl;
        return -1;
    }

    std::cout << "Textura cargada correctamente. Tamaño: " << texturaImagenY.getSize().x << "x" << texturaImagenY.getSize().y << std::endl;

        // Crear sprites para las imágenes sobre las O
    sf::Sprite spritesImagenY[9] = {
        sf::Sprite(texturaImagenY), sf::Sprite(texturaImagenY), sf::Sprite(texturaImagenY),
        sf::Sprite(texturaImagenY), sf::Sprite(texturaImagenY), sf::Sprite(texturaImagenY),
        sf::Sprite(texturaImagenY), sf::Sprite(texturaImagenY), sf::Sprite(texturaImagenY)
    };

    for(int i = 0; i < 9; i++) {
        std::cout << "Creando sprite " << i << std::endl;
        spritesImagenY[i] = sf::Sprite(texturaImagenY); // Asignar la textura al sprite

        // Posicionar cada sprite en la casilla correspondiente (centrado)
        int x = OFFSET_X + (i % 3) * CELL_SIZE + 20; // Ajustar posición dentro de la celda
        int y = OFFSET_Y + (i / 3) * CELL_SIZE + 20; // Ajustar posición dentro de la celda
        spritesImagenY[i].setPosition({x, y});

        // Ajustar tamaño para que encaje en la casilla
        sf::Vector2f escala(
            (CELL_SIZE - 40) / static_cast<float>(texturaImagenY.getSize().x),
            (CELL_SIZE - 40) / static_cast<float>(texturaImagenY.getSize().y)
        );
        spritesImagenY[i].setScale(escala);

        std::cout << "Sprite " << i << " creado y posicionado en (" << x << ", " << y << ") con escala (" << escala.x << ", " << escala.y << ")" << std::endl;
    }



    // Cargar textura para la imagen que va sobre las X
    sf::Texture texturaImagenX;
    if (!texturaImagenX.loadFromFile("assets/Fichas/Ross sin fondo definitivo.png")) {
        std::cerr << "Error al cargar la imagen para X" << std::endl;
        return -1;
    }

    std::cout << "Textura cargada correctamente. Tamaño: " << texturaImagenX.getSize().x << "x" << texturaImagenX.getSize().y << std::endl;

    // Crear sprites para las imágenes sobre las X
    sf::Sprite spritesImagenX[9] = {
        sf::Sprite(texturaImagenX), sf::Sprite(texturaImagenX), sf::Sprite(texturaImagenX),
        sf::Sprite(texturaImagenX), sf::Sprite(texturaImagenX), sf::Sprite(texturaImagenX),
        sf::Sprite(texturaImagenX), sf::Sprite(texturaImagenX), sf::Sprite(texturaImagenX)
    };

    for(int i = 0; i < 9; i++) {
        std::cout << "Creando sprite " << i << std::endl;
        spritesImagenX[i] = sf::Sprite(texturaImagenX); // Asignar la textura al sprite

        // Posicionar cada sprite en la casilla correspondiente (centrado)
        int x = OFFSET_X + (i % 3) * CELL_SIZE + 20; // Ajustar posición dentro de la celda
        int y = OFFSET_Y + (i / 3) * CELL_SIZE + 20; // Ajustar posición dentro de la celda
        spritesImagenX[i].setPosition({x, y});

        // Ajustar tamaño para que encaje en la casilla
        sf::Vector2f escala(
            (CELL_SIZE - 40) / static_cast<float>(texturaImagenX.getSize().x),
            (CELL_SIZE - 40) / static_cast<float>(texturaImagenX.getSize().y)
        );
        spritesImagenX[i].setScale(escala);

        std::cout << "Sprite " << i << " creado y posicionado en (" << x << ", " << y << ") con escala (" << escala.x << ", " << escala.y << ")" << std::endl;
    }

    // Crear fuente para el texto
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    sf::Text texto(fuente, "Bienvenidos al videojuego kuliao mas bomba", 50);

    sf::Text titulo1(fuente, "MendenWing", 70);
    sf::Text txtJvJ(fuente, "Jugador vs Jugador", 30);
    sf::Text txtJvIA(fuente, "Jugador vs IA", 30);
    sf::Text txtSalir(fuente, "Salir", 30);

    titulo1.setPosition({210, 260});
    txtJvJ.setPosition({280, 360});
    txtJvIA.setPosition({300, 435});
    txtSalir.setPosition({360, 510});



    // Crear las líneas del tablero 
    sf::RectangleShape lineas[4];

    for (auto& linea : lineas) {
        linea.setFillColor(sf::Color::White);
    }

    // Configuración de líneas verticales 
    lineas[0].setSize({2, BOARD_SIZE});  
    lineas[1].setSize({2, BOARD_SIZE});  

    // Configuración de líneas horizontales
    lineas[2].setSize({BOARD_SIZE, 2});  
    lineas[3].setSize({BOARD_SIZE, 2});  

    // Posicionamiento de líneas (CENTRADAS)
    lineas[0].setPosition({OFFSET_X + CELL_SIZE, OFFSET_Y});     
    lineas[1].setPosition({OFFSET_X + CELL_SIZE * 2, OFFSET_Y}); 
    lineas[2].setPosition({OFFSET_X, OFFSET_Y + CELL_SIZE});     
    lineas[3].setPosition({OFFSET_X, OFFSET_Y + CELL_SIZE * 2}); 

    // Crear símbolos para el tablero (CENTRADOS)
    sf::Text simbolos[9] = {
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, "")
    };

    // Crear símbolos para las fichas del tablero principal (X y O) (CENTRADOS)
    sf::Text fichasTablero[9] = {
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, ""),
        sf::Text(fuente, ""), sf::Text(fuente, ""), sf::Text(fuente, "")
    };

    // Posicionar fichas del tablero (CENTRADAS)
    for(int i = 0; i < 9; i++) {
        fichasTablero[i].setCharacterSize(80);
        fichasTablero[i].setFillColor(sf::Color::Red);

        int x = OFFSET_X + (i % 3) * CELL_SIZE + 60; 
        int y = OFFSET_Y + (i / 3) * CELL_SIZE + 40; 
        fichasTablero[i].setPosition({x, y});
    }

    // Posicionar símbolos de minijuegos (CENTRADOS)
    for(int i = 0; i < 9; i++) {
        simbolos[i].setFont(fuente);
        simbolos[i].setCharacterSize(50);
        simbolos[i].setFillColor(sf::Color::White);

        int x = OFFSET_X + (i % 3) * CELL_SIZE + 60;
        int y = OFFSET_Y + (i / 3) * CELL_SIZE + 40;
        simbolos[i].setPosition({x, y});
    }

    // Crear botón de ayuda circular
    sf::CircleShape btnAyuda(30);
    btnAyuda.setPosition({20, 740});
    btnAyuda.setFillColor(sf::Color::White);
    btnAyuda.setOutlineThickness(2);
    btnAyuda.setOutlineColor(sf::Color::Black);

    // Texto del signo de interrogación
    sf::Text txtAyuda(fuente, "?", 24);
    txtAyuda.setPosition({40, 755});
    txtAyuda.setFillColor(sf::Color::White);
    
    // Crear instancia del tablero
    Tablero tablero;

    // Asignar minijuegos a los nodos
    inicializarMinijuegosAleatorios(tablero);

    //Actualizar símbolos basándose en los minijuegos asignados
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
                    // Verificar click en botón Jugador vs Jugador
                    if (mousePos.x >= 250 && mousePos.x <= 550 &&
                        mousePos.y >= 350 && mousePos.y <= 400) {
                        currentState = GameState::GAME;
                        modoActual = JUGADOR_VS_JUGADOR;
                        std::cout << "Iniciando modo Jugador vs Jugador" << std::endl;
                    }
                    // Verificar click en botón Jugador vs IA
                    else if (mousePos.x >= 250 && mousePos.x <= 550 &&
                            mousePos.y >= 425 && mousePos.y <= 475) {
                        currentState = GameState::GAME;
                        modoActual = JUGADOR_VS_IA;
                        std::cout << "Iniciando modo Jugador vs IA" << std::endl;
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
                    
                    // Calcular posición relativa al tablero centrado
                    int relativeX = mousePos.x - OFFSET_X;
                    int relativeY = mousePos.y - OFFSET_Y;
                    
                    // Verificar si el click está dentro del área del tablero
                    if (relativeX >= 0 && relativeX < BOARD_SIZE && 
                        relativeY >= 0 && relativeY < BOARD_SIZE) {
                        
                        int columna = relativeX / CELL_SIZE;
                        int fila = relativeY / CELL_SIZE;
                        
                        if (columna < 3 && fila < 3) {
                            int index = fila * 3 + columna;

                            // Solo permitir jugar en casillas que no tengan ficha
                            if (tablero.getNodo(fila, columna).estaVacio()) {
                                // Obtener el tipo de minijuego directamente del tablero
                                TipoMiniJuego tipoMinijuego = tablero.getNodo(fila, columna).getTipoMiniJuego();
                                
                                // Switch para determinar el modo de juego
                                switch (modoActual) {
                                    case JUGADOR_VS_JUGADOR:
                                        // Modo original Jugador vs Jugador
                                        if (tipoMinijuego == TipoMiniJuego::HEX) {
                                            std::cout << "H en casilla " << index << " (JvJ)" << std::endl;
                                            // Iniciar juego Hex modo JvJ
                                            abrirHex(index, tablero, musicaFondo);

                                        } else if (tipoMinijuego == TipoMiniJuego::BATALLA_CARTAS) {
                                            std::cout << "C en casilla " << index << " (JvJ)" << std::endl;
                                            // Iniciar Batalla de Cartas modo JvJ
                                            abrirBatallaCartas(index, tablero, musicaFondo);
                                        } else if (tipoMinijuego == TipoMiniJuego::ADIVINA_NUMERO) {
                                            std::cout << "Abriendo Adivina el Numero en casilla " << index << " (JvJ)" << std::endl;
                                            // Abrir ventana de Adivina el Número modo JvJ
                                            abrirAdivinaNumero(index, tablero, musicaFondo);
                                        }
                                        break;

                                    case JUGADOR_VS_IA:
                                        // Modo Jugador vs IA
                                        if (tipoMinijuego == TipoMiniJuego::HEX) {
                                            std::cout << "H en casilla " << index << " (JvIA)" << std::endl;
                                            // Iniciar juego Hex vs IA
                                            abrirHexVsIA(index, tablero, musicaFondo);

                                        } else if (tipoMinijuego == TipoMiniJuego::BATALLA_CARTAS) {
                                            std::cout << "C en casilla " << index << " (JvIA)" << std::endl;

                                            abrirBatallaCartasVsIA(index, tablero, musicaFondo);
                                        } else if (tipoMinijuego == TipoMiniJuego::ADIVINA_NUMERO) {
                                            std::cout << "Abriendo Adivina el Numero en casilla " << index << " (JvIA)" << std::endl;

                                            abrirAdivinaNumeroVsIA(index, tablero, musicaFondo);
                                        }
                                        break;
                                }
                            } else {
                                std::cout << "Casilla " << index << " ya ocupada" << std::endl;
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        if (currentState == GameState::MENU) {
            window.draw(spriteFondoPortada);
            window.draw(titulo);
            window.draw(titulo1);
            window.draw(txtJvJ);
            window.draw(txtJvIA);
            window.draw(txtSalir);
        } 
        else {
            window.draw(spriteFondoTablero); // Dibujar el fondo del tablero
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
                    
                    // Dibujar imagen encima de la X
                    window.draw(spritesImagenX[i]);
                } else if (estado == EstadoNodo::JUGADOR2) {
                    fichasTablero[i].setString("O");
                    
                    window.draw(spritesImagenY[i]);
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
                    tablero.reiniciarEstadoJuego();
                    victoriaMostrada = false; // Resetear el estado de victoria
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

                    currentState = GameState::MENU;
                    

                } else if (opcion == OpcionJuego::SALIR) {
                    // Volver al menú principal
                    window.close();

                    std::cout << "Regresando al menú principal" << std::endl;
                    exit(0);
                }

                victoriaMostrada = false; // Resetear el estado de victoria para evitar mostrarla varias veces
            }
        }

        window.display();

    }

    return 0;

}