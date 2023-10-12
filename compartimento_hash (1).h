#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"


Cliente *criarCliente(int chavecliente, char *nomecliente){
    Cliente *novo = (Cliente *) malloc(sizeof(Cliente));
    if (novo) memset(novo, 0, sizeof(Cliente));
    novo->chave = chavecliente;
    strcpy(novo->nome, nomecliente);
    novo->estado = 1;
    novo->prox = -1;
    
    return novo;
}
void inserir(FILE *hash, FILE *meta, FILE *clientes, Cliente *info){
    int posicao, contador;
    int validade = 0;
    Cliente * checagem = (Cliente *) malloc(sizeof(Cliente));
    posicao = info->chave % 7;

    checagem = busca(hash, clientes, info->chave);
    if(checagem->chave == info->chave){
        printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso", checagem->nome);
        free(checagem);
        return 0;
    }
   

    rewind(hash);
    if(posicao != 0){
        fseek(hash, sizeof(int)*(posicao), SEEK_SET);
        fread(&posicao, sizeof(int), 1, hash);
    }
    else{
        fread(&posicao, sizeof(int), 1, hash);
    }

    rewind(meta);

    fread(&contador, sizeof(int), 1, meta);

    if(posicao != -1){
       while(validade == 0){
            rewind(clientes);
            fseek(clientes, sizeof(Cliente)*posicao, SEEK_SET);
            
            fread(&checagem->chave, sizeof(int), 1, clientes);
            fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
            fread(&checagem->estado, sizeof(int), 1, clientes);
            fread(&checagem->prox, sizeof(int), 1, clientes);
            
            if(checagem->estado == 0){
                validade = 2;
            }
            else if(checagem->prox == -1){
                validade = 1;
                fseek(clientes, sizeof(int)*-1, SEEK_SET);
                fwrite(&contador, sizeof(int), 1, clientes);
            }
            else{
                posicao = checagem->prox;
            }
        }
        
        rewind(clientes);
        if(validade == 2){
            fseek(clientes, sizeof(Cliente)*posicao, SEEK_SET);
            fwrite(&info->chave, sizeof(int), 1, clientes);
            fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
            fwrite(&info->estado, sizeof(int), 1, clientes);
            printf("Cliente cadastrado com sucesso em uma posição vazia");
        }
    }else{ //Não existe cliente naquela hash
        rewind(clientes);
        fseek(clientes, sizeof(Cliente)*contador, SEEK_SET);
        fwrite(&info->chave, sizeof(int), 1, clientes);
        fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
        fwrite(&info->estado, sizeof(int), 1, clientes);
        fwrite(&info->prox, sizeof(int), 1, clientes);
        contador++;
        rewind(meta);
        fwrite($contador, sizeof(int), 1, meta);
        if(validade != 1){
            rewind(hash);
            posicao = info->chave % 7;
            fseek(hash, sizeof(int)*(posicao), SEEK_SET);
            fwrite(&posicao, sizeof(int), 1, hash);
        }
    }
       
    }






void deletar(){
    /*int cod;
    printf("Digite o código do cliente que deseja deletar:");
    scanf("%d", &cod);

    FILE *hash;
    FILE *clientes;

    int posicao = cod % 7;

    if ((hash = fopen("tabHash.dat", "r+b")) == NULL){
        printf("Erro ao abrir o arquivo da tabela Hash");
        exit(1);
    }
    rewind(hash);
    if(posicao != 0){
        fseek(hash, sizeof(int)*(posicao), SEEK_SET);
    }
    else{
        fseek(hash, sizeof(int), SEEK_SET);
    }

    fread(&posicao, sizeof(int), 1, hash);

    if ((clientes = fopen("clientes.dat", "r+b")) == NULL){
        printf("Erro ao abrir o arquivo da tabela Clientes");
        exit(1);
    }

    while(0 < fread(&atual->chave, sizeof(int), 1, clientes)){
        fread(atual->nome, sizeof(char),sizeof(atual->nome), clientes);
        if(atual->chave == cod){
            atual->estado = 0; // Mark the client as deleted
            fseek(clientes,sizeof(cliente)*posicao, SEEK_SET);
            fwrite(atual, sizeof(cliente), 1, clientes); // Write the updated client back to the file
            printf("Cliente deletado\n");
            return;
        }
        if(atual->prox == -1){
            break;
        }
        posicao = atual->prox;
        fseek(clientes,sizeof(cliente)*posicao, SEEK_SET);
    }
    
    printf("Cliente não encontrado\n");
    */
}



Cliente *busca(FILE *hash, FILE*clientes, int chave){
    Cliente *procurado =(Cliente *) malloc(sizeof(Cliente));
    int validade=0;
    
    int posicao = chave%7;
    
    rewind(hash);
    if(posicao == 0){
        fread(&posicao, sizeof(int), 1, hash);
    }
    else{
        fseek(hash, sizeof(int)*posicao, SEEK_SET); 
        fread(&posicao, sizeof(int), 1, hash);
    }
    if(posicao != -1){
        
        while(validade == 0){
            rewind(clientes);
            fseek(clientes, sizeof(Cliente)*posicao, SEEK_SET);
            
            fread(&procurado->chave, sizeof(int), 1, clientes);
            fread(procurado->nome, sizeof(char), sizeof(procurado->nome), clientes);
            fread(&procurado->estado, sizeof(int), 1, clientes);
            fread(&procurado->prox, sizeof(int), 1, clientes);
            
            if(procurado->chave == chave){
                validade = 1;
            }
            else if(procurado->prox == -1){
                validade = -1;
                procurado->chave = -1;
            }
            else{
                posicao = procurado->prox;
            }
        }
        return procurado;
        
    }
    else{
        procurado->chave = -1;
        return procurado;
    }
}

void zerar(FILE *hash,FILE *meta,FILE *clientes){
    FILE* new;
    int contador = 0;

    int a = -1;
    rewind(hash);
    for(int i = 0; i<6; i++){
        fwrite(&a, sizeof(int),  1, hash);
    }
    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);
    fclose(clientes);

    if ((new = fopen("clientes.dat", "w+b")) == NULL){
        printf("Erro ao abrir o arquivo da tabela clientes");
        exit(1);
    }
    fclose(new);
}

