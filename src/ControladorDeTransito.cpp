#include "../include/ControladorDeTransito.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <climits>

ControladorDeTransito::~ControladorDeTransito() {
    for (Viagem* v : viagens) {
        Viagem* atual = v;
        while (atual != nullptr) {
            Viagem* prox = atual->getProxima();
            delete atual;
            atual = prox;
        }
    }
    for (auto p : passageiros) delete p;
    for (auto t : transportes) delete t;
    for (auto t : trajetos) delete t;
    for (auto c : cidades) delete c;
}

Cidade* ControladorDeTransito::buscarCidade(std::string nome) {
    for (Cidade* c : cidades) {
        if (c->getNome() == nome) return c;
    }
    return nullptr;
}
Transporte* ControladorDeTransito::buscarTransporte(std::string nome) {
    for (Transporte* t : transportes) {
        if (t->getNome() == nome) return t;
    }
    return nullptr;
}

Passageiro* ControladorDeTransito::buscarPassageiro(std::string nome) {
    for (Passageiro* p : passageiros) {
        if (p->getNome() == nome) return p;
    }
    return nullptr;
}

void ControladorDeTransito::cadastrarCidade(std::string nome) {
    if (buscarCidade(nome) != nullptr) {
        std::cout << "Ja existe uma cidade com esse nome.\n";
        return;
    }
    cidades.push_back(new Cidade(nome));
}
void ControladorDeTransito::cadastrarTrajeto(std::string nomeOrigem, std::string nomeDestino, char tipo, int distancia) {
    Cidade* origem = buscarCidade(nomeOrigem);
    Cidade* destino = buscarCidade(nomeDestino);
    if (origem == nullptr || destino == nullptr) {
        std::cout << "Cidade de origem ou destino nao encontrada.\n";
        return;
    }
    trajetos.push_back(new Trajeto(origem, destino, tipo, distancia));
}
void ControladorDeTransito::cadastrarTransporte(std::string nome, char tipo, int capacidade, int velocidade,
                                                 int distanciaEntreDescansos, int tempoDescanso, std::string localAtual) {
    Cidade* local = buscarCidade(localAtual);
    if (local == nullptr) {
        std::cout << "Cidade de local atual nao encontrada.\n";
        return;
    }
    transportes.push_back(new Transporte(nome, tipo, capacidade, velocidade, distanciaEntreDescansos, tempoDescanso, local));
}

void ControladorDeTransito::cadastrarPassageiro(std::string nome, std::string localAtual) {
    Cidade* local = buscarCidade(localAtual);
    if (local == nullptr) {
        std::cout << "Cidade nao encontrada.\n";
        return;
    }
    passageiros.push_back(new Passageiro(nome, local));
}

std::vector<Cidade*> ControladorDeTransito::getCidades() { return cidades; }
std::vector<Transporte*> ControladorDeTransito::getTransportes() { return transportes; }
std::vector<Passageiro*> ControladorDeTransito::getPassageiros() { return passageiros; }
std::vector<Cidade*> ControladorDeTransito::calcularMelhorTrajeto(Cidade* origem, Cidade* destino, char tipoTransporte) {
    std::vector<Cidade*> nos = cidades;
    std::vector<int> dist(nos.size(), INT_MAX);
    std::vector<int> anterior(nos.size(), -1);
    std::vector<bool> visitado(nos.size(), false);

    auto indexDe = [&](Cidade* c) {
        for (size_t i = 0; i < nos.size(); i++) if (nos[i] == c) return (int)i;
        return -1;
    };

    int idxOrigem = indexDe(origem);
    if (idxOrigem == -1) return {};
    dist[idxOrigem] = 0;

    for (size_t iter = 0; iter < nos.size(); iter++) {
        int u = -1;
        int menor = INT_MAX;
        for (size_t i = 0; i < nos.size(); i++) {
            if (!visitado[i] && dist[i] < menor) {
                menor = dist[i];
                u = (int)i;
            }
        }
        if (u == -1) break;
        visitado[u] = true;

        for (Trajeto* t : trajetos) {
            if (t->getTipo() != tipoTransporte) continue;
            if (t->getOrigem() != nos[u]) continue;
            int v = indexDe(t->getDestino());
            if (v == -1) continue;
            if (dist[u] != INT_MAX && dist[u] + t->getDistancia() < dist[v]) {
                dist[v] = dist[u] + t->getDistancia();
                anterior[v] = u;
            }
        }
    }

    int idxDestino = indexDe(destino);
    if (idxDestino == -1 || dist[idxDestino] == INT_MAX) {
        return {};
    }

    std::vector<Cidade*> caminho;
    int atual = idxDestino;
    while (atual != -1) {
        caminho.push_back(nos[atual]);
        atual = anterior[atual];
    }
    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}

bool ControladorDeTransito::iniciarViagem(std::string nomeTransporte, std::vector<std::string> nomesPassageiros,
                                           std::string nomeOrigem, std::string nomeDestino) {
    Transporte* transporte = buscarTransporte(nomeTransporte);
    Cidade* origem = buscarCidade(nomeOrigem);
    Cidade* destino = buscarCidade(nomeDestino);

    if (transporte == nullptr || origem == nullptr || destino == nullptr) {
        std::cout << "Transporte, origem ou destino nao encontrado.\n";
        return false;
    }
    if (transporte->getLocalAtual() != origem) {
        std::cout << "O transporte " << nomeTransporte << " nao esta em " << nomeOrigem << ".\n";
        return false;
    }

    std::vector<Passageiro*> lista;
    for (std::string nomeP : nomesPassageiros) {
        Passageiro* p = buscarPassageiro(nomeP);
        if (p == nullptr) {
            std::cout << "Passageiro " << nomeP << " nao encontrado.\n";
            return false;
        }
        if (p->getLocalAtual() != origem) {
            std::cout << "Passageiro " << nomeP << " nao esta em " << nomeOrigem << ".\n";
            return false;
        }
        lista.push_back(p);
    }
    if ((int)lista.size() > transporte->getCapacidade()) {
        std::cout << "Excedeu a capacidade do transporte.\n";
        return false;
    }

    std::vector<Cidade*> caminho = calcularMelhorTrajeto(origem, destino, transporte->getTipo());
    if (caminho.empty()) {
        std::cout << "Nao existe trajeto possivel entre " << nomeOrigem << " e " << nomeDestino << ".\n";
        return false;
    }

    std::vector<Viagem*> trechos;
    for (size_t i = 0; i + 1 < caminho.size(); i++) {
        Cidade* de = caminho[i];
        Cidade* para = caminho[i + 1];
        int distanciaTrecho = 0;
        for (Trajeto* t : trajetos) {
            if (t->getOrigem() == de && t->getDestino() == para && t->getTipo() == transporte->getTipo()) {
                distanciaTrecho = t->getDistancia();
                break;
            }
        }
        trechos.push_back(new Viagem(transporte, lista, de, para, distanciaTrecho));
    }

    for (size_t i = 0; i + 1 < trechos.size(); i++) {
        trechos[i]->setProxima(trechos[i + 1]);
    }

    viagens.push_back(trechos[0]);
    trechos[0]->iniciarViagem();
    return true;
}

void ControladorDeTransito::avancarHoras(int horas) {
    for (Viagem* v : viagens) {
        Viagem* atual = v;
        while (atual != nullptr) {
            if (atual->isEmAndamento()) {
                atual->avancarHoras(horas);
                break;
            }
            if (!atual->isFinalizada()) break;
            atual = atual->getProxima();
        }
    }
}

void ControladorDeTransito::relatarPassageiros() {
    std::cout << "\n--- Relatorio de Passageiros ---\n";
    for (Passageiro* p : passageiros) {
        if (p->getLocalAtual() != nullptr) {
            std::cout << p->getNome() << ": em " << p->getLocalAtual()->getNome() << "\n";
        } else {
            //ta em transito, preciso achar em qual viagem ele ta pra falar origem- destino-transporte
            bool achou = false;
            for (Viagem* v : viagens) {
                Viagem* atual = v;
                while (atual != nullptr) {
                    if (atual->isEmAndamento()) {
                        for (Passageiro* pp : atual->getPassageiros()) {
                            if (pp == p) {
                                std::cout << p->getNome() << ": em transito de "
                                          << atual->getOrigem()->getNome() << " para "
                                          << atual->getDestino()->getNome() << " (destino final: "
                                          << atual->getDestinoFinal()->getNome() << ") no transporte "
                                          << atual->getTransporte()->getNome() << "\n";
                                achou = true;
                            }
                        }
                    }
                    atual = atual->getProxima();
                }
            }
            if (!achou) {
                std::cout << p->getNome() << ": em transito (informacao indisponivel)\n";
            }
        }
    }
}

void ControladorDeTransito::relatarTransportes() {
    std::cout << "\n--- Relatorio de Transportes ---\n";
    for (Transporte* t : transportes) {
        if (t->getLocalAtual() != nullptr) {
            std::cout << t->getNome() << ": em " << t->getLocalAtual()->getNome() << "\n";
        } else {
            std::cout << t->getNome() << ": em transito\n";
        }
    }
}

void ControladorDeTransito::relatarViagensEmAndamento() {
    std::cout << "\n--- Viagens em Andamento ---\n";
    bool tem = false;
    for (Viagem* v : viagens) {
        Viagem* atual = v;
        while (atual != nullptr) {
            if (atual->isEmAndamento()) {
                atual->relatarEstado();
                tem = true;
            }
            atual = atual->getProxima();
        }
    }
    if (!tem) std::cout << "  Nenhuma viagem em andamento no momento.\n";
}

void ControladorDeTransito::relatarCidadesMaisVisitadas() {
    std::cout << "\n--- Cidades Mais Visitadas ---\n";
    std::vector<Cidade*> ordenadas = cidades;
    std::sort(ordenadas.begin(), ordenadas.end(), [](Cidade* a, Cidade* b) {
        return a->getVisitas() > b->getVisitas();
    });
    for (Cidade* c : ordenadas) {
        std::cout << c->getNome() << ": " << c->getVisitas() << " chegada(s)\n";
    }
}
void ControladorDeTransito::salvarDados(std::string pasta) {
    std::ofstream fc(pasta + "/cidades.txt");
    for (Cidade* c : cidades) {
        fc << c->getNome() << "|" << c->getVisitas() << "\n";
    }
    fc.close();

    std::ofstream ft(pasta + "/trajetos.txt");
    for (Trajeto* t : trajetos) {
        ft << t->getOrigem()->getNome() << "|" << t->getDestino()->getNome() << "|"
           << t->getTipo() << "|" << t->getDistancia() << "\n";
    }
    ft.close();

    std::ofstream ftr(pasta + "/transportes.txt");
    for (Transporte* t : transportes) {
        std::string local = (t->getLocalAtual() != nullptr) ? t->getLocalAtual()->getNome() : "EM_TRANSITO";
        ftr << t->getNome() << "|" << t->getTipo() << "|" << t->getCapacidade() << "|"
            << t->getVelocidade() << "|" << t->getDistanciaEntreDescansos() << "|"
            << t->getTempoDescanso() << "|" << local << "\n";
    }
    ftr.close();

    std::ofstream fp(pasta + "/passageiros.txt");
    for (Passageiro* p : passageiros) {
        std::string local = (p->getLocalAtual() != nullptr) ? p->getLocalAtual()->getNome() : "EM_TRANSITO";
        fp << p->getNome() << "|" << local << "\n";
    }
    fp.close();

    std::cout << "Dados salvos em " << pasta << "\n";
}
static std::vector<std::string> splitLinha(const std::string& linha) {
    std::vector<std::string> campos;
    std::stringstream ss(linha);
    std::string campo;
    while (std::getline(ss, campo, '|')) {
        campos.push_back(campo);
    }
    return campos;
}
void ControladorDeTransito::carregarDados(std::string pasta) {
    std::ifstream fc(pasta + "/cidades.txt");
    std::string linha;
    while (std::getline(fc, linha)) {
        if (linha.empty()) continue;
        auto campos = splitLinha(linha);
        Cidade* c = new Cidade(campos[0]);
        for (int i = 0; i < std::stoi(campos[1]); i++) c->incrementarVisitas();
        cidades.push_back(c);
    }
    fc.close();

    std::ifstream ft(pasta + "/trajetos.txt");
    while (std::getline(ft, linha)) {
        if (linha.empty()) continue;
        auto campos = splitLinha(linha);
        cadastrarTrajeto(campos[0], campos[1], campos[2][0], std::stoi(campos[3]));
    }
    ft.close();
    std::ifstream ftr(pasta + "/transportes.txt");
    while (std::getline(ftr, linha)) {
        if (linha.empty()) continue;
        auto campos = splitLinha(linha);
        std::string local = campos[6];
        if (local == "EM_TRANSITO") {
            local = cidades.empty() ? "" : cidades[0]->getNome();
        }
        cadastrarTransporte(campos[0], campos[1][0], std::stoi(campos[2]), std::stoi(campos[3]),
                             std::stoi(campos[4]), std::stoi(campos[5]), local);
    }
    ftr.close();
    std::ifstream fp(pasta + "/passageiros.txt");
    while (std::getline(fp, linha)) {
        if (linha.empty()) continue;
        auto campos = splitLinha(linha);
        std::string local = campos[1];
        if (local == "EM_TRANSITO") {
            local = cidades.empty() ? "" : cidades[0]->getNome();
        }
        cadastrarPassageiro(campos[0], local);
    }
    fp.close();

    std::cout << "Dados carregados de " << pasta << "\n";
}
