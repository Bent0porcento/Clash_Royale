#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include <locale.h>
#include <cctype>

using json = nlohmann::json;
// Paricido com std, serve pra não ficar escrevendo nlohmann::json toda vez quer quiser usar o tipo json

void ClearScreen()
{
    for (int i = 0; i < 50; ++i)
    {
        std::cout << std::endl;
    }
}

json CarregarCartas()
{
    std::ifstream arquivo_json("cartas.json"); // abrir o json

    if (!arquivo_json.is_open()) // verificar se deu certo a abertura
    {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo cartas.json" << std::endl;
        return json();
    }

    json dados_cartas; // parsear as cartas
    try
    {
        arquivo_json >> dados_cartas; // le os dados pra colocar na nossa variavel dados_cartas
    }
    catch (json::parse_error &e) // verificacao para erro na hora do parse
    {
        std::cerr << "Erro de parse no JSON: " << e.what() << std::endl;
        return json();
    }
    return dados_cartas; // retona pra gente o dodos_cartas, equivalente a lista de cartas
}

void ImprimirCarta(const json &carta) // pede somente uma carta e imprime suas informações
{
    std::cout << "============================================================" << std::endl;
    std::cout << "ID: " << carta["id"] << std::endl;
    std::cout << "Nome: " << carta["nome"] << std::endl;
    std::cout << "Elixir: " << carta["elixir"] << std::endl;
    std::cout << "Raridade: " << carta["raridade"] << std::endl;
    if (carta["tipo"] == "t")
    {
        std::cout << "Tipo: Tropa" << std::endl;
    }
    else if (carta["tipo"] == "f")
    {
        std::cout << "Tipo: Feitico" << std::endl;
    }
    else if (carta["tipo"] == "c")
    {
        std::cout << "Tipo: Contrucao" << std::endl;
    }

    std::cout << "============================================================" << std::endl;
}

void ImprimirCartas(const json &listadecartas)
// exige que passamos um json para poder ler, no nosso caso é o json com as cartas
{
    std::cout << "--- Catalogo de Cartas do Clash Royale ---" << std::endl
              << std::endl;

    for (const auto &carta : listadecartas) // imprime as carta de acordo com as infomarções que ele puxa do json
    {
        ImprimirCarta(carta);
    }
}

json ProcurarCarta(const json &listadecartas, std::string n)
// exige a lista de carta e uma string, essa string é o nome da carta que a gente quer
{
    for (const auto &c : listadecartas) // percorre todo o array
    {
        if (c["nome"] == n)
        {
            return c; // quando acha a carta que tem o nome igual, retorna ela saindo do looping
        }
    }
    std::cout << "Impossivel Encontrar sua carta" << std::endl;
    return json(); // se nao achar a carta retorna um jason vazio
}

// 1- Definir Estrutura

const int TamanhoDeck = 8;

struct Deck
{
    json CartasDoDeck[8];
    std::string nome;
    double peso;
    int final = -1;
};

Deck listaDeck[10];
std::string l;
bool sinal;
int aux;
char conf;
int contDeck = 0;

void ImprimirDeck(Deck &deck) // pede um deck e imprime as informações dele
{
    std::cout << "=============================================================" << std::endl;
    for (int i = 0; i < TamanhoDeck; i++)
    {
        if (deck.CartasDoDeck[i].contains("nome"))
        {
            std::cout << "Carta " << i + 1 << ": " << deck.CartasDoDeck[i]["nome"].get<std::string>() << std::endl;
        }
        else
        {
            std::cout << "Carta " << i + 1 << ": X" << std::endl;
        }
    }
    std::cout << "Peso: " << deck.peso << std::endl;
    std::cout << "=============================================================" << std::endl;
}

void CalculaPeso(Deck &deck) // pede um deck e calcula o peso dele
{
    int elixirT = 0;
    double peso;
    double T = 0;
    for (int i = 0; i < 8; i++)
    {
        if (deck.CartasDoDeck[i].contains("elixir"))
        {
            T++;
            elixirT += deck.CartasDoDeck[i]["elixir"].get<int>();
        }
        else
        {
            continue;
        }
        // soma o elixir de cada carta acessando elas pelo array dentro do struct
    } // o programa nn consegue supor que o elexir é do tipo int,
    peso = elixirT / T;
    // mesmo que no json ele esteja como int, por isso que tem q ter um cast pra int com o get<int>
    deck.peso = (std::round(peso * 10.0)) / 10.0;
    // faz com que fique com uma casa decimal, multiplicando por 10 primeiro e arredondando, depois divide pra voltar
}

json VerificarRepitida(std::string nome, Deck &novodeck, const json &listadecartas)
{
    json novaCarta;
    bool cartaOk = false;

    while (!cartaOk)
    {
        novaCarta = ProcurarCarta(listadecartas, nome);
        if (novaCarta.is_null() || novaCarta.empty())
        {
            std::cerr << " A Carta '" << nome << " nao existe, insra o nome novamente: " << std::endl;
            std::getline(std::cin, nome);
            continue;
        }

        bool Repetida = false;
        for (int i = 0; i <= novodeck.final; i++)
        {
            if (novodeck.CartasDoDeck[i].contains("nome") &&
                novodeck.CartasDoDeck[i]["nome"].get<std::string>() == nome)
            {
                Repetida = true;
                break;
            }
        }

        if (Repetida)
        {
            std::cerr << "ERRO: Voce ja possui a carta '" << nome << "' no deck. Tente novamente: " << std::endl;
            std::getline(std::cin, nome);
            continue;
        }
        cartaOk = true;
    }

    return novaCarta;
}

Deck CriarDeck(const json &listadecarta) // passa a lista de carta e começa o processo pra criar o deck
{
    Deck novodeck;
    novodeck.final = -1;

    std::string nome;

    std::cout << "========== Comecando Montagem de Deck ==========" << std::endl;

    for (int i = 0; i < TamanhoDeck; i++)
    {
        std::cout << "Informe o nome da carta " << i + 1 << " do seu novo deck: " << std::endl;
        std::getline(std::cin, nome); // pega a linha toda
        json cartaValidada = VerificarRepitida(nome, novodeck, listadecarta);
        novodeck.CartasDoDeck[i] = cartaValidada;
        novodeck.final = i;
    }
    CalculaPeso(novodeck);
    ImprimirDeck(novodeck);
    std::cout << "Como gostaria de chamar seu novo deck?" << std::endl;
    std::getline(std::cin, nome);
    novodeck.nome = nome;
    listaDeck[contDeck] = novodeck;
    contDeck++;
    return novodeck;
}

Deck GerarLogBait(const json &listadecartas)
{
    Deck logbait;
    logbait.CartasDoDeck[0] = ProcurarCarta(listadecartas, "cavaleiro");
    logbait.CartasDoDeck[1] = ProcurarCarta(listadecartas, "barril de goblins");
    logbait.CartasDoDeck[2] = ProcurarCarta(listadecartas, "torre inferno");
    logbait.CartasDoDeck[3] = ProcurarCarta(listadecartas, "foguete");
    logbait.CartasDoDeck[4] = ProcurarCarta(listadecartas, "o tronco");
    logbait.CartasDoDeck[5] = ProcurarCarta(listadecartas, "princesa");
    logbait.CartasDoDeck[6] = ProcurarCarta(listadecartas, "gangue de goblins");
    logbait.CartasDoDeck[7] = ProcurarCarta(listadecartas, "espirito de gelo");
    logbait.final = TamanhoDeck - 1;
    logbait.nome = "logbait";
    CalculaPeso(logbait);
    listaDeck[contDeck] = logbait;
    contDeck++;

    return logbait;
}

Deck GerarXBesta(const json &listadecartas)
{
    Deck Xbesta;
    Xbesta.CartasDoDeck[0] = ProcurarCarta(listadecartas, "cavaleiro");
    Xbesta.CartasDoDeck[1] = ProcurarCarta(listadecartas, "x-besta");
    Xbesta.CartasDoDeck[2] = ProcurarCarta(listadecartas, "tesla");
    Xbesta.CartasDoDeck[3] = ProcurarCarta(listadecartas, "mago de gelo");
    Xbesta.CartasDoDeck[4] = ProcurarCarta(listadecartas, "o tronco");
    Xbesta.CartasDoDeck[5] = ProcurarCarta(listadecartas, "foguete");
    Xbesta.CartasDoDeck[6] = ProcurarCarta(listadecartas, "esqueletos");
    Xbesta.CartasDoDeck[7] = ProcurarCarta(listadecartas, "tornado");
    Xbesta.final = TamanhoDeck - 1;
    Xbesta.nome = "X-Besta";
    CalculaPeso(Xbesta);
    listaDeck[contDeck] = Xbesta;
    contDeck++;

    return Xbesta;
}

// 2 Remover em K

int VerificacaoString(std::string l)
{
    int k;
    while (true)
    {
        try
        {
            k = std::stoi(l);
            break;
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Por favor, informe um valor valido: " << std::endl;
            std::cin >> l;
        }
    }
    return k;
}

json verificarVoid(std::string &nome, const json &listadecartas)
{
    json novaCarta = ProcurarCarta(listadecartas, nome);
    while (novaCarta.is_null() || novaCarta.empty())
    {
        std::cerr << "Insira o nome da carta que deseja inserir novamente: " << std::endl;
        std::getline(std::cin, nome);
        novaCarta = ProcurarCarta(listadecartas, nome);
    }
    return novaCarta;
}

bool RemoverEmK(Deck &deck)
{
    sinal = false;
    std::string l;
    if (deck.final < 0)
    {
        std::cout << "Deck nao existe" << std::endl;
        return sinal;
    }
    std::cout << "Informe uma posicao para remover" << std::endl;
    std::cin >> l;
    int k = VerificacaoString(l);

    while (true)
    {
        if (k < 1 || k > deck.final + 1)
        {
            std::cout << "Favor, inserir um numero de 1 a " << deck.final + 1 << ": " << std::endl;
            std::cin >> l;
            k = VerificacaoString(l);
        }
        else
        {
            break;
        }
    }

    std::cout << "Confirmar retirada da carta " << k << "\n"
              << " Nome: " << deck.CartasDoDeck[k - 1]["nome"].get<std::string>() << "\n"
              << " Elixir: " << deck.CartasDoDeck[k - 1]["elixir"].get<int>()
              << " do seu deck? (S/n)" << std::endl;
    std::cin >> conf;
    if (conf == 'S')
    {
        aux = k - 1;
        while (aux <= deck.final)
        {
            deck.CartasDoDeck[aux] = deck.CartasDoDeck[aux + 1];
            aux++;
        }
        deck.final--;
        sinal = true;
        CalculaPeso(deck);
        ImprimirDeck(deck);
        // TODO implementar CalculaPeso(deck) propriamente
    }
    else
    {
        std::cout << "Remocao nagada" << std::endl;
    }
    return sinal;
}

int ProcurarNO(std::string &k, const Deck &deckProcurado)
{
    for (int i = 0; i <= deckProcurado.final; i++)
    {
        if (deckProcurado.CartasDoDeck[i]["nome"] == k)
        {
            return i;
        }
    }
    std::cout << "Nome inserido nao existe no deck" << std::endl;
    return -1;
}

bool AlterarPosteriorK(Deck &deck, const json &listadecartas)
{
    std::string nome;
    sinal = false;
    if (deck.final > -1)
    {
        std::cout << "Insira o nome da carta que deseja procurar" << std::endl;
        std::getline(std::cin, nome);
        int k = ProcurarNO(nome, deck);
        while (true)
        {
            if (k == 0)
            {
                std::cout << "Por favor, insira o nome novamente: " << std::endl;
                std::getline(std::cin, nome);
                k = ProcurarNO(nome, deck);
            }
            else if (k == TamanhoDeck)
            {
                std::cout << "Carta encontrada, porem nao e impossivel altarar a carta posteiror, isira o nome novamnte: " << std::endl;
                std::getline(std::cin, nome);
                k = ProcurarNO(nome, deck);
            }
            else
            {
                break;
            }
        }
        std::cout << "Carta encontrada: " << deck.CartasDoDeck[k]["nome"] << std::endl;
        std::cout << "Deseja prosseguir com a alteracao da carta posterior: " << deck.CartasDoDeck[k + 1]["nome"] << " posicao: " << k + 1 << "? (S/n)" << std::endl;
        std::cin >> conf;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (conf == 'S')
        {
            std::cout << "Por favor, iforme o nome da carta que deseja colocar no lugar" << std::endl;
            std::getline(std::cin, nome);
            json novaCarta = ProcurarCarta(listadecartas, nome);
            novaCarta = verificarVoid(nome, listadecartas);

            deck.CartasDoDeck[k + 1] = novaCarta;
            CalculaPeso(deck);
            ImprimirDeck(deck);
            sinal = true;
        }
        else
        {
            std::cout << "Alteracao negada" << std::endl;
        }
    }
    return sinal;
}

bool InserirposteriorK(Deck &deck, const json &listadecartas)
{
    sinal = false;

    if (deck.final >= TamanhoDeck - 1)
    {
        std::cout << "Overflow!!! O deck ja esta cheio. Nao e possivel inserir." << std::endl;
        return false;
    }

    if (deck.final < 0)
    {
        std::cout << "deck nao existe" << std::endl;
        return false;
    }

    std::string nome;
    std::cout << "Informe a carta que deseja procurar: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, nome);
    int k = ProcurarNO(nome, deck);

    while (k == -1)
    {
        std::cout << "Por favor, insira o nome novamente: " << std::endl;
        std::getline(std::cin, nome);
        k = ProcurarNO(nome, deck);
    }

    std::cout << "Carta encontrada: " << deck.CartasDoDeck[k]["nome"] << std::endl;
    std::cout << "Deseja prosseguir com a insercao na posicao posterior a " << k + 1 << "? (S/n)" << std::endl;
    std::cin >> conf;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer

    if (toupper(conf) == 'S')
    {
        std::cout << "Insira o nome da carta que deseja inserir: " << std::endl;
        std::getline(std::cin, nome);
        json novaCarta = VerificarRepitida(nome, deck, listadecartas);

        for (int i = deck.final; i >= k + 1; i--)
        {
            deck.CartasDoDeck[i + 1] = deck.CartasDoDeck[i];
        }

        deck.CartasDoDeck[k + 1] = novaCarta;
        deck.final++;
        sinal = true;
        CalculaPeso(deck);
        ImprimirDeck(deck);
    }
    else
    {
        std::cout << "Insercao cancelada." << std::endl;
    }

    return sinal;
}

bool imprimirNoAnterior(Deck &deck, const json &listadecartas)
{
    std::string nome;
    sinal = false;
    if (deck.final < 0)
    {
        std::cout << "deck nao existe" << std::endl;
        return sinal;
    }

    std::cout << "Insira o nome da carta que gostaria de verificar: " << std::endl;
    std::getline(std::cin, nome);
    int k = ProcurarNO(nome, deck);
    if (k == -1)
    {
        std::cout << "Gostaria de iserir o nome novamnte? (S/n)" << std::endl;
        std::cin >> conf;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (conf == 'S')
        {
            std::cout << "Insira o nome da carta que gostaria de verificar: " << std::endl;
            std::getline(std::cin, nome);
            int k = ProcurarNO(nome, deck);
            if (k == -1)
            {
                std::cout << "Gostaria de iserir o nome novamnte? (S/n)" << std::endl;
                std::cin >> conf;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
            {
                break;
            }
        }
        if (conf != 'S')
        {
            std::cout << "encerrando busca..." << std::endl;
            return sinal = true;
        }
    }
    std::cout << "Carta encontrada!" << std::endl;
    std::cout << "Imprimindo conteudo da carta anterior: " << std::endl;
    ImprimirCarta(deck.CartasDoDeck[k - 1]);
}

void Imprimir7(const json &listadecartas)
{
    {
        for (const auto &carta : listadecartas) // imprime as carta de acordo com as infomarções que ele puxa do json
        {
            if (carta["elixir"] > 7)
            {
                ImprimirCarta(carta);
            }
        }
    }
}

bool Raridade(Deck &deck, const json &listadecartas)
{
    std::string k;
    sinal = false;
    int posicao[TamanhoDeck];
    json CartasEncontradas[TamanhoDeck];
    int contcarta = 0;
    if (deck.final < -1)
    {
        std::cout << "Deck inexistente" << std::endl;
        return sinal;
    }

    std::cout << "Digite uma raridade para procurar: " << std::endl;
    std::cin >> k;

    for (int i = 0; i <= deck.final; i++)
    {
        if (deck.CartasDoDeck[i]["raridade"] == k)
        {
            CartasEncontradas[contcarta] = deck.CartasDoDeck[i];
            posicao[contcarta] = i;
            contcarta++;
        }
    }
    if (contcarta == 0)
    {
        std::cout << "Nenhuma carta dessa raridade foi encontrada no deck" << std::endl;
        return sinal = true;
    }
    else
    {
        std::cout << "Cartas Encontradas: " << std::endl;
        for (int i = 0; i < contcarta; i++)
        {
            ImprimirCarta(CartasEncontradas[i]);
            std::cout << "Posicao: " << posicao[i] + 1 << std::endl;
        }
    }

    std::cout << "\nEscolha uma carta para fazer a alteracao na posicao posterior: " << std::endl;
    std::cin >> k;
    int l = VerificacaoString(k);
    int alterada = posicao[l - 1] + 1;
    std::cout << "Deseja continuar com a alteracao da carta: " << std::endl;
    ImprimirCarta(deck.CartasDoDeck[alterada]);
    std::cout << "(S/n)?" << std::endl;
    std::cin >> conf;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (conf == 'S')
    {
        std::cout << "Digite o nome da nova carta: " << std::endl;
        std::getline(std::cin, k);
        json novacarta = ProcurarCarta(listadecartas, k);
        deck.CartasDoDeck[alterada] = novacarta;
        CalculaPeso(deck);
        ImprimirDeck(deck);
        sinal = true;
        return sinal;
    }
    else
    {
        std::cout << "Alteracao negada" << std::endl;
        return sinal;
    }
}

int main()

{
    std::string op;
    json listaDeCartas = CarregarCartas();
    Deck bait = GerarLogBait(listaDeCartas);
    Deck besta = GerarXBesta(listaDeCartas);
    std::cout << "======================================================================" << std::endl;
    std::cout << "          Bem vindo ao gerenciador de decks do Clash Royale           " << std::endl;
    std::cout << "======================================================================" << std::endl;

    std::cout << "1) Remover uma Carta" << std::endl;
    std::cout << "2) Procurar uma Carta e alterar a carta anterior" << std::endl;
    std::cout << "3) Procurar uma Carta e inserir na posicao anterior" << std::endl;
    std::cout << "4) Procurar uma carta por nome e imprmir a anterior" << std::endl;
    std::cout << "5) Imprimir quantas cartas custam mais que 7 de elixir" << std::endl;
    std::cout << "6) Procurar uma carta pela raridade e alterar a carta posterior" << std::endl;
    std::cout << "7) Classificar um deck por tamanho de nome" << std::endl;
    std::cout << "8) Imprimir um deck ou a lsita de cartas" << std::endl;
    std::cout << "9) Imprimir todas as cartas que custam menos que 4 e mais que 8 de elixir" << std::endl;
    std::cout << "10) Procurar cartas por tipo" << std::endl;
    std::cout << "11) Inserir uma carta carta na posicao X+2" << std::endl;
    std::cout << "12) Procurar uma carta por nome e remover na posicao K-2" << std::endl;
    std::cout << "13) Classificar um deck por ordem crescente de raridade" << std::endl;
    std::cin >> op;

    switch (std::stoi(op))
    {
    case 1:
        system("cls");
        std::cout << "\nEscolha o deck que deseja executar a acao: " << std::endl;
        std::cout << "------------------------------" << std::endl;
        for (int i = 0; i < contDeck; i++)
        {
            std::cout << "Deck " << i + 1 << ": " << listaDeck[i].nome << std::endl;
        }
        std::cout << "------------------------------" << std::endl;

        break;

    case 5:
        system("cls");
        std::cout << "\n"
                  << std::endl;
        Imprimir7(listaDeCartas);
        break;
    case 6:
        system("cls");
        std::cout << "\n"
                  << std::endl;
        Raridade(bait, listaDeCartas);

    default:
        break;
    }

    return 0;
}
