package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Stack;

public class Profundidade {
	private List<List<Integer>> ciclos;
	
	public Profundidade(){
		ciclos = new ArrayList<List<Integer>>();
	}
	
	public List<List<Integer>> getCiclos(){
		return ciclos;
	}
	
	/*
	 * Testa se existem ciclos em um Grafo.
	 */
	
	public boolean testeCiclos(Grafo g){
//		g.imprime();
		char[] coresNos = new char[g.getNumVertices()];
		for(int i = 0; i < g.getNumVertices(); i++) coresNos[i] = 'b';
		Stack<Integer> back = new Stack<Integer>();
		ciclos = new ArrayList<List<Integer>>();
		
		int raiz = 0;
		while(raiz != -1){
			busca(g, raiz, coresNos, back);
			raiz = -1;
			
			// Se houver nos ainda nao coloridos, significa que o grafo eh
			// desconexo. Entao identifica-se um no de um desses subgrafos
			// e realiza a busca em cima desse subgrafo.
			for(int i = 0; i < g.getNumVertices(); i++){
				if(coresNos[i] == 'b'){
					raiz = i;
					break;
				}
			}
		}
		
		return ciclos.isEmpty();
	}
	
	
	/*
	 * verifica se o ciclo detectado ja esta mapeado nos ciclos
	 * descobertos anteriormente
	 */
	private void adicionaCiclo(List<Integer> novoCiclo){
		for(List<Integer> ciclo: ciclos){
			if(ciclo.containsAll(novoCiclo)) return;
		}
		
		ciclos.add(novoCiclo);
	}
	
	/*
	 * Realiza a busca pelos ciclos do grafo a partir do nodo raiz u.
	 */
	public int busca(Grafo g, int u, char[] coresNos, Stack<Integer> back){
		Grafo.Item v = g.getNewItem();
		coresNos[u] = 'c';
		back.push(u);
		
		if(g.primeiroAdj(u, v)){
			do{
				// verifica se o no adjacente em questao nao foi visitado na busca atual
				if(coresNos[v.i] == 'b' || coresNos[v.i] == 'm'){
					
					// realiza a busca para o no adjacente
					busca(g, v.i, coresNos, back);
				}
				else{
					// se o no jah foi visitado, entao foi completado um ciclo
					for(Iterator it = (Iterator) back.iterator(); it.hasNext();){
						Integer i = (Integer) it.next();
						
						// procura-se o ponto do arvore em que foi descoberto o no que
						// originou o ciclo
						if(i == v.i){
							List<Integer> novoCiclo = new ArrayList<Integer>();
							
							// a partir desse no, adiciona-se todos os proximos ate fechar
							// nele novamente
							novoCiclo.add(v.i);
							while(it.hasNext()) novoCiclo.add((Integer) it.next());
							novoCiclo.add(v.i);
							
							adicionaCiclo(novoCiclo);
							break;
						}
					}
				}
			}
			while(g.proxAdj(u, v));
		}
		
		// retira no que terminou de ser visitado e marca como não mais em uso,
		// mas que jah foi visitado alguma vez. 
		
		back.pop();
		coresNos[u] = 'm';
		return 0;
	}
	
}
