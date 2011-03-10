package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoCampus;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoDiaSemana;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoOferta;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoSala;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoTatico;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoUnidade;
import com.gapso.web.trieda.server.xml.output.TriedaOutput;

@Transactional
public class SolverOutput {

	private Cenario cenario;
	private TriedaOutput triedaOutput;
	private List<AtendimentoTatico> atendimentosTatico;

	public SolverOutput(Cenario cenario, TriedaOutput triedaOutput) {
		this.cenario = cenario;
		this.triedaOutput = triedaOutput;
		atendimentosTatico = new ArrayList<AtendimentoTatico>();
	}

	@Transactional
	public List<AtendimentoTatico> generateAtendimentosTatico() {
		List<ItemAtendimentoCampus> itemAtendimentoCampusList =  triedaOutput.getAtendimentos().getAtendimentoCampus();
		for(ItemAtendimentoCampus itemAtendimentoCampus : itemAtendimentoCampusList) {
			// Não há necessidade de procurar campus, pq essa informação já existe em sala
			// Campus campus = Campus.find(Long.valueOf(itemAtendimentoCampus.getCampusId()));
			List<ItemAtendimentoUnidade> itemAtendimentoUnidadeList = itemAtendimentoCampus.getAtendimentosUnidades().getAtendimentoUnidade();
			for(ItemAtendimentoUnidade itemAtendimentoUnidade : itemAtendimentoUnidadeList) {
				// Não há necessidade de procurar campus, pq essa informação já existe em sala
				// Unidade unidade = Unidade.find(Long.valueOf(itemAtendimentoUnidade.getUnidadeId()));
				List<ItemAtendimentoSala> itemAtendimentoSalaList = itemAtendimentoUnidade.getAtendimentosSalas().getAtendimentoSala();
				for(ItemAtendimentoSala itemAtendimentoSala : itemAtendimentoSalaList) {
					Sala sala = Sala.find(Long.valueOf(itemAtendimentoSala.getSalaId()));
					List<ItemAtendimentoDiaSemana> itemAtendimentoDiaSemanaList = itemAtendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana();
					for(ItemAtendimentoDiaSemana itemAtendimentoDiaSemana : itemAtendimentoDiaSemanaList) {
						Semanas semana = Semanas.get(itemAtendimentoDiaSemana.getDiaSemana());
						if(itemAtendimentoDiaSemana.getAtendimentosTatico() == null) continue;
						List<ItemAtendimentoTatico> itemAtendimentoTaticoList = itemAtendimentoDiaSemana.getAtendimentosTatico().getAtendimentoTatico();
						for(ItemAtendimentoTatico itemAtendimentoTatico : itemAtendimentoTaticoList) {
							int qtdeCreditosTeoricos = itemAtendimentoTatico.getQtdeCreditosTeoricos();
							int qtdeCreditosPraticos = itemAtendimentoTatico.getQtdeCreditosPraticos();
							ItemAtendimentoOferta itemAtendimentoOferta = itemAtendimentoTatico.getAtendimentoOferta();
							Oferta oferta = Oferta.find(Long.valueOf(itemAtendimentoOferta.getOfertaCursoCampiId()));
							// TODO CORRIGIR NO SOLVER IDs NEGATIVOS
							int disciplinaId = Math.abs(itemAtendimentoOferta.getDisciplinaId());
							Disciplina disciplina = Disciplina.find(Long.valueOf(disciplinaId));
							int quantidade = itemAtendimentoOferta.getQuantidade();
							String turma = "TURMA" + itemAtendimentoOferta.getTurma();
							
							AtendimentoTatico atendimentoTatico = new AtendimentoTatico();
							atendimentoTatico.setCenario(cenario);
							atendimentoTatico.setTurma(turma);
							atendimentoTatico.setSala(sala);
							atendimentoTatico.setSemana(semana);
							atendimentoTatico.setCreditosTeorico(qtdeCreditosTeoricos);
							atendimentoTatico.setCreditosPratico(qtdeCreditosPraticos);
							atendimentoTatico.setOferta(oferta);
							atendimentoTatico.setDisciplina(disciplina);
							atendimentoTatico.setQuantidadeAlunos(quantidade);
							
							atendimentosTatico.add(atendimentoTatico);
						}
					}
				}
			}
		}
		return atendimentosTatico;
	}
	
	@Transactional
	public void salvarAtendimentosTatico() {
		removerTodosAtendimentosTaticoJaSalvos();
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			atendimentoTatico.persist();
		}
	}
	
	@Transactional
	private void removerTodosAtendimentosTaticoJaSalvos() {
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findAll();
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			atendimentoTatico.remove();
		}
	}

}
