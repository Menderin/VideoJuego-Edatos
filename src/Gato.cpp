
#include <SFML/Graphics.hpp>


int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Tablero del Gato");
    
    // Crear las líneas del tablero
    sf::RectangleShape lineas[4];
    
    // Líneas verticales
    lineas[0].setSize(sf::Vector2f(5, 600));
    lineas[1].setSize(sf::Vector2f(5, 600));
    lineas[0].setPosition({200, 0});
    lineas[1].setPosition({400, 0});
    
    // Líneas horizontales
    lineas[2].setSize(sf::Vector2f(600, 5));
    lineas[3].setSize(sf::Vector2f(600, 5));
    lineas[2].setPosition({0, 200});
    lineas[3].setPosition({0, 400});
    
    // Color negro para las líneas
    for(int i = 0; i < 4; i++) {
        lineas[i].setFillColor(sf::Color::Black);
    }

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        
        // Dibujar las líneas
        for(int i = 0; i < 4; i++) {
            window.draw(lineas[i]);
        }
        
        window.display();
    }

    return 0;
}

