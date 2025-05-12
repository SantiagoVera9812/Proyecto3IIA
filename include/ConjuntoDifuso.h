#include <string>
#include <variant>
#include <array>
#include <vector>
#pragma once

//enum para los dos tipos de conjuntos difusos
enum class TipoDifuso {
    TRIANGULAR,  // Conjunto triangular
    TRAPEZOIDAL  // Conjunto trapezoidal
};

class ConjuntoDifuso {
    std::string nombre; // Nombre del conjunto difuso
    TipoDifuso tipo;  // Tipo de conjunto
    // Unión para almacenar puntos según el tipo
    union {
        std::array<double, 3> puntosTriangular; // 3 puntos para triangular
        std::array<double, 4> puntosTrapezoidal;// 4 puntos para trapezoidal
    };
    
    public: 
    // Constructor 
    ConjuntoDifuso(const std::string& name = "", TipoDifuso type = TipoDifuso::TRIANGULAR, const std::vector<double>& points = {0,0,0});
    //Calcular el grado de pertenencia de la variable 
    double calcularGradoPertenencia(double x) const;
    //Imprimir
    void imprimir() const;

    double activacion = 0;

    // Obtener nombre del conjunto
    std::string obtenerNombre() const;

    void setNombre(const std::string& newNombre);
  
    // Obtener puntos de definición
    std::vector<double> obtenerPuntos() const;

    ConjuntoDifuso recortar(double valor) const; 

    double calcularCentroide() const;
      
};
