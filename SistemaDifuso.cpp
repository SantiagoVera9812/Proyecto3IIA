#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "SistemaDifuso.h"
#include "Variable.h"
#include <c++/10/iomanip>
#include <iomanip>
#include <cmath>

void imprimirPertenencias(const std::map<std::string, double>& pertenencias, const std::string& nombreVariable);

void SistemaDifuso::agregarVariable(const Variable& variable) {

            variablesEntrada.push_back(variable);
        }
    
// Define la matriz de reglas (ej: "Frío" × "Débil" → "PA")
void SistemaDifuso::agregarRegla(const std::string& var1, const std::string& var2, const std::string& salida) {
    matrizReglas[{var1, var2}] = salida;
        }
    
// Evalúa las entradas y devuelve la salida lingüística
std::string SistemaDifuso::inferir(double valorVar1, double valorVar2, bool metodoAND) {

    if (variablesEntrada.size() < 2) {

        std::cerr << "Error: Se necesitan 2 variables de entrada.\n";
                return "";
    }
    
    // Paso 1: Fuzzificación (calcular grados de pertenencia)
    std::map<std::string, double> pertenenciasVar1, pertenenciasVar2;
        for (const auto& conjunto : variablesEntrada[0].obtenerConjuntosDifusos()) {
            conjunto.imprimir();
            double pertenencia = conjunto.calcularGradoPertenencia(valorVar1);
            printf("%f", pertenencia);
                if (pertenencia > 0.0) pertenenciasVar1[conjunto.obtenerNombre()] = pertenencia;
            }
            for (const auto& conjunto : variablesEntrada[1].obtenerConjuntosDifusos()) {
                conjunto.imprimir();
                double pertenencia = conjunto.calcularGradoPertenencia(valorVar2);
                if (pertenencia > 0.0) pertenenciasVar2[conjunto.obtenerNombre()] = pertenencia;
            }

    imprimirPertenencias(pertenenciasVar1, variablesEntrada[0].obtenerNombre());
    imprimirPertenencias(pertenenciasVar2, variablesEntrada[1].obtenerNombre());

    
            // Paso 2: Evaluar reglas activas y agregar conclusiones
    std::map<std::string, double> activacionesSalida;
    for (const auto& [key, salida] : matrizReglas) {
                auto [conjVar1, conjVar2] = key;
        if (pertenenciasVar1.count(conjVar1) && pertenenciasVar2.count(conjVar2)) {
            double activacion = std::min(pertenenciasVar1[conjVar1], pertenenciasVar2[conjVar2]);
            activacionesSalida[salida] = std::max(activacionesSalida[salida], activacion);
            }
        }
    
    if(metodoAND){
    // Paso 3: Elegir la salida con mayor activación (método máximo AND)
    std::string salidaFinal = "ZR"; // Valor por defecto
        double maxActivacion = 0.0;
        for (const auto& [salida, activacion] : activacionesSalida) {
                if (activacion > maxActivacion) {
                    maxActivacion = activacion;
                    salidaFinal = salida;
        }
    }
    
    return salidaFinal;
    } else {

        std::string salidaFinal = "ZR"; // Valor por defecto
        double minActivacion = INFINITY;
        for (const auto& [salida, activacion] : activacionesSalida) {
                if (activacion < minActivacion) {
                    minActivacion = activacion;
                    salidaFinal = salida;
        }
    }
    
    return salidaFinal;

    }
}


void imprimirPertenencias(const std::map<std::string, double>& pertenencias, const std::string& nombreVariable) {
    std::cout << "Grados de pertenencia para " << nombreVariable << ":\n";
    for (const auto& [conjunto, grado] : pertenencias) {
        std::cout << " - " << conjunto << ": " << std::fixed << std::setprecision(2) << grado << "\n";
    }
    std::cout << std::endl;
}


void SistemaDifuso::imprimirMatrizReglas() const {
    if (variablesEntrada.size() < 2) {
        std::cerr << "Error: Se necesitan 2 variables para imprimir la matriz.\n";
        return;
    }

    // Obtener nombres de conjuntos difusos de ambas variables
    const auto& conjuntosVar1 = variablesEntrada[0].obtenerConjuntosDifusos();
    const auto& conjuntosVar2 = variablesEntrada[1].obtenerConjuntosDifusos();

    // Imprimir cabecera de columnas (nombres de conjuntos de la segunda variable)
    std::cout << std::setw(10) << " "; // Espacio para la fila de cabecera
    for (const auto& conjunto2 : conjuntosVar2) {
        std::cout << std::setw(10) << conjunto2.obtenerNombre();
    }
    std::cout << "\n";

    // Imprimir cada fila (conjuntos de la primera variable vs. salidas)
    for (const auto& conjunto1 : conjuntosVar1) {
        std::cout << std::setw(10) << conjunto1.obtenerNombre();
        for (const auto& conjunto2 : conjuntosVar2) {
            auto clave = std::make_pair(conjunto1.obtenerNombre(), conjunto2.obtenerNombre());
            if (matrizReglas.count(clave)) {
                std::cout << std::setw(10) << matrizReglas.at(clave);
            } else {
                std::cout << std::setw(10) << "NaN"; // Regla no definida
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

bool SistemaDifuso::cargarReglasDesdeArchivo(const std::string& nombreArchivo){

    if (variablesEntrada.size() < 2) {
        std::cerr << "Error: Se necesitan 2 variables de entrada definidas.\n";
        return false;
    }

    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << "\n";
        return false;
    }

    // Obtener conjuntos difusos de ambas variables
    const auto& conjuntosVar1 = variablesEntrada[0].obtenerConjuntosDifusos();
    const auto& conjuntosVar2 = variablesEntrada[1].obtenerConjuntosDifusos();

    std::string linea;
    int numLinea = 0;

    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string valor;
        int numColumna = 0;

        while (ss >> valor) {
            // Verificar que tenemos suficientes conjuntos definidos
            if (numLinea >= conjuntosVar1.size() || numColumna >= conjuntosVar2.size()) {
                std::cerr << "Advertencia: El archivo tiene más reglas que conjuntos definidos. Línea " 
                          << numLinea + 1 << ", Columna " << numColumna + 1 << "\n";
                continue;
            }

            // Agregar la regla al sistema
            agregarRegla(
                conjuntosVar1[numLinea].obtenerNombre(),
                conjuntosVar2[numColumna].obtenerNombre(),
                valor
            );

            numColumna++;
        }

        // Verificar consistencia en el número de columnas

        //Primera linea obtener numero de columnas
        if (numLinea == 0) {
            columnasEsperadas = numColumna;
        //Lineas diferentes a la primera linea
        } else if (numColumna != columnasEsperadas) {
            std::cerr << "Advertencia: Número inconsistente de columnas en línea " << numLinea + 1 << "\n";
        }

        numLinea++;
    }

    std::cout << "Se cargaron " << numLinea << " filas y " << columnasEsperadas 
              << " columnas de reglas.\n";
    return true;
}