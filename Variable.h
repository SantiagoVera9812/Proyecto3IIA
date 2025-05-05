#include <string>
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
        int maximo = 0, 
        const std::string& unidad = "",
        const std::vector<ConjuntoDifuso>& conjuntos = {});

    void imprimir() const;

        


    
};