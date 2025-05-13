#include <stdexcept>
#include <iostream>
#include "Variable.h"
#include <algorithm>

// Método para insertar un conjunto difuso con validaciones
void Variable::insertarConjuntoDifuso(const ConjuntoDifuso& conjunto) {
    // Verificar que los puntos del conjunto estén dentro del rango de la variable
    auto puntos = conjunto.obtenerPuntos();
    for (double punto : puntos) {
        if (punto < rango.minimo || punto > rango.maximo) {
            throw std::out_of_range("Los puntos del conjunto difuso están fuera del rango de la variable");
        }
    }
    
    // Verificar que no exista ya un conjunto con el mismo nombre
    for (const auto& c : conjuntosDifusos) {
        if (c.obtenerNombre() == conjunto.obtenerNombre()) {
            throw std::invalid_argument("Ya existe un conjunto difuso con ese nombre");
        }
    }
    
    // Si pasa las validaciones, insertar el conjunto
    conjuntosDifusos.push_back(conjunto);
}

// Constructor de la clase Variable
Variable::Variable(const std::string& nombre, 
                 int minimo, 
                 int maximo, 
                 const std::string& unidad,
                 const std::vector<ConjuntoDifuso>& conjuntos)
    : nombre(nombre), 
      rango{minimo, maximo}, 
      unidad(unidad) 
{
    // Validación del rango
    if (minimo >= maximo) {
        printf("Para variable (%s) Rango invalido - minimo (%d) - maximo (%d)\n", 
              nombre.c_str(), minimo, maximo);
        throw std::invalid_argument("El mínimo debe ser menor que el máximo");
    }

    // Insertar cada conjunto con sus validaciones
    for (const auto& conjunto : conjuntos) {
        insertarConjuntoDifuso(conjunto);
    }
}

// Método para imprimir los detalles de la variable
void Variable::imprimir() const {
    std::cout << "Variable: " << nombre << "\n";
    std::cout << "Rango: [" << rango.minimo << ", " << rango.maximo << "]\n";
    std::cout << "Unidad: " << unidad << "\n";
    std::cout << "Conjuntos difusos (" << conjuntosDifusos.size() << "):\n";
    
    // Imprimir cada conjunto difuso asociado
    for (const auto& conjunto : conjuntosDifusos) {
        std::cout << "  - ";
        conjunto.imprimir(); 
    }
}

// Getter para el nombre de la variable
std::string Variable::obtenerNombre() const {
    return nombre;
}

// Getter para los conjuntos difusos
std::vector<ConjuntoDifuso> Variable::obtenerConjuntosDifusos() const {
    return conjuntosDifusos;
}

// Setter para el nombre de la variable
void Variable::establecerNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

// Setter para la unidad de medida
void Variable::establecerUnidad(const std::string& nuevaUnidad) {
    unidad = nuevaUnidad;
}

// Setter para el rango de valores
void Variable::establecerRango(const Range& nuevoRango) {
    rango = nuevoRango;
}

// Método para obtener un conjunto difuso por su nombre
ConjuntoDifuso& Variable::obtenerConjuntoPorNombre(const std::string& nombreConjunto) {
    // Buscar el conjunto usando algoritmo STL
    auto it = std::find_if(conjuntosDifusos.begin(), conjuntosDifusos.end(),
        [&nombreConjunto](const ConjuntoDifuso& conjunto) {
            return conjunto.obtenerNombre() == nombreConjunto;
        });

    // Si no se encuentra, lanzar excepción
    if (it == conjuntosDifusos.end()) {
        throw std::runtime_error("Conjunto '" + nombreConjunto + 
                               "' no encontrado en variable '" + nombre + "'");
    }
    
    // Retornar referencia al conjunto encontrado
    return *it;
}
