#include <iostream>
#include <fstream>
#include <string>
#include "ConjuntoDifuso.h"
#include "Variable.h"

using namespace std;

int main() {
    string nombreArchivo;
    string linea;

    
    //lectura del archivo txt
    ifstream archivo("descripcion-variables.txt");

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
        return 1;
    }

    //Procesar variables y logica fuzzy
    while (getline(archivo, linea)) {
        cout << linea << endl;
        ConjuntoDifuso conjuntoDifuso = ConjuntoDifuso("hola", TipoDifuso::TRIANGULAR, std::vector<double> {1,2,3});
        ConjuntoDifuso conjuntoDifuso2 = ConjuntoDifuso("holaTrapezoide", TipoDifuso::TRAPEZOIDAL, std::vector<double> {1,2,3,4});
        Variable variable = Variable("Range", 10,40, "Km", std::vector<ConjuntoDifuso> {conjuntoDifuso, conjuntoDifuso2});
        variable.imprimir();
        
    }

    archivo.close();

    return 0;
}
