#include <string>
#include "ConjuntoDifuso.h"
#include <stdexcept>
#include <iostream>

// Constructor: Crea un conjunto difuso con validación de parámetros
ConjuntoDifuso::ConjuntoDifuso(const std::string& name, TipoDifuso type, const std::vector<double>& points) 
    : nombre(name), tipo(type) 
{
    // Validación para conjuntos triangulares (deben tener 3 puntos)
    if (type == TipoDifuso::TRIANGULAR) {
        if (points.size() != 3) {
            throw std::invalid_argument("Conjunto triangular requiere 3 puntos");
        }
        puntosTriangular = {points[0], points[1], points[2]};
    } 
    // Validación para conjuntos trapezoidales (deben tener 4 puntos)
    else {
        if (points.size() != 4) {
            throw std::invalid_argument("Conjunto trapezoidal requiere 4 puntos");
        }
        puntosTrapezoidal = {points[0], points[1], points[2], points[3]};
    }
}

// Calcula el grado de pertenencia de un valor x al conjunto difuso
double ConjuntoDifuso::calcularGradoPertenencia(double x) const {
    // Lógica para conjuntos triangulares
    if (tipo == TipoDifuso::TRIANGULAR) {
        const auto& [a, b, c] = puntosTriangular;
        // Fuera del rango -> pertenencia 0
        if (x <= a || x >= c) return 0.0;
        // En el vértice -> pertenencia 1
        if (x == b) return 1.0;
        // Lados del triángulo
        return (x < b) ? (x - a)/(b - a) : (c - x)/(c - b);
    } 
    // Lógica para conjuntos trapezoidales
    else {
        const auto& [a, b, c, d] = puntosTrapezoidal;
        // Fuera del rango -> pertenencia 0
        if (x <= a || x >= d) return 0.0;
        // En la meseta -> pertenencia 1
        if (x >= b && x <= c) return 1.0;
        // Lados del trapecio
        return (x < b) ? (x - a)/(b - a) : (d - x)/(d - c);
    }
}

// Obtiene los puntos de definición del conjunto
std::vector<double> ConjuntoDifuso::obtenerPuntos() const {
    if (tipo == TipoDifuso::TRIANGULAR) {
        return {puntosTriangular.begin(), puntosTriangular.end()};
    }
    return {puntosTrapezoidal.begin(), puntosTrapezoidal.end()};
}

// Muestra información del conjunto por consola
void ConjuntoDifuso::imprimir() const {
    std::cout << "Conjunto: " << nombre << " (";
    std::cout << (tipo == TipoDifuso::TRIANGULAR ? "Triangular" : "Trapezoidal") << ")\n";
    
    auto pts = obtenerPuntos();
    std::cout << "Puntos: [";
    for (size_t i = 0; i < pts.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << pts[i];
    }
    std::cout << "]\n";
}

// Getter para el nombre del conjunto
std::string ConjuntoDifuso::obtenerNombre() const {
    return nombre;
}

// Calcula el centroide del conjunto (para desfuzzificación)
double ConjuntoDifuso::calcularCentroide() const {
    const int steps = 100; // Precisión de la integración numérica
    double numerador = 0.0;
    double denominador = 0.0;
    
    // Determinar los límites de integración según el tipo
    double min = 0.0, max = 0.0;
    std::vector<double> puntos = obtenerPuntos();
    
    if (tipo == TipoDifuso::TRIANGULAR) {
        min = puntos[0];
        max = puntos[2];
    } else { // TRAPEZOIDAL
        min = puntos[0];
        max = puntos[3];
    }
    
    // Integración numérica por el método del rectángulo
    double step_size = (max - min) / steps;
    for (int i = 0; i <= steps; ++i) {
        double x = min + i * step_size;
        double mu = calcularGradoPertenencia(x);
        
        numerador += x * mu;  // Sumatoria de x*μ(x)
        denominador += mu;    // Sumatoria de μ(x)
    }
    
    // Manejo de caso especial (conjunto vacío)
    if (denominador < 1e-10) return (min + max) / 2.0;
    
    return numerador / denominador;  // Centroide = ∫xμ(x)dx/∫μ(x)dx
}

// Crea una versión recortada del conjunto (para inferencia)
ConjuntoDifuso ConjuntoDifuso::recortar(double valor) const {
    ConjuntoDifuso resultado(*this); // Copia del conjunto original
    resultado.activacion = std::min(activacion, valor); // Aplica el recorte
    return resultado;
}

// Setter para el nombre con validación
void ConjuntoDifuso::setNombre(const std::string& newNombre) {
    if (newNombre.empty()) {
        throw std::invalid_argument("El nombre del conjunto difuso no puede estar vacío");
    }
    this->nombre = newNombre;
}

bool ConjuntoDifuso::esTrapezoidal() const{
    return tipo == TipoDifuso::TRAPEZOIDAL;
}

bool ConjuntoDifuso::esTriangular() const{
    return tipo == TipoDifuso::TRIANGULAR;
}


