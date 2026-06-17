#include "minirede.h"

node_arvore *insereAVL(node_arvore *a, int id, void *dado, bool& aumentouAltura){
    if(a == nullptr){
        node_arvore *novo = new node_arvore;
        novo->dado = dado;
        novo->id = id;
        novo->FB = 0;
        novo->esq = nullptr;
        novo->dir = nullptr;
        aumentouAltura = true;
        return novo;
    }

    if(id < a->id){
        a->esq = insereAVL(a->esq, id, dado, aumentouAltura);

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
        a->dir = insereAVL(a->dir, id, dado, aumentouAltura);

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

node_arvore *buscarAVL(node_arvore *a, int id){
    if(a == nullptr)
        return nullptr;
    int idenficador = a->id;
    if(idenficador == id){
        return a;
    }
    if(id < idenficador)
       return buscarAVL(a->esq, id);
    
    return buscarAVL(a->dir, id);
}

void imprimirUsuariosAVL(node_arvore *a, std::ostream& saida){
    if(a == nullptr)
        return;
    else{
        usuario *user = (usuario*) a->dado;
        imprimirUsuariosAVL(a->esq, saida);
        saida << "USER " << user->id << " " << user->username << " " << user->nome << "\n";
        imprimirUsuariosAVL(a->dir, saida);
    }
}
unsigned int calcularHash(const std::string& username) {
    unsigned int hash = 2166136261u; 
    unsigned int prime = 16777619u; 

    for(char c : username){
        hash ^= (unsigned char)c;
        hash *= prime;
    }

    return hash % TAM_HASH;
}

void insereHash(MiniRede& rede, usuario *usuario){
    unsigned int indice = calcularHash(usuario->username);
    node_hash *novo = new node_hash{usuario, rede.tabela_hash[indice]};
    rede.tabela_hash[indice] = novo;
}

node_hash *buscarHash(MiniRede& rede, std::string username){
    unsigned int indice = calcularHash(username);
    node_hash *atual = rede.tabela_hash[indice];

    while(atual != nullptr && atual->usuario->username != username){
        atual = atual->prox;
    }
    return atual;
}

void novoSeguidor(lista_usuarios *seguidos, usuario *novo){
    node_lista_usuarios *novo_seg = new node_lista_usuarios;
    novo_seg->usuario = novo;

    if(seguidos->inicio == nullptr || seguidos->inicio->usuario->id > novo_seg->usuario->id){
        novo_seg->prox = seguidos->inicio;
        seguidos->inicio = novo_seg;
        return;
    }
    node_lista_usuarios *atual = seguidos->inicio;
    node_lista_usuarios *anterior = nullptr;
    while((atual != nullptr) && novo_seg->usuario->id > atual->usuario->id){
        anterior = atual;
        atual = atual->prox;
    }
    anterior->prox = novo_seg;
    novo_seg->prox = atual;
}

bool jaSeguido(lista_usuarios *seguidos, usuario *novo){
    node_lista_usuarios *atual = seguidos->inicio;
    while(atual != nullptr){
        if(atual->usuario == novo){
            return true;
        }
        atual = atual->prox;
    }
    return false;
}

void novaPublicacaoLista(lista_publicacoes *publicacoes, publicacao *novo){
    node_lista_publicacoes *novo_post = new node_lista_publicacoes;
    novo_post->publicacao = novo;
    novo_post->prox = publicacoes->inicio;
    publicacoes->inicio = novo_post;
}

void adicionarNotificacao(usuario* usuario, tipoNotificacao tipo, int idOrigem, int idPost){
    node_fila *novo_no = new node_fila;
    novo_no->notificacao.tipo = tipo;
    novo_no->notificacao.idUsuario = idOrigem;
    novo_no->notificacao.idPost = idPost;
    novo_no->prox = nullptr;

    if(usuario->notificacoes.inicio == nullptr){
    usuario->notificacoes.inicio = novo_no;
    usuario->notificacoes.fim = novo_no;
    }
    else {
        usuario->notificacoes.fim->prox = novo_no;
        usuario->notificacoes.fim = novo_no;
    }
}

void guardarPost(usuario* usuario1, lista_publicacoes& copia_lista_publicacoes){
    node_lista_usuarios *atual = usuario1->seguindo.inicio;

    while(atual != nullptr){

        usuario *seguidos = atual->usuario;
        node_lista_publicacoes *post_atual = seguidos->publicacoes.inicio;

        while(post_atual != nullptr) {
            node_lista_publicacoes *novo_copia = new node_lista_publicacoes;
            novo_copia->publicacao = post_atual->publicacao;
            novo_copia->prox = copia_lista_publicacoes.inicio;
            copia_lista_publicacoes.inicio = novo_copia;
            post_atual = post_atual->prox;
        }
        atual = atual->prox;
    }
}

bool gerenciarPost(node_lista_publicacoes *post1, node_lista_publicacoes *post2){

    if (post1->publicacao->timestamp > post2->publicacao->timestamp || (post1->publicacao->timestamp == post2->publicacao->timestamp
        && post1->publicacao->id < post2->publicacao->id)){
        return true;
       }
    else {
        return false;
    }
}

void ordenacaoFeed(lista_publicacoes& copia_lista_publicacoes){
    lista_publicacoes lista_ordenada;
    lista_ordenada.inicio = nullptr;

    node_lista_publicacoes *separada_seguidores = copia_lista_publicacoes.inicio;

    while(separada_seguidores != nullptr){

       node_lista_publicacoes *aux = separada_seguidores->prox;

        if(lista_ordenada.inicio == nullptr || gerenciarPost(separada_seguidores, lista_ordenada.inicio)){
            separada_seguidores->prox = lista_ordenada.inicio;
            lista_ordenada.inicio = separada_seguidores;
        }
        else {
            node_lista_publicacoes *procura = lista_ordenada.inicio;

            while (procura->prox != nullptr && gerenciarPost(separada_seguidores, procura->prox) == false) {
                procura = procura->prox;
            }
            separada_seguidores->prox = procura->prox;
            procura->prox = separada_seguidores;
        }
        separada_seguidores = aux;
    }
    copia_lista_publicacoes.inicio = lista_ordenada.inicio;
}
bool rankCurtidas(node_lista_publicacoes *post1, node_lista_publicacoes *post2) {
    if(post1->publicacao->curtidas > post2->publicacao->curtidas || (post1->publicacao->curtidas == post2->publicacao->curtidas && post1->publicacao->id <
        post2->publicacao->id)){
        return true;
    }
    else {
        return false;
    }
}

void ordenarRanking(lista_publicacoes& copia_lista_publicacoes){
    lista_publicacoes lista_ordenada_rank;
    lista_ordenada_rank.inicio = nullptr;

    node_lista_publicacoes *curtidas_separadas = copia_lista_publicacoes.inicio;

    while(curtidas_separadas != nullptr){
        node_lista_publicacoes *atual = curtidas_separadas->prox;

        if(lista_ordenada_rank.inicio == nullptr || rankCurtidas(curtidas_separadas, lista_ordenada_rank.inicio)){
            curtidas_separadas->prox = lista_ordenada_rank.inicio;
            lista_ordenada_rank.inicio = curtidas_separadas;
        }
        else{
            node_lista_publicacoes *procura = lista_ordenada_rank.inicio;

            while (procura->prox != nullptr && rankCurtidas(curtidas_separadas, procura->prox) == false){
                procura = procura->prox;
            }
            curtidas_separadas->prox = procura->prox;
            procura->prox = curtidas_separadas;
        }
        curtidas_separadas = atual;
    }
    copia_lista_publicacoes.inicio = lista_ordenada_rank.inicio;
}

void armazenarPost(node_arvore *a, lista_publicacoes& lista_posts){
    if(a == nullptr){
        return;
    }

    armazenarPost(a->esq,lista_posts);

    usuario *user = (usuario*)a->dado;
    node_lista_publicacoes *post_atual = user->publicacoes.inicio;

    while(post_atual != nullptr){
        node_lista_publicacoes *copia_post = new node_lista_publicacoes;
        copia_post->publicacao = post_atual->publicacao;

        copia_post->prox = lista_posts.inicio;
        lista_posts.inicio = copia_post;

        post_atual = post_atual->prox;
    }
    armazenarPost(a->dir,lista_posts);
}

void liberarListaUsuarios(lista_usuarios &lista){
    node_lista_usuarios *atual = lista.inicio;
    node_lista_usuarios *anterior = nullptr;
    while(atual != nullptr){
        anterior = atual;
        atual = atual->prox;
        delete anterior;
    }
    lista.inicio = nullptr;
}

void liberarListaPublicacoes(lista_publicacoes &lista){
    node_lista_publicacoes *atual = lista.inicio;
    node_lista_publicacoes *anterior = nullptr;
    while(atual != nullptr){
        anterior = atual;
        atual = atual->prox;
        delete anterior;
    }
    lista.inicio = nullptr;
}

void liberarFilaNotificacoes(fila_notificacoes &fila){
    node_fila *atual = fila.inicio;
    node_fila *anterior = nullptr;
    while(atual != nullptr){
        anterior = atual;
        atual = atual->prox;
        delete anterior;
    }
    fila.inicio = nullptr;
    fila.fim = nullptr;
}

void liberarArvoreUsuarios(node_arvore *a){
    if(a == nullptr) 
        return;

    liberarArvoreUsuarios(a->esq);
    liberarArvoreUsuarios(a->dir);

    usuario *user = (usuario*)a->dado;
    liberarListaUsuarios(user->seguindo);
    liberarListaPublicacoes(user->publicacoes);
    liberarFilaNotificacoes(user->notificacoes);

    delete user;
    delete a;

}

void liberarArvorePublicacoes(node_arvore *a){
    if(a == nullptr)
        return;

    liberarArvorePublicacoes(a->esq);
    liberarArvorePublicacoes(a->dir);

    publicacao *post = (publicacao*)a->dado;
    liberarListaUsuarios(post->curtiram);

    delete post;
    delete a;
}