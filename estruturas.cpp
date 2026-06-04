#include "estruturas.h"
#include "minirede.h"

node_arvore *insereAVL(node_arvore *a, usuario *usuario, bool& aumentouAltura){
    if(a == nullptr){
        node_arvore *novo = new node_arvore;
        novo->usuario = usuario;
        novo->FB = 0;
        novo->esq = nullptr;
        novo->dir = nullptr;
        aumentouAltura = true;
        return novo;
    }

    if(usuario->id < a->usuario->id){
        a->esq = insereAVL(a->esq, usuario, aumentouAltura);

        if(aumentouAltura){
            switch (a->FB) {
                case -1:
                    a->FB = 0;
                    aumentouAltura = false;
                    break;

                case 0:
                    a->FB = 1;
                    break;

                case 1:
                    a = casoEsquerda(a, aumentouAltura);
                    break;
            }
        }
    }else{
        a->dir = insereAVL(a->dir, usuario, aumentouAltura);

        if(aumentouAltura){
            switch (a->FB) {
                case 1:
                    a->FB = 0;
                    aumentouAltura = false;
                    break;

                case 0:
                    a->FB = -1;
                    break;

                case -1:
                    a = casoDireita(a, aumentouAltura);
                    break;
            }
        }
    }


    return a;
}

node_arvore *casoEsquerda(node_arvore *a, bool& aumentouAltura) {
    if (a == nullptr || a->esq == nullptr) {
        aumentouAltura = false;
        return a;
    }

    node_arvore* z = a->esq;

    if (z->FB == 1) {
        a = rotacaoDireita(a);              
    } else {
        a = rotacaoEsquerdaDireita(a);      
    }

    a->FB = 0;
    aumentouAltura = false;
    return a;
}

node_arvore *casoDireita(node_arvore *a, bool& aumentouAltura) {
    if(a == nullptr || a->dir == nullptr){
        aumentouAltura = false;
        return a;
    }

    node_arvore* z = a->dir;

    if(z->FB == -1){
        a = rotacaoEsquerda(a);             
    }else{
        a = rotacaoDireitaEsquerda(a);      
    }

    a->FB = 0;
    aumentouAltura = false;
    return a;
}

node_arvore *rotacaoDireita(node_arvore *a) {
    if(a == nullptr || a->esq == nullptr){
        return a;
    }
    node_arvore* u = a->esq;
    a->esq = u->dir;
    u->dir = a;
    a->FB = 0;
    u->FB = 0;
    return u;
} 

node_arvore *rotacaoEsquerda(node_arvore *a) {
    if (a == nullptr || a->dir == nullptr) {
        return a;
    }
    node_arvore* z = a->dir;
    a->dir = z->esq;
    z->esq = a;
    a->FB = 0;
    z->FB = 0;
    return z;
}

node_arvore *rotacaoEsquerdaDireita(node_arvore *a) {
    if (a == nullptr || a->esq == nullptr || a->esq->dir == nullptr) {
        return a;
    }
    node_arvore* u = a->esq;
    node_arvore* v = u->dir;
    u->dir = v->esq;
    v->esq = u;
    a->esq = v->dir;
    v->dir = a;
    if(v->FB == 1){
        a->FB = -1;
    }else{
        a->FB = 0;
    }
    if(v->FB == -1){
        u->FB = 1;
    }else{
        u->FB = 0;
    }
    v->FB = 0;
    return v; 

}

node_arvore *rotacaoDireitaEsquerda(node_arvore *a) {
    if (a == nullptr || a->dir == nullptr || a->dir->esq == nullptr) {
        return a;
    }
    node_arvore* z = a->dir;
    node_arvore* y = z->esq;
    z->esq = y->dir;
    y->dir = z;
    a->dir = y->esq;
    y->esq = a;
    if(y->FB == -1){
         a->FB = 1;
    }else{
        a->FB = 0;
    }
    if(y->FB == 1){
        z->FB = -1;
    }else{
        z->FB = 0;
    }
    y->FB = 0;
    return y; 
}