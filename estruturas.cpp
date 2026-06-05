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

usuario *buscarAVL(node_arvore *a, int id){
    if(a == nullptr)
        return nullptr;
    usuario *user = a->usuario;
    if(user->id == id){
        return user;
    }
    if(id < user->id)
       return buscarAVL(a->esq,id);
    
    return buscarAVL(a->dir, id);
}

void imprimirAVL(node_arvore *a, std::ostream& saida){
    if(a == nullptr)
        return;
    else{
        usuario *user = a->usuario;
        imprimirAVL(a->esq, saida);
        saida << "USER " << user->id << " " << user->username << " " << user->nome << "\n";
        imprimirAVL(a->dir, saida);
    }
}
unsigned int hash(const std::string& username) {
    unsigned int hash = 2166136261u; 
    unsigned int prime = 16777619u; 

    for(char c : username){
        hash ^= (unsigned char)c;
        hash *= prime;
    }

    return hash % TAM_HASH;
}

void insereHash(MiniRede& rede, usuario *usuario){
    unsigned int indice = hash(usuario->username);
    node_hash *novo = new node_hash{usuario, rede.tabela_hash[indice]};
    rede.tabela_hash[indice] = novo;
}

node_hash *buscarHash(MiniRede& rede, std::string username){
    unsigned int indice = hash(username);
    node_hash *current = rede.tabela_hash[indice];

    while(current != nullptr && current->usuario->username != username){
        current = current->prox;
    }
    return current;
}