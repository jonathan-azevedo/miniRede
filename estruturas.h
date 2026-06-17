#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <iostream>
#include <string>

#include "minirede.h"


struct usuario;
struct MiniRede;
struct lista_usuarios;
struct lista_publicacoes;
struct publicacao;

enum tipoNotificacao{FOLLOW, LIKE};

struct notificacao{
    tipoNotificacao tipo;
    int idUsuario;
    int idPost;
};

struct node_arvore{
    void *dado;
    int id;
    int FB;
    node_arvore *esq;
    node_arvore *dir;
};

struct node_hash{
    usuario *usuario;
    node_hash *prox;
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

node_arvore *rotacaoDireita(node_arvore *a);
node_arvore *rotacaoEsquerda(node_arvore *a);
node_arvore *rotacaoEsquerdaDireita(node_arvore *a);
node_arvore *rotacaoDireitaEsquerda(node_arvore *a);
node_arvore *casoEsquerda(node_arvore* a, bool& aumentouAltura);
node_arvore *casoDireita(node_arvore* a, bool& aumentouAltura);
node_arvore *insereAVL(node_arvore* a, int id, void *dado, bool& aumentouAltura);
node_arvore *buscarAVL(node_arvore *a, int id);
void imprimirUsuariosAVL(node_arvore *a, std::ostream& saida);
unsigned int calcularHash(const std::string& username);
void insereHash(MiniRede& rede, usuario *novo_usuario);
node_hash *buscarHash(MiniRede& rede, std::string username);
void novoSeguidor(lista_usuarios *seguidos, usuario *novo);
bool jaSeguido(lista_usuarios *seguidos, usuario *novo);
void novaPublicacaoLista(lista_publicacoes *publicacoes, publicacao *novo);
void adicionarNotificacao(usuario* usuario1, tipoNotificacao tipo, int idOrigem, int idPost);
void guardarPost(usuario* usuario, lista_publicacoes& copia_lista_publicacoes);
bool gerenciarPost(node_lista_publicacoes *post1, node_lista_publicacoes *post2);
void ordenacaoFeed(lista_publicacoes& copia_lista_publicacoes);
bool rankCurtidas(node_lista_publicacoes *post1, node_lista_publicacoes *post2);
void ordenarRanking(lista_publicacoes& copia_lista_publicacoes);
void armazenarPost(node_arvore *a, lista_publicacoes& lista_posts);
void liberarListaUsuarios(lista_usuarios &lista);
void liberarListaPublicacoes(lista_publicacoes &lista);
void liberarArvoreUsuarios(node_arvore *a);
void liberarArvorePublicacoes(node_arvore *a);


#endif