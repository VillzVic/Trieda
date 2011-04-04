package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Incompatibilidade;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Dificuldades;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.client.util.view.CargaHorariaComboBox.CargaHoraria;
import com.gapso.web.trieda.server.xml.input.GrupoAreaTitulacao;
import com.gapso.web.trieda.server.xml.input.GrupoCampus;
import com.gapso.web.trieda.server.xml.input.GrupoCreditoDisponivel;
import com.gapso.web.trieda.server.xml.input.GrupoCurriculo;
import com.gapso.web.trieda.server.xml.input.GrupoCurso;
import com.gapso.web.trieda.server.xml.input.GrupoDemanda;
import com.gapso.web.trieda.server.xml.input.GrupoDeslocamento;
import com.gapso.web.trieda.server.xml.input.GrupoDiaSemana;
import com.gapso.web.trieda.server.xml.input.GrupoDisciplina;
import com.gapso.web.trieda.server.xml.input.GrupoDisciplinaPeriodo;
import com.gapso.web.trieda.server.xml.input.GrupoDivisaoCreditos;
import com.gapso.web.trieda.server.xml.input.GrupoFixacao;
import com.gapso.web.trieda.server.xml.input.GrupoGrupo;
import com.gapso.web.trieda.server.xml.input.GrupoHorario;
import com.gapso.web.trieda.server.xml.input.GrupoHorarioAula;
import com.gapso.web.trieda.server.xml.input.GrupoIdentificador;
import com.gapso.web.trieda.server.xml.input.GrupoNivelDificuldade;
import com.gapso.web.trieda.server.xml.input.GrupoOfertaCurso;
import com.gapso.web.trieda.server.xml.input.GrupoProfessor;
import com.gapso.web.trieda.server.xml.input.GrupoProfessorDisciplina;
import com.gapso.web.trieda.server.xml.input.GrupoSala;
import com.gapso.web.trieda.server.xml.input.GrupoTipoContrato;
import com.gapso.web.trieda.server.xml.input.GrupoTipoCurso;
import com.gapso.web.trieda.server.xml.input.GrupoTipoDisciplina;
import com.gapso.web.trieda.server.xml.input.GrupoTipoSala;
import com.gapso.web.trieda.server.xml.input.GrupoTipoTitulacao;
import com.gapso.web.trieda.server.xml.input.GrupoTurno;
import com.gapso.web.trieda.server.xml.input.GrupoUnidade;
import com.gapso.web.trieda.server.xml.input.ItemAreaTitulacao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoCampusSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoDiaSemanaSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoOfertaSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoSalaSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoTaticoSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoUnidadeSolucao;
import com.gapso.web.trieda.server.xml.input.ItemCalendario;
import com.gapso.web.trieda.server.xml.input.ItemCampus;
import com.gapso.web.trieda.server.xml.input.ItemCreditoDisponivel;
import com.gapso.web.trieda.server.xml.input.ItemCurriculo;
import com.gapso.web.trieda.server.xml.input.ItemCurso;
import com.gapso.web.trieda.server.xml.input.ItemDemanda;
import com.gapso.web.trieda.server.xml.input.ItemDeslocamento;
import com.gapso.web.trieda.server.xml.input.ItemDisciplina;
import com.gapso.web.trieda.server.xml.input.ItemDisciplinaPeriodo;
import com.gapso.web.trieda.server.xml.input.ItemDivisaoCreditos;
import com.gapso.web.trieda.server.xml.input.ItemFixacao;
import com.gapso.web.trieda.server.xml.input.ItemHorario;
import com.gapso.web.trieda.server.xml.input.ItemHorarioAula;
import com.gapso.web.trieda.server.xml.input.ItemNivelDificuldade;
import com.gapso.web.trieda.server.xml.input.ItemOfertaCurso;
import com.gapso.web.trieda.server.xml.input.ItemParametrosPlanejamento;
import com.gapso.web.trieda.server.xml.input.ItemParametrosPlanejamento.CargaHorariaSemanalAluno;
import com.gapso.web.trieda.server.xml.input.ItemParametrosPlanejamento.CargaHorariaSemanalProfessor;
import com.gapso.web.trieda.server.xml.input.ItemPercentualMinimo;
import com.gapso.web.trieda.server.xml.input.ItemProfessor;
import com.gapso.web.trieda.server.xml.input.ItemProfessorDisciplina;
import com.gapso.web.trieda.server.xml.input.ItemSala;
import com.gapso.web.trieda.server.xml.input.ItemTipoContrato;
import com.gapso.web.trieda.server.xml.input.ItemTipoCurso;
import com.gapso.web.trieda.server.xml.input.ItemTipoDisciplina;
import com.gapso.web.trieda.server.xml.input.ItemTipoSala;
import com.gapso.web.trieda.server.xml.input.ItemTipoTitulacao;
import com.gapso.web.trieda.server.xml.input.ItemTurno;
import com.gapso.web.trieda.server.xml.input.ItemUnidade;
import com.gapso.web.trieda.server.xml.input.ObjectFactory;
import com.gapso.web.trieda.server.xml.input.TriedaInput;

@Transactional
public class SolverInput {

	private Cenario cenario;
	private ObjectFactory of;
	private TriedaInput triedaInput;

	public SolverInput(Cenario cenario) {
		this.cenario = cenario;
		of = new ObjectFactory();
		triedaInput = of.createTriedaInput();
	}

	@Transactional
	public TriedaInput generateTaticoTriedaInput() {
		generate(true);
		return triedaInput;
	}
	
	@Transactional
	public TriedaInput generateOperacionalTriedaInput() {
		generate(false);
		return triedaInput;
	}
	
	@Transactional
	private void generate(boolean tatico) {
		generateCalendario();
		generateTiposSala();
		generateTiposContrato();
		generateTiposTitulacao();
		generateAreasTitulacao();
		generateTiposDisciplina();
		generateNiveisDificuldade();
		generateTiposCurso();
		generateDivisoesDeCredito();
		generateCampi(tatico);
		generateDeslocamentoCampi();
		generateDeslocamentoUnidades();
		generateDisciplinas();
		generateCurso();
		generateOfertaCursoCampi();
		generateDemandas();
		generateParametrosPlanejamento(tatico);
		generateFixacoes();
		if(!tatico) {
			generateTaticoInput();
		}
	}

	@Transactional
	private void generateCalendario() {
		ItemCalendario itemCalendario = of.createItemCalendario();
		SemanaLetiva calendario = SemanaLetiva.findAll().get(0);
		itemCalendario.setId(calendario.getId().intValue());
		itemCalendario.setCodigo(calendario.getCodigo());
		
		GrupoTurno grupoTurno = of.createGrupoTurno();
		Set<Turno> turnos = cenario.getTurnos();
		for(Turno turno : turnos) {
			ItemTurno itemTurno = of.createItemTurno();
			itemTurno.setId(turno.getId().intValue());
			itemTurno.setNome(turno.getNome());
			itemTurno.setTempoAula(turno.getTempo());
			
			GrupoHorarioAula grupoHorarioAula = of.createGrupoHorarioAula();
			Set<HorarioAula> horariosAula = turno.getHorariosAula();
			for(HorarioAula horarioAula : horariosAula) {
				ItemHorarioAula itemHorarioAula = of.createItemHorarioAula();
				itemHorarioAula.setId(horarioAula.getId().intValue());
				itemHorarioAula.setInicio(new XMLGregorianCalendarUtil(horarioAula.getHorario()));
				
				GrupoDiaSemana grupoDiasSemana = of.createGrupoDiaSemana();
				Set<HorarioDisponivelCenario> horariosDisponivelCenario = horarioAula.getHorariosDisponiveisCenario();
				for(HorarioDisponivelCenario horarioDisponivelCenario : horariosDisponivelCenario) {
					grupoDiasSemana.getDiaSemana().add(Semanas.toInt(horarioDisponivelCenario.getSemana()));
				}
				itemHorarioAula.setDiasSemana(grupoDiasSemana);
				
				grupoHorarioAula.getHorarioAula().add(itemHorarioAula);
				
			}
			itemTurno.setHorariosAula(grupoHorarioAula);
			
			grupoTurno.getTurno().add(itemTurno);
		}
		itemCalendario.setTurnos(grupoTurno);
		triedaInput.setCalendario(itemCalendario);
	}
	
	private void generateTiposSala() {
		GrupoTipoSala grupoTipoSala = of.createGrupoTipoSala();
		List<TipoSala> tipos = TipoSala.findAll();
		for(TipoSala tipo : tipos) {
			ItemTipoSala itemTipoSala = of.createItemTipoSala();
			itemTipoSala.setId(tipo.getId().intValue());
			itemTipoSala.setNome(tipo.getNome());
			grupoTipoSala.getTipoSala().add(itemTipoSala);
		}
		triedaInput.setTiposSala(grupoTipoSala);
	}
	
	private void generateTiposContrato() {
		GrupoTipoContrato grupoTipoContrato = of.createGrupoTipoContrato();
		List<TipoContrato> tipos = TipoContrato.findAll();
		for(TipoContrato tipo : tipos) {
			ItemTipoContrato itemTipoContrato = of.createItemTipoContrato();
			itemTipoContrato.setId(tipo.getId().intValue());
			itemTipoContrato.setNome(tipo.getNome());
			grupoTipoContrato.getTipoContrato().add(itemTipoContrato);
		}
		triedaInput.setTiposContrato(grupoTipoContrato);
	}
	
	private void generateTiposTitulacao() {
		GrupoTipoTitulacao grupoTipoTitulacao = of.createGrupoTipoTitulacao();
		List<Titulacao> tipos = Titulacao.findAll();
		for(Titulacao tipo : tipos) {
			ItemTipoTitulacao itemTipoTitulacao = of.createItemTipoTitulacao();
			itemTipoTitulacao.setId(tipo.getId().intValue());
			itemTipoTitulacao.setNome(tipo.getNome());
			grupoTipoTitulacao.getTipoTitulacao().add(itemTipoTitulacao);
		}
		triedaInput.setTiposTitulacao(grupoTipoTitulacao);
	}
	
	private void generateAreasTitulacao() {
		GrupoAreaTitulacao grupoAreaTitulacao = of.createGrupoAreaTitulacao();
		List<AreaTitulacao> tipos = AreaTitulacao.findAll();
		for(AreaTitulacao tipo: tipos) {
			ItemAreaTitulacao itemAreaTitulacao = of.createItemAreaTitulacao();
			itemAreaTitulacao.setId(tipo.getId().intValue());
			itemAreaTitulacao.setNome(tipo.getCodigo());
			grupoAreaTitulacao.getAreaTitulacao().add(itemAreaTitulacao);
		}
		triedaInput.setAreasTitulacao(grupoAreaTitulacao);
	}
	
	private void generateTiposDisciplina() {
		GrupoTipoDisciplina grupoTipoDisciplina = of.createGrupoTipoDisciplina();
		List<TipoDisciplina> tipos = TipoDisciplina.findAll();
		for(TipoDisciplina tipo: tipos) {
			ItemTipoDisciplina itemTipoDisciplina = of.createItemTipoDisciplina();
			itemTipoDisciplina.setId(tipo.getId().intValue());
			itemTipoDisciplina.setNome(tipo.getNome());
			grupoTipoDisciplina.getTipoDisciplina().add(itemTipoDisciplina);
		}
		triedaInput.setTiposDisciplina(grupoTipoDisciplina);
	}
	
	private void generateNiveisDificuldade() {
		GrupoNivelDificuldade grupoNivelDificuldade = of.createGrupoNivelDificuldade();
		for(Dificuldades dificuldade : Dificuldades.values()) {
			ItemNivelDificuldade itemNivelDificuldade = of.createItemNivelDificuldade();
			itemNivelDificuldade.setId(Dificuldades.toInt(dificuldade));
			itemNivelDificuldade.setNome(dificuldade.name());
			grupoNivelDificuldade.getNivelDificuldade().add(itemNivelDificuldade);
		}
		triedaInput.setNiveisDificuldade(grupoNivelDificuldade);
	}
	
	private void generateTiposCurso() {
		GrupoTipoCurso grupoTipoCurso = of.createGrupoTipoCurso();
		List<TipoCurso> tipos = TipoCurso.findAll();
		for(TipoCurso tipo : tipos) {
			ItemTipoCurso itemTipoCurso = of.createItemTipoCurso();
			itemTipoCurso.setId(tipo.getId().intValue());
			itemTipoCurso.setNome(tipo.getCodigo());
			grupoTipoCurso.getTipoCurso().add(itemTipoCurso);
		}
		triedaInput.setTiposCurso(grupoTipoCurso);
	}
	
	private void generateDivisoesDeCredito() {
		GrupoDivisaoCreditos grupoDivisaoCreditos = of.createGrupoDivisaoCreditos();
		Set<DivisaoCredito> regras = cenario.getDivisoesCredito();
		for(DivisaoCredito regra : regras) {
			ItemDivisaoCreditos item = of.createItemDivisaoCreditos();
			item.setId(regra.getId().intValue());
			item.setCreditos(regra.getCreditos());
			item.setDia1(regra.getDia1());
			item.setDia2(regra.getDia2());
			item.setDia3(regra.getDia3());
			item.setDia4(regra.getDia4());
			item.setDia5(regra.getDia5());
			item.setDia6(regra.getDia6());
			item.setDia7(regra.getDia7());
			grupoDivisaoCreditos.getDivisaoCreditos().add(item);
		}
		triedaInput.setRegrasDivisaoCredito(grupoDivisaoCreditos);
	}
	
	private void generateCampi(boolean tatico) {
		GrupoCampus grupoCampus = of.createGrupoCampus();
		Set<Campus> campi = cenario.getCampi();
		for(Campus campus : campi) {
			ItemCampus itemCampus = of.createItemCampus();
			itemCampus.setId(campus.getId().intValue());
			itemCampus.setCodigo(campus.getCodigo());
			itemCampus.setNome(campus.getNome());
			itemCampus.setHorariosDisponiveis(createGrupoHorario(campus.getHorarios()));
			
			Set<String> salasJaAssociadasADisciplina = new HashSet<String>();
			
			// COLETANDO UNIDADES
			GrupoUnidade grupoUnidade = of.createGrupoUnidade();
			Set<Unidade> unidades = campus.getUnidades();
			for(Unidade unidade : unidades) {
				ItemUnidade itemUnidade = of.createItemUnidade();
				itemUnidade.setId(unidade.getId().intValue());
				itemUnidade.setCodigo(unidade.getCodigo());
				itemUnidade.setNome(unidade.getNome());
				itemUnidade.setHorariosDisponiveis(createGrupoHorario(unidade.getHorarios()));
				
				GrupoSala grupoSala = of.createGrupoSala();
				Set<Sala> salas = unidade.getSalas();
				for(Sala sala : salas) {
					ItemSala itemSala = of.createItemSala();
					itemSala.setId(sala.getId().intValue());
					itemSala.setCodigo(sala.getCodigo());
					itemSala.setAndar(sala.getAndar());
					itemSala.setNumero(sala.getNumero());
					itemSala.setTipoSalaId(sala.getTipoSala().getId().intValue());
					itemSala.setCapacidade(sala.getCapacidade());
					if(tatico) { // Tático
						itemSala.setCreditosDisponiveis(createCreditosDisponiveis(createGrupoHorario(sala.getHorarios())));
					} else { // Operacional
						itemSala.setHorariosDisponiveis(createGrupoHorario(sala.getHorarios()));
					}
					
					GrupoIdentificador grupoIdentificador = of.createGrupoIdentificador();
					Set<CurriculoDisciplina> curriculoDisciplinas = sala.getCurriculoDisciplinas();
					for(CurriculoDisciplina curriculoDisciplina : curriculoDisciplinas) {
						if(salasJaAssociadasADisciplina.add(curriculoDisciplina.getDisciplina().getId() + "-" +sala.getId())) {
							grupoIdentificador.getId().add(curriculoDisciplina.getDisciplina().getId().intValue());
						}
					}
					itemSala.setDisciplinasAssociadas(grupoIdentificador);
					
					grupoSala.getSala().add(itemSala);
				}
				itemUnidade.setSalas(grupoSala);
				
				grupoUnidade.getUnidade().add(itemUnidade);
			}
			itemCampus.setUnidades(grupoUnidade);
			
			// COLETANDO PROFESSORES
			GrupoProfessor grupoProfessor = of.createGrupoProfessor();
			Set<Professor> professores = campus.getProfessores();
			for(Professor professor : professores) {
				ItemProfessor itemProfessor = of.createItemProfessor();
				itemProfessor.setId(professor.getId().intValue());
				itemProfessor.setCpf(professor.getCpf());
				itemProfessor.setNome(professor.getNome());
				itemProfessor.setTipoContratoId(professor.getTipoContrato().getId().intValue());
				itemProfessor.setChMin(professor.getCargaHorariaMin());
				itemProfessor.setChMax(professor.getCargaHorariaMax());
				itemProfessor.setTitulacaoId(professor.getTitulacao().getId().intValue());
				if(professor.getAreaTitulacao() != null) {
					itemProfessor.setAreaTitulacaoId(professor.getAreaTitulacao().getId().intValue());
				}
				itemProfessor.setCredAnterior(professor.getCreditoAnterior());
				itemProfessor.setValorCred(professor.getValorCredito());
				itemProfessor.setHorariosDisponiveis(createGrupoHorario(professor.getHorarios()));
				
				GrupoProfessorDisciplina grupoProfessorDisciplina = of.createGrupoProfessorDisciplina();
				Set<ProfessorDisciplina> professorDisciplinas = professor.getDisciplinas();
				for(ProfessorDisciplina professorDisciplina : professorDisciplinas) {
					ItemProfessorDisciplina itemProfessorDisciplina = of.createItemProfessorDisciplina();
					itemProfessorDisciplina.setNota(professorDisciplina.getNota());
					itemProfessorDisciplina.setPreferencia(professorDisciplina.getPreferencia());
					itemProfessorDisciplina.setDisciplinaId(professorDisciplina.getDisciplina().getId().intValue());
					grupoProfessorDisciplina.getProfessorDisciplina().add(itemProfessorDisciplina);
				}
				itemProfessor.setDisciplinas(grupoProfessorDisciplina);
				
				grupoProfessor.getProfessor().add(itemProfessor);
			}
			itemCampus.setProfessores(grupoProfessor);
			
			grupoCampus.getCampus().add(itemCampus);
		}
		triedaInput.setCampi(grupoCampus);
	}

	private void generateDeslocamentoCampi() {
		GrupoDeslocamento grupoDeslocamento = of.createGrupoDeslocamento();
		Set<Campus> campi = cenario.getCampi();
		for(Campus campus : campi) {
			Set<DeslocamentoCampus> deslocamentos = campus.getDeslocamentos();
			for(DeslocamentoCampus deslocamento : deslocamentos) {
				ItemDeslocamento itemDeslocamento = of.createItemDeslocamento();
				itemDeslocamento.setOrigemId(deslocamento.getOrigem().getId().intValue());
				itemDeslocamento.setDestinoId(deslocamento.getDestino().getId().intValue());
				itemDeslocamento.setTempo(deslocamento.getTempo());
				itemDeslocamento.setCusto(deslocamento.getCusto());
				grupoDeslocamento.getDeslocamento().add(itemDeslocamento);
			}
		}
		triedaInput.setTemposDeslocamentosCampi(grupoDeslocamento);
	}
	
	private void generateDeslocamentoUnidades() {
		GrupoDeslocamento grupoDeslocamento = of.createGrupoDeslocamento();
		Set<Campus> campi = cenario.getCampi();
		for(Campus campus : campi) {
			Set<Unidade> unidades = campus.getUnidades();
			for(Unidade unidade : unidades) {
				Set<DeslocamentoUnidade> deslocamentos = unidade.getDeslocamentos();
				for(DeslocamentoUnidade deslocamento : deslocamentos) {
					ItemDeslocamento itemDeslocamento = of.createItemDeslocamento();
					itemDeslocamento.setOrigemId(deslocamento.getOrigem().getId().intValue());
					itemDeslocamento.setDestinoId(deslocamento.getDestino().getId().intValue());
					itemDeslocamento.setTempo(deslocamento.getTempo());
					itemDeslocamento.setCusto(deslocamento.getCusto());
					grupoDeslocamento.getDeslocamento().add(itemDeslocamento);
				}
			}
		}
		triedaInput.setTemposDeslocamentosUnidades(grupoDeslocamento);
	}
	
	private void generateDisciplinas() {
		GrupoDisciplina grupoDisciplina = of.createGrupoDisciplina();
		Set<Disciplina> disciplinas = cenario.getDisciplinas();
		for(Disciplina disciplina : disciplinas) {
			ItemDisciplina itemDisciplina = of.createItemDisciplina();
			itemDisciplina.setId(disciplina.getId().intValue());
			itemDisciplina.setCodigo(disciplina.getCodigo());
			itemDisciplina.setNome(disciplina.getNome());
			itemDisciplina.setCredTeoricos(disciplina.getCreditosTeorico());
			itemDisciplina.setCredPraticos(disciplina.getCreditosPratico());
			itemDisciplina.setLaboratorio(disciplina.getLaboratorio());
			itemDisciplina.setMaxAlunosTeorico(disciplina.getMaxAlunosTeorico());
			itemDisciplina.setMaxAlunosPratico(disciplina.getMaxAlunosPratico());
			itemDisciplina.setTipoDisciplinaId(disciplina.getTipoDisciplina().getId().intValue());
			itemDisciplina.setNivelDificuldadeId(Dificuldades.toInt(disciplina.getDificuldade()));
			
			if(Fixacao.findAllBy(disciplina).size() == 0) {
				DivisaoCredito divisaoCredito = disciplina.getDivisaoCreditos();
				divisaoCredito = (divisaoCredito != null)? divisaoCredito : DivisaoCredito.findByCredito(disciplina.getTotalCreditos());
				if(divisaoCredito != null) {
					ItemDivisaoCreditos itemDivisaoCreditos = of.createItemDivisaoCreditos();
					itemDivisaoCreditos.setId(divisaoCredito.getId().intValue());
					itemDivisaoCreditos.setCreditos(divisaoCredito.getCreditos());
					itemDivisaoCreditos.setDia1(divisaoCredito.getDia1());
					itemDivisaoCreditos.setDia2(divisaoCredito.getDia2());
					itemDivisaoCreditos.setDia3(divisaoCredito.getDia3());
					itemDivisaoCreditos.setDia4(divisaoCredito.getDia4());
					itemDivisaoCreditos.setDia5(divisaoCredito.getDia5());
					itemDivisaoCreditos.setDia6(divisaoCredito.getDia6());
					itemDivisaoCreditos.setDia7(divisaoCredito.getDia7());
					itemDisciplina.setDivisaoDeCreditos(itemDivisaoCreditos);
				}
			}
			
			GrupoIdentificador grupoIdentificadorEquivalencias = of.createGrupoIdentificador();
			Set<Equivalencia> equivalencias = disciplina.getEquivalencias();
			for(Equivalencia equivalencia : equivalencias) {
//				 TODO Arrumar para eliminar mais de 1 disciplina
				Set<Disciplina> eliminas = equivalencia.getElimina();
				if(eliminas != null && eliminas.size() > 0) {
					List<Disciplina> disciplinasAux = new ArrayList<Disciplina>(eliminas);
					grupoIdentificadorEquivalencias.getId().add(disciplinasAux.get(0).getId().intValue());
				}
			}
			itemDisciplina.setDisciplinasEquivalentes(grupoIdentificadorEquivalencias);
			
			GrupoIdentificador grupoIdentificadorIncompativeis = of.createGrupoIdentificador();
			Set<Incompatibilidade> incompatibilidades = disciplina.getIncompatibilidades();
			for(Incompatibilidade incompatibilidade : incompatibilidades) {
				grupoIdentificadorIncompativeis.getId().add(incompatibilidade.getDisciplina2().getId().intValue());
			}
			itemDisciplina.setDisciplinasIncompativeis(grupoIdentificadorIncompativeis);
			
			itemDisciplina.setHorariosDisponiveis(createGrupoHorario(disciplina.getHorarios()));
			grupoDisciplina.getDisciplina().add(itemDisciplina);
		}
		triedaInput.setDisciplinas(grupoDisciplina);
	}
	
	private void generateCurso() {
		GrupoCurso grupoCurso = of.createGrupoCurso();
		Set<Curso> cursos = cenario.getCursos();
		for(Curso curso : cursos) {
			ItemCurso itemCurso = of.createItemCurso();
			itemCurso.setId(curso.getId().intValue());
			itemCurso.setCodigo(curso.getCodigo());
			itemCurso.setTipoId(curso.getTipoCurso().getId().intValue());

			ItemPercentualMinimo itemPercentualMinimoMestres = of.createItemPercentualMinimo();
			itemPercentualMinimoMestres.setPercMinimo(curso.getNumMinMestres());
			itemPercentualMinimoMestres.setTipoTitulacaoId(4); // TODO pegar titulacao pela constante
			itemCurso.setRegraPercMinMestres(itemPercentualMinimoMestres);
			
			ItemPercentualMinimo itemPercentualMinimoDoutores = of.createItemPercentualMinimo();
			itemPercentualMinimoDoutores.setPercMinimo(curso.getNumMinDoutores());
			itemPercentualMinimoDoutores.setTipoTitulacaoId(5); // TODO pegar titulacao pela constante
			itemCurso.setRegraPercMinDoutores(itemPercentualMinimoDoutores);
			
			itemCurso.setQtdMaxProfDisc(curso.getMaxDisciplinasPeloProfessor());
			itemCurso.setMaisDeUmaDiscPeriodo(curso.getAdmMaisDeUmDisciplina());
			
			GrupoIdentificador grupoIdentificadorAreasTitulacao = of.createGrupoIdentificador();
			Set<AreaTitulacao> areas = curso.getAreasTitulacao();
			for(AreaTitulacao area : areas) {
				grupoIdentificadorAreasTitulacao.getId().add(area.getId().intValue());
			}
			itemCurso.setAreasTitulacao(grupoIdentificadorAreasTitulacao);
			
			GrupoCurriculo grupoCurriculo = of.createGrupoCurriculo();
			Set<Curriculo> curriculos = curso.getCurriculos();
			for(Curriculo curriculo : curriculos) {
				ItemCurriculo itemCurriculo = of.createItemCurriculo();
				itemCurriculo.setId(curriculo.getId().intValue());
				itemCurriculo.setCodigo(curriculo.getCodigo());
				
				GrupoDisciplinaPeriodo grupoDisciplinaPeriodo = of.createGrupoDisciplinaPeriodo();
				Set<CurriculoDisciplina> curriculoPeriodos = curriculo.getDisciplinas();
				for(CurriculoDisciplina curriculoPeriodo : curriculoPeriodos) {
					ItemDisciplinaPeriodo itemDisciplinaPeriodo = of.createItemDisciplinaPeriodo();
					itemDisciplinaPeriodo.setPeriodo(curriculoPeriodo.getPeriodo());
					itemDisciplinaPeriodo.setDisciplinaId(curriculoPeriodo.getDisciplina().getId().intValue());
					grupoDisciplinaPeriodo.getDisciplinaPeriodo().add(itemDisciplinaPeriodo);
				}
				itemCurriculo.setDisciplinasPeriodo(grupoDisciplinaPeriodo);
				
				grupoCurriculo.getCurriculo().add(itemCurriculo);
			}
			itemCurso.setCurriculos(grupoCurriculo);
			
			grupoCurso.getCurso().add(itemCurso);
		}
		triedaInput.setCursos(grupoCurso);
	}
	
	private void generateOfertaCursoCampi() {
		GrupoOfertaCurso grupoOfertaCurso = of.createGrupoOfertaCurso();
		Set<Campus> campi = cenario.getCampi();
		for(Campus campus : campi) {
			Set<Oferta> ofertas = campus.getOfertas();
			for(Oferta oferta : ofertas) {
				ItemOfertaCurso itemOfertaCurso = of.createItemOfertaCurso();
				itemOfertaCurso.setId(oferta.getId().intValue());
				Curriculo curriculo = oferta.getCurriculo();
				itemOfertaCurso.setCurriculoId(curriculo.getId().intValue());
				itemOfertaCurso.setCursoId(curriculo.getCurso().getId().intValue());
				itemOfertaCurso.setTurnoId(oferta.getTurno().getId().intValue());
				itemOfertaCurso.setCampusId(campus.getId().intValue());
				grupoOfertaCurso.getOfertaCurso().add(itemOfertaCurso);
			}
		}
		triedaInput.setOfertaCursosCampi(grupoOfertaCurso);
	}
	
	private void generateDemandas() {
		GrupoDemanda grupoDemanda = of.createGrupoDemanda();
		Set<Campus> campi = cenario.getCampi();
		for(Campus campus : campi) {
			Set<Oferta> ofertas = campus.getOfertas();
			for(Oferta oferta : ofertas) {
				Set<Demanda> demandas = oferta.getDemandas();
				for(Demanda demanda : demandas) {
					ItemDemanda itemDemanda = of.createItemDemanda();
					itemDemanda.setOfertaCursoCampiId(oferta.getId().intValue());
					itemDemanda.setDisciplinaId(demanda.getDisciplina().getId().intValue());
					itemDemanda.setQuantidade(demanda.getQuantidade());
					grupoDemanda.getDemanda().add(itemDemanda);
				}
			}
		}
		triedaInput.setDemandas(grupoDemanda);
	}
	
	private void generateParametrosPlanejamento(boolean tatico) {
		
		Parametro parametro = cenario.getParametro();
		
		ItemParametrosPlanejamento itemParametrosPlanejamento = of.createItemParametrosPlanejamento();
		
		itemParametrosPlanejamento.setModoOtimizacao(tatico ? "TATICO" : "OPERACIONAL");
		
		itemParametrosPlanejamento.setFuncaoObjetivo(parametro.getFuncaoObjetivo());
		
		CargaHorariaSemanalAluno cargaHorariaSemanalAluno = of.createItemParametrosPlanejamentoCargaHorariaSemanalAluno();
		cargaHorariaSemanalAluno.setIndiferente("");
		if(parametro.getCargaHorariaAluno()) {
			if(parametro.getCargaHorariaAlunoSel().equals(CargaHoraria.CONCENTRAR.name())) {
				cargaHorariaSemanalAluno.setMinimizarDias("");
			} else if(parametro.getCargaHorariaAlunoSel().equals(CargaHoraria.DISTRIBUIR.name())) {
				cargaHorariaSemanalAluno.setEquilibrar("");
			}
		}
		itemParametrosPlanejamento.setCargaHorariaSemanalAluno(cargaHorariaSemanalAluno);
		itemParametrosPlanejamento.setAlunosMesmoPeriodoNaMesmaSala(parametro.getAlunoDePeriodoMesmaSala());
		itemParametrosPlanejamento.setPermitirAlunosEmVariosCampi(parametro.getAlunoEmMuitosCampi());
		itemParametrosPlanejamento.setMinimizarDeslocAluno(parametro.getMinimizarDeslocamentoAluno());
		
		
		CargaHorariaSemanalProfessor cargaHorariaSemanaProfessor = of.createItemParametrosPlanejamentoCargaHorariaSemanalProfessor();
		cargaHorariaSemanaProfessor.setIndiferente("");
		if(parametro.getCargaHorariaProfessor()) {
			if(parametro.getCargaHorariaProfessorSel().equals(CargaHoraria.CONCENTRAR.name())) {
				cargaHorariaSemanaProfessor.setMinimizarDias("");
			} else if(parametro.getCargaHorariaAlunoSel().equals(CargaHoraria.DISTRIBUIR.name())) {
				cargaHorariaSemanaProfessor.setEquilibrar("");
			}
		}
		itemParametrosPlanejamento.setCargaHorariaSemanalProfessor(cargaHorariaSemanaProfessor);
		itemParametrosPlanejamento.setPermitirProfessorEmVariosCampi(parametro.getProfessorEmMuitosCampi());
		itemParametrosPlanejamento.setMinimizarDeslocProfessor(parametro.getMinimizarDeslocamentoProfessor());
		itemParametrosPlanejamento.setMaxDeslocProfessor(parametro.getMinimizarDeslocamentoProfessorValue());
		itemParametrosPlanejamento.setMinimizarHorariosVaziosProfessor(parametro.getMinimizarGapProfessor());
		itemParametrosPlanejamento.setEvitarReducaoCargaHorariaProfValor(parametro.getEvitarReducaoCargaHorariaProfessorValue());
		itemParametrosPlanejamento.setEvitarReducaoCargaHorariaProf(parametro.getEvitarReducaoCargaHorariaProfessor());
		itemParametrosPlanejamento.setEvitarProfUltimoPrimeiroHor(parametro.getEvitarUltimoEPrimeiroHorarioProfessor());
		itemParametrosPlanejamento.setPreferenciaProfessorDisciplina(parametro.getPreferenciaDeProfessores());
		itemParametrosPlanejamento.setDesempenhoProfDisponibilidade(parametro.getAvaliacaoDesempenhoProfessor());
		
		itemParametrosPlanejamento.setMinAlunosAberturaTurmas(parametro.getMinAlunosParaAbrirTurma());
		itemParametrosPlanejamento.setMinAlunosAberturaTurmasValor(parametro.getMinAlunosParaAbrirTurmaValue());
		// TODO
		itemParametrosPlanejamento.setNiveisDificuldadeHorario(of.createGrupoNivelDificuldadeHorario());
		itemParametrosPlanejamento.setEquilibrarDiversidadeDiscDia(parametro.getCompatibilidadeDisciplinasMesmoDia());
		itemParametrosPlanejamento.setRegrasGenericasDivisaoCredito(parametro.getRegrasGenericasDivisaoCredito());
		itemParametrosPlanejamento.setRegrasEspecificasDivisaoCredito(parametro.getRegrasEspecificasDivisaoCredito());
		itemParametrosPlanejamento.setMaximizarAvaliacaoCursosSel(parametro.getMaximizarNotaAvaliacaoCorpoDocente());
		if(parametro.getMaximizarNotaAvaliacaoCorpoDocente()) {
			//  TODO
			Set<Curso> cursos = cenario.getCursos();
			GrupoIdentificador cursosAvaliacaoGrupo = of.createGrupoIdentificador();
			for(Curso curso : cursos) {
				cursosAvaliacaoGrupo.getId().add(curso.getId().intValue());
			}
			itemParametrosPlanejamento.setMaximizarAvaliacaoCursos(cursosAvaliacaoGrupo);
		}
		
		itemParametrosPlanejamento.setMinimizarCustoDocenteCursosSel(parametro.getMinimizarCustoDocenteCursos());
		if(parametro.getMinimizarCustoDocenteCursos()) {
			// TODO
			Set<Curso> cursos = cenario.getCursos();
			GrupoIdentificador cursosCustoGrupo = of.createGrupoIdentificador();
			for(Curso curso : cursos) {
				cursosCustoGrupo.getId().add(curso.getId().intValue());
			}
			itemParametrosPlanejamento.setMinimizarCustoDocenteCursos(cursosCustoGrupo);
		}
		itemParametrosPlanejamento.setPermiteCompartilhamentoTurmaSel(parametro.getCompartilharDisciplinasCampi());
		if(parametro.getCompartilharDisciplinasCampi()) {
			// TODO
			Set<Curso> cursos = cenario.getCursos();
			GrupoIdentificador cursosCompartilharTurmas = of.createGrupoIdentificador();
			for(Curso curso : cursos) {
				cursosCompartilharTurmas.getId().add(curso.getId().intValue());
			}
			GrupoGrupo cursosCompartilharTurmaGrupo = of.createGrupoGrupo();
			cursosCompartilharTurmaGrupo.getGrupoIdentificador().add(cursosCompartilharTurmas);
			itemParametrosPlanejamento.setPermiteCompartilhamentoTurma(cursosCompartilharTurmaGrupo);
		}
		itemParametrosPlanejamento.setPercentuaisMinimoMestres(parametro.getPercentuaisMinimosMestres());
		itemParametrosPlanejamento.setPercentuaisMinimoDoutores(parametro.getPercentuaisMinimosDoutores());
		itemParametrosPlanejamento.setAreaTitulacaoProfessorCurso(parametro.getAreaTitulacaoProfessoresECursos());
		itemParametrosPlanejamento.setMaximoDisciplinasDeUmProfessorPorCurso(parametro.getLimitarMaximoDisciplinaProfessor());
		
		itemParametrosPlanejamento.setCustoProfDisponibilidade(false);

		triedaInput.setParametrosPlanejamento(itemParametrosPlanejamento);
	}
	
	private void generateFixacoes() {
		int id = 1;
		GrupoFixacao grupoFixacao = of.createGrupoFixacao();

		List<Fixacao> fixacoes = Fixacao.findAll(); // Pegar fixações do cenário
		for(Fixacao fixacao : fixacoes) {
			Set<HorarioDisponivelCenario> horarios = fixacao.getHorarios();
			if(horarios.size() > 0) {
				for(HorarioDisponivelCenario horario : horarios) {
					ItemFixacao itemFixacao = of.createItemFixacao();
					itemFixacao.setId(id++);
					itemFixacao.setDiaSemana(Semanas.toInt(horario.getSemana()));
					itemFixacao.setTurnoId(horario.getHorarioAula().getTurno().getId().intValue());
					itemFixacao.setHorarioAulaId(horario.getHorarioAula().getId().intValue());
					if(fixacao.getProfessor() != null) {
						itemFixacao.setProfessorId(fixacao.getProfessor().getId().intValue());
					}
					if(fixacao.getDisciplina() != null) {
						itemFixacao.setDisciplinaId(fixacao.getDisciplina().getId().intValue());
					}
					if(fixacao.getSala() != null) {
						itemFixacao.setSalaId(fixacao.getSala().getId().intValue());
					}
					grupoFixacao.getFixacao().add(itemFixacao);
				}
			} else {
				ItemFixacao itemFixacao = of.createItemFixacao();
				itemFixacao.setId(id++);
				if(fixacao.getProfessor() != null) {
					itemFixacao.setProfessorId(fixacao.getProfessor().getId().intValue());
				}
				if(fixacao.getDisciplina() != null) {
					itemFixacao.setDisciplinaId(fixacao.getDisciplina().getId().intValue());
				}
				if(fixacao.getSala() != null) {
					itemFixacao.setSalaId(fixacao.getSala().getId().intValue());
				}
				grupoFixacao.getFixacao().add(itemFixacao);
			}
			
		}
		
		triedaInput.setFixacoes(grupoFixacao);
	}
	
	private void generateTaticoInput() {
		Set<AtendimentoTatico> ats = cenario.getAtendimentosTaticos();
		for(AtendimentoTatico at : ats) {
			createItemAtendimentoTaticoSolucao(at.getSala(), at.getSemana(), at.getOferta(), at.getDisciplina(), at.getQuantidadeAlunos(), at.getTurma(), at.getCreditosTeorico(), at.getCreditosPratico());
		}
	}
	private ItemAtendimentoCampusSolucao getItemAtendimentoCampusSolucao(Campus campus) {
		if(triedaInput.getAtendimentosTatico() == null) {
			triedaInput.setAtendimentosTatico(of.createGrupoAtendimentoCampusSolucao());
		}
		for(ItemAtendimentoCampusSolucao atSolucao : triedaInput.getAtendimentosTatico().getAtendimentoCampus()) {
			if(atSolucao.getCampusId() == campus.getId().intValue()) {
				return atSolucao;
			}
		}
		ItemAtendimentoCampusSolucao atSolucao = of.createItemAtendimentoCampusSolucao();
		atSolucao.setCampusId(campus.getId().intValue());
		atSolucao.setCampusCodigo(campus.getCodigo());
		triedaInput.getAtendimentosTatico().getAtendimentoCampus().add(atSolucao);
		return atSolucao;
	}
	private ItemAtendimentoUnidadeSolucao getItemAtendimentoUnidadeSolucao(Unidade unidade) {
		for(ItemAtendimentoCampusSolucao atCampusSolucao : triedaInput.getAtendimentosTatico().getAtendimentoCampus()) {
			if(atCampusSolucao.getAtendimentosUnidades() == null) {
				atCampusSolucao.setAtendimentosUnidades(of.createGrupoAtendimentoUnidadeSolucao());
			}
			for(ItemAtendimentoUnidadeSolucao atUnidadeSolucao : atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade()) {
				if(atUnidadeSolucao.getUnidadeId() == unidade.getId().intValue()) {
					return atUnidadeSolucao;
				}
			}
			
		}
		ItemAtendimentoUnidadeSolucao atUnidadeSolucao = of.createItemAtendimentoUnidadeSolucao();
		atUnidadeSolucao.setUnidadeId(unidade.getId().intValue());
		atUnidadeSolucao.setUnidadeCodigo(unidade.getCodigo());
		ItemAtendimentoCampusSolucao atCampusSolucao = getItemAtendimentoCampusSolucao(unidade.getCampus());
		if(atCampusSolucao.getAtendimentosUnidades() == null) atCampusSolucao.setAtendimentosUnidades(of.createGrupoAtendimentoUnidadeSolucao());
		atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade().add(atUnidadeSolucao);
		return atUnidadeSolucao;
	}
	private ItemAtendimentoSalaSolucao getItemAtendimentoSalaSolucao(Sala sala) {
		if(triedaInput.getAtendimentosTatico() == null) {
			triedaInput.setAtendimentosTatico(of.createGrupoAtendimentoCampusSolucao());
		}
		for(ItemAtendimentoCampusSolucao atCampusSolucao : triedaInput.getAtendimentosTatico().getAtendimentoCampus()) {
			if(atCampusSolucao.getAtendimentosUnidades() == null) {
				atCampusSolucao.setAtendimentosUnidades(of.createGrupoAtendimentoUnidadeSolucao());
			}
			for(ItemAtendimentoUnidadeSolucao atUnidadeSolucao : atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade()) {
				if(atUnidadeSolucao.getAtendimentosSalas() == null) {
					atUnidadeSolucao.setAtendimentosSalas(of.createGrupoAtendimentoSalaSolucao());
				}
				for(ItemAtendimentoSalaSolucao atSalaSolucao : atUnidadeSolucao.getAtendimentosSalas().getAtendimentoSala()) {
					if(atSalaSolucao.getSalaId() == sala.getId().intValue()) {
						return atSalaSolucao;
					}
				}
			}
		}
		ItemAtendimentoSalaSolucao atSalaSolucao = of.createItemAtendimentoSalaSolucao();
		atSalaSolucao.setSalaId(sala.getId().intValue());
		atSalaSolucao.setSalaNome(sala.getCodigo());
		ItemAtendimentoUnidadeSolucao atUnidadeSolucao = getItemAtendimentoUnidadeSolucao(sala.getUnidade());
		if(atUnidadeSolucao.getAtendimentosSalas() == null) atUnidadeSolucao.setAtendimentosSalas(of.createGrupoAtendimentoSalaSolucao());
		atUnidadeSolucao.getAtendimentosSalas().getAtendimentoSala().add(atSalaSolucao);
		return atSalaSolucao;
	}
	private ItemAtendimentoDiaSemanaSolucao getItemAtendimentoDiaSemanaSolucao(Sala sala, Semanas semana) {
		if(triedaInput.getAtendimentosTatico() == null) {
			triedaInput.setAtendimentosTatico(of.createGrupoAtendimentoCampusSolucao());
		}
		for(ItemAtendimentoCampusSolucao atCampusSolucao : triedaInput.getAtendimentosTatico().getAtendimentoCampus()) {
			if(atCampusSolucao.getAtendimentosUnidades() == null) {
				atCampusSolucao.setAtendimentosUnidades(of.createGrupoAtendimentoUnidadeSolucao());
			}
			for(ItemAtendimentoUnidadeSolucao atUnidadeSolucao : atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade()) {
				if(atUnidadeSolucao.getAtendimentosSalas() == null) {
					atUnidadeSolucao.setAtendimentosSalas(of.createGrupoAtendimentoSalaSolucao());
				}
				for(ItemAtendimentoSalaSolucao atSalaSolucao : atUnidadeSolucao.getAtendimentosSalas().getAtendimentoSala()) {
					if(atSalaSolucao.getSalaId() != sala.getId().intValue()) continue;
					if(atSalaSolucao.getAtendimentosDiasSemana() == null) {
						atSalaSolucao.setAtendimentosDiasSemana(of.createGrupoAtendimentoDiaSemanaSolucao());
					}
					for(ItemAtendimentoDiaSemanaSolucao atDiaSemanaSolucao : atSalaSolucao.getAtendimentosDiasSemana().getAtendimentoDiaSemana()) {
						if(Semanas.get(atDiaSemanaSolucao.getDiaSemana()).equals(semana)) {
							return atDiaSemanaSolucao;
						}
					}
				}
			}
		}
		ItemAtendimentoDiaSemanaSolucao atDiaSemanaSolucao = of.createItemAtendimentoDiaSemanaSolucao();
		atDiaSemanaSolucao.setDiaSemana(Semanas.toInt(semana));
		ItemAtendimentoSalaSolucao atSalaSolucao = getItemAtendimentoSalaSolucao(sala);
		if(atSalaSolucao.getAtendimentosDiasSemana() == null) atSalaSolucao.setAtendimentosDiasSemana(of.createGrupoAtendimentoDiaSemanaSolucao());
		atSalaSolucao.getAtendimentosDiasSemana().getAtendimentoDiaSemana().add(atDiaSemanaSolucao);
		return atDiaSemanaSolucao;
	}
	private ItemAtendimentoTaticoSolucao createItemAtendimentoTaticoSolucao(Sala sala, Semanas semana, Oferta oferta, Disciplina disciplina, int quantidade, String turma, int qtdCreditosTeoricos, int qtdCreditosPraticos) {
		ItemAtendimentoDiaSemanaSolucao atDiaSemanaSolucao = getItemAtendimentoDiaSemanaSolucao(sala, semana);
		
		ItemAtendimentoOfertaSolucao atOfertaSolucao = of.createItemAtendimentoOfertaSolucao();
		atOfertaSolucao.setOfertaCursoCampiId(oferta.getId().intValue());
		atOfertaSolucao.setDisciplinaId(disciplina.getId().intValue());
		atOfertaSolucao.setQuantidade(quantidade);
		atOfertaSolucao.setTurma(turma);
		
		ItemAtendimentoTaticoSolucao atTaticoSolucao = of.createItemAtendimentoTaticoSolucao();
		atTaticoSolucao.setAtendimentoOferta(atOfertaSolucao);
		atTaticoSolucao.setQtdeCreditosPraticos(qtdCreditosPraticos);
		atTaticoSolucao.setQtdeCreditosTeoricos(qtdCreditosTeoricos);
		
		if(atDiaSemanaSolucao.getAtendimentosTatico() == null) atDiaSemanaSolucao.setAtendimentosTatico(of.createGrupoAtendimentoTaticoSolucao());
		atDiaSemanaSolucao.getAtendimentosTatico().getAtendimentoTatico().add(atTaticoSolucao);
		
		return atTaticoSolucao;
	}
	
	/* **************
	 * MÉTODOS AUXILIARES
	 */
	
	private GrupoHorario createGrupoHorario(Set<HorarioDisponivelCenario> horarios) {
		
		GrupoHorario grupoHorario = of.createGrupoHorario();
		for(HorarioDisponivelCenario horarioDisponivelCenario : horarios) {
			HorarioAula horarioAula = horarioDisponivelCenario.getHorarioAula();
			Semanas semana = horarioDisponivelCenario.getSemana();
			ItemHorario itemHorarioAux = null;
			for(ItemHorario itemHorario : grupoHorario.getHorario()) {
				if(itemHorario.getHorarioAulaId() == horarioAula.getId()) {
					itemHorarioAux = itemHorario;
					break;
				}
			}
			if(itemHorarioAux != null) {
				itemHorarioAux.getDiasSemana().getDiaSemana().add(Semanas.toInt(semana));
			} else {
				itemHorarioAux = of.createItemHorario();
				itemHorarioAux.setHorarioAulaId(horarioAula.getId().intValue());
				itemHorarioAux.setTurnoId(horarioAula.getTurno().getId().intValue());
				itemHorarioAux.setDiasSemana(of.createGrupoDiaSemana());
				itemHorarioAux.getDiasSemana().getDiaSemana().add(Semanas.toInt(semana));
				grupoHorario.getHorario().add(itemHorarioAux);
			}
		}
		return grupoHorario;
	}
	
	
	private GrupoCreditoDisponivel createCreditosDisponiveis(GrupoHorario horariosDisponiveis) {
		GrupoCreditoDisponivel grupoCreditoDisponivel = of.createGrupoCreditoDisponivel();
		List<ItemHorario> horarios = horariosDisponiveis.getHorario();
		
		for(Semanas semana : Semanas.values()) {
			for(Turno turno : cenario.getTurnos()) {
				ItemCreditoDisponivel itemCD = of.createItemCreditoDisponivel();
				itemCD.setDiaSemana(Semanas.toInt(semana));
				itemCD.setTurnoId(turno.getId().intValue());
				itemCD.setMaxCreditos(0);
				for(ItemHorario itemHorario : horarios) {
					if(itemHorario.getTurnoId() == itemCD.getTurnoId() && itemHorario.getDiasSemana().getDiaSemana().contains(Semanas.toInt(semana))) {
						itemCD.setMaxCreditos(itemCD.getMaxCreditos() + 1);
					}
				}
				if(itemCD.getMaxCreditos() > 0) {
					grupoCreditoDisponivel.getCreditoDisponivel().add(itemCD);
				}
			}
		}
		
		return grupoCreditoDisponivel;
	}
}
