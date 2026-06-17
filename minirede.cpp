#include "minirede.h"


void inicializarMiniRede(MiniRede& rede){
    rede.raiz_usuarios = nullptr;
    rede.raiz_publicacoes = nullptr;
    for(int i = 0; i < TAM_HASH; i++) {
        rede.tabela_hash[i] = nullptr;
    }
}

void liberarMiniRede(MiniRede& rede){
    // TODO
}

void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida){
    std::string comando;
    while(entrada >> comando){
        if(comando == "END"){
            break;
        }
        else if(comando == "ADD_USER"){
            int id;
            std::string username;
            std::string nomeCompleto;
            entrada >> id >> username >> nomeCompleto;
            cadastrarUsuario(rede, id, username, nomeCompleto, saida);
        }
        else if(comando == "FIND_USER"){
            int id;
            entrada >> id;
            buscarUsuarioPorId(rede, id, saida);
        }
        else if(comando == "LIST_USERS"){
            listarUsuarios(rede, saida);
        }
        else if(comando == "FIND_USERNAME"){
            std::string username;
            entrada >> username;
            buscarUsuarioPorUsername(rede, username, saida);
        }
        else if(comando == "FOLLOW"){
            int id,id2;
            entrada >> id >> id2;
            seguirUsuario(rede, id, id2, saida);
        }
        else if(comando == "LIST_FOLLOWING"){
            int id;
            entrada >> id;
            listarSeguindo(rede, id, saida);
        }
        else if(comando == "ADD_POST"){
            int postid;
            int id;
            int timestamp;
            std::string texto;
            entrada >> postid >> id >> timestamp;
            entrada.ignore();
            std::getline(entrada, texto);
            cadastrarPublicacao(rede, postid, id, timestamp, texto, saida);
        }
        else if(comando == "LIKE"){
            int id,idpost;
            entrada >> id >> idpost;
            curtirPublicacao(rede, id, idpost, saida);
        }
        else if(comando == "GET_NOTIFICATIONS"){
            int id, k_notificacoes;
            entrada >> id >> k_notificacoes;
            consultarNotificacoes(rede,id, k_notificacoes, saida);
        }
        else if(comando == "FEED"){
            int id,k_publicacoes;
            entrada >> id >> k_publicacoes;
            gerarFeed( rede, id,k_publicacoes, saida);
        }
        else if(comando == "TOP_POSTS"){
           int publicacoes;
            entrada >> publicacoes;
            listarTopPosts( rede, publicacoes, saida);
        }
        else{
            saida << "ERROR INVALID_COMMAND\n";
            std::string restoDaLinha;
            std::getline(entrada, restoDaLinha);
        }
    }
}

void cadastrarUsuario(MiniRede& rede, int id, std::string username, std::string nomeCompleto, std::ostream& saida){
    node_arvore *checagem_arvore = buscarAVL(rede.raiz_usuarios, id);
    node_hash *checagem_hash = buscarHash(rede, username);
    if(checagem_arvore != nullptr || checagem_hash != nullptr){
        saida << "ERROR USER_EXISTS\n";
        return;
    }
    usuario *novo_usuario = new usuario{id,username,nomeCompleto};
    bool aumentouAltura = false;
    rede.raiz_usuarios = insereAVL(rede.raiz_usuarios, novo_usuario->id, novo_usuario, aumentouAltura);
    insereHash(rede, novo_usuario);
    saida << "USER_ADDED\n";
}

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida){
    node_arvore *node = buscarAVL(rede.raiz_usuarios, id);
    if(node != nullptr){
        usuario *user = (usuario*)node->dado;
        saida << "USER " << user->id << " " << user->username << " " << user->nome << "\n";
    }
    else{
        saida << "ERROR USER_NOT_FOUND" << "\n";
    }
    
}

void buscarUsuarioPorUsername(MiniRede& rede, std::string username, std::ostream& saida){
    node_hash *resultado = buscarHash(rede, username);

    if(resultado == nullptr){
        saida << "ERROR USER_NOT_FOUND" << "\n";
        return;
    }
    saida << "USER " << resultado->usuario->id << " " << resultado->usuario->username << " " << resultado->usuario->nome << "\n";
}

void listarUsuarios(MiniRede& rede, std::ostream& saida){
    saida << "USERS_BEGIN\n";
    imprimirUsuariosAVL(rede.raiz_usuarios, saida);
    saida << "USERS_END\n";
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida){
    node_arvore *node_seguidor = buscarAVL(rede.raiz_usuarios, idSeguidor);
    node_arvore *node_seguido = buscarAVL(rede.raiz_usuarios, idSeguido);

    if(node_seguidor == nullptr || node_seguido == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if(idSeguidor == idSeguido){
        saida << "ERROR CANNOT_FOLLOW_SELF\n";
        return;
    }
    usuario *seguidor = (usuario*)node_seguidor->dado;
    usuario *seguido  = (usuario*)node_seguido->dado;

    if(!jaSeguido(&(seguidor->seguindo), seguido)){
        novoSeguidor(&(seguidor->seguindo), seguido);
        saida << "FOLLOWED\n";

        adicionarNotificacao(seguido, FOLLOW,idSeguidor,-1);
        //ADICIONAR NOTIFICAÇÃO - Adicionada
        return;
    }

    saida << "ERROR ALREADY_FOLLOWING\n";
}

void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida){
    node_arvore *node = buscarAVL(rede.raiz_usuarios, idUsuario);
    if(node == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    saida << "FOLLOWING_BEGIN\n";
    node_lista_usuarios *atual = ((usuario*)node->dado)->seguindo.inicio;
    while(atual != nullptr){
        saida << "USER " << atual->usuario->id << " " << atual->usuario->username << " " << atual->usuario->nome << "\n";
        atual = atual->prox;
    }
    saida << "FOLLOWING_END\n";
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, std::string texto, std::ostream& saida){
    node_arvore *node = buscarAVL(rede.raiz_usuarios, idAutor);
    if(node == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    node_arvore *checagem = buscarAVL(rede.raiz_publicacoes, idPost);
    if(checagem != nullptr){
        saida << "ERROR POST_EXISTS\n";
        return;
    }

    publicacao *novo_post = new publicacao{idPost, idAutor, timestamp, texto};
    bool aumentou = false;
    rede.raiz_publicacoes = insereAVL(rede.raiz_publicacoes, idPost, novo_post, aumentou);
    novaPublicacaoLista(&(((usuario*)node->dado)->publicacoes), novo_post);
    saida << "POST_ADDED" << "\n";
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida){
    node_arvore *node_usuario = buscarAVL(rede.raiz_usuarios, idUsuario);
    if(node_usuario == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    node_arvore *node_post = buscarAVL(rede.raiz_publicacoes, idPost);
    if(node_post == nullptr){
        saida << "ERROR POST_NOT_FOUND\n";
        return;
    }
    usuario *user = (usuario*)node_usuario->dado;
    publicacao *post = (publicacao*)node_post->dado;
    node_lista_usuarios *atual = post->curtiram.inicio;

    while(atual != nullptr){
        if(atual->usuario->id == idUsuario){
            saida << "ERROR ALREADY_LIKED\n";
            return;
        }
        atual = atual->prox;
    }
    post->curtidas += 1;
    node_lista_usuarios *novo = new node_lista_usuarios{user, post->curtiram.inicio};  
    post->curtiram.inicio = novo;
    saida << "LIKED\n";

    node_arvore *node_dono = buscarAVL(rede.raiz_usuarios,post->idUsuario);
    usuario *dono_post = (usuario*)node_dono->dado;
    adicionarNotificacao(dono_post, LIKE,idUsuario,idPost);
    //ADICIONAR NOTIFICAÇÃO - Adicionada
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida){

    node_arvore *node = buscarAVL(rede.raiz_usuarios, idUsuario);
    if(node == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }

    usuario *user = (usuario*)node->dado;

    saida << "NOTIFICATIONS_BEGIN\n";

    int notificacoes = 0;

    while(notificacoes != k && user->notificacoes.inicio != nullptr){
        node_fila *aux = user->notificacoes.inicio;
        if (aux->notificacao.tipo == FOLLOW) {
            saida << "NOTIFICATION FOLLOW " << " " << aux->notificacao.idUsuario << "\n";
        }
        else if(aux->notificacao.tipo == LIKE){
            saida << "NOTIFICATION LIKE " <<  aux->notificacao.idUsuario << " " << aux->notificacao.idPost << "\n";
        }
        user->notificacoes.inicio = aux->prox;
        delete aux;

        notificacoes++;
    }
    saida << "NOTIFICATIONS_END\n"; // está com algum erro que eu não sei qual seria o problema
}

void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida){
    node_arvore *node_usuario = buscarAVL(rede.raiz_usuarios, idUsuario);
    if (node_usuario == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    usuario *user = (usuario*)node_usuario->dado;
    lista_publicacoes copia_lista_publicacoes;
    copia_lista_publicacoes.inicio = nullptr;

    guardarPost(user, copia_lista_publicacoes);
    ordenacaoFeed(copia_lista_publicacoes);

    saida << "FEED_BEGIN\n";

    int publicacoes = 0;

    node_lista_publicacoes * atual = copia_lista_publicacoes.inicio;

    while(publicacoes != k && atual != nullptr){

        saida << "POST" << " " << atual->publicacao->id << " " << atual->publicacao->idUsuario << " " << atual->publicacao->timestamp << " " << atual->publicacao->curtidas << " " << atual->publicacao->texto << "\n";
        atual = atual->prox;
        publicacoes++;
    }
    saida << "FEED_END\n";

    node_lista_publicacoes *liberar = copia_lista_publicacoes.inicio;
    while(liberar != nullptr){
        node_lista_publicacoes *aux = liberar;
        liberar = liberar->prox;
        delete aux;
    }
}

void listarTopPosts(MiniRede& rede, int k, std::ostream& saida){
    lista_publicacoes copia_lista_publicacoes;
    copia_lista_publicacoes.inicio = nullptr;

    armazenarPost(rede.raiz_usuarios, copia_lista_publicacoes);
    ordenarRanking(copia_lista_publicacoes);

    saida << "TOP_POSTS_BEGIN\n";

    int publicacoes = 0;
    node_lista_publicacoes * atual = copia_lista_publicacoes.inicio;

    while(publicacoes != k && atual != nullptr){
        saida << "POST " << atual->publicacao->id << " " << atual->publicacao->idUsuario << " " << atual->publicacao->timestamp
        << " " << atual->publicacao->curtidas << " " << atual->publicacao->texto << "\n";

        atual = atual->prox;
        publicacoes++;
    }
    saida << "TOP_POSTS_END\n";

    node_lista_publicacoes *liberar = copia_lista_publicacoes.inicio;
    while(liberar != nullptr){
        node_lista_publicacoes *aux = liberar;
        liberar = liberar->prox;
        delete aux;
    }
}

int main(){
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);

    return 0;
}

