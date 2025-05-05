#include <stdexcept>
#include <iostream>
#include "Variable.h"

//insertar un ConjuntoDisuso con sus validaciones
void Variable::insertarConjuntoDifuso(const ConjuntoDifuso& conjunto){

    //Verificar que el conjunto insertado este dentro del rango
    auto puntos = conjunto.obtenerPuntos();
    for (double punto : puntos) {
        if (punto < rango.minimo || punto > rango.maximo) {
            throw std::out_of_range("Los puntos del conjunto difuso están fuera del rango de la variable");
        }
    }
    
    //Verificar por duplicados
    for (const auto& c : conjuntosDifusos) {
        if (c.obtenerNombre() == conjunto.obtenerNombre()) {
            throw std::invalid_argument("Ya existe un conjunto difuso con ese nombre");
        }
    }
    //Insertar conjuntos
    conjuntosDifusos.push_back(conjunto);

}

Variable::Variable(const std::string& nombre, 
    int minimo, 
    int maximo, 
    const std::string& unidad,
    const std::vector<ConjuntoDifuso>& conjuntos)
: nombre(nombre), 
rango{minimo, maximo}, 
unidad(unidad)
{
//Validaciones
if (minimo >= maximo) {
    printf("Para variable (%s) Rango invalido - minimo (%d) - maximo (%d)\n", nombre.c_str(), minimo, maximo);
throw std::invalid_argument("El mínimo debe ser menor que el máximo");
}

//Realizar la validacion para insertar un conjunto
for (const auto& conjunto : conjuntos) {
    insertarConjuntoDifuso(conjunto);
}

}

//Funcion para imprimir
void Variable::imprimir() const {
    std::cout << "Variable: " << nombre << "\n";
    std::cout << "Rango: [" << rango.minimo << ", " << rango.maximo << "]\n";
    std::cout << "Unidad: " << unidad << "\n";
    std::cout << "Conjuntos difusos (" << conjuntosDifusos.size() << "):\n";
    
    for (const auto& conjunto : conjuntosDifusos) {
        std::cout << "  - ";
        
        conjunto.imprimir(); 
    }
}

std::string Variable::obtenerNombre() const{
    return nombre;
};

std::vector<ConjuntoDifuso> Variable::obtenerConjuntosDifusos() const{
    return conjuntosDifusos;
}

void Variable::establecerNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Variable::establecerUnidad(const std::string& nuevaUnidad){
    unidad = nuevaUnidad;
}

void Variable::establecerRango(const Range& nuevoRango){
    rango = nuevoRango;
}