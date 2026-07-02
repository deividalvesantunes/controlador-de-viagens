#ifndef CONTROLADOR_DE_TRANSITO_H
#define CONTROLADOR_DE_TRANSITO_H

#include <vector>
#include <string>
#include "Cidade.h"
#include "Trajeto.h"
#include "Transporte.h"
#include "Passageiro.h"
#include "Viagem.h"

class ControladorDeTransito {
private:
    std::vector<Cidade*> cidades;
    std::vector<Trajeto*> trajetos;
    std::vector<Transporte*> transportes;
    std::vector<Passageiro*> passageiros;
    std::vector<Viagem*> viagens; 

    Cidade* buscarCidade(std::string nome);
    Transporte* buscarTransporte(std::string nome);
    Passageiro* buscarPassageiro(std::string nome);

// procura o melhor caminho entre origem e destino.
//usa apenas trajetos do tipo do transporte.
//se não achar nenhum caminho, retorna vazio.
    std::vector<Cidade*> calcularMelhorTrajeto(Cidade* origem, Cidade* destino, char tipoTransporte);

public:
    ~ControladorDeTransito();

    void cadastrarCidade(std::string nome);
    void cadastrarTrajeto(std::string nomeOrigem, std::string nomeDestino, char tipo, int distancia);
    void cadastrarTransporte(std::string nome, char tipo, int capacidade, int velocidade,
                              int distanciaEntreDescansos, int tempoDescanso, std::string localAtual);
    void cadastrarPassageiro(std::string nome, std::string localAtual);

// monta a cadeia de Viagens (direta ou c conexao) e ja inicia a primeira
    bool iniciarViagem(std::string nomeTransporte, std::vector<std::string> nomesPassageiros,
                        std::string nomeOrigem, std::string nomeDestino);

    void avancarHoras(int horas);

    // relatorio
    void relatarPassageiros();
    void relatarTransportes();
    void relatarViagensEmAndamento();
    void relatarCidadesMaisVisitadas();

    void salvarDados(std::string pasta);
    void carregarDados(std::string pasta);

    std::vector<Cidade*> getCidades();
    std::vector<Transporte*> getTransportes();
    std::vector<Passageiro*> getPassageiros();
};

#endif
