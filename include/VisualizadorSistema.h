#pragma once
#include "SistemaDifuso.h"
#include <iomanip>

class VisualizadorSistema {
public:
    static void mostrarSistemaCompleto(const SistemaDifuso& sistema) {
        std::cout << "\n=== SISTEMA DIFUSO ===\n";
        
        // Mostrar variables de entrada
        std::cout << "\n=== VARIABLES DE ENTRADA ===\n";
        for (const auto& variable : sistema.obtenerVariablesEntrada()) {
            variable.imprimir();
        }
        
        // Mostrar variable de salida
       /* std::cout << "\n=== VARIABLE DE SALIDA ===\n";
        sistema.obtenerVariableSalida().imprimir(); */
        
        // Mostrar matriz de reglas
        std::cout << "\n=== MATRIZ DE REGLAS ===\n";
        sistema.imprimirMatrizReglas();
    }
    
    static void mostrarResultadoInferencia(const ResultadoInferencia& resultado) {
        std::cout << "\n=== RESULTADO DE INFERENCIA ===\n";
        std::cout << "Salida lingüística: " << resultado.salidaLinguistica << "\n";
        std::cout << "Conjunto Variable 1: " << resultado.conjuntoVar1 
                  << " (Pertenencia: " << resultado.pertenenciaVar1 << ")\n";
        std::cout << "Conjunto Variable 2: " << resultado.conjuntoVar2 
                  << " (Pertenencia: " << resultado.pertenenciaVar2 << ")\n";
        std::cout << "Activación final: " << resultado.activacionFinal << "\n";
    }
};