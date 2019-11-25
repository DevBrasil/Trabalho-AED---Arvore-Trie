#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define tamanho_alfabeto 26

#define char_int(c) ((int)c - (int)'a')
using namespace std;

//Estrutura representante do no
struct noTrie
{
    struct noTrie *filho[tamanho_alfabeto];
    bool folha;
};

//Cria um novo No
//Pre-condicao: Nenhuma
//Pos-condicao: Retorna um novo no
struct noTrie *novoNo(void)
{
    struct noTrie *pNode = new noTrie;
    pNode->folha = false;

    for (int i = 0; i < tamanho_alfabeto; i++)
        pNode->filho[i] = NULL;

    return pNode;
}
//Verifica se a palavra existe
//Pre-condicao: Arvore nao vazia
//Pos-condicao: nenhuma
//Retorno: true ou false se a palavra existe
bool palavra_existe(struct noTrie *raiz, string palavra)
{
    struct noTrie *temp = raiz;

    for (int i = 0; i < palavra.length(); i++)
    {
        int index = palavra[i] - 'a';
        if (!temp->filho[index])
            return false;

        temp = temp->filho[index];
    }

    return (temp != NULL && temp->folha);
}

//Insere a palavra na arvore
//Pre-condicao: String nao vazia
//Pos-condicao: Palavra inserida na estrutura
void inserir(struct noTrie *raiz, const string palavra)
{
    struct noTrie *temp = raiz;
    int index;

    for (int level = 0; level < palavra.length(); level++)
    {
        index = char_int(palavra[level]);
        
        if (!temp->filho[index])
            temp->filho[index] = novoNo();

        temp = temp->filho[index];
    }

    temp->folha = true;
}

//Verifica se o no é folha
//Pre-condicao: Raiz nao pode ser nulla
//Pos-condicao: true ou false caso seja ou nao
bool eh_folha(struct noTrie *raiz)
{
    return raiz->folha != false;
}

//Verifica se é o ultimo No
//Pre-condicao: No nao pode ser nulla
//Pos-condicao: o ou 1 caso seja ou nao
bool eh_ultimo_no(struct noTrie *no)
{
    for (int i = 0; i < tamanho_alfabeto; i++)
        if (no->filho[i])
            return 0;
    return 1;
}

//Verifica se o no é vazio
//Pre-condicao: No nao pode ser nulla
//Pos-condicao: true ou false caso seja vazio ou nao
bool vazio(noTrie *no)
{
    for (int i = 0; i < tamanho_alfabeto; i++)
        if (no->filho[i])
            return false;
    return true;
}

//Remove palavra da arvore
//Pre-condicao: String e raiz nao pode ser nulla
//Pos-condicao: Palavra removida da estrutura
noTrie *remover(noTrie *raiz, string palavra, int level = 0)
{
    if (!raiz)
        return NULL;

    if (level == palavra.size())
    {

        if (raiz->folha)
            raiz->folha = false;

        if (vazio(raiz))
        {
            delete (raiz);
            raiz = NULL;
        }

        return raiz;
    }

    int index = palavra[level] - 'a';
    raiz->filho[index] =
        remover(raiz->filho[index], palavra, level + 1);

    if (vazio(raiz) && raiz->folha == false)
    {
        delete (raiz);
        raiz = NULL;
    }

    return raiz;
}

//Imprimi todas as palavras que estao na arvore
//Pre-condicao: No nao pode ser nulla
//Pos-condicao: Imprimi todos as palavras da arvore
void imprimir(struct noTrie *raiz, char str[], int level)
{
    if (eh_folha(raiz))
    {
        str[level] = '\0';
        cout << str << endl;
    }

    int i;
    for (i = 0; i < tamanho_alfabeto; i++)
    {
        if (raiz->filho[i])
        {
            str[level] = i + 'a';
            imprimir(raiz->filho[i], str, level + 1);
        }
    }
}

//Funcao auxiliar recursiva para mostrar palavra
//Pre-condicao: Raiz nao pode ser nulla
//Pos-condicao: Mostra palavra
void consultar_palavra_recursiva(struct noTrie *raiz, string palavra)
{
    if (raiz->folha)
    {
        cout << palavra;
        cout << endl;
    }

    if (eh_ultimo_no(raiz))
        return;

    for (int i = 0; i < tamanho_alfabeto; i++)
    {
        if (raiz->filho[i])
        {
            palavra.push_back(97 + i);
            consultar_palavra_recursiva(raiz->filho[i], palavra);
            palavra.pop_back();
        }
    }
}

//Funcao para mostrar palavra
//Pre-condicao: raiz nao pode ser nula
//Pos-condicao: Mostra palavra
int consultar_palavra(noTrie *raiz, const string palavra)
{
    struct noTrie *temp = raiz;

    int level;
    int n = palavra.length();
    for (level = 0; level < n; level++)
    {
        int index = char_int(palavra[level]);

        if (!temp->filho[index])
            return 0;

        temp = temp->filho[index];
    }

    bool ehpalavra = (temp->folha == true);

    bool ehFolha = eh_ultimo_no(temp);

    if (ehpalavra && ehFolha)
    {
        cout << palavra << endl;
        return -1;
    }

    if (!ehFolha)
    {
        string prefixo = palavra;
        consultar_palavra_recursiva(temp, prefixo);
        return 1;
    }
}
//Inserir palavras via arquivo txt
//Pre-condicao: Arquivo deve existir
//Pos-condicao: Grava todas as palavras na arvore
void inserir_via_arquivo_txt(noTrie *raiz)
{

    FILE *file1;
    file1 = fopen("dados.txt", "r");

    char type;
    char palavra[200];

    if (file1 == NULL)
    {
        printf("Problemas na LEITURA do arquivo\n");
    }
    else
    {
        printf("ARQUIVO ABERTO COM SUCESSO LEITURA SENDO FEITA ...\n");
        while (fscanf(file1, "%[^\n]%*c", palavra) != EOF)
        {
            /*  cout << "Palavra no arquivo: " << palavra << endl; */
            inserir(raiz, palavra);
        }
    }
    printf("LEITURA FEITA COM SUCESSO\n");

    fclose(file1);
}
//Funcao para remover palavras contidas no arquivo
//Pre-condicao: raiz nao pode nula
//Pos-condicao: Remove palavras
void stopwords(noTrie *raiz)
{

    FILE *file1;
    file1 = fopen("stopwords.txt", "r");

    char type;
    char palavra[200];

    if (file1 == NULL)
    {
        printf("Problemas na LEITURA do arquivo\n");
    }
    else
    {
        printf("ARQUIVO ABERTO COM SUCESSO LEITURA SENDO FEITA ...\n");
        while (fscanf(file1, "%[^\n]%*c", palavra) != EOF)
        {
            /*  cout << "Palavra no arquivo: " << palavra << endl; */
           remover(raiz,palavra);
        }
    }
    printf("LEITURA FEITA COM SUCESSO\n");

    fclose(file1);
}

bool CheckSimilares(string palavraPrinciapl, string PalavraTeste, int distancia) {
    if (distancia >= 0) {
    	if( palavraPrinciapl.length() == 0 && PalavraTeste.length() == 0) 
    		return true;
        if ( palavraPrinciapl.length() == 0 ) 
        	return (PalavraTeste.length() <= distancia);
        if ( PalavraTeste.length() == 0 ) 
        	return (palavraPrinciapl.length() <= distancia);
        char a[1];  a[0]  = palavraPrinciapl[0];
        char b[1];  b[0]  = PalavraTeste[0];
        if (strcmp(a,b) == 0) 
            return CheckSimilares(palavraPrinciapl.substr(1, palavraPrinciapl.length()), PalavraTeste.substr(1, PalavraTeste.length()), distancia);
        else
            return CheckSimilares(palavraPrinciapl, PalavraTeste.substr(1, PalavraTeste.length()), distancia-1);
    }
    return false;
}



void imprimirSimilar(struct noTrie *raiz, char str[], int level,int distancia, char palavra[]){
    if (eh_folha(raiz)){
        if(CheckSimilares(palavra,str,distancia)){
            str[level] = '\0';
            cout << str << endl;
        }
    }
    int i;
    for (i = 0; i < tamanho_alfabeto; i++){
        if (raiz->filho[i]){
            str[level] = i + 'a';
            imprimirSimilar(raiz->filho[i], str, level + 1,distancia,palavra);
        }
    }
}



//Limpa terminal
//Pre-condicao: Nenhuma
//Pos-condicao: Limpa o terminal
void limparTerminal()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main()
{

    int level = 0;
    char str[50];

    int opcao;
    struct noTrie *raiz = novoNo();

    while (2 == 2)
    {

        printf("----------------------------------------------");
        printf("\n\tMENU\n");
        printf("1. Carregar arquivo de inicializacao:\n");
        printf("2. Consultar palavra\n");
        printf("3. Imprimir todas as palavras\n");
        printf("4. Remover livro(stopwords)\n");
        printf("5. Bonus\n");
        printf("0. Sair\n");
        printf("----------------------------------------------\n");

        scanf("%d", &opcao);

        switch (opcao)
        {

        case 1:
            limparTerminal();
            inserir_via_arquivo_txt(raiz);
            printf("Opcao 1\n");
            break;
        case 2:
            limparTerminal();
            printf("Opcao 2\n");
            char palavra[256];
            cin >> palavra;
            int opc;
            opc = consultar_palavra(raiz, palavra);
            if (opc == 0)
            {
                cout << "Nao foi encontrado" << endl;
                ;
            }
            break;
        case 3:
            limparTerminal();
            printf("Opcao 3\n");
            imprimir(raiz, str, level);
            break;
        case 4:
            limparTerminal();
            stopwords(raiz);
            break;
        case 5:
            limparTerminal();
            //void imprimirSimilar(  int level,int distancia, char palavra[]){
 
            imprimirSimilar(raiz, "", 1,2,"escreve");
            break;
        case 0:
            limparTerminal();
            printf("\nPrograma finalizado\n");
            exit(0);
            break;
        }
    }
}
