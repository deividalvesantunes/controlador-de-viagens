#include "../include/Viagem.h"
#include <iostream>
#include <cmath>

Viagem::Viagem(Transporte* transporte, std::vector<Passageiro*> passageiros, Cidade* origem,
               Cidade* destino, int distancia) {
    this->transporte = transporte;
    this->passageiros = passageiros;
    this->origem = origem;
    this->destino = destino;
    this->distancia = distancia;
    this->proxima = nullptr;
    this->horasEmTransito = 0;
    this->emAndamento = false;
    this->finalizada = false;
    this->horasNecessarias = calcularHorasNecessarias();
}

int Viagem::calcularHorasNecessarias() {
    int velocidade = transporte->getVelocidade();
    int horasBase = (int) std::ceil((double) distancia / velocidade);

    int distDescanso = transporte->getDistanciaEntreDescansos();
    int horasDescanso = 0;
    if (distDescanso > 0) {
        int numDescansos = distancia / distDescanso; // quantas vezes bateu o limite
        horasDescanso = numDescansos * transporte->getTempoDescanso();
    }
    return horasBase + horasDescanso;
}

void Viagem::setProxima(Viagem* proxima) { this->proxima = proxima; }
Viagem* Viagem::getProxima() { return proxima; }
void Viagem::iniciarViagem() {
    // tira o transporte e os passageiros da cidade de origem
    transporte->setLocalAtual(nullptr);
    for (Passageiro* p : passageiros) {
        p->setLocalAtual(nullptr);
    }
    emAndamento = true;
    horasEmTransito = 0;
    std::cout << "Viagem iniciada: " << transporte->getNome() << " saindo de "
              << origem->getNome() << " para " << destino->getNome()
              << " (" << horasNecessarias << "h previstas)\n";
}
void Viagem::avancarHoras(int horas) {
    if (!emAndamento) return;

    horasEmTransito += horas;
    if (horasEmTransito >= horasNecessarias) {
        emAndamento = false;
        finalizada = true;

        if (proxima != nullptr) {
            // ainda tem trecho pela frente, transporte/passageiro continuam em transito
            std::cout << transporte->getNome() << " chegou em " << destino->getNome()
                      << " e vai seguir viagem (conexao)\n";
            proxima->iniciarViagem();
        } else {
            transporte->setLocalAtual(destino);
            for (Passageiro* p : passageiros) {
                p->setLocalAtual(destino);
            }
            destino->incrementarVisitas();
            std::cout << transporte->getNome() << " chegou no destino final: "
                      << destino->getNome() << "\n";
        }
    }
}

void Viagem::relatarEstado() {
    if (emAndamento) {
        std::cout << "  " << transporte->getNome() << ": em transito de "
                  << origem->getNome() << " -> " << destino->getNome()
                  << " (" << horasEmTransito << "/" << horasNecessarias << "h)\n";
    }
}

bool Viagem::isEmAndamento() { return emAndamento; }
bool Viagem::isFinalizada() { return finalizada; }
Cidade* Viagem::getOrigem() { return origem; }
Cidade* Viagem::getDestino() { return destino; }

Cidade* Viagem::getDestinoFinal() {
    if (proxima == nullptr) return destino;
    return proxima->getDestinoFinal();
}
Transporte* Viagem::getTransporte() { return transporte; }
std::vector<Passageiro*> Viagem::getPassageiros() { return passageiros; }
