#include "MaquinaDeTuring.h"

struct Maquina * criarMaquina()
{
	struct Maquina * MT = malloc(sizeof(struct Maquina));
	MT->estadoInicial = 0;
	MT->nEstados = 0;
	MT->nEstadosFinais = 0;
	MT->nTransicoes = 0;
	MT->nSimbolosEntrada = 0;
	MT->nSimbolosSaida = 0;
	return MT;
}

int estaNoAlfabeto(const char c, const char * alfabeto, const int tamanho)
{
	int i;
	for(i = 0; i < tamanho; ++i)
	{
		if(c == alfabeto[i])
			return 1;
	}
	return 0;
}

int transicaoValida(struct Transicao transicao, struct Maquina * MT)
{
	int i;
	int leituraValida, escritaValida;
	leituraValida = estaNoAlfabeto(transicao.leitura, MT->alfabetoSaida, MT->nSimbolosSaida);
	escritaValida = estaNoAlfabeto(transicao.escrita, MT->alfabetoSaida, MT->nSimbolosSaida);
	if(!leituraValida || !escritaValida)
		return 0;
	return 1;
}		
		
int addTransicao(struct Transicao transicao, struct Maquina * MT)
{
	if(transicaoValida(transicao, MT))
	{
		MT->transicoes[MT->nTransicoes] = transicao;
		MT->nTransicoes += 1;
		return 1;
	}
	else return 0;
}

void addEstado(char * estado, struct Maquina * MT, Estado * estados, int final)
{	
	if(!estado) return;
	int novoEstado = estado[1] - 48;
	if(final)
	{
		estados[MT->nEstadosFinais] = novoEstado;
		MT->nEstadosFinais += 1;
	}
	else
	{
		estados[MT->nEstados] = novoEstado;
		MT->nEstados += 1;
	}
}

void listarEstados(struct Maquina * MT)
{
	int i;
	for(i = 0; i < MT->nEstados; i++)
		printf("%d\n", MT->estados[i]);
}

void listarAlfabeto(const char * alfabeto)
{
	printf("%s\n", alfabeto);
}

void listarTransicoes(struct Maquina * MT)
{
	int i;
	for(i = 0; i < MT->nTransicoes; i++)
		printf("%d -- %c---%d\n", MT->transicoes[i].estadoEntrada, MT->transicoes[i].leitura, MT->transicoes[i].proxEstado);
}
