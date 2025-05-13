#include <iomanip>  // Para formateo de salida
#include <algorithm> // Para algoritmos STL
#include <stdexcept> // Para manejo de excepciones
#include <iostream>  // Para entrada/salida estándar

#include "ValorReal.h" // Incluye la definición de la clase

// Método para obtener el valor numérico almacenado
double ValorReal::getValorNumerico() const {
    return valorNumerico;
}

// Método para obtener el nombre de la variable asociada
std::string ValorReal::getVariablePerteneciente() const {
    return nombreVariableCorrespondiente;
}

// Constructor de la clase ValorReal
ValorReal::ValorReal(const std::string& nombreVariableCorrespondiente,
                    double valorNumerico,
                    std::vector<Variable>& variables)
    : nombreVariableCorrespondiente(nombreVariableCorrespondiente),
      valorNumerico(valorNumerico) {

    // Buscar la variable correspondiente en la lista
    auto it = std::find_if(variables.begin(), variables.end(),
        [this](const Variable& var) {
            return var.obtenerNombre() == this->nombreVariableCorrespondiente;
        });

    // Si no se encuentra la variable, lanzar excepción
    if (it == variables.end()) {
        throw std::runtime_error("Variable no encontrada: " + nombreVariableCorrespondiente);
    }

    // Obtener los conjuntos difusos de la variable
    const auto& conjuntos = it->obtenerConjuntosDifusos();
    
    // Calcular el grado de pertenencia para cada conjunto
    for (const auto& conjunto : conjuntos) {
        double pertenencia = conjunto.calcularGradoPertenencia(valorNumerico);
        valoresPertenecientes.emplace_back(conjunto.obtenerNombre(), pertenencia);
    }
}

// Método para imprimir la información del valor real
void ValorReal::imprimir() const {
    // Encabezado de la salida
    std::cout << "====================================\n";
    std::cout << "Valor Real\n";
    std::cout << "====================================\n";
    std::cout << "Variable: " << nombreVariableCorrespondiente << "\n";
    std::cout << "Valor numérico: " << valorNumerico << "\n\n";
    
    // Tabla de grados de pertenencia
    std::cout << "Grados de pertenencia:\n";
    std::cout << "------------------------------------\n";
    std::cout << std::left << std::setw(20) << "Conjunto Difuso" 
              << std::setw(15) << "Pertenencia" << "\n";
    std::cout << "------------------------------------\n";
    
    // Imprimir cada valor de pertenencia formateado
    for (const auto& [nombre, valor] : valoresPertenecientes) {
        std::cout << std::left << std::setw(20) << nombre 
                  << std::fixed << std::setprecision(4) 
                  << std::setw(15) << valor << "\n";
    }
    
    // Pie de la salida
    std::cout << "====================================\n";
}
