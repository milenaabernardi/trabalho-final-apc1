#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib> // funcao rand()
#include <ctime> // funcao time()
#include <algorithm> // comparar os resultados para mostrar o vencedor
#include <conio.h> // para reconhecer a tecla _getch()
#include <windows.h> // para limpar a tela
using namespace std;

struct Jogador {
    string nome;
    string cor;
    int pontuacao;
    int linha;
    int coluna;
    bool pularRodada;
};

// codigos de cor ANSI
const string RESET = "\033[0m";
const string VERMELHO = "\033[31m";
const string VERDE = "\033[32m";
const string AMARELO = "\033[33m";
const string AZUL = "\033[34m";
const string MAGENTA = "\033[35m";
const string CIANO = "\033[36m";

const int LINHAS = 10;
const int COLUNAS = 10;
const char VAZIO = '.';
const char TUNEL = 'O';
const char ESCONDERIJO = '-';
const char PROSSEGUIR = '+';
const char TELEPORTE = 'T';
const char EXPLOSAO = 'X';

void nomeJogo();
void menu();
void menuComTeclas();
void mostrarRegras();
void limparTela();
void iniciarMina(vector<vector<char>>& mina, vector<vector<int>>& diamantes);
void armadilhasPistas(vector<vector<char>>& mina);
void exibirMina(const vector<vector<char>>& mina, const vector<Jogador>& jogadores);
void exibirRanking(const vector<Jogador>& jogadores);
void jogar(vector<vector<char>>& mina, vector<vector<int>>& diamantes, vector<Jogador>& jogadores);
bool movimentoValido(int linha, int coluna);
void vencedor(const vector<Jogador>& jogadores);
void escolherNumeroJogadores(int& numJogadores);
void inserirNomes(vector<Jogador>& jogadores);
void escolherCores(vector<Jogador>& jogadores);

int main(){
    srand(time(0));
    nomeJogo();
    menuComTeclas();

    return 0;
}

void nomeJogo(){
    cout << "\033[1;34m";
    cout << "==============================" << endl;
    cout << "  CACADORES DE DIAMANTES" << endl;
    cout << "==============================" << endl;
    cout << "\033[0m";
    cout << endl;
}

void limparTela(){
    system("cls");
}

void iniciarMina(vector<vector<char>>& mina, vector<vector<int>>& diamantes){
    // distribuir de forma aleátoria os diamantes
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            diamantes[i][j] = rand() % 10 + 1;
        }
    }
}

void armadilhasPistas(vector<vector<char>>& mina){
    // adiciona armadilhas e pistas aleatoriamente
    int totalQuadros = LINHAS * COLUNAS;
    int numPistas = totalQuadros * 5 / 100;
    int numArmadilhas = totalQuadros * 10 / 100;

    while (numPistas > 0){
        int r = rand() % LINHAS;
        int c = rand() % COLUNAS;
        if (mina[r][c] == VAZIO){
            mina[r][c] = PROSSEGUIR;
            numPistas--;
        }
    }

    while (numArmadilhas > 0){
        int r = rand() % LINHAS;
        int c = rand() % COLUNAS;
        if (mina[r][c] == VAZIO){
            mina[r][c] = (numArmadilhas % 2 == 0) ? TUNEL : ESCONDERIJO;
            numArmadilhas--;
        }
    }

    int numTeleportes = totalQuadros * 3 / 100;
    int numExplosoes = totalQuadros * 2 / 100;

    while (numTeleportes > 0){
        int r = rand() % LINHAS;
        int c = rand() % COLUNAS;
        if (mina[r][c] == VAZIO){
            mina[r][c] = TELEPORTE;
            numTeleportes--;
        }
    }

    while (numExplosoes > 0){
        int r = rand() % LINHAS;
        int c = rand() % COLUNAS;
        if (mina[r][c] == VAZIO){
            mina[r][c] = EXPLOSAO;
            numExplosoes--;
        }
    }
}

void exibirMina(const vector<vector<char>>& mina, const vector<Jogador>& jogadores){
    cout << "   +";
    for (int j = 0; j < COLUNAS; j++){
        cout << "---";
    }
    cout << "+\n";

    for (int i = 0; i < LINHAS; i++){
        cout << " |";
        for (int j = 0; j < COLUNAS; j++){
            bool ehJogador = false;
            for (const auto& jogador : jogadores) {
                if (jogador.linha == i && jogador.coluna == j){
                    cout << jogador.cor << setw(3) << 'J' << RESET;
                    ehJogador = true;
                    break;
                }
            }
            if (!ehJogador){
                switch (mina[i][j]){
                    case VAZIO: cout << setw(3) << '.'; 
                    break;
                    case TUNEL: cout << setw(3) << 'O'; 
                    break;
                    case ESCONDERIJO: cout << setw(3) << '-'; 
                    break;
                    case PROSSEGUIR: cout << setw(3) << '+'; 
                    break;
                    case TELEPORTE: cout << setw(3) << 'T'; 
                    break;
                    case EXPLOSAO: cout << setw(3) << 'X'; 
                    break;
                    default: cout << setw(3) << mina[i][j]; 
                    break;
                }
            }
        }
        cout << "   |\n";
    }

    cout << "   +";
    for (int j = 0; j < COLUNAS; j++){
        cout << "---";
    }
    cout << "+\n";
    cout << '\n';
}

void exibirRanking(const vector<Jogador>& jogadores){
    cout << "Ranking:" << endl;
    for (const auto& jogador : jogadores){
        cout << jogador.cor << jogador.nome << " " << jogador.pontuacao << " quilates" << RESET << endl;
    }
    cout << endl;
}

void jogar(vector<vector<char>>& mina, vector<vector<int>>& diamantes, vector<Jogador>& jogadores){
    int totalJogadas = 0;
    int numJogadores = jogadores.size();
    
    while (totalJogadas < LINHAS * COLUNAS){
        for (int i = 0; i < numJogadores; i++){
            if (jogadores[i].pularRodada) {
                jogadores[i].pularRodada = false;
                continue;
            }
            
            limparTela();
            exibirRanking(jogadores);
            exibirMina(mina, jogadores);

            char direcao;
            cout << jogadores[i].cor << "Turno de " << jogadores[i].nome << "." << endl;
            cout << "Mova-se (w/a/s/d): " << RESET;
            direcao = _getch();

            int novaLinha = jogadores[i].linha;
            int novaColuna = jogadores[i].coluna;

            if (direcao == 'w' && movimentoValido(novaLinha - 1, novaColuna)) novaLinha--;
            else if (direcao == 'a' && movimentoValido(novaLinha, novaColuna - 1)) novaColuna--;
            else if (direcao == 's' && movimentoValido(novaLinha + 1, novaColuna)) novaLinha++;
            else if (direcao == 'd' && movimentoValido(novaLinha, novaColuna + 1)) novaColuna++;

            char conteudoNovaPosicao = mina[novaLinha][novaColuna];
            mina[jogadores[i].linha][jogadores[i].coluna] = '0' + i; // deixa um rastro do jogador na posição anterior
            jogadores[i].linha = novaLinha;
            jogadores[i].coluna = novaColuna;

            if (conteudoNovaPosicao == VAZIO){
                jogadores[i].pontuacao += diamantes[novaLinha][novaColuna];
                mina[novaLinha][novaColuna] = '0' + i;
            } else if (conteudoNovaPosicao == TUNEL){
                jogadores[i].pontuacao -= 10;
                mina[novaLinha][novaColuna] = 'X';
            } else if (conteudoNovaPosicao == ESCONDERIJO){
                jogadores[i].pontuacao -= 20;
                mina[novaLinha][novaColuna] = '!';
            } else if (conteudoNovaPosicao == PROSSEGUIR){
                jogadores[i].pontuacao += 5;
                mina[novaLinha][novaColuna] = '>';
            } else if (conteudoNovaPosicao == TELEPORTE){
                jogadores[i].pontuacao += 5;

                do {
                    jogadores[i].linha = rand() % LINHAS;
                    jogadores[i].coluna = rand() % COLUNAS;
                } 
                while (mina[jogadores[i].linha][jogadores[i].coluna] != VAZIO);
                mina[jogadores[i].linha][jogadores[i].coluna] = '0' + i;
            } else if (conteudoNovaPosicao == EXPLOSAO){
                jogadores[i].pontuacao -= 15;
                jogadores[i].pularRodada = true;
                mina[novaLinha][novaColuna] = '!';
            }
            
            jogadores[i].linha = novaLinha;
            jogadores[i].coluna = novaColuna;
            totalJogadas++;
        }
    }

    limparTela();
    exibirRanking(jogadores);
    exibirMina(mina, jogadores);
    vencedor(jogadores);

    cout << "Pressione qualquer tecla para voltar ao menu...";
    _getch();
}

bool movimentoValido(int linha, int coluna){
    return linha >= 0 && linha < LINHAS && coluna >= 0 && coluna < COLUNAS;
}

void vencedor(const vector<Jogador>& jogadores){
    auto vencedor = max_element(jogadores.begin(), jogadores.end(), [](const Jogador& a, const Jogador& b){
        return a.pontuacao < b.pontuacao;
    });

    cout << vencedor->cor << vencedor->nome << RESET << " GANHOU!" << endl; 
    cout << "COM " << vencedor->pontuacao << " QUILATES!" << endl;
}

void mostrarRegras(){
    cout << "Regras do Jogo:" << endl;
    cout << endl;
    cout << "1. Cada jogador inicia em uma posicao aleatoria na mina." << endl;
    cout << "2. O objetivo e coletar o maior numero de diamantes possivel." << endl;
    cout << "3. As jogadas sao realizadas com as teclas w (cima), a (esquerda), s (baixo) e d (direita)." << endl;
    cout << "4. Cuidado com as armadilhas:" << endl;
    cout << "   - Tunel (O): reduz sua pontuacao em 10 pontos." << endl;
    cout << "   - Esconderijo (-): reduz sua pontuacao em 20 pontos." << endl;
    cout << "   - Explosao (X): reduz sua pontuacao em 15 pontos, e o jogador perde a proxima rodada." << endl;
    cout << "5. Pistas (+): aumentam sua pontuacao em 5 pontos." << endl;
    cout << "6. Teleporte (T): teleporta o jogador para uma nova posicao aleatoria e aumenta a pontuacao em 5 pontos." << endl;
    cout << "7. O jogo termina quando sao realizadas 100 jogadas." << endl;
    cout << "8. O jogador com a maior pontuacao ao final do jogo e o vencedor." << endl;
    cout << endl;
    cout << "Pressione qualquer tecla para voltar ao menu...";
    _getch();
}

void menuComTeclas(){
    int opcao = 1;
    char tecla;
    bool executando = true;

    while (executando){
        limparTela();
        nomeJogo();

        cout << "Menu Principal:" << endl;
        cout << (opcao == 1 ? "> " : "  ") << "Jogar" << endl;
        cout << (opcao == 2 ? "> " : "  ") << "Regras" << endl;
        cout << (opcao == 3 ? "> " : "  ") << "Sair" << endl;
        cout << endl;
        cout << "Use W/S para navegar e Enter para selecionar." << endl;

        tecla = _getch();

        if (tecla == 'w'){
            opcao = (opcao == 1) ? 3 : opcao - 1;
        } else if (tecla == 's'){
            opcao = (opcao == 3) ? 1 : opcao + 1;
        } else if (tecla == 13){ 
            limparTela();

            switch (opcao){
                case 1: { 
                    int numJogadores;
                    escolherNumeroJogadores(numJogadores);

                    vector<vector<char>> mina(LINHAS, vector<char>(COLUNAS, VAZIO));
                    vector<vector<int>> diamantes(LINHAS, vector<int>(COLUNAS, 0));
                    vector<Jogador> jogadores(numJogadores);

                    inserirNomes(jogadores);
                    escolherCores(jogadores);

                    iniciarMina(mina, diamantes);
                    armadilhasPistas(mina);
                    jogar(mina, diamantes, jogadores);
                    break;
                }
                case 2:
                    mostrarRegras();
                    break;
                case 3:
                    cout << "Obrigado por jogar! :)" << endl;
                    cout << "Saindo do jogo..." << endl;
                    Sleep(2000);
                    executando = false;
                    break;
            }
        }

    }
}

void escolherNumeroJogadores(int& numJogadores){
    int opcao = 1;
    char tecla;
    bool escolhendo = true;

    while (escolhendo){
        limparTela();
        nomeJogo();

        cout << "Escolha o numero de jogadores:" << endl;
        cout << (opcao == 1 ? "> " : "  ") << "2 Jogadores" << endl;
        cout << (opcao == 2 ? "> " : "  ") << "3 Jogadores" << endl;
        cout << (opcao == 3 ? "> " : "  ") << "4 Jogadores" << endl;
        cout << endl;
        cout << "Use w/s para navegar e Enter para selecionar." << endl;

        tecla = _getch();

        if (tecla == 'w'){
            opcao = (opcao == 1) ? 3 : opcao - 1;
        } else if (tecla == 's'){
            opcao = (opcao == 3) ? 1 : opcao + 1;
        } else if (tecla == 13){ 
            numJogadores = opcao + 1; 
            escolhendo = false;
        }

    }
}

void inserirNomes(vector<Jogador>& jogadores){
    for (int i = 0; i < jogadores.size(); i++){
        cout << "Digite o nome do jogador " << i + 1 << ": ";
        cin >> jogadores[i].nome;
        jogadores[i].pontuacao = 0;
        jogadores[i].pularRodada = false;

        // inicia a posição do jogador de forma randomica
        jogadores[i].linha = rand() % LINHAS;
        jogadores[i].coluna = rand() % COLUNAS;
    }
}

void escolherCores(vector<Jogador>& jogadores){
    vector<string> cores = { VERMELHO, VERDE, AMARELO, AZUL, MAGENTA, CIANO };
    vector<string> nomesCores = { "Vermelho", "Verde", "Amarelo", "Azul", "Magenta", "Ciano" };

    for (int i = 0; i < jogadores.size(); i++){
        int opcao = 1;
        char tecla;
        bool escolhendoCor = true;

        while (escolhendoCor){
            limparTela();
            nomeJogo();

            cout << "Escolha a cor para o jogador " << i + 1 << " (" << jogadores[i].nome << "):" << endl;

            cout << "Cores Disponiveis:" << endl;
            for (int j = 0; j < cores.size(); j++){
                cout << (opcao == j + 1 ? "> " : "  ") << nomesCores[j] << endl;
            }
            cout << "Use W/S para navegar e Enter para selecionar." << endl;

            tecla = _getch();

            if (tecla == 'w'){
                opcao = (opcao == 1) ? cores.size() : opcao - 1;
            } else if (tecla == 's'){
                opcao = (opcao == cores.size()) ? 1 : opcao + 1;
            } else if (tecla == 13){
                jogadores[i].cor = cores[opcao - 1];
                cores.erase(cores.begin() + (opcao - 1));
                nomesCores.erase(nomesCores.begin() + (opcao - 1));
                escolhendoCor = false;
            }
        }
    }
}