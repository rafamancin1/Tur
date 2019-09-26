#include "MaquinaDeTuring.h"

/******
 * Autor: Rafael Mancini Santos.
 * NOTAS SOBRE O PROGRAMA:
 * A ordem de declaração das variáveis no struct Maquina no arquivo MaquinaDeTuring.h importa. Se o char for declarado depois, 
 a manipulação das variáves produz comportamento inesperado. Não sei o porquê.
 * Por algum motivo, após o uso da função strlen da biblioteca string na função entradaValida, os ponteiros dos dois alfabetos bugavam, 
 fazendo com que a máquina escrevesse caracteres inesperados no arquivo saida, além de fazer a própria máquina funcionar de forma 
 inesperada, rejeitando entradas que deveriam ser aceitas. Por isso, criei uma função análoga chamada strTam para o mesmo propósito
 da strlen, porém mais simples.
******/

void abortar(const char * desc)//Aborta o programa, escrevendo na stream de saída a descrição do motivo.
{
	printf("%s\n", desc);
	exit(EXIT_FAILURE);
}

int strTam(const char * str)//Mesma propósito da função strlen da biblioteca string
{
	int i;
	while(str[i] != '\0') i++;
	return i;
}

char * carregarArq(char * conteudoArq, char * nomeArq)
{
	/***
	 * Esta função carrega todo o conteúdo do arquivo de descrição dentro de um vetor de
	 caracteres para, assim, facilitar a configuração da máquina a ser simulada.
	 ***/
	FILE * confMT = fopen(nomeArq, "r");
	if(!confMT) return NULL;
	conteudoArq = malloc(512*sizeof(char));
	int i = 0;
	char c;
	c = fgetc(confMT);
	while(c != EOF)
	{
		if(conteudoArq[i] != ' ' && conteudoArq[i] != '\n' && conteudoArq[i] != '\t')
			conteudoArq[i] = c;
		c = fgetc(confMT);
		i += 1;
	}
	fclose(confMT);
	return conteudoArq;
}

struct Maquina * configurarSimulacao(char * conteudoArq, struct Maquina * MT)
{
	/***
	 * Esta função configura o funcionamento da máquina de Turing a ser simulada.
	 * Para isso, a função lê a variável conteudoArq que tem todo o conteudo do arquivo de descrição da máquina.
	 * Através da estrutura switch, a função sabe como tratar cada '{' que ela encontrar no caminho,
	 pois cada um indica algo diferente a se tratar. Ex.: Ao ler o primeiro '{', a função sabe que deve
	 ler o conjunto de estados da máquina.
	 ***/
	int i = 0;
	while(conteudoArq[i] != '\0')
	{	
		if(conteudoArq[i] == '(')
		{
			i += 1;
			int flag = 0; //Conta quantas vezes o caractere { foi lido
			while(conteudoArq[i] != ')')
			{
				
				i += 1;
				if(conteudoArq[i] == '{')
				{
					i += 1;
					flag += 1; //Soma cada vez que o caractere é lido, indicando o que será lido a seguir
					int j = 0; //Para percorrer o alfabeto de entrada da máquina
					while(conteudoArq[i] != '}') 
					{
						switch(flag)
						{
							case 1: //Lê os estados
								if(conteudoArq[i] == 'q')
								{
									char novoEstado[2];
									novoEstado[0] = conteudoArq[i];
									novoEstado[1] = conteudoArq[i+1];
									addEstado(novoEstado, MT, MT->estados, 0);
								}
								break;
							case 2: //Lê o alfabeto de entrada
								if(conteudoArq[i] != ',')
								{
									MT->alfabetoEntrada[j] = conteudoArq[i];
									MT->nSimbolosEntrada += 1;
									j += 1;
								}
								break;
								
							case 3: //Lê o alfabeto da fita
								if(conteudoArq[i] != ',')
								{
									MT->alfabetoSaida[j] = conteudoArq[i];
									MT->nSimbolosSaida += 1;
									j += 1;
								}
								break;
							case 4:
								if(conteudoArq[i] == '(')
								{
									i += 2;
									struct Transicao novaTransicao;
									novaTransicao.estadoEntrada = conteudoArq[i] - 48;
									i += 2;
									novaTransicao.leitura = conteudoArq[i];
									i += 6;
									novaTransicao.proxEstado = conteudoArq[i]-48;
									i += 2;
									novaTransicao.escrita = conteudoArq[i];
									i += 2;
									novaTransicao.direcao = conteudoArq[i];
									int valida = addTransicao(novaTransicao, MT);
									if(!valida)
									{
										free(conteudoArq);
										free(MT);
										abortar("Alguma transição não possui a leitura ou escrita válida.");//Melhorar o aviso da função abortar.
									}
								}
								break;
							case 5:
								if(conteudoArq[i] == 'q')
								{
									char novoEstado[2];
									novoEstado[0] = conteudoArq[i];
									novoEstado[1] = conteudoArq[i+1];
									addEstado(novoEstado, MT, MT->estadosFinais, 1);
								}
								break;
									
						}
						i += 1;
					}
				}
			if(conteudoArq[i] == 'q' && flag == 4)
				MT->estadoInicial = conteudoArq[i+1]-48;
			}
		}
		i += 1;
	}
	MT->alfabetoEntrada[MT->nSimbolosEntrada] = '\0';
	MT->alfabetoSaida[MT->nSimbolosSaida] = '\0';
	return MT;
}

void escreverConf(char * estadoAtual, char * entrada, int cabeca, FILE * arqSaida)
{
	/***
	 * Estafunção escreve a configuração atual no arquivo de saída.
	 * O uso da strlen aqui não buga os alfabetos.
	 ***/
	int i;
	for(i = 0; i < strlen(entrada); ++i)
	{
		if(i == cabeca) fprintf(arqSaida, "%s", estadoAtual);
		fprintf(arqSaida, "%c", entrada[i]);
	}
	if(cabeca == strlen(entrada)) fprintf(arqSaida, "%s", estadoAtual);
	fprintf(arqSaida, "\n");
}

void gerarEstadoString(char * estadoArq, Estado atual)
{
	/***
	 * A função gera uma string do estado(Ex.: {q0}) para que seja escrita no arquivo de saída.
	 ***/
	estadoArq[0] = '{';
	estadoArq[1] = 'q';
	char estadoID = atual+48;
	estadoArq[2] = estadoID;
	estadoArq[3] = '}';
	estadoArq[4] = '\0';
}
	
void rodarSimulacao(struct Maquina * MT, char * entrada, char * nomeArq)
{
	/******************
	 * Essa é a função principal desta simulação de uma máquina de Turing.
	 * A variável cabeca funciona como a cabeça de leitura de uma máquina de Turing.
	 * Primeiro, atualiza-se a entrada, colocando os caracteres 'B' no começo e no fim;(O uso de strlen
	 aqui também não buga os alfabetos)
	 * depois guarda-se os estados finais e o estado inicial em variáveis locais;
	 * assim, entra-se num loop que só finaliza se:
	 * --->o estado atual está contido no conjunto dos estados finais. A máquina para aceitando a entrada;
	 * --->o estado atual não lê o caractere em que a cabeça de leitura(variável cabeca) está. A máquina para
	 rejeitando a entrada.
	 ******************/
	FILE * arqSaida = fopen(nomeArq, "w+");
	if(!arqSaida) return;
	int cabeca = 0;
	int i;
	char entradaN[strlen(entrada)+7];
	entradaN[0] = 'B';
	//Por algum motivo, não dava muito certo usar strcat em vez da atribuição acima.
	strcat(entradaN, entrada);
	strcat(entradaN, "B");
	//EntradaN é a entrada após a inserção dos brancos no início e no final. Esta será usada nos processos a seguir.
	Estado atual = MT->estadoInicial;
	Estado finais[MT->nEstadosFinais];
	for(i = 0; i < MT->nEstadosFinais; ++i)
		finais[i] = MT->estadosFinais[i];
	int final = 0;
	int inicialEfinal = 0;
	//Checar se o estado inicial é igual àlgum final
	for(i = 0; i < MT->nEstadosFinais; ++i)
	{
		if(atual == finais[i])
			inicialEfinal = 1;
	}
	while(atual != finais[final] && !inicialEfinal) //Corpo do funcionamento da máquina.
	{
		char estadoArq[5];
		gerarEstadoString(estadoArq, atual);
		escreverConf(estadoArq, entradaN, cabeca, arqSaida);
		int flag_2 = 0; //Indica se alguma transição leu a entrada.
		for(i = 0; i < MT->nTransicoes; i++)
		{
			if(atual == MT->transicoes[i].estadoEntrada)
			{
				if(entradaN[cabeca] == MT->transicoes[i].leitura)
				{
					entradaN[cabeca] = MT->transicoes[i].escrita;
					if(MT->transicoes[i].direcao == 'D')
						cabeca += 1;
					else
						cabeca -= 1;
					atual = MT->transicoes[i].proxEstado;
					flag_2 = 1; //Se leu, o valor é alterado para 1, representando True.
					break;
				}
			}
		}
		if(!flag_2)//Se nenhuma entrada foi lida, a máquina rejeita a entrada.
		{
			fprintf(arqSaida, "rejeita\n");
			fclose(arqSaida);
			return;
		}
		for(i = 0; i < MT->nEstadosFinais; ++i)
		{
			//Procura se o estado atual está contido no conjunto de estados finais.
			if(atual == finais[i])
				final = i;
		}
	}
	char estadoArq[5];
	gerarEstadoString(estadoArq, atual);
	escreverConf(estadoArq, entradaN, cabeca, arqSaida);
	fprintf(arqSaida, "aceita\n");
	fclose(arqSaida);
}

int entradaValida(const char * entrada, const char * alfabetoEntrada, const int tamanho)
{
	/***
	 * Esta função procura se a entrada possui caracteres que não estão contidos no alfabeto de entrada
	 da máquina.
	 ***/
	int i;
	int valida;
	for(i = 0; i < strTam(entrada); ++i)
	{
		valida = estaNoAlfabeto(entrada[i], alfabetoEntrada, tamanho);
		if(!valida) return 0;
	}
	return 1;
}

int main(int argc, char * argv[])
{
	if(!argv[1] || !argv[2] || !argv[3])
		abortar("Uso: ./Sim <arquivo de descrição> <fita de entrada> <arquivo de saída>");
	char * conteudoArq = NULL;
	conteudoArq = carregarArq(conteudoArq, argv[1]);
	if(!conteudoArq)
		abortar("Arquivo não foi carregado com sucesso. Abortando.");
	struct Maquina * MT = criarMaquina();
	if(!MT)
	{
		free(conteudoArq);
		abortar("Memória insuficiente. Máquina não pode ser criada");
	}
	MT = configurarSimulacao(conteudoArq, MT);
	if(!entradaValida(argv[2], MT->alfabetoEntrada, MT->nSimbolosEntrada))
	{
		free(conteudoArq);
		free(MT);
		abortar("Algum caractere da entrada não está contido no alfabeto de entrada da máquina.");
	}
	rodarSimulacao(MT, argv[2], argv[3]);
	free(conteudoArq);
	free(MT);
	return EXIT_SUCCESS;
}
