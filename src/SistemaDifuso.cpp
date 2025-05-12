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
std::map<std::string, double> SistemaDifuso::inferirActivaciones(double valorVar1, double valorVar2) {

    std::map<std::string, double> activacionesSalida;

    if (variablesEntrada.size() < 2) {

        std::cerr << "Error: Se necesitan 2 variables de entrada.\n";
                return activacionesSalida;
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
    
    for (const auto& [key, salida] : matrizReglas) {
        //Coseguir la combinacion de parejas de ambos conjuntos difusos
                auto [conjVar1, conjVar2] = key;
        if (pertenenciasVar1.count(conjVar1) && pertenenciasVar2.count(conjVar2)) {
            //Activacion de reglas  (una regla se activa sólo hasta donde su antecedente más restrictivo lo permite)
            double activacion = std::min(pertenenciasVar1[conjVar1], pertenenciasVar2[conjVar2]);
            //combinar reglas: si varias reglas contribuyen a la misma salida, nos quedamos con la contribución más fuerte
            activacionesSalida[salida] = std::max(activacionesSalida[salida], activacion);
            }
        }

    imprimirPertenencias(activacionesSalida, "Activacion de salida" );

    return activacionesSalida;
    
   
}

ResultadoInferencia SistemaDifuso::ultimoIntentoDess(double valorVar1, double valorVar2) {
    ResultadoInferencia resultado;
    
    // 1. Obtenemos las activaciones usando la función existente
    auto activaciones = inferirActivaciones(valorVar1, valorVar2);
    
    
    
    // 3. Para cada activación, creamos un conjunto difuso resultante
    for (const auto& [nombreConjunto, gradoActivacion] : activaciones) {
        try {
            // Obtenemos el conjunto original de las variables de salida del sistema
            const auto& conjuntoOriginal = variableSalida.obtenerConjuntoPorNombre(nombreConjunto);
            
            // Creamos un nuevo conjunto recortado
            ConjuntoDifuso conjuntoResultante = conjuntoOriginal;
            conjuntoResultante.calcularGradoPertenencia(gradoActivacion);
            conjuntoResultante.setNombre(nombreConjunto);
            
            // Agregamos a la variable de salida
            variableSalida.insertarConjuntoDifuso(conjuntoResultante);
            
        } catch (const std::exception& e) {
            std::cerr << "Advertencia: " << e.what() << " - No se pudo crear conjunto resultante para '" 
                      << nombreConjunto << "'" << std::endl;
        }
    }
    
    // 4. Buscamos la salida con mayor activación (para el resultado)
    double maxActivacion = 0.0;
    for (const auto& [salida, activacion] : activaciones) {
        if (activacion > maxActivacion) {
            maxActivacion = activacion;
            resultado.salidaLinguistica = salida;
            resultado.activacionFinal = activacion;
        }
    }
    
    return resultado;
}

std::pair<std::string, std::pair<std::string, std::string>> 
SistemaDifuso::inferirConExplicacion(double valorVar1, double valorVar2) {
    auto activaciones = inferirActivaciones(valorVar1, valorVar2);
    std::string mejorSalida;
    std::pair<std::string, std::string> conjuntos;
    double maxActivacion = 0.0;


    
    for (const auto& [salida, activacion] : activaciones) {
        if (activacion > maxActivacion) {
            maxActivacion = activacion;
            mejorSalida = salida;
            // Buscar los conjuntos que generaron esta activación
            for (const auto& [key, reglaSalida] : matrizReglas) {
                if (reglaSalida == salida) {
                    conjuntos = key;
                    break;
                }
            }
        }
    }
    
    return {mejorSalida, conjuntos};
}


std::string SistemaDifuso::inferirSalidaLinguistica(double valorVar1, double valorVar2, bool metodoAND) {

    auto activacionesSalida = inferirActivaciones(valorVar1, valorVar2);
    
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

std::tuple<std::map<std::string, double>, 
           std::map<std::string, double>, 
           std::map<std::string, double>> 
SistemaDifuso::inferirActivacionesCompleto(double valorVar1, double valorVar2) {
    
    std::map<std::string, double> activacionesSalida;
    std::map<std::string, double> pertenenciasVar1;
    std::map<std::string, double> pertenenciasVar2;

    if (variablesEntrada.size() < 2) {
        std::cerr << "Error: Se necesitan 2 variables de entrada.\n";
        return std::make_tuple(pertenenciasVar1, pertenenciasVar2, activacionesSalida);
    }
    
    // Paso 1: Fuzzificación (calcular grados de pertenencia)
    for (const auto& conjunto : variablesEntrada[0].obtenerConjuntosDifusos()) {
        conjunto.imprimir();
        double pertenencia = conjunto.calcularGradoPertenencia(valorVar1);
        printf("%f", pertenencia);
        if (pertenencia > 0.0) {
            pertenenciasVar1[conjunto.obtenerNombre()] = pertenencia;
        }
    }
    
    for (const auto& conjunto : variablesEntrada[1].obtenerConjuntosDifusos()) {
        conjunto.imprimir();
        double pertenencia = conjunto.calcularGradoPertenencia(valorVar2);
        if (pertenencia > 0.0) {
            pertenenciasVar2[conjunto.obtenerNombre()] = pertenencia;
        }
    }

    imprimirPertenencias(pertenenciasVar1, variablesEntrada[0].obtenerNombre());
    imprimirPertenencias(pertenenciasVar2, variablesEntrada[1].obtenerNombre());

    // Paso 2: Evaluar reglas activas
    for (const auto& [key, salida] : matrizReglas) {
        auto [conjVar1, conjVar2] = key;
        if (pertenenciasVar1.count(conjVar1) && pertenenciasVar2.count(conjVar2)) {
            double activacion = std::min(pertenenciasVar1[conjVar1], pertenenciasVar2[conjVar2]);
            activacionesSalida[salida] = std::max(activacionesSalida[salida], activacion);
        }
    }

    imprimirPertenencias(activacionesSalida, "Activacion de salida");

    return std::make_tuple(pertenenciasVar1, pertenenciasVar2, activacionesSalida);
}

ResultadoInferencia SistemaDifuso::inferirConExplicacionCompleta(double valorVar1, double valorVar2) {
    ResultadoInferencia resultado;
    
    // Obtenemos toda la información necesaria
    auto [pertenenciasVar1, pertenenciasVar2, activaciones] = inferirActivacionesCompleto(valorVar1, valorVar2);
    
    // Encontramos la salida con mayor activación
    double maxActivacion = 0.0;
    for (const auto& [salida, activacion] : activaciones) {
        if (activacion > maxActivacion) {
            maxActivacion = activacion;
            resultado.salidaLinguistica = salida;
            resultado.activacionFinal = activacion;
        }
    }
    
    // Buscamos la regla que produjo esta activación máxima
    for (const auto& [key, salidaRegla] : matrizReglas) {
        auto [conjVar1, conjVar2] = key;
        if (salidaRegla == resultado.salidaLinguistica && 
            pertenenciasVar1.count(conjVar1) && 
            pertenenciasVar2.count(conjVar2)) {
            
            double activacionRegla = std::min(pertenenciasVar1[conjVar1], pertenenciasVar2[conjVar2]);
            
            if (std::abs(activacionRegla - resultado.activacionFinal) < 0.0001) {
                resultado.conjuntoVar1 = conjVar1;
                resultado.conjuntoVar2 = conjVar2;
                resultado.pertenenciaVar1 = pertenenciasVar1[conjVar1];
                resultado.pertenenciaVar2 = pertenenciasVar2[conjVar2];
                break;
            }
        }
    }

    for (const auto& [nombreConjunto, activacion] : activaciones) {
        // Obtener el conjunto original de la variable de salida
        ConjuntoDifuso conjuntoOriginal;
        bool encontrado = false;
        for (const auto& conj : variableSalida.obtenerConjuntosDifusos()) {
            std::cout<< conj.obtenerNombre() << std::endl;
            if (conj.obtenerNombre() == nombreConjunto) {
                conjuntoOriginal = conj;
                encontrado = true;
                break;
            }
        }
        
        if (encontrado) {
            conjuntoOriginal.activacion = activacion;
            // Crear un nuevo conjunto difuso "recortado" por la activación
            ConjuntoDifuso conjuntoRecortado = conjuntoOriginal.recortar(activacion);
            variableSalida.insertarConjuntoDifuso(conjuntoRecortado);
        }
    }
 
    return resultado;
}

double SistemaDifuso::desfuzzificar(const std::map<std::string, double>& activacionesSalida) {
    double numerador = 0.0;
    double denominador = 0.0;

    for (const auto& [nombreConjunto, activacion] : activacionesSalida) {
        const auto& conjunto = variableSalida.obtenerConjuntoPorNombre(nombreConjunto);
        double centroide = conjunto.calcularCentroide();
        
        numerador += activacion * centroide;
        denominador += activacion;
    }

    if (denominador == 0.0) return 0.0; // evitar división por cero

    return numerador / denominador;
}


double SistemaDifuso::defusificarPorCentroide(const ResultadoInferencia& resultado) {
    // Obtener todos los conjuntos difusos de salida posibles
    std::vector<std::string> conjuntosSalida;
    for (const auto& regla : matrizReglas) {
        if (std::find(conjuntosSalida.begin(), conjuntosSalida.end(), regla.second) == conjuntosSalida.end()) {
            conjuntosSalida.push_back(regla.second);
        }
    }
    
    // Calcular centroides y activaciones para cada conjunto de salida
    double numerador = 0.0;
    double denominador = 0.0;
    
    for (const auto& nombreConjunto : conjuntosSalida) {

        std::cout<<nombreConjunto<<std::endl;
        // Buscar el conjunto difuso correspondiente en las variables de entrada
        // (Asumimos que los conjuntos de salida están definidos en alguna variable)
        double centroide = 0.0;
        double activacion = 0.0;
        
        for (const auto& variable : variablesEntrada) {
            for (const auto& conjunto : variable.obtenerConjuntosDifusos()) {
                
                    
                    centroide = conjunto.calcularCentroide();
                    printf("centroide %f", centroide);
                    break;
                
            }
        }
        
        // Usar la activación del resultado o calcularla si hay múltiples
        if (nombreConjunto == resultado.salidaLinguistica) {
            activacion = resultado.activacionFinal;
        } else {
            // Para otros conjuntos de salida, podrías querer usar activación cero
            // o implementar lógica para calcular sus activaciones
            activacion = 0.0;
        }
        
        printf("antes de suma %f, %f \n", numerador,denominador);
        numerador += centroide * activacion;
        denominador += activacion;
        printf("despues de suma %f, %f \n", numerador,denominador);
    }
    
    if (denominador < 1e-10) {
        printf("denominador peque \b");
        printf("%f, %f \n", numerador,denominador);
        // Caso por defecto si no hay activaciones significativas
        // Podrías devolver el centroide del conjunto con mayor activación
        for (const auto& variable : variablesEntrada) {
            for (const auto& conjunto : variable.obtenerConjuntosDifusos()) {
                if (conjunto.obtenerNombre() == resultado.salidaLinguistica) {
                    return conjunto.calcularCentroide();
                }
            }
        }
        return 0.0;
    }
    
    printf("Ultima operacion %f, %f \n", numerador, denominador);
    return numerador / denominador;
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