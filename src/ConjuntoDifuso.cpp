#include <string>
#include "ConjuntoDifuso.h"
#include <stdexcept>
#include <iostream>

ConjuntoDifuso::ConjuntoDifuso(const std::string& name, TipoDifuso type, const std::vector<double>& points) 
    : nombre(name), tipo(type) 
{
    if (type == TipoDifuso::TRIANGULAR) {
        if (points.size() != 3) {
            throw std::invalid_argument("Conjunto triangular requiere 3 puntos");
        }
        puntosTriangular = {points[0], points[1], points[2]};
    } 
    else {
        if (points.size() != 4) {
            throw std::invalid_argument("Conjunto trapezoidal requiere 4 puntos");
        }
        puntosTrapezoidal = {points[0], points[1], points[2], points[3]};
    }
}

double ConjuntoDifuso::calcularGradoPertenencia(double x) const{

    if (tipo == TipoDifuso::TRIANGULAR) {
        const auto& [a, b, c] = puntosTriangular;
        if (x <= a || x >= c) return 0.0;
        if (x == b) return 1.0;
        return (x < b) ? (x - a)/(b - a) : (c - x)/(c - b);
    } else {
        const auto& [a, b, c, d] = puntosTrapezoidal;
        if (x <= a || x >= d) return 0.0;
        if (x >= b && x <= c) return 1.0;
        return (x < b) ? (x - a)/(b - a) : (d - x)/(d - c);
    }
};

std::vector<double> ConjuntoDifuso::obtenerPuntos() const {
    if (tipo == TipoDifuso::TRIANGULAR) {
        return {puntosTriangular.begin(), puntosTriangular.end()};
    }
    return {puntosTrapezoidal.begin(), puntosTrapezoidal.end()};
}

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

std::string ConjuntoDifuso::obtenerNombre() const{

    return nombre;
}

double ConjuntoDifuso::calcularCentroide() const {
    const int steps = 100; // Número de pasos para la integración numérica
    double numerador = 0.0;
    double denominador = 0.0;
    
    // Determinar los límites de integración
    double min = 0.0, max = 0.0;
    std::vector<double> puntos = obtenerPuntos();
    
    if (tipo == TipoDifuso::TRIANGULAR) {
        min = puntos[0];
        max = puntos[2];
    } else { // TRAPEZOIDAL
        min = puntos[0];
        max = puntos[3];
    }
    
    // Integración numérica
    double step_size = (max - min) / steps;
    for (int i = 0; i <= steps; ++i) {
        double x = min + i * step_size;
        double mu = calcularGradoPertenencia(x);
        
        numerador += x * mu;
        denominador += mu;
    }
    
    // Evitar división por cero
    if (denominador < 1e-10) return (min + max) / 2.0;
    
    return numerador / denominador;
}

ConjuntoDifuso ConjuntoDifuso::recortar(double valor) const{
    ConjuntoDifuso resultado(*this); // Copia del conjunto original
        resultado.activacion = std::min(activacion, valor);
        return resultado;
}

void ConjuntoDifuso::setNombre(const std::string& newNombre) {
    if (newNombre.empty()) {
        throw std::invalid_argument("El nombre del conjunto difuso no puede estar vacío");
    }
    this->nombre = newNombre;
}