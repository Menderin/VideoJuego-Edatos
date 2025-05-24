#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

enum class GameState {
    MENU,
    GAME
};

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
                    }
                    // Verificar click en botón Salir
                    else if (mousePos.x >= 300 && mousePos.x <= 500 &&
                             mousePos.y >= 500 && mousePos.y <= 550) {
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
                            // Iniciar juego Hex
                            // TODO: Implementar redirección a Hex
                        }
                        else if (symbol == "C") {
                            // Iniciar Batalla de Cartas
                            // TODO: Implementar redirección a Batalla de Cartas
                        }
                        else if (symbol == "?") {
                            // TODO: Mostrar input para Adivina el Número
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