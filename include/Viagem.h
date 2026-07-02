#ifndef VIAGEM_H
#define VIAGEM_H

#include <vector>
#include "Transporte.h"
#include "Passageiro.h"
#include "Cidade.h"

//um objeto representa um trecho da viagem.
// se tiver conexão, os trechos ficam encadeados.
// só no último trecho passageiros e transporte chegam ao destino final.
class Viagem {
private:
    Transporte* transporte;
    std::vector<Passageiro*> passageiros;
    Cidade* origem;
    Cidade* destino;
    int distancia; 
    Viagem* proxima; 
    int horasEmTransito;
    int horasNecessarias; 
    bool emAndamento;
    bool finalizada;

    int calcularHorasNecessarias();

public:
    Viagem(Transporte* transporte, std::vector<Passageiro*> passageiros, Cidade* origem,
           Cidade* destino, int distancia);

    void setProxima(Viagem* proxima);
    Viagem* getProxima();

    void iniciarViagem();
    void avancarHoras(int horas);
    void relatarEstado();

    bool isEmAndamento();
    bool isFinalizada();
    Cidade* getOrigem();
    Cidade* getDestino();
    Cidade* getDestinoFinal(); 
    Transporte* getTransporte();
    std::vector<Passageiro*> getPassageiros();
};

#endif
