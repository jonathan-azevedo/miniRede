#include "minirede.h"


void inicializarMiniRede(MiniRede& rede){
    node_arvore *raiz = nullptr;
    rede.raiz_arvore = raiz;
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
    }
}

void cadastrarUsuario(MiniRede& rede, int id, std::string username, std::string nomeCompleto, std::ostream& saida){
    usuario *novo_usuario = new usuario{id,username,nomeCompleto};
    bool aumentouAltura = false;
    rede.raiz_arvore = insereAVL(rede.raiz_arvore, novo_usuario, aumentouAltura);
    saida << "USER_ADDED\n";
}

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida){
    // TODO
}

void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida){
    // TODO
}

void listarUsuarios(MiniRede& rede, std::ostream& saida){
    // TODO
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida){
    // TODO
}

void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida){
    // TODO
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida){
    // TODO
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

