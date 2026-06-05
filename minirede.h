#ifndef MINI_REDE_H
#define MINI_REDE_H


#include <iostream>
#include "estruturas.h"
//#include "minirede.h"

const int TAM_USERNAME = 50;
const int TAM_NOME = 100;
const int TAM_TEXTO = 280;
const int TAM_COMANDO = 30;
const int TAM_HASH = 181;

// TODO: definir as structs principais do trabalho.
//
// Sugestao de structs que provavelmente serao necessarias:
// - Usuario
// - Publicacao
// - MiniRede
// - nos para lista encadeada
// - nos para arvore binaria de usuarios por id
// - nos para tabela hash de usernames
// - nos para fila de notificacoes
//
// Os campos de cada struct fazem parte do projeto dos alunos.
struct usuario;
struct publicacao;
struct node_fila;
struct node_lista_publicacoes;
struct node_lista_usuarios;
struct lista_publicacoes;
struct lista_usuarios;
struct fila_notificacoes;

struct MiniRede {
    node_arvore *raiz_arvore;
    node_hash *tabela_hash[TAM_HASH];
    // TODO: declarar aqui os ponteiros/estruturas principais da rede.
    //
    // Exemplos de responsabilidades:
    // - usuarios armazenados por id
    // - usuarios acessiveis por username
    // - publicacoes cadastradas
};

enum tipoNotificacao{FOLLOW, LIKE};

struct notificacao{
    tipoNotificacao tipo;
    int idUsuario;
    int idPost;
};

struct node_lista_usuarios{
    usuario *usuario;
    node_lista_usuarios *prox;
};

struct node_lista_publicacoes{
    publicacao *publicacao;
    node_lista_publicacoes *prox;
};

struct node_fila{
    notificacao notificacao;
    node_fila *prox;
};

struct lista_usuarios{
    node_lista_usuarios *inicio;
};
struct lista_publicacoes{
    node_lista_publicacoes *inicio;
};

struct fila_notificacoes{
    node_fila *inicio;
    node_fila *fim;
};

struct usuario{ 
    int id;
    std::string username;
    std::string nome;
    lista_usuarios seguindo;
    lista_publicacoes publicacoes;
    fila_notificacoes notificacoes;

    usuario(int ID, std::string USERNAME, std::string NOME){
        id = ID;
        username = USERNAME;
        nome = NOME;
        seguindo.inicio = nullptr;
        publicacoes.inicio = nullptr;
        notificacoes.inicio = nullptr;
        notificacoes.fim = nullptr;
    }
};

struct publicacao{
    int id;
    int idUsuario;
    int timestamp;
    std::string texto;
    int curtidas;
    lista_usuarios curtiram;
};

void inicializarMiniRede(MiniRede& rede);
void liberarMiniRede(MiniRede& rede);
void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida);

void cadastrarUsuario(MiniRede& rede, int id, std::string username, std::string nomeCompleto, std::ostream& saida);
void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida);
void buscarUsuarioPorUsername(MiniRede& rede, std::string username, std::ostream& saida);
void listarUsuarios(MiniRede& rede, std::ostream& saida);

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida);
void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida);

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, std::string texto, std::ostream& saida);
void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida);

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void listarTopPosts(MiniRede& rede, int k, std::ostream& saida);


void novo_seguidor(lista_usuarios *seguidos,usuario *novo);
bool ja_seguido(lista_usuarios *seguidos,usuario *novo);
void nova_publicacao(lista_publicacoes *publicacoes, publicacao *novo);

// TODO: declarar aqui as funcoes auxiliares escolhidas pelo grupo.
//
// Exemplos de responsabilidades auxiliares:
// - buscar usuario por id
// - buscar usuario por username
// - buscar publicacao por id
// - inserir/listar/liberar arvore
// - inserir/buscar/liberar tabela hash
// - enfileirar/desenfileirar notificacoes
// - manipular listas encadeadas
// - ordenar vetores auxiliares para feed e ranking

#endif
