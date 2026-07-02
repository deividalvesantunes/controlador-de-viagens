#ifndef TRAJETO_H
#define TRAJETO_H

#include <string>
#include "Cidade.h"

//liga duas cidades. tipo A = aquatico, T = terrestre
class Trajeto {
private:
    Cidade* origem;
    Cidade* destino;
    char tipo;
    int distancia;

public:
    Trajeto(Cidade* origem, Cidade* destino, char tipo, int distancia);
    Cidade* getOrigem();
    Cidade* getDestino();
    char getTipo();
    int getDistancia();
};

#endif
