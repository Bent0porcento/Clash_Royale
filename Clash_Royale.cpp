#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include <locale.h>
#include <cctype>

using json = nlohmann::json;
// Paricido com std, serve pra não ficar escrevendo nlohmann::json toda vez quer quiser usar o tipo json

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

void ImprimirCartas(const json &listadecartas)
// exige que passamos um json para poder ler, no nosso caso é o json com as cartas
{
    std::cout << "--- Catalogo de Cartas do Clash Royale ---" << std::endl
              << std::endl;

    for (const auto &carta : listadecartas) // imprime as carta de acordo com as infomarções que ele puxa do json
    {
        int id = carta["id"]; // guarda cada informação das cartas em uma variavel para imprimir dps
        std::string nome = carta["nome"];
        // talvez daria pra imprimir direto sem colocar em uma variavel mas n tenho certeza
        std::string raridade = carta["raridade"];
        std::string tipo = carta["tipo"];

        std::cout << "ID: " << id << std::endl;
        std::cout << "Nome: " << nome << std::endl; // impressões
        std::cout << "Raridade: " << raridade << std::endl;
        std::cout << "Tipo: " << tipo << std::endl;
        std::cout << "----------------------------------------" << std::endl;
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

// 1- Definir Estrutura

int TamanhoDeck = 8;

struct Deck
{
    json CartasDoDeck[8];
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
    // IMPORTANTE: Inicializa o 'final' para -1 (deck vazio)
    // Isso é crucial para a função de verificação de duplicatas funcionar
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
    contDeck++;
    CalculaPeso(novodeck);
    ImprimirDeck(novodeck);
    listaDeck[contDeck] = novodeck;
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
    CalculaPeso(logbait);
    ImprimirDeck(logbait);
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
    CalculaPeso(Xbesta);
    ImprimirDeck(Xbesta);
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

int main()

{
    json listaDeCartas = CarregarCartas();
    std::cout << "======================================================================" << std::endl;
    std::cout << "          Bem vindo ao gerenciador de decks do Clash Royale           " << std::endl;
    std::cout << "======================================================================" << std::endl;

    std::cout << "/* message */" << std::endl;

    return 0;
}
