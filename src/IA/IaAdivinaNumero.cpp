#include "IA/IaAdivinaNumero.h"
#include <algorithm>

IaAdivinaNumero::IaAdivinaNumero() {
    reiniciar();
}

void IaAdivinaNumero::reiniciar() {
    min = 1;
    max = 100;
    intentosAnteriores.clear();
}

int IaAdivinaNumero::hacerPrediccion() {
    // Implementar búsqueda binaria
    int prediccion = min + (max - min) / 2;
    
    // Si la predicción ya fue intentada, ajustar ligeramente
    while (numeroYaIntentado(prediccion)) {
        prediccion++;
        if (prediccion > max) {
            prediccion = min;
        }
    }
    
    intentosAnteriores.push_back(prediccion);
    return prediccion;
}

void IaAdivinaNumero::actualizarLimites(bool esMayor) {
    if (intentosAnteriores.empty()) {
        return;
    }

    int ultimaPrediccion = intentosAnteriores.back();
    
    if (esMayor) {
        // El número a adivinar es mayor que nuestra predicción
        min = ultimaPrediccion + 1;
    } else {
        // El número a adivinar es menor que nuestra predicción
        max = ultimaPrediccion - 1;
    }
}

bool IaAdivinaNumero::numeroYaIntentado(int numero) const {
    return std::find(intentosAnteriores.begin(), intentosAnteriores.end(), numero) 
           != intentosAnteriores.end();
}

