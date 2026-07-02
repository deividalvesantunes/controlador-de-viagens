#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include <string>
#include "Cidade.h"

class Transporte {
private:
    std::string nome;
    char tipo; //A ou T, tem que bater com o tipo do trajeto p poder usar
    int capacidade;
    int velocidade; // km/h
    int distanciaEntreDescansos; // km
    int tempoDescanso; // horas de descanso quando bate o limite
    int tempoDescansoAtual; //quanto já descansou (se tiver descansando)
    Cidade* localAtual; //se for nullptr, ta em transito (a viagem que sabe onde ele ta)

public:
    Transporte(std::string nome, char tipo, int capacidade, int velocidade,
               int distanciaEntreDescansos, int tempoDescanso, Cidade* localAtual);

    std::string getNome();
    char getTipo();
    int getCapacidade();
    int getVelocidade();
    int getDistanciaEntreDescansos();
    int getTempoDescanso();
    int getTempoDescansoAtual();
    Cidade* getLocalAtual();
    void setLocalAtual(Cidade* local);
    void setTempoDescansoAtual(int t);
};

#endif
