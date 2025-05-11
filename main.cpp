#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ConjuntoDifuso.h"
#include "Variable.h"
#include "ValorReal.h"
#include "SistemaDifuso.h"
// Procesar archivo de entrada y crear lista de variables
std::vector<Variable> procesarArchivo(const std::string& nombreArchivo);

int main() {

    try {
        // Crear sistema difuso
        SistemaDifuso sistema;
           // Procesar archivo de definición de variables
        std::vector<Variable> variables = procesarArchivo("descripcion-variables.txt");
      // Añadir variables al sistema
         for (auto& var : variables) {
            sistema.agregarVariable(var);
        }
        // Cargar reglas desde base de conocimiento
        sistema.cargarReglasDesdeArchivo("base-de-conocimiento.txt");
// Imprimir matriz de reglas para verificación
        sistema.imprimirMatrizReglas();
    
        // Evaluar un caso de la vida real
        double temp = 18; // °C
        double pres = 2;  // bar
        std::string salida = sistema.inferir(temp, pres, false);
    // Imprimir resultado de inferencia
        std::cout << "Para Temperatura = " << temp << "°C y Presión = " << pres << " bar, la salida es: " << salida << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


//Procesar archivo y crear lista de variables
std::vector<Variable> procesarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::vector<Variable> variables;
    
    if (!archivo.is_open()) {
        throw std::runtime_error("Error al abrir el archivo: " + nombreArchivo);
    }

    Variable* variableActual = nullptr;
    std::string linea;

    while (getline(archivo, linea)) {
        // Eliminar espacios en blanco al inicio y final
        linea.erase(0, linea.find_first_not_of(" \t"));
        linea.erase(linea.find_last_not_of(" \t") + 1);

        if (linea.empty()) continue;

        if (linea == "[Variable]") {
            // Nueva variable, 
            if (variableActual) {
                variables.push_back(*variableActual);
                delete variableActual;
            }
            variableActual = new Variable();
        }
        else if (linea.find("Nombre: ") == 0 && variableActual) {
            std::string nombre = linea.substr(8);
            if (variableActual->obtenerNombre().empty()) {
                // Es el nombre de la variable
                variableActual->establecerNombre(nombre);
            }
            else {
                // Es el nombre de un conjunto difuso
                TipoDifuso tipo;
                std::vector<double> puntos;
                std::string tipoStr;

                // Leer tipo
                getline(archivo, linea);
                tipoStr = linea.substr(6); // "Tipo: "
                tipo = (tipoStr == "triangular") ? TipoDifuso::TRIANGULAR : TipoDifuso::TRAPEZOIDAL;

                // Leer puntos
                getline(archivo, linea);
                std::string puntosStr = linea.substr(8); // "Puntos: "
                std::stringstream ss(puntosStr);
                std::string token;
                while (getline(ss, token, ',')) {
                    puntos.push_back(stod(token));
                }

                // Crear y agregar el conjunto difuso
                ConjuntoDifuso conjunto(nombre, tipo, puntos);
                variableActual->insertarConjuntoDifuso(conjunto);
            }
        }
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
        else if (linea.find("Unidades: ") == 0 && variableActual) {
            variableActual->establecerUnidad(linea.substr(10));
        }
    }

    // Agregar la última variable procesada
    if (variableActual) {
        variables.push_back(*variableActual);
        delete variableActual;
    }

    archivo.close();
    return variables;
}
