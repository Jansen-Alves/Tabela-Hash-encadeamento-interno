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
