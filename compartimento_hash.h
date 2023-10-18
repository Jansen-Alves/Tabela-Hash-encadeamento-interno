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
    rewind(clientes);
    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
    fread(&procurado->chave, sizeof(int), 1, clientes);

    if (procurado->chave != -1) {
        while (validade == 0) {
            //printf("Busca: Procurando cliente...\n");
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
                printf("Busca: Cliente encontrado\n");
            } else if (procurado->prox == -1) {
                validade = -1;
                procurado->chave = -1;
                printf("Busca: Chegamos ao fim da fila \n");
            } else {
                posicao = procurado->prox;
                //printf("%d \n", posicao);
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
    int posicao, contador, valor, i, pxchave, proxchave, proxposi;
    int validade = 0;
    Cliente *checagem = (Cliente *)malloc(sizeof(Cliente));
    posicao = info->chave % TAMANHO_HASH;
    //printf("Posicao na hash eh %d", posicao);

    checagem = busca(clientes, info->chave);
    if (checagem->chave == info->chave) {
        printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso \n", checagem->nome);
        free(checagem);
        //free(info);
        return;
    }
    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    
    i = posicao;

    rewind(clientes);
    fseek(clientes, sizeof(Cliente) * (posicao), SEEK_SET);
    fread(&checagem->chave, sizeof(int), 1, clientes);
    if(checagem->chave == -1){
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * (posicao), SEEK_SET);
        fwrite(&info->chave, sizeof(int), 1, clientes);
        fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
        fwrite(&info->estado, sizeof(int), 1, clientes);
        fwrite(&info->prox, sizeof(int), 1, clientes);
    } else{
        while(validade == 0){
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
            fread(&checagem->chave, sizeof(int), 1, clientes);
            fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
            //printf("nome na fila: %s \n", checagem->nome);
            fread(&checagem->estado, sizeof(int), 1, clientes);
            //printf("estado na fila: %d \n", checagem->estado);
            fread(&checagem->prox, sizeof(int), 1, clientes);
            //printf("\nAQUI %d", i);
            //printf("COntador: %d \n", contador);
            fread(&pxchave, sizeof(int), 1, clientes);
            //printf("chave do proximo cliente: %d \n", pxchave);
            if(i + 1 > contador ){
                validade = 3;
                break;
            } else if(checagem->estado == 0){
                validade = 1;
            } else if(checagem->prox == -1){
                
                if(pxchave != -1){
                    //printf("%d", pxchave);
                    proxposi = i; // vai rodar a lista até achar uma posição livre e gravar qual posicao é.
                    proxchave = pxchave;
                    while(proxchave != -1 && proxposi < contador){
                        printf("posicao pós fim da fila: %d \n", proxposi);
                        rewind(clientes);
                        fseek(clientes, sizeof(Cliente) * proxposi, SEEK_SET);
                        fread(&proxchave, sizeof(int), 1, clientes);
                        if(proxchave != -1){
                        proxposi = proxposi +1;
                        }
                    }
                    if(proxchave == -1){
                        posicao = proxposi;
                    }
                    else{
                        validade = 3;
                    }
                }
                else{
                   posicao = i+1; 
                }
                    if(i<contador && validade != 3){
                    //printf("\ninserção !!!!!");
                    validade = 2;
                    rewind(clientes);
                    fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
                    fread(&checagem->chave, sizeof(int), 1, clientes);
                    fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
                    //printf("\n nome: %s",checagem->nome);
                    fread(&checagem->estado, sizeof(int), 1, clientes);
                    fwrite(&posicao, sizeof(int), 1, clientes);
                    //printf("\n POS : %d",pos);
                    }
                    else{
                    validade = 3;
                    }
            }
            else{
                i = checagem->prox;
                posicao = checagem->prox;
            }
        }
        //printf("\nvalidade: %d", validade);
        rewind(clientes);
        if (validade == 1) {
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(&info->chave, sizeof(int), 1, clientes);
            fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
            fwrite(&info->estado, sizeof(int), 1, clientes);
            printf("Cliente cadastrado com sucesso em uma posição vazia");
        }
        else if(validade != 1 && validade != 3){
            printf("\n> Final da fila encontrado \n");
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(&info->chave, sizeof(int), 1, clientes);
            fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
            fwrite(&info->estado, sizeof(int), 1, clientes);
            fwrite(&info->prox, sizeof(int), 1, clientes);
            //contador = contador + 1;
            //printf("contador: %d \n", contador);
            printf("\nCliente cadastrado(a) com sucesso! \n");
        } else if(validade == 3){
            printf("\n> Overflow: hash lotada \n");
        }
    }
    free(checagem);   
}


void deletar(FILE *meta, FILE *clientes, int chave){
    int validade = 0, proximo;
    int posicao = chave % TAMANHO_HASH;
    Cliente *atual = (Cliente *)malloc(sizeof(Cliente));

    rewind(clientes);

    fseek(clientes, sizeof(Cliente) * (posicao), SEEK_SET);

    //printf("%d", posicao);
    printf("\n> Procurando cliente...\n");
    fread(&atual->chave, sizeof(int), 1, clientes);
    if (atual->chave != -1) {
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

                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

                fread(&atual->chave, sizeof(int), 1, clientes);
                fread(atual->nome, sizeof(char), sizeof(atual->nome), clientes);
                fread(&atual->estado, sizeof(int), 1, clientes);

                printf("Estado do cliente deletado: %d", atual->estado);

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
    printf("\nREGISTROS\n");
    rewind(clientes);
    
    for(i = 0; i < contador; i++){ 
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fread(&reg->chave, sizeof(int), 1, clientes);
        /*if(reg->chave < 0){   
            continue;
        }*/
        fread(reg->nome, sizeof(char), sizeof(reg->nome), clientes);
        fread(&reg->estado, sizeof(int), 1, clientes);
        fread(&reg->prox, sizeof(int), 1, clientes);
        printf("--------------------------------------------------\n");
        printf("Posição: %d\n", i);
        printf("Cliente: %s\n", reg->nome);
        printf("Chave do cliente: %d\n", reg->chave);
        printf( "Posição do próximo cliente dessa fila encadeada: %d\n", reg->prox);
    }
    free(reg);
}

/*void zerar(FILE *meta, FILE *clientes){
    int contador = TAMANHO_HASH;
    int novo;
    int a = -1;
    int b;


    fclose(meta);


    if ((meta = fopen(METADADOS, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }


    for (int i = 0; i < TAMANHO_HASH; i++) {
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fwrite(&a, sizeof(int), 1, clientes);
    }

    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);

    fclose(meta);

    if ((meta = fopen(METADADOS, "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    rewind(meta);

    fread(&novo, sizeof(int), 1 ,meta);
    printf("> Contador: %d\n", novo);
    printf("> Tabela Clientes zerada\n");
    printf("Arquivos zerados com sucesso!");

    for (int i = 0; i < TAMANHO_HASH; i++) {
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fread(&b, sizeof(int), 1, clientes);
        printf("Local da hash %d, recebe %d \n", i, b);
    }
    rewind(clientes);


}*/


void zerar(FILE *meta, FILE *clientes){
    int contador = 7;
    int novo;
    int a = -1;
    Cliente *atual = (Cliente *)malloc(sizeof(Cliente));
    int b, estado, prox;

    atual->chave = -1;
    strcpy(atual->nome, "-----");
    atual->estado = 0;
    atual->prox = -1;
    
    


    fclose(meta);


    if ((meta = fopen(METADADOS, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fwrite(&atual->chave, sizeof(int), 1, clientes);
        fwrite(atual->nome, sizeof(Cliente), sizeof(atual->nome), clientes);
        fwrite(&atual->estado, sizeof(int), 1, clientes);
        fwrite(&atual->prox, sizeof(int), 1, clientes);
    }

    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);

    fclose(meta);

    if ((meta = fopen(METADADOS, "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    rewind(meta);

    fread(&novo, sizeof(int), 1 ,meta);

    printf("\n\n> Contador: %d\n", novo);
    printf("> Tabela Clientes zerada\n");
    printf("Arquivos zerados com sucesso!\n\n");

    /*for (int i = 0; i < TAMANHO_HASH; i++) {
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fread(&b, sizeof(int), 1, clientes);
        printf("Local da hash %d, recebe %d \n", i, b);
    }*/
    rewind(clientes);
    free(atual);

}
