#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct Cliente{
    int chave;
    char nome[100];
    int estado;
    int prox;
}Cliente;

#endif
