#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include "Variable.h"
#pragma once

struct ResultadoInferencia {
    std::string salidaLinguistica;
    std::string conjuntoVar1;
    double pertenenciaVar1;
    std::string conjuntoVar2;
    double pertenenciaVar2;
    double activacionFinal;
};

class SistemaDifuso {
    private:
        std::map<std::pair<std::string, std::string>, std::string> matrizReglas;
        std::vector<Variable> variablesEntrada;
        std::vector<std::string> salidasPosibles;
        Variable variableSalida;
        int columnasEsperadas = 0;

        std::tuple<std::map<std::string, double>, 
           std::map<std::string, double>, 
           std::map<std::string, double>> 
                inferirActivacionesCompleto(double valorVar1, double valorVar2);

    
    public:
        // Añade una variable difusa (ej: "Temperatura")
        void agregarVariable(const Variable& variable);

        std::map<std::string, double> inferirActivaciones(double valorVar1, double valorVar2);
    
        // Define la matriz de reglas (ej: "Frío" × "Débil" → "PA")
        void agregarRegla(const std::string& var1, const std::string& var2, const std::string& salida);

        //Infiere la salida linguistica conjunto con los conjuntos difusos que generaron esa salida linguistica
        std::pair<std::string, std::pair<std::string, std::string>> inferirConExplicacion(double valorVar1, double valorVar2);

        //Conseguir la salida linguistica
        std::string inferirSalidaLinguistica(double valorVar1, double valorVar2, bool metodoAND);

        //Imprimir la matriz de reglas
        void imprimirMatrizReglas() const;

        //Cargar desde archivo
        bool cargarReglasDesdeArchivo(const std::string& nombreArchivo);

        //Conseguir la salida linguistica, los conjuntos difusos con su pertenencia y la activacion final y dejarla en un struct ResultadoInferencia
        ResultadoInferencia inferirConExplicacionCompleta(double valorVar1, double valorVar2);

        //Deffuzificacion para conseguir un valor numerico
        double defusificarPorCentroide(const ResultadoInferencia& resultado);

        double desfuzzificar(const std::map<std::string, double>& activacionesSalida);

        ResultadoInferencia ultimoIntentoDess(double valorVar1, double valorVar2);

        void ordenarVariables();

        const Variable& obtenerVariableSalida() const;

        const std::vector<Variable>& obtenerVariablesEntrada() const;

        double calcularCentroideSalida(const std::map<std::string, double>& activaciones);

    };