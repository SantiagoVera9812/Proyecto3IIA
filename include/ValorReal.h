#include <string>
#include <vector>
#include "Variable.h"

class ValorReal{

    std::string nombreVariableCorrespondiente;
    double valorNumerico;
    std::vector<std::pair<std::string, double>> valoresPertenecientes;

    public:

    ValorReal(const std::string& nombreVariableCorrespondiente, double valorNumerico, std::vector<Variable>& variables);

    double getValorNumerico() const;
    std::string getVariablePerteneciente() const;
    void imprimir() const;
};