#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class PortadaJuego {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titulo;
    sf::Text botonJugar;
    sf::Text botonSalir;
    sf::RectangleShape rectJugar;
    sf::RectangleShape rectSalir;
    
    int botonSeleccionado; // 0 = Jugar, 1 = Salir
    bool fontCargada;

    // Colores
    sf::Color colorFondo;
    sf::Color colorTitulo;
    sf::Color colorBoton;
    sf::Color colorBotonHover;
    sf::Color colorTexto;

public:
    PortadaJuego() : 
        window(sf::VideoMode(800, 600), "Mendewing"),
        botonSeleccionado(0),
        fontCargada(false),
        colorFondo(20, 25, 40),
        colorTitulo(255, 220, 100),
        colorBoton(60, 80, 120),
        colorBotonHover(80, 100, 150),
        colorTexto(255, 255, 255) {
        
        inicializar();
    }

    void inicializar() {
        // Intentar cargar fuente del sistema
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf") && 
            !font.loadFromFile("/System/Library/Fonts/Arial.ttf") &&
            !font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
            std::cout << "Usando fuente por defecto." << std::endl;
            fontCargada = false;
        } else {
            fontCargada = true;
        }

        // Configurar título "Mendewing"
        titulo.setString("MENDEWING");
        if (fontCargada) {
            titulo.setFont(font);
        }
        titulo.setCharacterSize(72);
        titulo.setFillColor(colorTitulo);
        titulo.setStyle(sf::Text::Bold);
        
        // Centrar título
        sf::FloatRect tituloRect = titulo.getLocalBounds();
        titulo.setOrigin(tituloRect.width / 2.0f, tituloRect.height / 2.0f);
        titulo.setPosition(400, 200);

        // Configurar botón JUGAR
        botonJugar.setString("JUGAR");
        if (fontCargada) {
            botonJugar.setFont(font);
        }
        botonJugar.setCharacterSize(32);
        botonJugar.setFillColor(colorTexto);
        
        // Rectángulo del botón Jugar
        rectJugar.setSize(sf::Vector2f(200, 60));
        rectJugar.setFillColor(colorBoton);
        rectJugar.setPosition(300, 350);
        
        // Centrar texto en botón Jugar
        sf::FloatRect jugarRect = botonJugar.getLocalBounds();
        botonJugar.setOrigin(jugarRect.width / 2.0f, jugarRect.height / 2.0f);
        botonJugar.setPosition(400, 380);

        // Configurar botón SALIR
        botonSalir.setString("SALIR");
        if (fontCargada) {
            botonSalir.setFont(font);
        }
        botonSalir.setCharacterSize(32);
        botonSalir.setFillColor(colorTexto);
        
        // Rectángulo del botón Salir
        rectSalir.setSize(sf::Vector2f(200, 60));
        rectSalir.setFillColor(colorBoton);
        rectSalir.setPosition(300, 450);
        
        // Centrar texto en botón Salir
        sf::FloatRect salirRect = botonSalir.getLocalBounds();
        botonSalir.setOrigin(salirRect.width / 2.0f, salirRect.height / 2.0f);
        botonSalir.setPosition(400, 480);

        window.setFramerateLimit(60);
    }

    void manejarEventos() {
        sf::Event evento;
        while (window.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                window.close();
            }
            
            // Navegación con teclado
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Key::Up || evento.key.code == sf::Key::Down) {
                    botonSeleccionado = (botonSeleccionado == 0) ? 1 : 0;
                }
                else if (evento.key.code == sf::Key::Enter || evento.key.code == sf::Key::Space) {
                    ejecutarAccion();
                }
                else if (evento.key.code == sf::Key::Escape) {
                    window.close();
                }
            }
            
            // Navegación con mouse
            if (evento.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (rectJugar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    botonSeleccionado = 0;
                }
                else if (rectSalir.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    botonSeleccionado = 1;
                }
            }
            
            // Click del mouse
            if (evento.type == sf::Event::MouseButtonPressed) {
                if (evento.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                    if (rectJugar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        botonSeleccionado = 0;
                        ejecutarAccion();
                    }
                    else if (rectSalir.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        botonSeleccionado = 1;
                        ejecutarAccion();
                    }
                }
            }
        }
    }

    void actualizar() {
        // Actualizar apariencia de los botones según selección
        if (botonSeleccionado == 0) {
            rectJugar.setFillColor(colorBotonHover);
            rectSalir.setFillColor(colorBoton);
        } else {
            rectJugar.setFillColor(colorBoton);
            rectSalir.setFillColor(colorBotonHover);
        }
    }

    void dibujar() {
        window.clear(colorFondo);
        
        // Dibujar título
        window.draw(titulo);
        
        // Dibujar botones
        window.draw(rectJugar);
        window.draw(botonJugar);
        
        window.draw(rectSalir);
        window.draw(botonSalir);
        
        window.display();
    }

    void ejecutarAccion() {
        if (botonSeleccionado == 0) {
            std::cout << "¡Iniciando juego!" << std::endl;
            // Aquí iría la lógica para iniciar el juego
        } else {
            std::cout << "Saliendo del juego..." << std::endl;
            window.close();
        }
    }

    void ejecutar() {
        while (window.isOpen()) {
            manejarEventos();
            actualizar();
            dibujar();
        }
    }
};

int main() {
    try {
        PortadaJuego portada;
        portada.ejecutar();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}