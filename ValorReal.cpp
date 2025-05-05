#include <iomanip> 
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "ValorReal.h"

double ValorReal::getValorNumerico() const{
    return valorNumerico;
}

std::string ValorReal::getVariablePerteneciente() const{
    return nombreVariableCorrespondiente;
}

ValorReal::ValorReal(const std::string& nombreVariableCorrespondiente, 
    double valorNumerico, 
    std::vector<Variable>& variables)
:   nombreVariableCorrespondiente(nombreVariableCorrespondiente), 
    valorNumerico(valorNumerico) {

    auto it = std::find_if(variables.begin(), variables.end(),
    [this](const Variable& var) {
    return var.obtenerNombre() == this->nombreVariableCorrespondiente;
    });

    if (it == variables.end()) {
    throw std::runtime_error("Variable no encontrada: " + nombreVariableCorrespondiente);
    }

    const auto& conjuntos = it->obtenerConjuntosDifusos();
        for (const auto& conjunto : conjuntos) {
            double pertenencia = conjunto.calcularGradoPertenencia(valorNumerico);
            valoresPertenecientes.emplace_back(conjunto.obtenerNombre(), pertenencia);
            }
        }

void ValorReal::imprimir() const {
            // imprimir informacion valor real
            std::cout << "====================================\n";
            std::cout << "Valor Real\n";
            std::cout << "====================================\n";
            std::cout << "Variable: " << nombreVariableCorrespondiente << "\n";
            std::cout << "Valor numérico: " << valorNumerico << "\n\n";
            
            // Imprimir tabla de valores de pertenencia
            std::cout << "Grados de pertenencia:\n";
            std::cout << "------------------------------------\n";
            std::cout << std::left << std::setw(20) << "Conjunto Difuso" 
                      << std::setw(15) << "Pertenencia" << "\n";
            std::cout << "------------------------------------\n";
            
            for (const auto& [nombre, valor] : valoresPertenecientes) {
                std::cout << std::left << std::setw(20) << nombre 
                          << std::fixed << std::setprecision(4) 
                          << std::setw(15) << valor << "\n";
            }
            
            std::cout << "====================================\n";
        }


