#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"

#define TAMANHO_HASH 7
#define REGISTRO_CLIENTE "clientes.dat"
#define METADADOS "meta.dat"

Cliente *criarCliente(int chavecliente, char *nomecliente){
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo) {
        memset(novo, 0, sizeof(Cliente));
        novo->chave = chavecliente;
        strcpy(novo->nome, nomecliente);
        novo->estado = 1;
        novo->prox = -1;
        printf("\n> Novo cliente criado\n");
    }
    return novo;
}

Cliente *busca(FILE*clientes, int chave){
    Cliente *procurado = (Cliente *)malloc(sizeof(Cliente));
    int validade = 0;

    int posicao = chave % TAMANHO_HASH;
    //printf("Fazendo busca\n");
    
    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
   
    if (-1 <=  fread(&procurado->chave, sizeof(int), 1, clientes);) {
        while (validade == 0) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

            fread(&procurado->chave, sizeof(int), 1, clientes);
            //printf("chave: %d \n", procurado->chave);
            fread(procurado->nome, sizeof(char), sizeof(procurado->nome), clientes);
            //printf("nome: %s \n", procurado->nome);
            fread(&procurado->estado, sizeof(int), 1, clientes);
            //printf("estado: %d \n", procurado->estado);
            fread(&procurado->prox, sizeof(int), 1, clientes);
            //printf("proximo: %d \n", procurado->prox);

            if (procurado->chave == chave) {
                validade = 1;
            } else if (procurado->prox == -1) {
                validade = -1;
                procurado->chave = -1;
               // printf("Busca: Chegamos ao fim da fila \n");
            } else {
                posicao = procurado->prox;
            }
        }
        return procurado;

    } else {
       // printf("Não tem ninguem cadastrado \n");
        procurado->chave = -1;
        return procurado;
    }
}

void inserir(FILE *meta, FILE *clientes, Cliente *info){
    int posicao, contador, valor, posiantiga;
    int validade = 0;
    Cliente *checagem = (Cliente *)malloc(sizeof(Cliente));
    posicao = info->chave % TAMANHO_HASH;
    //printf("Posicao na hash eh %d", posicao);

    checagem = busca(tabhash, clientes, info->chave);
    if (checagem->chave == info->chave) {
        printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso \n", checagem->nome);
        free(checagem);
        //free(info);
        return;
    }
    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    
    posiantiga = posicao;
    rewind(clientes);
    fseek(cliente, sizeof(Cliente) * (posicao), SEEK_SET);
    if(0 >= fread(&checagem->chave, sizeof(int), 1, clientes)){
        rewind(clientes);
        fseek(cliente, sizeof(int) * (posicao), SEEK_SET);
        fwrite(&info->chave, sizeof(int), 1, clientes);
        fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
        fwrite(&info->estado, sizeof(int), 1, clientes);
        fwrite(&info->estado, sizeof(int), 1, clientes);
    }
    else{
        while(validade == 0){
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fread(&checagem->chave, sizeof(int), 1, clientes);
            fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
            //printf("nome na fila: %s \n", checagem->nome);
            fread(&checagem->estado, sizeof(int), 1, clientes);
            fread(&checagem->prox, sizeof(int), 1, clientes);

            if(checagem->estado == 0){
                validade = 1;
            }
            else if(checagem->prox == -1){
                validade = 2;
                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                fread(&checagem->chave, sizeof(int), 1, clientes);
                fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
                fread(&checagem->estado, sizeof(int), 1, clientes);
                fwrite(&contador, sizeof(int), 1, clientes);
                
                /*if(posicao <= 7){ // Método alternativo
                    validade = 2;
                    rewind(clientes);
                    posiantiga = posicao;
                    posicao = posicao +( 7 - posicao);
                    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                    while(0<= fread(&checagem->chave, sizeof(int), 1, clientes)){
                        fread(&checagem->chave, sizeof(int), 1, clientes);
                        fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
                        fread(&checagem->estado, sizeof(int), 1, clientes);
                        fread(&checagem->prox, sizeof(int), 1, clientes);
                        posicao += 1;
                    }                    
                }
                else{
                    validade = 3;
                    rewind(clientes);
                    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                    fread(&checagem->chave, sizeof(int), 1, clientes);
                    fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
                    fread(&checagem->estado, sizeof(int), 1, clientes);
                    fwrite()
                }*/
            }
            else{
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
            printf("Cliente cadastrado com sucesso em uma posição vazia");
        }
        else if(validade != 1){
            printf("\n> Final da fila encontrado \n");
            fseek(clientes, sizeof(Cliente) * contador, SEEK_SET);
            fwrite(&info->chave, sizeof(int), 1, clientes);
            fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
            fwrite(&info->estado, sizeof(int), 1, clientes);
            fwrite(&info->prox, sizeof(int), 1, clientes);
            contador = contador + 1;
            //printf("contador: %d \n", contador);
            rewind(meta);
            fwrite(&contador, sizeof(int), 1, meta);
            rewind(meta);
            fread(&contador, sizeof(int), 1, meta);
            printf("> Contador do metadados: %d \n", contador);
            printf("\nCliente cadastrado(a) com sucesso! \n");
        }
    }
    free(contagem);   
}


void deletar(FILE *hash, FILE *meta, FILE *clientes, int chave){
    int validade = 0, proximo;
    int posicao = chave % TAMANHO_HASH;
    Cliente *atual = (Cliente *)malloc(sizeof(Cliente));
    rewind(hash);

    fseek(hash, sizeof(int) * (posicao), SEEK_SET);

    //printf("%d", posicao);
    printf("\n> Procurando cliente...\n");
    if (-1 <= fread(&atual->chave, sizeof(int), 1, clientes) ) {
        while (validade == 0) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fread(&atual->chave, sizeof(int), 1, clientes);
            fread(atual->nome, sizeof(char), sizeof(atual->nome), clientes);
            fread(&atual->estado, sizeof(int), 1, clientes);
            fread(&atual->prox, sizeof(int), 1, clientes);
            //printf("\nteste");
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
                printf("Cliente não encontrado, por favor insira ele na Hash primeiro.\n");
                break;
            } else {
                posicao = atual->prox;
            }
        }
    } else {
        printf("Cliente não existe, por favor insira ele na Hash primeiro\n");
    }
    free(atual);
}

void lerarqs(FILE *tabhash, FILE *meta){
    int contador;
    int b;
    rewind(tabhash);
    printf("\n");
    for (int i = 0; i < TAMANHO_HASH; i++) {
        fread(&b, sizeof(int), 1, tabhash);
        printf("hash %d: %d \n", i, b);
    }
    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    printf("Contador: %d", contador);
}

void mostrarRegistros(FILE *clientes, FILE*meta){
    Cliente *reg = (Cliente *)malloc(sizeof(Cliente));
    int contador, i = 0;

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    if(contador == 0){
        printf("Não existe nenhum registro de clientes nesse arquivo");
        free(reg);
        return;
    }
    print("REGISTROS");
    rewind(clientes);
    for(i = 0; i<contador; i++){ 
        if(-1 >= fread(reg->chave, sizeof(int), 1, cliente)){   
            continue;
        }
        fread(reg->nome, sizeof(char), sizeof(atual->nome), clientes);
        fread(&reg->estado, sizeof(int), 1, clientes);
        fread(&reg->prox, sizeof(int), 1, clientes);
        print("--------------------------------------------------")
        printf("Posição: %d", i);
        print("Cliente: %s", reg->nome);
        print("Chave do cliente: %d", reg->chave);
        print("Posição do próximo cliente dessa fila encadeada: %d", reg->prox);

    }
    free(reg);
}


void zerar(FILE *tabhash, FILE *meta, FILE *clientes){
    FILE *nhash;
    FILE *nmeta;
    FILE *nclientes;
    int contador = 7;
    int novo;
    int a = -1;
    int b;

    fclose(tabhash);
    fclose(meta);

    if ((nhash = fopen(TABELA_HASH, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela hash\n");
        exit(1);
    }

    if ((nmeta = fopen(METADADOS, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }


    for (int i = 0; i < TAMANHO_HASH; i++) {
        fwrite(&a, sizeof(int), 1, nhash);
    }
    fclose(nhash);

    if ((nhash = fopen(TABELA_HASH, "a+ b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela clientes\n");
        exit(1);
    }

    rewind(nhash);

    /*for (int i = 0; i < TAMANHO_HASH; i++) {
        fread(&b, sizeof(int), 1, nhash);
        printf("hash: %d \n", b);
    }*/

    printf("\n> Arquivo hash zerado\n");

    rewind(nmeta);
    fwrite(&contador, sizeof(int), 1, nmeta);

    fclose(nmeta);

    if ((nmeta = fopen(METADADOS, "a+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    rewind(nmeta);

    fread(&novo, sizeof(int), 1 ,nmeta);
    printf("> Contador: %d\n", novo);
    printf("> Tabela Clientes zerada\n");
    printf("Arquivos zerados com sucesso!");
    fclose(clientes);

    if ((nclientes = fopen(REGISTRO_CLIENTE, "w+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela clientes\n");
        exit(1);
    }
    fclose(nclientes);
    fclose(nhash);
    fclose(nmeta);

}
