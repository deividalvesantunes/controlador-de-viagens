#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "../include/ControladorDeTransito.h"

//so pra nao ficar repetindo esse std::cin.ignore toda hora
void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void mostrarMenu() {
    std::cout << "\n===== Controlador de Viagens =====\n";
    std::cout << "1  - Cadastrar cidade\n";
    std::cout << "2  - Cadastrar trajeto\n";
    std::cout << "3  - Cadastrar transporte\n";
    std::cout << "4  - Cadastrar passageiro\n";
    std::cout << "5  - Iniciar viagem\n";
    std::cout << "6  - Avancar horas\n";
    std::cout << "7  - Relatorio de passageiros\n";
    std::cout << "8  - Relatorio de transportes\n";
    std::cout << "9  - Relatorio de viagens em andamento\n";
    std::cout << "10 - Relatorio de cidades mais visitadas\n";
    std::cout << "11 - Listar cidades/transportes/passageiros cadastrados\n";
    std::cout << "0  - Salvar e sair\n";
    std::cout << "Escolha: ";
}

int main() {
    ControladorDeTransito controlador;
    std::string pastaDados = "data";

    //tentar carregar o que ja tinha salvo antes
    controlador.carregarDados(pastaDados);

    int opcao = -1;
    while (opcao != 0) {
        mostrarMenu();
        std::cin >> opcao;
        if (std::cin.fail()) {
            limparBuffer();
            std::cout << "Entrada invalida.\n";
            continue;
        }

        if (opcao == 1) {
            std::cout << "Nome da cidade: ";
            std::string nome;
            limparBuffer();
            std::getline(std::cin, nome);
            controlador.cadastrarCidade(nome);

        } else if (opcao == 2) {
            std::string origem, destino;
            char tipo;
            int distancia;
            std::cout << "Cidade de origem: ";
            limparBuffer();
            std::getline(std::cin, origem);
            std::cout << "Cidade de destino: ";
            std::getline(std::cin, destino);
            std::cout << "Tipo (A = aquatico, T = terrestre): ";
            std::cin >> tipo;
            std::cout << "Distancia (km): ";
            std::cin >> distancia;
            controlador.cadastrarTrajeto(origem, destino, toupper(tipo), distancia);

        } else if (opcao == 3) {
            std::string nome, local;
            char tipo;
            int capacidade, velocidade, distDescanso, tempoDescanso;
            std::cout << "Nome do transporte: ";
            limparBuffer();
            std::getline(std::cin, nome);
            std::cout << "Tipo (A/T): ";
            std::cin >> tipo;
            std::cout << "Capacidade de passageiros: ";
            std::cin >> capacidade;
            std::cout << "Velocidade (km/h): ";
            std::cin >> velocidade;
            std::cout << "Distancia entre descansos (km, 0 se nao precisar): ";
            std::cin >> distDescanso;
            std::cout << "Tempo de descanso (horas): ";
            std::cin >> tempoDescanso;
            std::cout << "Cidade onde ele esta agora: ";
            limparBuffer();
            std::getline(std::cin, local);
            controlador.cadastrarTransporte(nome, toupper(tipo), capacidade, velocidade, distDescanso, tempoDescanso, local);

        } else if (opcao == 4) {
            std::string nome, local;
            std::cout << "Nome do passageiro: ";
            limparBuffer();
            std::getline(std::cin, nome);
            std::cout << "Cidade onde ele esta agora: ";
            std::getline(std::cin, local);
            controlador.cadastrarPassageiro(nome, local);

        } else if (opcao == 5) {
            std::string transporte, origem, destino;
            std::cout << "Nome do transporte: ";
            limparBuffer();
            std::getline(std::cin, transporte);
            std::cout << "Cidade de origem: ";
            std::getline(std::cin, origem);
            std::cout << "Cidade de destino: ";
            std::getline(std::cin, destino);
            std::cout << "Quantos passageiros vao nessa viagem? ";
            int qtd;
            std::cin >> qtd;
            std::vector<std::string> nomesPassageiros;
            limparBuffer();
            for (int i = 0; i < qtd; i++) {
                std::cout << "  Nome do passageiro " << (i + 1) << ": ";
                std::string nomeP;
                std::getline(std::cin, nomeP);
                nomesPassageiros.push_back(nomeP);
            }
            controlador.iniciarViagem(transporte, nomesPassageiros, origem, destino);

        } else if (opcao == 6) {
            std::cout << "Quantas horas avancar? ";
            int horas;
            std::cin >> horas;
            controlador.avancarHoras(horas);
            std::cout << "Horas avancadas.\n";

        } else if (opcao == 7) {
            controlador.relatarPassageiros();
        } else if (opcao == 8) {
            controlador.relatarTransportes();
        } else if (opcao == 9) {
            controlador.relatarViagensEmAndamento();
        } else if (opcao == 10) {
            controlador.relatarCidadesMaisVisitadas();
        } else if (opcao == 11) {
            std::cout << "\nCidades: ";
            for (auto c : controlador.getCidades()) std::cout << c->getNome() << ", ";
            std::cout << "\nTransportes: ";
            for (auto t : controlador.getTransportes()) std::cout << t->getNome() << ", ";
            std::cout << "\nPassageiros: ";
            for (auto p : controlador.getPassageiros()) std::cout << p->getNome() << ", ";
            std::cout << "\n";
        } else if (opcao == 0) {
            controlador.salvarDados(pastaDados);
            std::cout << "Ate mais!\n";
        } else {
            std::cout << "Opcao invalida.\n";
        }
    }

    return 0;
}
