//<<<<<<< HEAD
//#include <SFML/Graphics.hpp> 
//=======
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

enum class GameState {
    MENU,
    GAME
};
//>>>>>>> f8b5f19284811fb598502605c0f751e36db62362

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Mini Juegos");
    
    // Estado actual del juego
    GameState currentState = GameState::MENU;
    
    // Crear botones del menú
    sf::RectangleShape btnIniciar({200, 50});
    sf::RectangleShape btnSalir({200, 50});
    
    btnIniciar.setPosition({200, 200});
    btnSalir.setPosition({200, 300});
    
    btnIniciar.setFillColor(sf::Color::Green);
    btnSalir.setFillColor(sf::Color::Red);
    
    // Crear fuente para el texeso'
    sf::Font fuente("c:/WINDOWS/Fonts/ARIALI.TTF");
    sf::Text texto(fuente, "Bienvenidos al videojuego kuliao mas bomba", 50);

    sf::Text txtIniciar(fuente, "iniciar", 30);
    sf::Text txtSalir(fuente, "salir", 30);
    
    txtIniciar.setPosition({250, 210});
    txtSalir.setPosition({270, 310});
    
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
                    if (mousePos.x >= 200 && mousePos.x <= 400 &&
                        mousePos.y >= 200 && mousePos.y <= 250) {
                        currentState = GameState::GAME;
                    }
                    // Verificar click en botón Salir
                    else if (mousePos.x >= 200 && mousePos.x <= 400 &&
                             mousePos.y >= 300 && mousePos.y <= 350) {
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
            window.draw(btnIniciar);
            window.draw(btnSalir);
            window.draw(txtIniciar);
            window.draw(txtSalir);
        }
        else {
            // Dibujar tablero
            for(int i = 0; i < 4; i++) {
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