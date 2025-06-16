#ifndef IA_ADIVINA_NUMERO_H
#define IA_ADIVINA_NUMERO_H

#include "MiniJuegos/AdivinaNumero/AdivinaNumero.h"
#include <vector>

class IaAdivinaNumero {
private:
    int min;            // Límite inferior del rango de búsqueda
    int max;            // Límite superior del rango de búsqueda
    std::vector<int> intentosAnteriores; // Historial de intentos

public:
    // Constructor que inicializa los límites de búsqueda
    IaAdivinaNumero();

    // Método principal para hacer una predicción
    int hacerPrediccion();

    // Método para actualizar los límites basado en la respuesta
    void actualizarLimites(bool esMayor);

    // Método para reiniciar la IA
    void reiniciar();

    // Método para verificar si un número ya fue intentado
    bool numeroYaIntentado(int numero) const;
};

#endif // IA_ADIVINA_NUMERO_H

