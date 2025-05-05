#include <stdexcept>
#include <iostream>
#include "Variable.h"

Variable::Variable(const std::string& nombre, 
    int minimo, 
    int maximo, 
    const std::string& unidad,
    const std::vector<ConjuntoDifuso>& conjuntos)
: nombre(nombre), 
rango{minimo, maximo}, 
unidad(unidad),
conjuntosDifusos(conjuntos) 
{
//Validaciones
if (minimo >= maximo) {
throw std::invalid_argument("El mínimo debe ser menor que el máximo");
}
}

void Variable::imprimir() const {
    std::cout << "Variable: " << nombre << "\n";
    std::cout << "Rango: [" << rango.minimo << ", " << rango.maximo << "]\n";
    std::cout << "Unidad: " << unidad << "\n";
    std::cout << "Conjuntos difusos (" << conjuntosDifusos.size() << "):\n";
    
    for (const auto& conjunto : conjuntosDifusos) {
        std::cout << "  - ";
        // Assuming ConjuntoDifuso has an imprimir() method
        conjunto.imprimir(); 
    }
}