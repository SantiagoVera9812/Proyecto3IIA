#include <string>
#include <variant>
#include <array>
#include <vector>
#pragma once

//enum para los dos tipos de conjuntos difusos
enum class TipoDifuso {
    TRIANGULAR,  
    TRAPEZOIDAL  
};

class ConjuntoDifuso {
    std::string nombre;
    TipoDifuso tipo;

    union {
        std::array<double, 3> puntosTriangular;
        std::array<double, 4> puntosTrapezoidal;
    };
    
    public: 
    // Constructor 
    ConjuntoDifuso(const std::string& name, TipoDifuso type, const std::vector<double>& points);
    //Calcular el grado de pertenencia de la variable 
    double calcularGradoPertenencia(double x) const;
    //Imprimir
    void imprimir() const;

    std::string obtenerNombre() const;

    std::vector<double> obtenerPuntos() const;
      

};