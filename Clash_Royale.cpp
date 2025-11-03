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

void ImprimirCarta(const json &carta)
{
    std::cout << "============================================================" << std::endl;

    if (carta.contains("id") && carta["id"].is_number())
    {
        std::cout << "ID: " << carta["id"].get<int>() << std::endl;
    }
    else
    {
        std::cout << "ID: [INVALIDO]" << std::endl;
    }

    if (carta.contains("nome") && carta["nome"].is_string())
    {
        std::cout << "Nome: " << carta["nome"].get<std::string>() << std::endl;
    }
    else
    {
        std::cout << "Nome: [INVALIDO OU TIPO OBJETO]" << std::endl;
    }
    if (carta.contains("elixir") && carta["elixir"].is_number())
    {
        std::cout << "Elixir: " << carta["elixir"].get<int>() << std::endl;
    }
    else
    {
        std::cout << "Elixir: [INVALIDO]" << std::endl;
    }
    if (carta.contains("raridade") && carta["raridade"].is_string())
    {
        std::cout << "Raridade: " << carta["raridade"].get<std::string>() << std::endl;
    }
    else
    {
        std::cout << "Raridade: [INVALIDA]" << std::endl;
    }

    if (carta.contains("tipo") && carta["tipo"].is_string())
    {
        std::string tipo = carta["tipo"].get<std::string>();
        if (tipo == "t")
        {
            std::cout << "Tipo: Tropa" << std::endl;
        }
        else if (tipo == "f")
        {
            std::cout << "Tipo: Feitico" << std::endl;
        }
        else if (tipo == "c")
        {
            std::cout << "Tipo: Construcao" << std::endl;
        }
        else
        {
            std::cout << "Tipo: [DESCONHECIDO]" << std::endl;
        }
    }
    else
    {
        std::cout << "Tipo: [INVALIDO]" << std::endl;
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
    std::string tl;
    for (auto letras : n)
    {
        tl += std::tolower(letras);
    }

    for (const auto &c : listadecartas) // percorre todo o array
    {
        if (c["nome"] == tl)
        {
            return c; // quando acha a carta que tem o nome igual, retorna ela saindo do looping
        }
    }
    std::cout << "Impossivel Encontrar sua carta" << std::endl;
    return json(); // se nao achar a carta retorna um jason vazio
}

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
Deck deckgenerico;

Deck EscolherDeck()
{

    std::string escolha;
    int op;

    std::cout << "\nEscolha o deck que deseja executar a acao: " << std::endl;
    std::cout << "------------------------------" << std::endl;
    for (int i = 0; i < contDeck; i++)
    {

        std::cout << "Deck " << i + 1 << ": " << listaDeck[i].nome << std::endl;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "------------------------------" << std::endl;
    std::getline(std::cin, escolha);
    op = VerificacaoString(escolha);

    while (op > contDeck)
    {
        std::cout << "Insira um valor valido: ";
        std::getline(std::cin, escolha);
        op = VerificacaoString(escolha);
    }

    system("cls");
    std::cout << listaDeck[op - 1].nome << std::endl;
    return listaDeck[op - 1];
}

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

void ImprimirDeckcomRaridade(Deck &deck) // pede um deck e imprime as informações dele
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

        if (deck.CartasDoDeck[i].contains("raridade"))
        {
            std::cout << "Raridade: " << deck.CartasDoDeck[i]["raridade"].get<std::string>() << std::endl;
        }
        else
        {
            std::cout << "Raridade: X" << std::endl;
        }
        std::cout << std::endl;
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
            std::cerr << " A Carta '" << nome << "' nao existe, insra o nome novamente: " << std::endl;
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
            std::cerr << "Voce ja possui a carta '" << nome << "' no deck, escolha outra carta: " << std::endl;
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

void InserirDeck(Deck &deck)
{
    for (int i = 0; i < contDeck; i++)
    {
        if (listaDeck[i].nome == deck.nome)
        {
            listaDeck[i] = deck;
        }
    }
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
    logbait.nome = "LogBait";
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

Deck GerarGGreal(const json &listadecartas)
{
    Deck GG;
    GG.CartasDoDeck[0] = ProcurarCarta(listadecartas, "gigante real");
    GG.CartasDoDeck[1] = ProcurarCarta(listadecartas, "monge");
    GG.CartasDoDeck[2] = ProcurarCarta(listadecartas, "pescador");
    GG.CartasDoDeck[3] = ProcurarCarta(listadecartas, "bruxa mae");
    GG.CartasDoDeck[4] = ProcurarCarta(listadecartas, "o tronco");
    GG.CartasDoDeck[5] = ProcurarCarta(listadecartas, "fenix");
    GG.CartasDoDeck[6] = ProcurarCarta(listadecartas, "goblins");
    GG.CartasDoDeck[7] = ProcurarCarta(listadecartas, "relampago");
    GG.final = TamanhoDeck - 1;
    GG.nome = "Gigante Real";
    CalculaPeso(GG);
    listaDeck[contDeck] = GG;
    contDeck++;

    return GG;
}

Deck GerarSplash(const json &listadecartas)
{
    Deck Splash;
    Splash.CartasDoDeck[0] = ProcurarCarta(listadecartas, "cabana de goblins");
    Splash.CartasDoDeck[1] = ProcurarCarta(listadecartas, "valquiria");
    Splash.CartasDoDeck[2] = ProcurarCarta(listadecartas, "cemiterio");
    Splash.CartasDoDeck[3] = ProcurarCarta(listadecartas, "mago de gelo");
    Splash.CartasDoDeck[4] = ProcurarCarta(listadecartas, "veneno");
    Splash.CartasDoDeck[5] = ProcurarCarta(listadecartas, "barril de barbaro");
    Splash.CartasDoDeck[6] = ProcurarCarta(listadecartas, "bebe dragao");
    Splash.CartasDoDeck[7] = ProcurarCarta(listadecartas, "vinhas");
    Splash.final = TamanhoDeck - 1;
    Splash.nome = "SplashYard";
    CalculaPeso(Splash);
    listaDeck[contDeck] = Splash;
    contDeck++;

    return Splash;
}

Deck GerarHog(const json &listadecartas)
{
    Deck hog;
    hog.CartasDoDeck[0] = ProcurarCarta(listadecartas, "espirito de gelo");
    hog.CartasDoDeck[1] = ProcurarCarta(listadecartas, "corredor");
    hog.CartasDoDeck[2] = ProcurarCarta(listadecartas, "esqueletos");
    hog.CartasDoDeck[3] = ProcurarCarta(listadecartas, "golem de gelo");
    hog.CartasDoDeck[4] = ProcurarCarta(listadecartas, "canhao");
    hog.CartasDoDeck[5] = ProcurarCarta(listadecartas, "o tronco");
    hog.CartasDoDeck[6] = ProcurarCarta(listadecartas, "mosqueteira");
    hog.CartasDoDeck[7] = ProcurarCarta(listadecartas, "bola de fogo");
    hog.final = TamanhoDeck - 1;
    hog.nome = "Hog 2.6";
    CalculaPeso(hog);
    listaDeck[contDeck] = hog;
    contDeck++;

    return hog;
}

Deck GerarGalego(const json &listadecartas)
{
    Deck ga;
    ga.CartasDoDeck[0] = ProcurarCarta(listadecartas, "golem de elixir");
    ga.CartasDoDeck[1] = ProcurarCarta(listadecartas, "torre inferno");
    ga.CartasDoDeck[2] = ProcurarCarta(listadecartas, "mago eletrico");
    ga.CartasDoDeck[3] = ProcurarCarta(listadecartas, "gelo");
    ga.CartasDoDeck[4] = ProcurarCarta(listadecartas, "flecha");
    ga.CartasDoDeck[5] = ProcurarCarta(listadecartas, "bombardeiro");
    ga.CartasDoDeck[6] = ProcurarCarta(listadecartas, "pirotecnica");
    ga.CartasDoDeck[7] = ProcurarCarta(listadecartas, "mega cavaleiro");
    ga.final = TamanhoDeck - 1;
    ga.nome = "Hog 2.6";
    CalculaPeso(ga);
    listaDeck[contDeck] = ga;
    contDeck++;

    return ga;
}

// 2 Remover em K

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
    std::cout << "Informe uma posicao para remover: " << std::endl;
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

    std::cout << "Confirmar retirada da carta: " << std::endl;
    std::cout << "Nome: " << deck.CartasDoDeck[k - 1]["nome"] << std::endl;
    std::cout << "Raridade: " << deck.CartasDoDeck[k - 1]["raridade"] << std::endl;
    std::cout << "Elixir: " << deck.CartasDoDeck[k - 1]["elixir"] << std::endl;
    std::cout << "(S/n)" << std::endl;
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
        InserirDeck(deck);
    }
    else
    {
        std::cout << "Remocao negada" << std::endl;
    }
    return sinal;
}

int ProcurarNO(std::string &k, const Deck &deckProcurado)
{
    std::string l;
    for (auto letra : k)
    {
        l += std::tolower(letra);
    }

    for (int i = 0; i <= deckProcurado.final; i++)
    {
        if (deckProcurado.CartasDoDeck[i]["nome"] == l)
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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Insira o nome da carta que deseja procurar" << std::endl;
        std::getline(std::cin, nome);
        int k = ProcurarNO(nome, deck);
        while (true)
        {
            if (k == 7)
            {
                std::cout << "Impossivel alterar posicao posterior, insira o nome novamente: " << std::endl;
                std::getline(std::cin, nome);
                k = ProcurarNO(nome, deck);
            }
            else if (k == -1)
            {
                std::cout << "Insira o nome novamente:" << std::endl;
                std::getline(std::cin, nome);
                k = ProcurarNO(nome, deck);
            }
            else
            {
                break;
            }
        }
        std::cout << "Carta encontrada: " << deck.CartasDoDeck[k]["nome"] << std::endl;
        std::cout << "Deseja prosseguir com a alteracao da carta posterior: " << deck.CartasDoDeck[k + 1]["nome"] << " posicao: " << k + 2 << "? (S/n)" << std::endl;
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
            InserirDeck(deck);

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
        InserirDeck(deck);
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

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
    int cont = 0;
    {
        for (const auto &carta : listadecartas) // imprime as carta de acordo com as infomarções que ele puxa do json
        {
            if (carta["elixir"] > 7)
            {
                ImprimirCarta(carta);
                cont++;
            }
        }
    }
    std::cout << "Quantidade de cartas: " << cont << std::endl;
}
void Imprimir48(const json &listadecartas)
{
    {
        for (const auto &carta : listadecartas) // imprime as carta de acordo com as infomarções que ele puxa do json
        {
            if (carta["elixir"] > 4 && carta["elixir"] < 8)
            {
                ImprimirCarta(carta);
            }
        }
    }
}

// TODO
void Impirmirtipo(const json &listadecartas)
{
    std::string a;
    std::string tipo;
    std::string chartropa;
    int b;

    while (true)
    {
        std::cout << "Digite um tipo de carta para filtrar (Tropa, Feitico ou Construcao): " << std::endl;
        std::getline(std::cin, a);
        tipo.clear();
        for (auto letras : a)
        {
            tipo += tolower(static_cast<unsigned char>(letras));
        }

        if (tipo == "tropa" || tipo == "feitico" || tipo == "construcao")
        {
            break;
        }
        else
        {
            system("cls");
            std::cout << "ERRO: '" << a << "' nao e um tipo valido. Tente novamente." << std::endl;
        }
    }

    if (tipo == "tropa")
    {
        chartropa = "t";
    }
    else if (tipo == "feitico")
    {
        chartropa = "f";
    }
    else if (tipo == "construcao")
    {
        chartropa = "c";
    }
    std::cout << "Tipo encontrado: " << tipo << std::endl;
    std::cout << "O que deseja imprimir, um Deck (1), ou todas as cartas (2): " << std::endl;
    while (true)
    {
        std::getline(std::cin, a);
        try
        {
            b = std::stoi(a);
            if (b == 1 || b == 2)
            {
                break;
            }
            else
            {
                std::cout << "Opcao invalida. Digite 1 (Deck) ou 2 (Todas): " << std::endl;
            }
        }
        catch (const std::exception &)
        {
            std::cout << "Entrada invalida. Digite 1 ou 2: " << std::endl;
        }
    }

    if (b == 1)
    {
        deckgenerico = EscolherDeck();
        for (int i = 0; i <= deckgenerico.final; i++)
        {

            if (deckgenerico.CartasDoDeck[i].contains("tipo") &&
                deckgenerico.CartasDoDeck[i]["tipo"].is_string() &&
                deckgenerico.CartasDoDeck[i]["tipo"].get<std::string>() == chartropa)
            {
                ImprimirCarta(deckgenerico.CartasDoDeck[i]);
            }
        }
    }
    else if (b == 2)
    {
        for (const auto &c : listadecartas)
        {

            if (c.contains("tipo") &&
                c["tipo"].is_string() &&
                c["tipo"].get<std::string>() == chartropa)
            {
                ImprimirCarta(c);
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
    while (alterada == 8)
    {
        std::cout << "Nao e possivel acessar posicao posterior, escolha outra carta" << std::endl;
        std::cin >> k;
        int l = VerificacaoString(k);
        alterada = posicao[l - 1] + 1;
    }

    while (true)
    {
        if (!deck.CartasDoDeck[alterada].contains("nome"))
        {
            std::cout << "Nao e possivel acessar posicao posterior, escolha outra carta" << std::endl;
            std::cin >> k;
            int l = VerificacaoString(k);
            alterada = posicao[l - 1] + 1;
        }
        else
        {
            break;
        }
    }

    std::cout << "Deseja continuar com a alteracao da carta: " << std::endl;
    ImprimirCarta(deck.CartasDoDeck[alterada]);
    std::cout << "(S/n)?" << std::endl;
    std::cin >> conf;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (conf == 'S')
    {
        std::cout << "Digite o nome da nova carta: " << std::endl;
        std::getline(std::cin, k);
        json novacarta = VerificarRepitida(k, deck, listadecartas);
        deck.CartasDoDeck[alterada] = novacarta;
        CalculaPeso(deck);
        ImprimirDeck(deck);
        InserirDeck(deck);

        sinal = true;
        return sinal;
    }
    else
    {
        std::cout << "Alteracao negada" << std::endl;
        return sinal;
    }
}

bool ClassificarNome(Deck &deck)
{
    std::cout << "Deck antes da cassificacao:7" << std::endl;
    ImprimirDeck(deck);
    int contletra = 0;
    int tamanho[TamanhoDeck];
    aux = 0;
    for (int i = 0; i <= deck.final; i++)
    {
        for (auto letras : deck.CartasDoDeck[i]["nome"].get<std::string>())
        {
            if (letras == ' ' || letras == ' ')
            {
                continue;
            }
            else
            {
                contletra++;
            }
        }
        tamanho[i] = contletra;
        contletra = 0;
    }

    for (int i = 0; i <= deck.final; i++)
    {
        for (int j = 0; j < (deck.final - i); j++)
        {
            if (tamanho[j] > tamanho[j + 1])
            {
                std::swap(tamanho[j], tamanho[j + 1]);
                std::swap(deck.CartasDoDeck[j], deck.CartasDoDeck[j + 1]);
            }
        }
    }
    std::cout << "\nDeck apos classificacao: " << std::endl;
    ImprimirDeck(deck);
    InserirDeck(deck);
}

void GerarDecksProntos(const json &listadecartas)
{
    std::string escolha;
    int op;

    std::cout << "===========Decks Prontos===========" << std::endl;
    std::cout << "1) X-Besta" << std::endl;
    std::cout << "2) Log Bait" << std::endl;
    std::cout << "3) Gigante Real" << std::endl;
    std::cout << "4) SplashYard" << std::endl;
    std::cout << "5) Hog 2.6" << std::endl;
    std::cout << "6) Peitinho do Galego" << std::endl;
    std::cout << "\nEscolha sua opcao: ";
    std::getline(std::cin, escolha);
    op = VerificacaoString(escolha);
    while (op == -1)
    {
        std::cout << "Insira um valor valido: ";
        std::getline(std::cin, escolha);
        op = VerificacaoString(escolha);
    }
    switch (op)
    {
    case 1:
        GerarXBesta(listadecartas);
        std::cout << "Opcao escolhida!" << std::endl;
        std::cout << "Deck de X-besta gerado!" << std::endl;
        break;
    case 2:
        GerarLogBait(listadecartas);
        std::cout << "Opcao escolhida!" << std::endl;
        std::cout << "Deck de Log Bait gerado!" << std::endl;
        break;
    case 3:
        GerarGGreal(listadecartas);
        std::cout << "Opcao escolhida!" << std::endl;
        std::cout << "Deck de Gigante Real gerado!" << std::endl;
        break;
    case 4:
        GerarSplash(listadecartas);
        std::cout << "Opcao escolhida!" << std::endl;
        std::cout << "Deck de SplashYard gerado!" << std::endl;
        break;
    case 5:
        GerarHog(listadecartas);
        std::cout << "Opcao escolhida!" << std::endl;
        std::cout << "Deck de 2.6 gerado!" << std::endl;
        break;
    case 6:
        GerarGalego(listadecartas);
        std::cout << "Opcao escolhida!" << std::endl;
        std::cout << "Deck do  Peitinho do Galego gerado!" << std::endl;
        break;
    default:
        std::cout << "Insira um valor de 1 a 5" << std::endl;
        break;
    }
}

bool ClassicarRaridade(Deck &deck)
{
    std::cout << "Deck antes da cassificacao:7" << std::endl;
    ImprimirDeckcomRaridade(deck);
    aux = 0;
    enum Raridade
    {
        COMUM,
        RARA,
        EPICA,
        LENDARIA,
        CAMPEAO
    };
    Raridade raridades[TamanhoDeck];

    for (int i = 0; i <= deck.final; i++)
    {
        if (deck.CartasDoDeck[i]["raridade"] == "comum")
        {
            raridades[i] = COMUM;
        }
        else if (deck.CartasDoDeck[i]["raridade"] == "rara")
        {
            raridades[i] = RARA;
        }
        else if (deck.CartasDoDeck[i]["raridade"] == "epica")
        {
            raridades[i] = EPICA;
        }
        else if (deck.CartasDoDeck[i]["raridade"] == "lendaria")
        {
            raridades[i] = LENDARIA;
        }
        else
        {
            raridades[i] = CAMPEAO;
        }
    }

    for (int i = 0; i <= deck.final; i++) // troca
    {
        for (int j = 0; j < (deck.final - i); j++)
        {
            if (raridades[j] > raridades[j + 1])
            {
                std::swap(raridades[j], raridades[j + 1]);
                std::swap(deck.CartasDoDeck[j], deck.CartasDoDeck[j + 1]);
            }
        }
    }
    std::cout << "\nDeck apos classificacao: " << std::endl;
    ImprimirDeckcomRaridade(deck);
    InserirDeck(deck);
}

bool InserirK2(const json &listadecartas)
{
    int b;
    std::string a;
    sinal = false;
    deckgenerico = EscolherDeck();
    if (deckgenerico.final < -1)
    {
        std::cout << "Deck não existe" << std::endl;
        return sinal;
    }
    if (deckgenerico.final == TamanhoDeck - 1)
    {
        std::cout << "Overflow!!, impossivel inserir em um deck cheio" << std::endl;
        return sinal;
    }

    ImprimirDeck(deckgenerico);
    std::cout << "Informe a posicao que deseja inserir: " << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true)
    {
        std::getline(std::cin, a);
        b = VerificacaoString(a);
        if (b > 0 && b <= 6)
        {
            break;
        }
        else
        {
            std::cout << "Impossivel acessar a posicao desejada " << std::endl;
        }
    }

    std::cout << "Posicao valida" << std::endl;
    std::cout << "Deseja prosseguir com a insercao na posicao " << b << " + 2? (S/n)" << std::endl;
    std::cout << "Posicao final: " << b + 2 << std::endl;
    std::cin >> conf;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer

    if (toupper(conf) == 'S')
    {
        std::cout << "Insira o nome da carta que deseja inserir: " << std::endl;
        std::getline(std::cin, a);
        json novaCarta = VerificarRepitida(a, deckgenerico, listadecartas);

        for (int i = deckgenerico.final; i >= b + 1; i--)
        {
            deckgenerico.CartasDoDeck[i + 1] = deckgenerico.CartasDoDeck[i];
        }

        deckgenerico.CartasDoDeck[b + 1] = novaCarta;
        deckgenerico.final++;
        sinal = true;
        CalculaPeso(deckgenerico);
        ImprimirDeck(deckgenerico);
        InserirDeck(deckgenerico);
    }
    else
    {
        std::cout << "Insercao cancelada." << std::endl;
    }

    return sinal;
}

bool ProcurarRemover2(const json &listadecartas)
{
    int posicao;
    std::string a;
    sinal = false;
    deckgenerico = EscolherDeck();
    if (deckgenerico.final < -1)
    {
        std::cout << "Deck não existe" << std::endl;
        return sinal;
    }
    ImprimirDeck(deckgenerico);
    std::cout << "Informe o nome da carta que deseja procurar para fazer a remocao na posicao posterior + 2: " << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, a);
    posicao = ProcurarNO(a, deckgenerico);

    while (true)
    {
        if (posicao == -1)
        {
            std::cout << "Por favor, insira o nome novamente" << std::endl;
            std::getline(std::cin, a);
            posicao = ProcurarNO(a, deckgenerico);
        }
        else if (posicao + 2 > 7)
        {
            std::cout << "Impossivel remover na posicao, insira o nome novamnete" << std::endl;
            std::getline(std::cin, a);
            posicao = ProcurarNO(a, deckgenerico);
        }
        else
        {
            break;
        }
    }

    std::cout << "Carta encontrada" << std::endl;
    std::cout << "Deseja prosseguir com a remocao na posicao " << posicao + 1 << " + 2? (S/n)" << std::endl;
    std::cout << "Posicao final: " << posicao + 3 << std::endl;
    std::cout << "Carta:\n ";
    ImprimirCarta(deckgenerico.CartasDoDeck[posicao + 2]);
    std::cin >> conf;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer

    if (toupper(conf) == 'S')
    {
        aux = posicao + 2;

        while (aux <= deckgenerico.final)
        {
            deckgenerico.CartasDoDeck[aux] = deckgenerico.CartasDoDeck[aux + 1];
            aux++;
        }
        deckgenerico.final--;
        sinal = true;
        CalculaPeso(deckgenerico);
        ImprimirDeck(deckgenerico);
        InserirDeck(deckgenerico);
    }
    else
    {
        std::cout << "Remocao cancelada." << std::endl;
    }

    return sinal;
}

int GerarInterface()
{
    int op;
    json listaDeCartas = CarregarCartas();

    while (true)
    {
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
        std::cout << "14) Criar um deck" << std::endl;
        std::cout << "15) Gerar decks prontos" << std::endl;
        std::cout << "0) Sair" << std::endl;
        std::cout << "\nEscolha uma opcao: ";

        std::cin >> op;

        while (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Digite um valor valido: ";
            std::cin >> op;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        system("cls");
        if (contDeck == 0 && op != 14 && op != 15 && op != 0)
        {
            std::cout << "----------------------------------------------" << std::endl;
            std::cout << "Voce ainda nao possui nenhum Deck!" << std::endl;
            std::cout << "Escolha a opcao 14) para criar um deck \nou 15) para gerar decks ja prontos" << std::endl;
            std::cout << "---------------------------------------------" << std::endl;
            std::cout << "\n\nPressione [ENTER] para voltar ao menu principal..." << std::endl;
            std::cin.get();
            system("cls");
            std::cin.clear();
            continue;
        }
        switch (op)
        {
        case 1:
            deckgenerico = EscolherDeck();
            ImprimirDeck(deckgenerico);
            RemoverEmK(deckgenerico);
            break;

        case 2:
            deckgenerico = EscolherDeck();
            AlterarPosteriorK(deckgenerico, listaDeCartas);
            break;

        case 3:
            deckgenerico = EscolherDeck();
            InserirposteriorK(deckgenerico, listaDeCartas);
            break;

        case 4:
            deckgenerico = EscolherDeck();
            imprimirNoAnterior(deckgenerico, listaDeCartas);
            break;
        case 5:
            Imprimir7(listaDeCartas);
            break;
        case 6:
            deckgenerico = EscolherDeck();
            Raridade(deckgenerico, listaDeCartas);
            break;

        case 7:
            deckgenerico = EscolherDeck();
            ClassificarNome(deckgenerico);
            std::cin.get();

            break;

        case 8:
        {
            std::string a;
            int b;
            std::cout << "O que deseja imprimir, um Deck (1), ou todas as cartas (2): " << std::endl;
            std::cin >> a;
            b = VerificacaoString(a);
            if (b == 1)
            {
                deckgenerico = EscolherDeck();
                ImprimirDeck(deckgenerico);
            }
            else if (b == 2)
            {
                ImprimirCartas(listaDeCartas);
                std::cin.get();
            }
            else
            {
                std::cout << "Escolha entre 1 e 2:" << std::endl;
            }
            std::cin.get();
            break;
        }

        case 9:
            Imprimir48(listaDeCartas);
            break;

        case 10:
            Impirmirtipo(listaDeCartas);
            break;

        case 11:
            InserirK2(listaDeCartas);
            std::cin.get();
            break;

        case 12:
            ProcurarRemover2(listaDeCartas);
            break;

        case 13:
            deckgenerico = EscolherDeck();
            ClassicarRaridade(deckgenerico);
            break;

        case 14:
            CriarDeck(listaDeCartas);
            break;

        case 15:
            GerarDecksProntos(listaDeCartas);

            break;

        case 0:
            std::cout << "Encerrando o programa..." << std::endl;
            return 0;

        default:
            std::cout << "Opcao " << op << " nao e valida. Tente novamente." << std::endl;
            break;
        }

        std::cout << "\n\nPressione [ENTER] para voltar ao menu principal..." << std::endl;
        std::cin.get();
        system("cls");
    }

    return 0;
}

int main()

{
    GerarInterface();

    return 0;
}
