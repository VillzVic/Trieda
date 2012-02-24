package com.gapso.web.trieda.server.util;

public class Grafo {
	
	public class Item{
		public int i;
		
		public Item(){
			i = 0;
		}
	}
	private int matriz[][];
	private int numVertices;
	
	public int getNumVertices(){
		return numVertices;
	}
	
	/*
	 * Inicializa a estrutura de um Grafo atribuindo a todas as arcos possiveis
	 * um peso igual a -1. Esse valor indica que tal arco nao existe no grafo.
	 */

	Grafo(int numVertices){
		this.numVertices = numVertices;
		int i,j;
		matriz = new int[numVertices][numVertices];
		for(i = 0; i < numVertices; i++){
			for(j = 0; j < numVertices; j++) matriz[i][j] = 0;
		}
	}
	
	public Item getNewItem(){
		return new Item();
	}

	/*
	 * Retorna o peso da arco (v1, v2)
	 */

	int getPesoArco(int v1, int v2){
		return matriz[v1][v2];
	}

	/*
	 * Insere uma arco de peso p, ligando os vertices v1 e v2 no grafo
	 */
	
	void insereArco(int v1, int v2){
		matriz[v1][v2] = 1;
	}

	void insereArco(int v1, int v2, int p){
		matriz[v1][v2] = p;
	}

	/*
	 * Retorna em aux o indice do primeiro vertice da lista de adjacencia do vertice v.
	 * Caso nao exista nenhum vertice adjacente, o metodo retorna 0. Caso contrario,
	 * retorna 1.
	 */

	boolean primeiroAdj(int v, Item aux){
		int i;
		for(i = 0; i < numVertices; i++){
			if(matriz[v][i] > 0){
				aux.i = i;
				return true;
			}
		}
		return false;
	}

	/*
	 * Coloca em prox o valor do proximo vertice adjacente a v. Retorna 0 caso
	 * nao exista mais vertices adjacentes. Caso contrario, retorna 1.
	 */

	boolean proxAdj(int v, Item prox){
		for(prox.i++; (prox.i < numVertices) && (matriz[v][prox.i] == 0); prox.i++);
		return prox.i != numVertices;
	}

	/*
	 * Esse metodo imprime um grafo. Os valores da primeira linha e da primeira
	 * coluna sao os vertices do grafo. Dessa forma, um elemento xij tal que
	 * i != 0 e j!=0 indica o peso da arco que liga o vertice i ao vertice j.
	 * Caso nao exista uma arco conectando dois vertices, imprime-se 's' no lugar
	 */

	void imprime(){
		int i, j;
		System.out.print("    ");
		for(i = 0; i < numVertices; i++) System.out.print("   " + i);
		System.out.println("");
		for (i = 0; i < numVertices; i++){
			System.out.print("   " + i);
			for (j = 0; j < numVertices; j++){
				if(matriz[i][j] == 0) System.out.print("   -");
				else System.out.print("   " + matriz[i][j]);
			}
			System.out.println("");
		}
	}
}