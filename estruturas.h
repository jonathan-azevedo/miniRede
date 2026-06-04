#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <iostream>

struct usuario;

struct node_arvore{
    usuario *usuario;
    int FB;
    node_arvore *esq;
    node_arvore *dir;
};

node_arvore *rotacaoDireita(node_arvore *a);
node_arvore *rotacaoEsquerda(node_arvore *a);
node_arvore *rotacaoEsquerdaDireita(node_arvore *a);
node_arvore *rotacaoDireitaEsquerda(node_arvore *a);
node_arvore *casoEsquerda(node_arvore* a, bool& aumentouAltura);
node_arvore *casoDireita(node_arvore* a, bool& aumentouAltura);
node_arvore *insereAVL(node_arvore* a, usuario *usuario, bool& aumentouAltura);
usuario *buscaAVL(node_arvore *a, int id);
void imprimirAVL(node_arvore *a, std::ostream& saida);

#endif