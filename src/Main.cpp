#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>



int main() {
    std::cout << "=== MENDEWING - JUEGO DEL GATO ===" << std::endl;
    std::cout << "Inicializando aplicación..." << std::endl;
    
    // Configuración de la ventana
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;
    const std::string WINDOW_TITLE = "Mendewing - El Gato con Minijuegos";
    
    // Crear ventana
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
        WINDOW_TITLE,
        sf::Style::Titlebar | sf::Style::Close
    );
    
    // Configurar ventana
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    
    // Crear el manejador de estados
    GameStateManager stateManager(window);
    
    // Inicializar con el menú principal
    auto menuPrincipal = std::make_unique<MenuPrincipal>(&stateManager);
    stateManager.pushState(std::move(menuPrincipal));
    
    std::cout << "Ventana creada: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
    std::cout << "Estado inicial: Menú Principal" << std::endl;
    std::cout << "¡Aplicación lista!" << std::endl;
    
    // Clock para manejar deltaTime
    sf::Clock deltaClock;
    
    // Game Loop principal
    while (window.isOpen() && !stateManager.isEmpty()) {
        // Calcular tiempo transcurrido
        float deltaTime = deltaClock.restart().asSeconds();
        
        // Procesar eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            // Eventos globales
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            
            // Eventos específicos del estado actual
            stateManager.handleEvents(event);
        }
        
        // Actualizar estado actual
        stateManager.update(deltaTime);
        
        // Renderizar
        stateManager.render();
        
        // Mostrar en pantalla
        window.display();
    }
    
    std::cout << "Cerrando aplicación..." << std::endl;
    std::cout << "¡Gracias por jugar Mendewing!" << std::endl;
    
    return 0;
}