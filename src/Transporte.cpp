#include "../include/Transporte.h"

Transporte::Transporte(std::string nome, char tipo, int capacidade, int velocidade,
                        int distanciaEntreDescansos, int tempoDescanso, Cidade* localAtual) {
    this->nome = nome;
    this->tipo = tipo;
    this->capacidade = capacidade;
    this->velocidade = velocidade;
    this->distanciaEntreDescansos = distanciaEntreDescansos;
    this->tempoDescanso = tempoDescanso;
    this->tempoDescansoAtual = 0;
    this->localAtual = localAtual;
}

std::string Transporte::getNome() { return nome; }
char Transporte::getTipo() { return tipo; }
int Transporte::getCapacidade() { return capacidade; }
int Transporte::getVelocidade() { return velocidade; }
int Transporte::getDistanciaEntreDescansos() { return distanciaEntreDescansos; }
int Transporte::getTempoDescanso() { return tempoDescanso; }
int Transporte::getTempoDescansoAtual() { return tempoDescansoAtual; }
Cidade* Transporte::getLocalAtual() { return localAtual; }
void Transporte::setLocalAtual(Cidade* local) { localAtual = local; }
void Transporte::setTempoDescansoAtual(int t) { tempoDescansoAtual = t; }
