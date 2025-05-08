#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include "Variable.h"
#pragma once

class SistemaDifuso {
    private:
        std::map<std::pair<std::string, std::string>, std::string> matrizReglas;
        std::vector<Variable> variablesEntrada;
        std::vector<std::string> salidasPosibles;
        int columnasEsperadas = 0;
    
    public:
        // Añade una variable difusa (ej: "Temperatura")
        void agregarVariable(const Variable& variable);
    
        // Define la matriz de reglas (ej: "Frío" × "Débil" → "PA")
        void agregarRegla(const std::string& var1, const std::string& var2, const std::string& salida);
    
        // Evalúa las entradas y devuelve la salida lingüística
        std::string inferir(double valorVar1, double valorVar2, bool metodoAND);

        void imprimirMatrizReglas() const;

        bool cargarReglasDesdeArchivo(const std::string& nombreArchivo);
    };