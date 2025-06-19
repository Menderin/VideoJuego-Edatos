# VideoJuego-Edatos
Integrantes: 
Víctor Jopia Santander; 21.719.697-3; ITI
Diego Contreras Valenzuela; 21.614.196-2; ITI 

Linea de compilacion con la ultima IA

g++ -Iinclude src/GUI/gui2.cpp src/Nodo.cpp src/Tablero.cpp src/MiniJuegos/AdivinaNumero/AdivinaNumeroV2.cpp src/MiniJuegos/BatallaDeCartas/BatallaDeCartas.cpp src/MiniJuegos/Hex/Hex.cpp src/IA/IAHex2.cpp src/IA/IaAdivinaNumero.cpp src/IA/IaBatallaCartas.cpp -o bin/testTableroNodoIa.exe -I"lib/SFML-3.0.0/include" -L"lib/SFML-3.0.0/lib" -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lvorbisenc -lvorbisfile -lvorbis -logg -lFLAC -lopengl32 -lfreetype -lwinmm -lgdi32 -DSFML_STATIC

Linea de Ejecucion

.\bin\testTableroNodoIa.exe  

-----------------------------------------------------------------


#Descripcion del juego

El juego consiste en un tablero de 3 en raya, con la particularidad de que cada celda del tablero es un minijuego por separado, se implementaran los siguientes minijuegos de manera aleatoria en el tablero:

- Hex: Consiste en lograr conectar los extremos opuestos de cada jugador en un tablero de 11x11, es un juego por turnos.

- Adivinar el nunero: ambos jugadores eligen un numero secreto del 1 al 100, luego por turnos cada jugador debe intentar adivinar el numero del otro, en caso de fallar se  daran pistas para que cierte (por ejemplo: "El numero del jugador 2 es mayor").

- Batalla de Cartas: Cada jugador recibe una mano aleatorio de 5 cartas de un mazo personal (cartas con valores del 1 al 15), se juegan 5 rondas en las que por turnos cada jugador debera elegir la carta queestime mas conveniente para dicha ronda, luego se comparan y gana la ronda quien haya jugado la carta mas alta.

Cuando se ganen dichos minijuegos, el ganador colocara su ficha en el tablero principal, la idea es que gane 3 minijuegos en raya para ganar el juego principal, en caso de que ningun jugador pueda realizar un 3 en raya, ganara el jugador que tenga mas fichas colocadas en el tablero.

Se utilizo SFML-3.0.0 para la creacion de la GUI


Implementacion de estructuras de datos

Se utilizaron nodos para construir el tablero principal el cual es una matriz de nodos de 3x3, el nodo contiene un puntero al minijuego que se le asigno, su fila y columna y su estado.

Se utilizaron vectores para almacenar los valores de las cartas de los jugadores(vector<int>mazoJ1 y vector<int>mazoJ2) o para almacenar objetos de la gui como por ejemplo vector<sf::text> cartasTexto.

Se utilizaron arrays para almacenar cosas como las fichas o simbolos del tablero.

Se utilizaron grafos para el tablero y la funcionalidad de hex,  donde cada casilla es un nodo y las conexiones entre nodos representan los movimientos válidos en el juego. La búsqueda en profundidad (DFS) se utiliza para verificar si un jugador ha ganado al conectar sus lados del tablero.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Documentacion de codigo
-------------------------------

gui2.cpp

El loop principal del juego maneja la lógica de interacción con el usuario, incluyendo la actualizacion de la gui, la detección de clicks en el tablero y la ejecución de los minijuegos correspondientes.

Atributos

    currentState: Representa el estado actual del juego (menú, juego).
    tablero: Instancia del tablero de juego.
    ventana: Ventana principal del juego.
    fuente: Fuente utilizada para el texto en el juego.
    musicaFondo: Música de fondo del juego.

Métodos

Constructor

    main(): Inicializa la ventana del juego, carga los recursos necesarios (imágenes, música, fuentes), y configura el estado inicial del juego.

Métodos de Inicialización

    inicializarMinijuegosAleatorios(Tablero& tablero): Asigna minijuegos aleatorios a cada casilla del tablero.
    obtenerSimboloMinijuego(TipoMiniJuego tipo): Retorna el símbolo correspondiente al tipo de minijuego.

Métodos de Juego

    mostrarVentanaOpciones(): Muestra una ventana de opciones al final del juego para volver a jugar o salir.
    mostrarVentanaGanador(int puntosJ1, int puntosJ2): Muestra la ventana del ganador en el minijuego de Batalla de Cartas.
    mostrarVentanaVictoriaTablero(char simboloGanador): Muestra la ventana de victoria en el tablero principal.
    mostrarVentanaVictoriaFichas(char simboloGanador): Muestra la ventana de victoria por fichas en el tablero principal.
    mostrarVentanaVictoria(int jugadorGanador, int numeroSecreto, Tablero& tablero): Muestra la ventana de victoria en el minijuego de Adivina el Número.
    mostrarVentanaAyuda(): Muestra la ventana de ayuda del juego.
    mostrarVentanaAyudaHex(): Muestra la ventana de ayuda específica para el juego Hex.
    mostrarVentanaAyudaAdivinaNumero(): Muestra la ventana de ayuda específica para el juego Adivina el Número.
    mostrarVentanaAyudaBatallaCartas(): Muestra la ventana de ayuda específica para el juego Batalla de Cartas.

Métodos de Minijuegos

    abrirHex(int casilla, Tablero& tablero, sf::Music& musicaFondo): Abre la ventana del minijuego Hex.
    abrirAdivinaNumero(int casilla, Tablero& tablero, sf::Music& musicaFondo): Abre la ventana del minijuego Adivina el Número.
    abrirBatallaCartas(int casilla, Tablero& tablero, sf::Music& musicaFondo): Abre la ventana del minijuego Batalla de Cartas.

Métodos de Utilidad

    ventanaJugador(bool esJugador1, std::vector<int>& valoresCartas, int& cartaSeleccionada, int puntosJ1, int puntosJ2): Muestra la ventana de selección de cartas para un jugador en el minijuego Batalla de Cartas.
    crearHexagono(float radio, sf::Vector2f posicion): Crea un hexágono para el minijuego Hex.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------

Nodo.cpp

Atributos

    estado: Representa el estado actual del nodo (vacío, jugador 1, jugador 2).
    fila: Fila del nodo en el tablero.
    columna: Columna del nodo en el tablero.
    activo: Indica si el nodo está activo.
    miniJuego: Puntero al minijuego asociado al nodo.
    tipoMiniJuego: Tipo de minijuego asignado al nodo.
    tieneMiniJuego: Indica si el nodo tiene un minijuego asignado.

Constructores

    Nodo(): Constructor por defecto que inicializa un nodo vacío en una posición no válida.
    Nodo(int f, int c): Constructor que inicializa un nodo vacío en la posición especificada por f (fila) y c (columna).

Getters

    getEstado(): Retorna el estado actual del nodo.
    getFila(): Retorna la fila del nodo.
    getColumna(): Retorna la columna del nodo.
    estaActivo(): Retorna si el nodo está activo.
    estaVacio(): Retorna si el nodo está vacío.
    tieneMiniJuegoAsignado(): Retorna si el nodo tiene un minijuego asignado.
    getTipoMiniJuego(): Retorna el tipo de minijuego asignado al nodo.
    getMiniJuego(): Retorna un puntero al minijuego asociado al nodo.

Setters

    setEstado(EstadoNodo nuevoEstado): Establece el estado del nodo.
    setPosicion(int f, int c): Establece la posición del nodo.
    setActivo(bool nuevoActivo): Establece si el nodo está activo.

Métodos de Juego

    ocuparNodo(EstadoNodo jugador): Intenta ocupar el nodo con el jugador especificado. Si el nodo tiene un minijuego, lo juega primero.
    reiniciar(): Reinicia el nodo, estableciendo su estado a vacío y reactivándolo. También reinicia el minijuego si existe.
    asignarMiniJuego(TipoMiniJuego tipo): Asigna un minijuego al nodo basado en el tipo especificado.
    jugarMiniJuego(): Juega el minijuego asociado al nodo y retorna el estado resultante.

Utilidades

    mostrarInfo(): Muestra información sobre el nodo, incluyendo su estado, posición, y tipo de minijuego si lo tiene.

Funciones Auxiliares

    limpiarBuffer(): Limpia el buffer de entrada estándar para manejar entradas inválidas.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------
Tablero.cpp

Atributos

    turnoActual: Indica el jugador actual (1 o 2).
    estadoJuego: Representa el estado actual del juego (en curso, ganador jugador 1, ganador jugador 2, empate).
    fichasJ1: Contador de fichas del jugador 1.
    fichasJ2: Contador de fichas del jugador 2.
    matrizNodos: Matriz 3x3 de objetos Nodo que representan el tablero.

Métodos

Constructor

    Tablero(): Inicializa el tablero, estableciendo el turno actual a 1, el estado del juego a "en curso", y los contadores de fichas a 0. También inicializa la matriz de nodos.

Métodos de Inicialización

    inicializarMatriz(): Inicializa la matriz de nodos con objetos Nodo.
    reiniciar(): Reinicia el tablero, limpiando todos los nodos y restableciendo el turno y el estado del juego.

Métodos de Juego

    jugarNodo(int fila, int columna): Intenta jugar en el nodo especificado por fila y columna. Retorna true si el movimiento es válido y se realiza con éxito.
    verificarVictoria(): Verifica si hay un ganador o si el juego ha terminado en empate.
    cambiarTurno(): Cambia el turno al siguiente jugador.
    Métodos Privados para Verificación de Victoria
    verificarFilas(): Verifica si hay una línea completa en alguna fila.
    verificarColumnas(): Verifica si hay una línea completa en alguna columna.
    verificarDiagonales(): Verifica si hay una línea completa en alguna diagonal.
    tableroLleno(): Verifica si el tablero está completamente lleno.
    determinarGanadorPorFichas(): Determina el ganador basado en el número de fichas si el tablero está lleno y no hay líneas completas.

Getters

    getNodo(int fila, int columna): Retorna el nodo en la posición especificada.
    getTurnoActual(): Retorna el turno actual.
    getEstadoJuego(): Retorna el estado actual del juego.
    getFichasJ1(): Retorna el número de fichas del jugador 1.
    getFichasJ2(): Retorna el número de fichas del jugador 2.

Utilidades

    mostrarTablero(): Muestra el estado actual del tablero en la consola.
    mostrarEstadoJuego(): Muestra el estado actual del juego en la consola.
    esMovimientoValido(int fila, int columna): Verifica si un movimiento es válido.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------
Minijuego.h

La clase MiniJuego es una clase abstracta que define la interfaz para los minijuegos en el sistema.

Atributos

    estado: Representa el estado actual del minijuego (en progreso, ganador jugador 1, ganador jugador 2).
    juegoTerminado: Indica si el juego ha terminado.

Constructor

    MiniJuego(): Inicializa el estado del juego a "en progreso" y establece que el juego no ha terminado.

Métodos Virtuales Puros

    procesarMovimiento(int jugador, int input): Procesa un movimiento de un jugador. Debe ser implementado por las clases derivadas.
    mostrarEstado(): Muestra el estado actual del juego. Debe ser implementado por las clases derivadas.
    reiniciar(): Reinicia el juego. Debe ser implementado por las clases derivadas.

Métodos Comunes

    getEstado(): Retorna el estado actual del minijuego.
    estaTerminado(): Retorna si el juego ha terminado.

Métodos Protegidos

    terminarJuego(EstadoMiniJuego ganador): Establece el estado del juego a terminado y define el ganador.


----------------------------------------------------------------------------------------------------------------------------------------------------------------------


AdivinaNumeroV2.cpp (version utilizada para la GUI)

Atributos

    numeroJugador1: Número secreto del jugador 1.
    numeroJugador2: Número secreto del jugador 2.
    numeroJ1Elegido: Indica si el jugador 1 ha elegido su número secreto.
    numeroJ2Elegido: Indica si el jugador 2 ha elegido su número secreto.
    turnoActual: Indica el turno actual (1 para jugador 1, 2 para jugador 2).
    generador: Generador de números aleatorios.
    intentosJugador1: Lista de intentos del jugador 1.
    intentosJugador2: Lista de intentos del jugador 2.
    estado: Estado actual del minijuego (en progreso, ganador jugador 1, ganador jugador 2).
    juegoTerminado: Indica si el juego ha terminado.

Constructor

    AdivinaNumero(): Inicializa el juego, estableciendo los números secretos a -1, indicando que no han sido elegidos, y el turno actual a 1.

Métodos

Configuración y Validación

    establecerNumeroSecreto(int jugador, int numero): Establece el número secreto para el jugador especificado si es válido.
    validarNumero(int numero): Valida que el número esté dentro del rango permitido (1-100).
    ambosNumerosElegidos(): Retorna si ambos jugadores han elegido sus números secretos.

Procesamiento del Juego

    procesarMovimiento(int jugador, int numeroAdivinado): Procesa un intento de adivinar el número secreto del otro jugador. Retorna true si el jugador adivina correctamente.
    getNumeroJugador(int jugador): Retorna el número secreto del jugador especificado.
    setNumeroJugador(int jugador, int numero): Establece el número secreto para el jugador especificado.

Estado del Juego

    getIntentosJugador(int jugador): Retorna los intentos del jugador especificado.
    mostrarEstado(): Muestra el estado actual del juego, incluyendo los números secretos si el juego ha terminado.
    reiniciar(): Reinicia el juego, restableciendo los números secretos, los intentos, y el estado del juego.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------

BatallaDeCartas.cpp 

Atributos

    rondasGanadasJ1: Número de rondas ganadas por el jugador 1.
    rondasGanadasJ2: Número de rondas ganadas por el jugador 2.
    totalRondas: Número total de rondas en el juego.
    rng: Generador de números aleatorios.
    mazoJugador1: Mazos de cartas del jugador 1.
    mazoJugador2: Mazos de cartas del jugador 2.

Constructor

    BatallaDeCartas(): Inicializa el juego, estableciendo las rondas ganadas a 0 y el número total de rondas a 5. También inicializa los mazos de cartas para ambos jugadores.
Métodos

Inicialización

    inicializarMazos(): Inicializa y mezcla los mazos de cartas para ambos jugadores.

Visualización

    mostrarTitulo(): Muestra el título y las reglas del juego.
    mostrarMano(const std::vector<int>& mano, const std::string& jugador): Muestra la mano de cartas de un jugador.
    mostrarResultadoRonda(int cartaJ1, int cartaJ2, int ronda): Muestra el resultado de una ronda.
    mostrarResultadoFinal(): Muestra el resultado final del juego.
    mostrarEstado(): Muestra el estado actual del juego, incluyendo las rondas ganadas y las cartas restantes.

Procesamiento del Juego

    seleccionarCarta(std::vector<int>& mano, const std::string& jugador): Permite a un jugador seleccionar una carta de su mano.
    procesarMovimiento(int jugador, int input): Procesa un movimiento de un jugador. Retorna true si el movimiento es válido.
    procesarRonda(int cartaJ1, int cartaJ2, int numeroRonda): Procesa una ronda del juego, eliminando las cartas jugadas de los mazos y mostrando el resultado.
    jugar(): Ejecuta el juego completo, incluyendo la selección de cartas y la determinación del ganador.

Reinicio

    reiniciar(): Reinicia el juego, restableciendo las rondas ganadas y los mazos de cartas.
    reiniciarJuego(): Reinicia el juego utilizando el método reiniciar.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------

Hex.cpp

Atributos

    jugadorActual: Indica el jugador actual (1 o 2).
    juegoTerminado: Indica si el juego ha terminado.
    ganador: Indica el ganador del juego (1 o 2).
    primerMovimiento: Indica si es el primer movimiento del juego.
    puedeRobar: Indica si el jugador 2 puede robar la ficha del jugador 1.
    ultimaFila: Fila de la última ficha colocada.
    ultimaColumna: Columna de la última ficha colocada.
    grafo: Instancia de GrafoHex que gestiona el tablero y las conexiones.

Constructor

    Hex(): Inicializa el juego, estableciendo el jugador actual a 1, indicando que el juego no ha terminado, y que no hay ganador. También inicializa las variables relacionadas con la regla del robo.

Métodos

Procesamiento del Juego

    procesarMovimiento(int jugador, int input): Procesa un movimiento de un jugador. Retorna true si el movimiento es válido.
    hacerMovimiento(int fila, int col): Realiza un movimiento en el tablero. Retorna true si el movimiento es válido.
    aplicarReglaRobo(): Aplica la regla del robo, permitiendo al jugador 2 robar la ficha del jugador 1. Retorna true si la regla se aplica correctamente.
    cambiarTurno(): Cambia el turno al siguiente jugador.

Visualización

    mostrarEstado(): Muestra el estado actual del juego, incluyendo el tablero, el jugador actual, y el estado del juego.
    mostrarTablero(): Muestra el tablero del juego.
    mostrarInstrucciones(): Muestra las instrucciones del juego.
    obtenerSimbolo(EstadoCasilla estado): Retorna el símbolo correspondiente al estado de una casilla.

Reinicio

    reiniciar(): Reinicia el juego, restableciendo el tablero, el jugador actual, y el estado del juego.
    Juego
    jugar(): Ejecuta el juego completo, incluyendo la selección de movimientos y la determinación del ganador.

Getters

    getGanador(): Retorna el ganador del juego.
    estaTerminado(): Retorna si el juego ha terminado.
    getCasilla(int fila, int col): Retorna el estado de una casilla en el tablero.
    getJugadorActual(): Retorna el jugador actual.
    getPuedeRobar(): Retorna si el jugador 2 puede robar la ficha del jugador 1.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------

JuegoPorConsola.cpp

Este archivo implementa una versión del juego que funciona por consola sin la implementacion de sfml.

Funciones

Visualización

    mostrarMenu(): Muestra el menú principal del juego.
    mostrarInstrucciones(): Muestra las instrucciones del juego, incluyendo cómo jugar y las reglas.
    mostrarTableroConMinijuegos(const Tablero& tablero): Muestra el estado actual del tablero, incluyendo los minijuegos asignados a cada casilla.

Inicialización

    inicializarMinijuegosAleatorios(Tablero& tablero): Asigna minijuegos aleatorios a cada casilla del tablero. Los minijuegos disponibles son "Adivina el Número", "Hex", y "Batalla de Cartas".

Función Principal

    main(): Función principal que gestiona el flujo del juego. Inicializa el tablero, muestra el menú y las instrucciones, y permite a los jugadores realizar movimientos hasta que el juego termina.