#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 50
#define MAX_SIMBOLOS 50
#define MAX_TRANSICOES 50

typedef int Estado;

struct Transicao
{
	Estado estadoEntrada;
	char leitura;
	Estado proxEstado;
	char escrita;
	char direcao;
};

struct Maquina
{
	//Os vetores de char DEVEM ser declarados antes, não sei porque.
	char alfabetoEntrada[MAX_SIMBOLOS];
	char alfabetoSaida[MAX_SIMBOLOS];
	Estado estadoInicial;
	Estado estadosFinais[MAX_ESTADOS];  
	Estado estados[MAX_ESTADOS];
	struct Transicao transicoes[MAX_TRANSICOES];
	int nSimbolosEntrada;
	int nSimbolosSaida;
	int nEstados;
	int nEstadosFinais;
	int nTransicoes;
	
};

struct Maquina * criarMaquina();	
int addTransicao(struct Transicao transicao, struct Maquina * MT);
void addEstado(char * estado, struct Maquina * MT, Estado * estados, int final);
/*
 * A função addAlfabeto() não é usada na simulacao, mas a mantive  no módulo por questões de coerência.
 * Caso o módulo fosse usado para simular uma máquina de Turing sem receber a descrição de um arquivo, a 
 função poderia ser usada para facilitar a adição de alfabetos.
 * Eu não a usei pois achei mais conveniente adicionar o alfabeto da forma como fiz, considerando a estrutura switch
 usada.
 */
void addAlfabeto(char * alfabeto, struct Maquina * MT);
void listarEstados(struct Maquina * MT); //funcao para teste
void listarAlfabeto(const char * alfabeto);
void listarTransicoes(struct Maquina * MT);
int estaNoAlfabeto(const char c, const char * alfabeto, const int tamanho);


