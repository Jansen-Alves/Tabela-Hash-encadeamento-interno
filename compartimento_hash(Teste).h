#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"

#define TAMANHO_HASH 7
#define REGISTRO_CLIENTE "clientes.dat"
#define METADADOS "meta.dat"

typedef struct Cliente {
    int chave;
    char nome[50];
    int estado;
    int prox;
} Cliente;

Cliente *criarCliente(int chavecliente, const char *nomecliente) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo) {
        novo->chave = chavecliente;
        strncpy(novo->nome, nomecliente, sizeof(novo->nome) - 1);
        novo->nome[sizeof(novo->nome) - 1] = '\0';  // Garante que a string seja terminada corretamente
        novo->estado = 1;
        novo->prox = -1;
        printf("\n> Novo cliente criado\n");
    }
    return novo;
}

    // Cliente *criarCliente(int chavecliente, char *nomecliente) {
    //     Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    //     if (novo) {
    //         novo->chave = chavecliente;
    //         strcpy(novo->nome, nomecliente);
    //         novo->estado = 1;
    //         novo->prox = -1;
    //         printf("\n> Novo cliente criado\n");
    //     }
    //     return novo;
    // }

Cliente *busca(FILE *clientes, int chave) {
    int posicao = chave % TAMANHO_HASH;
    Cliente *procurado = (Cliente *)malloc(sizeof(Cliente));

    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

    while (fread(procurado, sizeof(Cliente), 1, clientes) == 1) {
        if (procurado->chave == chave) {
            return procurado;
        } else if (procurado->prox == -1) {
            break;
        } else {
            posicao = procurado->prox;
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
        }
    }

    free(procurado);
    printf("Não há registros de clientes com a chave %d.\n", chave);
    return NULL;
}

void inserir(FILE *meta, FILE *clientes, Cliente *info) {
    int posicao = info->chave % TAMANHO_HASH;
    Cliente *checagem = busca(clientes, info->chave);

    if (checagem) {
        printf("A chave escolhida já está em uso pelo cliente %s. Escolha uma chave diferente.\n", checagem->nome);
        free(checagem);
        return;
    }

    fseek(meta, 0, SEEK_SET);
    int contador;
    fread(&contador, sizeof(int), 1, meta);
    
    int posicao_original = posicao;

    while (1) {
        fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
        fread(checagem, sizeof(Cliente), 1, clientes);

        if (checagem->estado == 0) {
            break;
        } else if (checagem->prox == -1) {
            posicao = posicao_original;
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

            info->prox = -1;
            fwrite(info, sizeof(Cliente), 1, clientes);

            contador++;
            fseek(meta, 0, SEEK_SET);
            fwrite(&contador, sizeof(int), 1, meta);

            printf("Cliente cadastrado com sucesso!\n");
            free(checagem);
            return;
        } else {
            posicao_original = posicao;
            posicao = checagem->prox;
        }
    }

    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
    info->prox = -1;
    fwrite(info, sizeof(Cliente), 1, clientes);
    
    contador++;
    fseek(meta, 0, SEEK_SET);
    fwrite(&contador, sizeof(int), 1, meta);

    printf("Cliente cadastrado com sucesso!\n");
    free(checagem);
}

// Restante do código permanece o mesmo
//ARQUIVO COMPARTIMENTO_HASH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"

#define TAMANHO_HASH 7
#define REGISTRO_CLIENTE "clientes.dat"
#define METADADOS "meta.dat"

typedef struct Cliente {
    int chave;
    char nome[50];
    int estado;
    int prox;
} Cliente;

Cliente *criarCliente(int chavecliente, char *nomecliente) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo) {
        novo->chave = chavecliente;
        strcpy(novo->nome, nomecliente);
        novo->estado = 1;
        novo->prox = -1;
        printf("\n> Novo cliente criado\n");
    }
    return novo;
}

Cliente *busca(FILE *clientes, int chave) {
    Cliente *procurado = (Cliente *)malloc(sizeof(Cliente));
    int validade = 0;

    int posicao = chave % TAMANHO_HASH;
    
    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
   
    if (fread(&procurado->chave, sizeof(int), 1, clientes) > 0) {
        while (validade == 0) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

            fread(&procurado->chave, sizeof(int), 1, clientes);
            fread(procurado->nome, sizeof(char), sizeof(procurado->nome), clientes);
            fread(&procurado->estado, sizeof(int), 1, clientes);
            fread(&procurado->prox, sizeof(int), 1, clientes);

            if (procurado->chave == chave) {
                validade = 1;
            } else if (procurado->prox == -1) {
                validade = -1;
                procurado->chave = -1;
            } else {
                posicao = procurado->prox;
            }
        }
        return procurado;
    } else {
        printf("Não há registros de clientes.\n");
        procurado->chave = -1;
        return procurado;
    }
}

void inserir(FILE *meta, FILE *clientes, Cliente *info) {
    int posicao, contador, posiantiga;
    int validade = 0;
    Cliente *checagem = (Cliente *)malloc(sizeof(Cliente));
    
    posicao = info->chave % TAMANHO_HASH;

    checagem = busca(clientes, info->chave);
    
    if (checagem->chave == info->chave) {
        printf("A chave escolhida já está em uso pelo cliente %s. Escolha uma chave diferente.\n", checagem->nome);
        free(checagem);
        return;
    }

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    
    posiantiga = posicao;
    rewind(clientes);
    fseek(clientes, sizeof(Cliente) * (posicao), SEEK_SET);
    
    if (fread(&checagem->chave, sizeof(int), 1, clientes) <= 0) {
        rewind(clientes);
        fseek(clientes, sizeof(int) * (posicao), SEEK_SET);
        fwrite(&info->chave, sizeof(int), 1, clientes);
        fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
        fwrite(&info->estado, sizeof(int), 1, clientes);
        fwrite(&info->prox, sizeof(int), 1, clientes);
        printf("Cliente cadastrado com sucesso em uma posição vazia.\n");
    } else {
        while (validade == 0) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fread(&checagem->chave, sizeof(int), 1, clientes);
            fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
            fread(&checagem->estado, sizeof(int), 1, clientes);
            fread(&checagem->prox, sizeof(int), 1, clientes);

            if (checagem->estado == 0) {
                validade = 1;
            } else if (checagem->prox == -1) {
                validade = 2;
                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                fread(&checagem->chave, sizeof(int), 1, clientes);
                fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
                fread(&checagem->estado, sizeof(int), 1, clientes);
                fwrite(&contador, sizeof(int), 1, clientes);
                
                contador = contador + 1;
                rewind(meta);
                fwrite(&contador, sizeof(int), 1, meta);
                
                printf("Cliente cadastrado com sucesso!\n");
            } else {
                posiantiga = posicao;
                posicao = checagem->prox;
            }
        }
        rewind(clientes);
        if (validade == 1) {
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(&info->chave, sizeof(int), 1, clientes);
            fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
            fwrite(&info->estado, sizeof(int), 1, clientes);
            printf("Cliente cadastrado com sucesso em uma posição vazia.\n");
        }
        else if (validade != 1) {
            printf("Final da fila encontrado.\n");
            fseek(clientes, sizeof(Cliente) * contador, SEEK_SET);
            fwrite(&info->chave, sizeof(int), 1, clientes);
            fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
            fwrite(&info->estado, sizeof(int), 1, clientes);
            fwrite(&info->prox, sizeof(int), 1, clientes);
            contador = contador + 1;
            rewind(meta);
            fwrite(&contador, sizeof(int), 1, meta);
            printf("Cliente cadastrado com sucesso!\n");
        }
    }
    free(checagem);
}

void deletar(FILE *hash, FILE *meta, FILE *clientes, int chave) {
    int validade = 0, proximo;
    int posicao = chave % TAMANHO_HASH;
    
    Cliente *atual = (Cliente *)malloc(sizeof(Cliente));
    rewind(hash);

    fseek(hash, sizeof(int) * (posicao), SEEK_SET);

    printf("\n> Procurando cliente...\n");
    if (fread(&atual->chave, sizeof(int), 1, clientes) > 0) {
        while (validade == 0) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fread(&atual->chave, sizeof(int), 1, clientes);
            fread(atual->nome, sizeof(char), sizeof(atual->nome), clientes);
            fread(&atual->estado, sizeof(int), 1, clientes);
            fread(&atual->prox, sizeof(int), 1, clientes);

            if (atual->chave == chave) {
                validade = 1;
                atual->chave = -1;
                strcpy(atual->nome, "----");
                atual->estado = 0;
                proximo = atual->prox;

                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

                fwrite(&atual->chave, sizeof(int), 1, clientes);
                fwrite(atual->nome, sizeof(char), sizeof(atual->nome), clientes);
                fwrite(&atual->estado, sizeof(int), 1, clientes);

                printf("\nCliente deletado com sucesso!\n");
                return;
            } else if (atual->prox == -1) {
                validade = -1;
                printf("Cliente não encontrado. Insira-o na Hash primeiro.\n");
                break;
            } else {
                posicao = atual->prox;
            }
        }
    } else {
        printf("Cliente não existe. Insira-o na Hash primeiro.\n");
    }
    free(atual);
}

void lerArquivos(FILE *tabhash, FILE *meta) {
    int contador;
    int b;
    rewind(tabhash);
    printf("\nTabela Hash:\n");
    for (int i = 0; i < TAMANHO_HASH; i++) {
        fread(&b, sizeof(int), 1, tabhash);
        printf("hash %d: %d\n", i, b);
    }
    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    printf("Contador: %d\n", contador);
}

void mostrarRegistros(FILE *clientes, FILE *meta) {
    Cliente *reg = (Cliente *)malloc(sizeof(Cliente));
    int contador, i = 0;

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    if (contador == 0) {
        printf("Não existe nenhum registro de clientes nesse arquivo.\n");
        free(reg);
        return;
    }
    printf("\nREGISTROS:\n");
    rewind(clientes);
    for (i = 0; i < contador; i++) {
        if (fread(&reg->chave, sizeof(int), 1, clientes) <= 0) {
            continue;
        }
        fread(reg->nome, sizeof(char), sizeof(reg->nome), clientes);
        fread(&reg->estado, sizeof(int), 1, clientes);
        fread(&reg->prox, sizeof(int), 1, clientes);
        printf("--------------------------------------------------\n");
        printf("Posição: %d\n", i);
        printf("Cliente: %s\n", reg->nome);
        printf("Chave do cliente: %d\n", reg->chave);
        printf("Posição do próximo cliente nessa fila encadeada: %d\n", reg->prox);
    }
    free(reg);
}

void zerar(FILE *tabhash, FILE *meta, FILE *clientes) {
    int contador = 0;
    int a = -1;

    fclose(tabhash);
    fclose(meta);

    if ((tabhash = fopen(REGISTRO_CLIENTE, "wb")) == NULL) {
        perror("Erro ao abrir o arquivo da tabela hash\n");
        exit(1);
    }

    if ((meta = fopen(METADADOS, "wb")) == NULL) {
        perror("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        fwrite(&a, sizeof(int), 1, tabhash);
    }
    fclose(tabhash);

    if ((tabhash = fopen(REGISTRO_CLIENTE, "a+b")) == NULL) {
        perror("Erro ao abrir o arquivo de registros de clientes\n");
        exit(1);
    }

    printf("\n> Arquivo hash zerado\n");

    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);

    fclose(meta);

    if ((meta = fopen(METADADOS, "a+b")) == NULL) {
        perror("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    rewind(meta);

    int novo;
    fread(&novo, sizeof(int), 1, meta);
    printf("> Contador: %d\n", novo);
    printf("> Tabela de Clientes zerada\n");
    printf("Arquivos zerados com sucesso!\n");
    fclose(clientes);

    if ((clientes = fopen(REGISTRO_CLIENTE, "w+b")) == NULL) {
        perror("Erro ao abrir o arquivo de registros de clientes\n");
        exit(1);
    }
    fclose(clientes);
    fclose(tabhash);
    fclose(meta);
}

//ARQUIVO MAIN
#include <stdio.h>
#include <stdlib.h>
#include "clientes.h"
#include "compartimento_hash.h"


void menu(FILE *meta, FILE *clientes){
    int escolha, chave;
    char nome[100];
    Cliente *novo;


    printf("-------------Menu-------------\n 1 - Inserção \n 2 - Deletar \n 3 - Busca \n 4 - Zerar  \n 5 - Mostrar registros  \n 6 - Sair \n -----------------------------");
    printf("\nDigite uma das opcoes acima:");
    scanf("%d", &escolha);
    switch (escolha)
    {
    case (1) :
        printf("Digite o número da chave do cliente que você deseja inserir: \n");
        scanf("%d", &chave);
        printf("Digite o nome do cliente que você deseja inserir:\n");
        scanf("%s", nome);
        novo = criarCliente(chave, nome);
        inserir(meta, clientes, novo);
        free(novo);
        fclose(meta);
        fclose(clientes);
        //printf("fechando arquivos e ponteiros");
        break;
    case (2):
        printf("Digite a chave do cliente que você quer deletar:\n");
        scanf("%d", &chave);
        deletar(meta, clientes, chave);
        //menu(hash, meta, clientes);
        free(novo);
        fclose(meta);
        fclose(clientes);
        break;
    case (3):
        printf("Digite a chave do cliente que você quer buscar:\n");
        scanf("%d", &chave);

        novo = busca(clientes, chave);
        if(novo->chave == -1){
            printf("Cliente não encontrado,por favor insira ele na Hash primeiro");
        }
        else{
            printf("> Chave do cliente: %d \n", novo->chave);
            printf("> Nome do cliente: %s \n", novo->nome);
            if(novo->prox != -1){
                printf("> Proximo cliente dessa faixa se encontra no ' %d ' lugar", novo->prox);
            }
            else{
                print("Esse cliente se encontra no final da fila");
            }
        }
        free(novo);
        fclose(meta);
        fclose(clientes);
        break;
    case (4):
        zerar(hash, meta, clientes);
        free(novo);

        break;
    case(5):
        mostrarRegs(clientes, meta);
        break;
    case (6):
        free(novo);
        fclose(meta);
        fclose(clientes);
        exit(1);
        break;
    default:
        printf("Escolha um dos números do menu\n");
        break;
    }

}

int main(){
    FILE *meta;
    FILE *clientes;

    if ((meta = fopen("meta.dat", "r+b")) == NULL){
    printf("Erro ao abrir o arquivo da tabela Meta");
    exit(1);
    }

    if ((clientes = fopen("clientes.dat", "r+b")) == NULL){
    printf("Erro ao abrir o arquivo da tabela Clientes");
    exit(1);
    }

    menu(meta, clientes);

    return 0;
}
