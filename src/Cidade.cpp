#include "../include/Cidade.h"

Cidade::Cidade(std::string nome) {
    this->nome = nome;
    this->visitas = 0;
}

std::string Cidade::getNome() {
    return nome;
}

void Cidade::incrementarVisitas() {
    visitas++;
}

int Cidade::getVisitas() {
    return visitas;
}
