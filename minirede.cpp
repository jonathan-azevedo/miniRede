#include "minirede.h"


void inicializarMiniRede(MiniRede& rede){
    rede.raiz_arvore = nullptr;
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
            std::getline(std::cin, texto);
            cadastrarPublicacao(rede, postid, id, timestamp, texto, saida);

        }
        else{
            saida << "ERROR INVALID_COMMAND\n";
        }
    }
}

void cadastrarUsuario(MiniRede& rede, int id, std::string username, std::string nomeCompleto, std::ostream& saida){
    usuario *checagem_arvore = buscarAVL(rede.raiz_arvore, id);
    node_hash *checagem_hash = buscarHash(rede, username);
    if(checagem_arvore != nullptr || checagem_hash != nullptr){
        saida << "ERROR USER_EXISTS\n";
        return;
    }
    usuario *novo_usuario = new usuario{id,username,nomeCompleto};
    bool aumentouAltura = false;
    rede.raiz_arvore = insereAVL(rede.raiz_arvore, novo_usuario, aumentouAltura);
    insereHash(rede, novo_usuario);
    saida << "USER_ADDED\n";
}

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida){
    usuario *user = buscarAVL(rede.raiz_arvore, id);

    if(user != nullptr){
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
    imprimirAVL(rede.raiz_arvore, saida);
    saida << "USERS_END\n";
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida){
    usuario *seguidor = buscarAVL(rede.raiz_arvore, idSeguidor);
    usuario *seguido = buscarAVL(rede.raiz_arvore, idSeguido);

    if(seguidor == nullptr || seguido == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if(idSeguidor == idSeguido){
        saida << "ERROR CANNOT_FOLLOW_SELF\n";
        return;
    }

    if(!jaSeguido(&seguidor->seguindo, seguido)){
        novoSeguidor(&seguidor->seguindo, seguido);
        saida << "FOLLOWED\n";
        //notificar
        return;
    }

    saida << "ERROR ALREADY_FOLLOWING\n";
}

void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida){
    usuario *user = buscarAVL(rede.raiz_arvore, idUsuario);
    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    saida << "FOLLOWING_BEGIN\n";
    node_lista_usuarios *atual = user->seguindo.inicio;
    while(atual != nullptr){
        saida << "USER " << atual->usuario->id << " " << atual->usuario->username << " " << atual->usuario->nome << "\n";
        atual = atual->prox;
    }
    saida << "FOLLOWING_END\n";
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, std::string texto, std::ostream& saida){
    usuario *id_usuario = buscarAVL(rede.raiz_arvore, idAutor);
    if(id_usuario == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }



    publicacao *novo_post = new publicacao{idPost, idAutor, timestamp, texto};

    novaPublicacao(&(id_usuario->publicacoes), novo_post);
    saida << "POST_ADDED" << "\n";
    // OBS FALTA AINDA ARRUMAR A PARTE DE " ERROR POST_EXISTS" ainda não está implementado
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida){
    // TODO
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida){
    // TODO
}

void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida){
    // TODO
}

void listarTopPosts(MiniRede& rede, int k, std::ostream& saida){
    // TODO
}

int main(){
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);

    return 0;
}

