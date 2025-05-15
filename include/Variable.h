#include <string>
#include <climits> 
#include "ConjuntoDifuso.h"
#pragma once

// Rango con minimo y Maximo
struct Range {
    int minimo;
    int maximo;
};

// Variable struct using Range
class Variable {
    
    std::string nombre;
    Range rango;
    std::string unidad;
    std::vector<ConjuntoDifuso> conjuntosDifusos;
    
    
    public:

    
    Variable(const std::string& nombre = "", 
        int minimo = 0, 
        int maximo = INT_MAX,
        const std::string& unidad = "",
        const std::vector<ConjuntoDifuso>& conjuntos = {});

    void insertarConjuntoDifuso(const ConjuntoDifuso& conjunto);

    void ordenarConjuntoDifuso();

    void imprimir() const;

    std::string obtenerNombre() const;
    std::vector<ConjuntoDifuso> obtenerConjuntosDifusos() const;

    void establecerNombre(const std::string& nuevoNombre);
    void establecerUnidad(const std::string& nuevaUnidad);
    void establecerRango(const Range& nuevoRango);
    ConjuntoDifuso& obtenerConjuntoPorNombre(const std::string& nombreConjunto);

};