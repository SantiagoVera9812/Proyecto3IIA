#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ConjuntoDifuso.h"
#include "Variable.h"
#include "ValorReal.h"
#include "SistemaDifuso.h"

// Función para procesar archivo de entrada y crear lista de variables
std::vector<Variable> procesarArchivo(const std::string& nombreArchivo);

int main() {
    try {
        // 1. Creación del sistema difuso
        SistemaDifuso sistema;
        
        // 2. Procesamiento del archivo de definición de variables
        std::vector<Variable> variables = procesarArchivo("descripcion-variables.txt");

        // 3. Agregar cada variable al sistema difuso
        for (auto& var : variables) {
            sistema.agregarVariable(var);
        }

        // 4. Carga de reglas desde el archivo de base de conocimiento
        sistema.cargarReglasDesdeArchivo("base-de-conocimiento.txt");

        // 5. Impresión de la matriz de reglas para verificación
        sistema.imprimirMatrizReglas();
    
        // 6. Evaluación de un caso concreto
        double temp = 62; // Valor de temperatura en °C
        double pres = 4.5; // Valor de presión en bar
        
        // 7. Inferencia con explicación (devuelve salida y conjuntos activados)
        auto [salida, conjuntos] = sistema.inferirConExplicacion(temp, pres);
        
        // 8. Obtención del último intento de desfuzzificación
        auto obtener = sistema.ultimoIntentoDess(temp, pres);
        
        // 9. Obtención de las activaciones de salida
        auto activacionesSalida = sistema.inferirActivaciones(temp, pres);
        
        // 10. Impresión de resultados básicos
        std::cout << "Salida: " << salida 
                  << " generada por los conjuntos: "
                  << conjuntos.first << " y " 
                  << conjuntos.second << std::endl;

        // 11. Inferencia con explicación completa (más detallada)
        auto resultado = sistema.inferirConExplicacionCompleta(temp, pres);

        // 12. Impresión detallada de los resultados
        std::cout << "Salida lingüística: " << resultado.salidaLinguistica << "\n"
                  << "Conjunto Variable 1: " << resultado.conjuntoVar1 
                  << " (Pertenencia: " << resultado.pertenenciaVar1 << ")\n"
                  << "Conjunto Variable 2: " << resultado.conjuntoVar2 
                  << " (Pertenencia: " << resultado.pertenenciaVar2 << ")\n"
                  << "Activación final: " << resultado.activacionFinal << std::endl;

        // 13. Desfuzzificación por método del centroide
        double salidaDefusificada = sistema.defusificarPorCentroide(resultado);
        std::cout << "Salida defusificada (centroide): " << salidaDefusificada << std::endl;

        // 14. Desfuzzificación alternativa
        double respuesta = sistema.desfuzzificar(activacionesSalida);
        std::cout << "Salida defusificada (centroide) otro metodo: " << respuesta << std::endl;
    }
    catch (const std::exception& e) {
        // Manejo de errores
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Implementación de la función para procesar archivo de variables
std::vector<Variable> procesarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::vector<Variable> variables;
    
    // Verificación de apertura correcta del archivo
    if (!archivo.is_open()) {
        throw std::runtime_error("Error al abrir el archivo: " + nombreArchivo);
    }

    Variable* variableActual = nullptr;
    std::string linea;

    // Procesamiento línea por línea del archivo
    while (getline(archivo, linea)) {
        // Limpieza de espacios en blanco
        linea.erase(0, linea.find_first_not_of(" \t"));
        linea.erase(linea.find_last_not_of(" \t") + 1);

        if (linea.empty()) continue;

        // Detección de nueva variable
        if (linea == "[Variable]") {
            // Si ya hay una variable en procesamiento, la guardamos
            if (variableActual) {
                variables.push_back(*variableActual);
                delete variableActual;
            }
            variableActual = new Variable();
        }
        // Procesamiento de nombre (puede ser de variable o conjunto difuso)
        else if (linea.find("Nombre: ") == 0 && variableActual) {
            std::string nombre = linea.substr(8);
            if (variableActual->obtenerNombre().empty()) {
                // Es el nombre de la variable principal
                variableActual->establecerNombre(nombre);
            }
            else {
                // Es el nombre de un conjunto difuso
                TipoDifuso tipo;
                std::vector<double> puntos;
                std::string tipoStr;

                // Lectura del tipo de conjunto difuso
                getline(archivo, linea);
                tipoStr = linea.substr(6); // "Tipo: "
                tipo = (tipoStr == "triangular") ? TipoDifuso::TRIANGULAR : TipoDifuso::TRAPEZOIDAL;

                // Lectura de los puntos de definición
                getline(archivo, linea);
                std::string puntosStr = linea.substr(8); // "Puntos: "
                std::stringstream ss(puntosStr);
                std::string token;
                while (getline(ss, token, ',')) {
                    puntos.push_back(stod(token));
                }

                // Creación y agregado del conjunto difuso a la variable
                ConjuntoDifuso conjunto(nombre, tipo, puntos);
                variableActual->insertarConjuntoDifuso(conjunto);
            }
        }
        // Procesamiento del rango de la variable
        else if (linea.find("Rango: ") == 0 && variableActual) {
            std::string rangoStr = linea.substr(7);
            std::stringstream ss(rangoStr);
            std::string token;
            std::vector<int> rango;
            while (getline(ss, token, ',')) {
                rango.push_back(stoi(token));
            }
            variableActual->establecerRango({rango[0], rango[1]});
        }
        // Procesamiento de las unidades de la variable
        else if (linea.find("Unidades: ") == 0 && variableActual) {
            variableActual->establecerUnidad(linea.substr(10));
        }
    }

    // Agregar la última variable procesada (si existe)
    if (variableActual) {
        variables.push_back(*variableActual);
        delete variableActual;
    }

    // Cierre del archivo y retorno de las variables procesadas
    archivo.close();
    return variables;
}
