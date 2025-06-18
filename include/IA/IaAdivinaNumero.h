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
    IaAdivinaNumero();
    int hacerPrediccion();
    void actualizarLimites(bool esMayor);
    void reiniciar();
    bool numeroYaIntentado(int numero) const;
};

#endif

