#ifndef CIDADE_H
#define CIDADE_H

#include <string>

// classe bem simples, só guarda o nome da cidade mesmo
class Cidade {
private:
    std::string nome;
    int visitas; // conta quantas vezes um transporte chegou aqui, uso isso no relatório de "mais visitadas"

public:
    Cidade(std::string nome);
    std::string getNome();
    void incrementarVisitas();
    int getVisitas();
};

#endif
