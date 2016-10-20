package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.UniqueConstraint;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Dificuldades;
import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "DIS_ID")
@Table(name = "DISCIPLINAS", uniqueConstraints = @UniqueConstraint(columnNames = {
		"DIS_CODIGO", "CEN_ID" }))
public class Disciplina implements Serializable, Comparable<Disciplina>,
		Clonable<Disciplina> {
	private static final long serialVersionUID = 7980821696468062987L;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Cenario.class, fetch = FetchType.LAZY)
	@JoinColumn(name = "CEN_ID")
	private Cenario cenario;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = TipoDisciplina.class, fetch = FetchType.LAZY)
	@JoinColumn(name = "TDI_ID")
	private TipoDisciplina tipoDisciplina;

	@OneToOne(cascade = CascadeType.ALL, targetEntity = DivisaoCredito.class, fetch = FetchType.LAZY)
	@JoinColumn(name = "DCR_ID")
	private DivisaoCredito divisaoCreditos;

	@NotNull
	@Column(name = "DIS_CODIGO")
	@Size(min = 1, max = 100)
	private String codigo;

	@NotNull
	@Column(name = "DIS_NOME")
	@Size(min = 1, max = 500)
	private String nome;

	@NotNull
	@Column(name = "DIS_CRED_TEORICO")
	@Min(0L)
	@Max(99L)
	private Integer creditosTeorico;

	@NotNull
	@Column(name = "DIS_CRED_PRATICO")
	@Min(0L)
	@Max(99L)
	private Integer creditosPratico;

	@Column(name = "DIS_LABORATORIO")
	private Boolean laboratorio;

	@Column(name = "DIS_USA_SABADO")
	private Boolean usaSabado;

	@Column(name = "DIS_USA_DOMINGO")
	private Boolean usaDomingo;

	@NotNull
	@Enumerated
	private Dificuldades dificuldade;

	@NotNull
	@Column(name = "DIS_MAX_ALUN_TEORICO")
	@Min(0L)
	@Max(999L)
	private Integer maxAlunosTeorico;

	@NotNull
	@Column(name = "DIS_MAX_ALUN_PRATICO")
	@Min(0L)
	@Max(999L)
	private Integer maxAlunosPratico;

	@NotNull
	@Column(name = "DIS_AULAS_CONTINUAS")
	private Boolean aulasContinuas;

	@NotNull
	@Column(name = "DIS_PROFESSOR_UNICO")
	private Boolean professorUnico;

	@Column(name = "DIS_CARGA_HORARIA")
	@Max(999L)
	private Integer cargaHoraria;

	/*
	 * @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE },
	 * mappedBy = "disciplinas" ) private Set< HorarioDisponivelCenario >
	 * horarios = new HashSet< HorarioDisponivelCenario >();
	 */

	@NotNull
	@OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
	private Set<ProfessorDisciplina> professores = new HashSet<ProfessorDisciplina>();

	@NotNull
	@OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina1")
	private Set<Incompatibilidade> incompatibilidades = new HashSet<Incompatibilidade>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "cursou")
	private Set<Equivalencia> eliminam = new HashSet<Equivalencia>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "elimina")
	private Set<Equivalencia> eliminadasPor = new HashSet<Equivalencia>();

	@NotNull
	@OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
	private Set<CurriculoDisciplina> curriculos = new HashSet<CurriculoDisciplina>();

	@NotNull
	@OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
	private Set<Demanda> demandas = new HashSet<Demanda>();

	@OneToMany(cascade = CascadeType.REMOVE, mappedBy = "disciplina")
	private Set<Fixacao> fixacoes = new HashSet<Fixacao>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
	private Set<AtendimentoOperacional> atendimentosOperacionais = new HashSet<AtendimentoOperacional>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
	private Set<AtendimentoTatico> atendimentosTaticos = new HashSet<AtendimentoTatico>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
	private Set<Turma> turmas = new HashSet<Turma>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "DisponibilidadeDisciplina")
	private Set<DisponibilidadeDisciplina> disponibilidades = new HashSet<DisponibilidadeDisciplina>();

	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, mappedBy = "disciplinas")
	private Set<ProfessorVirtual> professoresVirtuais = new HashSet<ProfessorVirtual>();

	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH, CascadeType.DETACH })
	@JoinTable(name = "disciplinas_salas", joinColumns = { @JoinColumn(name = "dis_id") }, inverseJoinColumns = { @JoinColumn(name = "sal_id") })
	private Set<Sala> salas = new HashSet<Sala>();

	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH, CascadeType.DETACH })
	@JoinTable(name = "disciplinas_grupos_sala", joinColumns = { @JoinColumn(name = "dis_id") }, inverseJoinColumns = { @JoinColumn(name = "grs_id") })
	private Set<GrupoSala> gruposSala = new HashSet<GrupoSala>();

	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH, CascadeType.DETACH })
	@JoinTable(name = "curriculos_disciplinas_disciplinas_prerequisitos", joinColumns = { @JoinColumn(name = "dis_id") }, inverseJoinColumns = { @JoinColumn(name = "cdi_id") })
	private Set<CurriculoDisciplina> preRequisitos = new HashSet<CurriculoDisciplina>();

	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH, CascadeType.DETACH })
	@JoinTable(name = "curriculos_disciplinas_disciplinas_corequisitos", joinColumns = { @JoinColumn(name = "dis_id") }, inverseJoinColumns = { @JoinColumn(name = "cdi_id") })
	private Set<CurriculoDisciplina> coRequisitos = new HashSet<CurriculoDisciplina>();

	public String toString() {
		StringBuilder sb = new StringBuilder();

		sb.append("Id: ").append(getId()).append(", ");
		sb.append("Version: ").append(getVersion()).append(", ");
		sb.append("Cenario: ").append(getCenario()).append(", ");
		sb.append("TipoDisciplina: ").append(getTipoDisciplina()).append(", ");
		sb.append("DivisaoCreditos: ").append(getDivisaoCreditos())
				.append(", ");
		sb.append("Codigo: ").append(getCodigo()).append(", ");
		sb.append("Nome: ").append(getNome()).append(", ");
		sb.append("CreditosTeorico: ").append(getCreditosTeorico())
				.append(", ");
		sb.append("CreditosPratico: ").append(getCreditosPratico())
				.append(", ");
		sb.append("Laboratorio: ").append(getLaboratorio()).append(", ");
		sb.append("Dificuldade: ").append(getDificuldade()).append(", ");
		sb.append("MaxAlunosTeorico: ").append(getMaxAlunosTeorico())
				.append(", ");
		sb.append("MaxAlunosPratico: ").append(getMaxAlunosPratico())
				.append(", ");
		sb.append("UsaSabado: ").append(getUsaSabado()).append(", ");
		sb.append("UsaDomingo: ").append(getUsaDomingo()).append(", ");
		sb.append("CargaHoraria: ").append(getCargaHoraria()).append(", ");
		sb.append("Professores: ")
				.append(getProfessores() == null ? "null" : getProfessores()
						.size()).append(", ");
		sb.append("Incompatibilidades: ")
				.append(getIncompatibilidades() == null ? "null"
						: getIncompatibilidades().size()).append(", ");
		sb.append("Equivalencias: ")
				.append(getEliminam() == null ? "null" : getEliminam().size())
				.append(", ");
		sb.append("EliminadaPor: ")
				.append(getEliminadasPor() == null ? "null"
						: getEliminadasPor().size()).append(", ");
		sb.append("Curriculos: ")
				.append(getCurriculos() == null ? "null" : getCurriculos()
						.size()).append(", ");
		sb.append("Demandas: ")
				.append(getDemandas() == null ? "null" : getDemandas().size())
				.append(", ");
		sb.append("Fixacoes: ")
				.append(getFixacoes() == null ? "null" : getFixacoes().size())
				.append(", ");
		sb.append("Atendimentos Operacionais: ")
				.append(getAtendimentosOperacionais() == null ? "null"
						: getAtendimentosOperacionais().size()).append(", ");
		sb.append("Atendimentos Taticos: ").append(
				getAtendimentosTaticos() == null ? "null"
						: getAtendimentosTaticos().size());
		sb.append("Salas: ")
				.append(getSalas() == null ? "null" : getSalas().size())
				.append(", ");
		sb.append("GruposSala: ")
				.append(getGruposSala() == null ? "null" : getGruposSala()
						.size()).append(", ");
		sb.append("PreRequisitos: ")
				.append(getPreRequisitos() == null ? "null"
						: getPreRequisitos().size()).append(", ");
		sb.append("CoRequisitos: ").append(
				getCoRequisitos() == null ? "null" : getCoRequisitos().size());

		return sb.toString();
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	@Column(name = "DIS_ID")
	private Long id;

	@Version
	@Column(name = "version")
	private Integer version;

	public Long getId() {
		return this.id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public Integer getVersion() {
		return this.version;
	}

	public void setVersion(Integer version) {
		this.version = version;
	}

	public Set<Sala> getSalas() {
		return this.salas;
	}

	public void setSalas(Set<Sala> salas) {
		this.salas = salas;
	}

	public Set<GrupoSala> getGruposSala() {
		return this.gruposSala;
	}

	public void setGruposSala(Set<GrupoSala> gruposSala) {
		this.gruposSala = gruposSala;
	}

	public Set<CurriculoDisciplina> getPreRequisitos() {
		return this.preRequisitos;
	}

	public void setPreRequisitos(Set<CurriculoDisciplina> preRequisitos) {
		this.preRequisitos = preRequisitos;
	}

	public Set<CurriculoDisciplina> getCoRequisitos() {
		return this.coRequisitos;
	}

	public void setCoRequisitos(Set<CurriculoDisciplina> coRequisitos) {
		this.coRequisitos = coRequisitos;
	}

	@Transactional
	public void detach() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}

		this.entityManager.detach(this);
	}

	@Transactional
	public void persistAndPreencheHorarios() {
		persist();
		preencheHorarios();
	}

	@Transactional
	public void persistAndPreencheHorarios(Set<SemanaLetiva> semanas) {
		persist();
		preencheHorarios(semanas);
	}

	@Transactional
	public void persist() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}

		this.entityManager.persist(this);
	}

	@Transactional
	static public void preencheHorariosDasDisciplinas(
			List<Disciplina> disciplinas, List<SemanaLetiva> semanasLetivas,
			InstituicaoEnsino instituicaoEnsino) {
		Map<SemanaLetiva, List<HorarioAula>> mapSemanaLetivaHorarioAula = new HashMap<SemanaLetiva, List<HorarioAula>>();
		for (HorarioAula ha : HorarioAula.findAll(instituicaoEnsino)) {
			if (mapSemanaLetivaHorarioAula.get(ha.getSemanaLetiva()) == null) {
				List<HorarioAula> listHa = new ArrayList<HorarioAula>();
				listHa.add(ha);
				mapSemanaLetivaHorarioAula.put(ha.getSemanaLetiva(), listHa);
			} else {
				mapSemanaLetivaHorarioAula.get(ha.getSemanaLetiva()).add(ha);
			}
		}

		Map<HorarioAula, List<HorarioDisponivelCenario>> mapHorarioAulaHorarioDisponivel = new HashMap<HorarioAula, List<HorarioDisponivelCenario>>();
		for (HorarioDisponivelCenario ha : HorarioDisponivelCenario
				.findAll(instituicaoEnsino)) {
			if (mapHorarioAulaHorarioDisponivel.get(ha.getHorarioAula()) == null) {
				List<HorarioDisponivelCenario> listHa = new ArrayList<HorarioDisponivelCenario>();
				listHa.add(ha);
				mapHorarioAulaHorarioDisponivel
						.put(ha.getHorarioAula(), listHa);
			} else {
				mapHorarioAulaHorarioDisponivel.get(ha.getHorarioAula())
						.add(ha);
			}
		}

		/*
		 * for (SemanaLetiva semanaLetiva : semanasLetivas) { for (HorarioAula
		 * horarioAula : mapSemanaLetivaHorarioAula.get(semanaLetiva)) { for
		 * (HorarioDisponivelCenario hdc :
		 * mapHorarioAulaHorarioDisponivel.get(horarioAula)) { //Remove as
		 * disciplinas para atualizar os horarios
		 * hdc.getDisciplinas().removeAll(disciplinas); //Verifica no sábado se
		 * existem disciplinas marcadas com usaSabado if (hdc.getDiaSemana() ==
		 * Semanas.SAB) { for(Disciplina d : disciplinas) { if
		 * (d.getUsaSabado()) { hdc.getDisciplinas().add(d); } } } //Verifica no
		 * domingo se existem disciplinas marcadas com usaDomingo else if
		 * (hdc.getDiaSemana() == Semanas.DOM){ for(Disciplina d : disciplinas)
		 * { if (d.getUsaDomingo()) { hdc.getDisciplinas().add(d); } } }
		 * //Adiciona para todos os outros dias da semana else {
		 * hdc.getDisciplinas().addAll(disciplinas); } hdc.merge(); count++;if
		 * (count == 100)
		 * {System.out.println("   100 horários de disciplinas processadas");
		 * count = 0;} } } }
		 */
	}

	@Transactional
	public void preencheHorarios() {
		preencheHorarios(new HashSet<SemanaLetiva>());
	}

	@Transactional
	public void preencheHorarios(Set<SemanaLetiva> semanas) {
		InstituicaoEnsino instituicaoEnsino = this.getTipoDisciplina()
				.getInstituicaoEnsino();

		Set<SemanaLetiva> listDomains = semanas;
		if (semanas.isEmpty()) {
			List<SemanaLetiva> semanasLetivas = SemanaLetiva.findByCenario(
					instituicaoEnsino, this.getCenario());
			listDomains.addAll(semanasLetivas);
		}

		// Cria estrutura de dados inicial com mapeamento de horarios de todas
		// as semanas letivas para cada dia da semana
		Map<Semanas, Map<Date, Integer>> diaSemanaMapHorarioInicioMapHorarioFim = new HashMap<Semanas, Map<Date, Integer>>();
		for (SemanaLetiva semanaLetiva : listDomains) {
			for (HorarioAula ha : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : ha
						.getHorariosDisponiveisCenario()) {
					if (diaSemanaMapHorarioInicioMapHorarioFim.get(hdc
							.getDiaSemana()) == null) {
						Map<Date, Integer> horaInicioMapHoraFim = new HashMap<Date, Integer>();
						horaInicioMapHoraFim.put(ha.getHorario(),
								semanaLetiva.getTempo());
						diaSemanaMapHorarioInicioMapHorarioFim.put(
								hdc.getDiaSemana(), horaInicioMapHoraFim);
					} else {
						if (diaSemanaMapHorarioInicioMapHorarioFim
								.get(hdc.getDiaSemana()).keySet()
								.contains(ha.getHorario())) {
							if (diaSemanaMapHorarioInicioMapHorarioFim.get(
									hdc.getDiaSemana()).get(ha.getHorario()) < semanaLetiva
									.getTempo())
								diaSemanaMapHorarioInicioMapHorarioFim.get(
										hdc.getDiaSemana()).put(
										ha.getHorario(),
										semanaLetiva.getTempo());
						} else {
							diaSemanaMapHorarioInicioMapHorarioFim.get(
									hdc.getDiaSemana()).put(ha.getHorario(),
									semanaLetiva.getTempo());
						}
					}
				}
			}
		}
		this.populaDisponibilidades(diaSemanaMapHorarioInicioMapHorarioFim);
		/*
		 * //Cria a segunda estrutura de dados com os horarios concatenados para
		 * cada dia da semana Map<Semanas, Map<Date, Date>>
		 * diaSemanaMapHorarioInicioMapHorarioFimConcatenado = new
		 * HashMap<Semanas, Map<Date, Date>>(); Set<Date> todosHorarios = new
		 * HashSet<Date>(); for (Semanas diaSemana :
		 * diaSemanaMapHorarioInicioMapHorarioFim.keySet()) { Map<Date, Date>
		 * horarioInicioHorarioFimMap = new HashMap<Date, Date>(); List<Date>
		 * horariosOrdenados = new ArrayList<Date>();
		 * horariosOrdenados.addAll(diaSemanaMapHorarioInicioMapHorarioFim
		 * .get(diaSemana).keySet()); Collections.sort(horariosOrdenados);
		 * 
		 * Date primeiroHorario = horariosOrdenados.get(0); Calendar horaFim =
		 * Calendar.getInstance(); horaFim.setTime(horariosOrdenados.get(0));
		 * horaFim.set(1979,Calendar.NOVEMBER,6);
		 * horaFim.add(Calendar.MINUTE,diaSemanaMapHorarioInicioMapHorarioFim
		 * .get(diaSemana).get(horariosOrdenados.get(0))); for (int i = 1; i <
		 * horariosOrdenados.size(); i++) { Calendar horaInicio =
		 * Calendar.getInstance(); horaInicio.setTime(horariosOrdenados.get(i));
		 * horaInicio.set(1979,Calendar.NOVEMBER,6);
		 * 
		 * if (horaFim.compareTo(horaInicio) >= 0) {
		 * horaFim.setTime(horariosOrdenados.get(i));
		 * horaFim.set(1979,Calendar.NOVEMBER,6);
		 * horaFim.add(Calendar.MINUTE,diaSemanaMapHorarioInicioMapHorarioFim
		 * .get(diaSemana).get(horariosOrdenados.get(i))); } else {
		 * horarioInicioHorarioFimMap.put(primeiroHorario, horaFim.getTime());
		 * primeiroHorario = horariosOrdenados.get(i); horaFim =
		 * Calendar.getInstance(); horaFim.setTime(horariosOrdenados.get(i));
		 * horaFim.set(1979,Calendar.NOVEMBER,6);
		 * horaFim.add(Calendar.MINUTE,diaSemanaMapHorarioInicioMapHorarioFim
		 * .get(diaSemana).get(horariosOrdenados.get(i))); } }
		 * horarioInicioHorarioFimMap.put(primeiroHorario, horaFim.getTime());
		 * todosHorarios.addAll(horarioInicioHorarioFimMap.values());
		 * todosHorarios.addAll(horarioInicioHorarioFimMap.keySet());
		 * diaSemanaMapHorarioInicioMapHorarioFimConcatenado.put(diaSemana,
		 * horarioInicioHorarioFimMap); }
		 * 
		 * //Cria a ultima estrutura de dados juntando os dias da semana. Para
		 * juntar os dias da semana os horarios concatenados //deverao ser
		 * quebrados caso exista diferenca entre os dias da semana List<Date>
		 * todosHorariosOrdenados = new ArrayList<Date>();
		 * todosHorariosOrdenados.addAll(todosHorarios);
		 * Collections.sort(todosHorariosOrdenados, new Comparator<Date>() {
		 * public int compare(Date o1, Date o2) { Calendar horaInicio =
		 * Calendar.getInstance(); horaInicio.setTime(o1);
		 * horaInicio.set(1979,Calendar.NOVEMBER,6);
		 * 
		 * Calendar horaFim = Calendar.getInstance(); horaFim.setTime(o2);
		 * horaFim.set(1979,Calendar.NOVEMBER,6);
		 * 
		 * return horaInicio.compareTo(horaFim); } }); for (int i = 1; i <
		 * todosHorariosOrdenados.size() ; i++) { DisponibilidadeDisciplina
		 * disponibilidade = new DisponibilidadeDisciplina();
		 * disponibilidade.setHorarioInicio(todosHorariosOrdenados.get(i-1));
		 * disponibilidade.setHorarioFim(todosHorariosOrdenados.get(i));
		 * disponibilidade.setDisciplina(this);
		 * disponibilidade.setDisciplina(this);
		 * disponibilidade.setSegunda(false); disponibilidade.setTerca(false);
		 * disponibilidade.setQuarta(false); disponibilidade.setQuinta(false);
		 * disponibilidade.setSexta(false); disponibilidade.setSabado(false);
		 * disponibilidade.setDomingo(false); boolean nenhumaDisponibilidade =
		 * true; for (Semanas diaSemana : Semanas.values()) { if (estaContidoEm(
		 * todosHorariosOrdenados.get(i-1), todosHorariosOrdenados.get(i),
		 * diaSemanaMapHorarioInicioMapHorarioFimConcatenado.get(diaSemana))) {
		 * switch(diaSemana) { case SEG: disponibilidade.setSegunda(true);
		 * nenhumaDisponibilidade = false; break; case TER:
		 * disponibilidade.setTerca(true); nenhumaDisponibilidade = false;
		 * break; case QUA: disponibilidade.setQuarta(true);
		 * nenhumaDisponibilidade = false; break; case QUI:
		 * disponibilidade.setQuinta(true); nenhumaDisponibilidade = false;
		 * break; case SEX: disponibilidade.setSexta(true);
		 * nenhumaDisponibilidade = false; break; case SAB:
		 * disponibilidade.setSabado(this.getUsaSabado()); if
		 * (this.getUsaSabado()) nenhumaDisponibilidade = false; break; case
		 * DOM: disponibilidade.setDomingo(this.getUsaDomingo()); if
		 * (this.getUsaDomingo()) nenhumaDisponibilidade = false; break;
		 * default: break; } } } if (!nenhumaDisponibilidade)
		 * disponibilidade.persist(); }
		 */
	}

	private boolean estaContidoEm(Date inicio, Date fim,
			Map<Date, Date> horarioInicioMapHorarioFim) {

		boolean estaContido = false;
		Calendar horaInicio = Calendar.getInstance();
		horaInicio.setTime(inicio);
		horaInicio.set(1979, Calendar.NOVEMBER, 6);

		Calendar horaFim = Calendar.getInstance();
		horaFim.setTime(fim);
		horaFim.set(1979, Calendar.NOVEMBER, 6);
		if (horarioInicioMapHorarioFim == null)
			return false;
		for (Entry<Date, Date> horarios : horarioInicioMapHorarioFim.entrySet()) {
			Calendar dispInicio = Calendar.getInstance();
			dispInicio.setTime(horarios.getKey());
			dispInicio.set(1979, Calendar.NOVEMBER, 6);

			Calendar dispFim = Calendar.getInstance();
			dispFim.setTime(horarios.getValue());
			dispFim.set(1979, Calendar.NOVEMBER, 6);
			if (dispInicio.compareTo(horaInicio) <= 0
					&& dispFim.compareTo(horaFim) >= 0) {
				estaContido = true;
			}
		}
		return estaContido;
	}

	@Transactional
	public void remove() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}

		if (this.entityManager.contains(this)) {
			this.removeDisciplinasAssociadasProfessor(this);
			this.removeProfessoresVirtuais();
			this.removeEliminadasPor();

			this.entityManager.remove(this);
		} else {
			Disciplina attached = this.entityManager.find(this.getClass(),
					this.id);

			if (attached != null) {
				this.removeDisciplinasAssociadasProfessor(attached);
				attached.removeProfessoresVirtuais();
				attached.removeEliminadasPor();

				this.entityManager.remove(attached);
			}
		}
	}

	private void removeDisciplinasAssociadasProfessor(Disciplina disciplina) {
		if (disciplina.professores != null && disciplina.professores.size() > 0) {
			for (ProfessorDisciplina pd : disciplina.professores) {
				pd.remove();
			}

			disciplina.professores.clear();
		}
	}

	@Transactional
	public void removeProfessoresVirtuais() {
		Set<ProfessorVirtual> professoresVirtuais = this
				.getProfessoresVirtuais();

		for (ProfessorVirtual professorVirtual : professoresVirtuais) {
			professorVirtual.getDisciplinas().remove(this);
			professorVirtual.merge();
		}
	}

	public Set<ProfessorVirtual> getProfessoresVirtuais() {
		return this.professoresVirtuais;
	}

	public void setProfessoresVirtuais(Set<ProfessorVirtual> professoresVirtuais) {
		this.professoresVirtuais = professoresVirtuais;
	}

	@Transactional
	public void removeEliminadasPor() {
		Set<Equivalencia> eliminadasPor = this.getEliminadasPor();

		for (Equivalencia equivalencia : eliminadasPor) {
			equivalencia.getElimina().remove();
			equivalencia.merge();
		}
	}

	@Transactional
	public void flush() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}

		this.entityManager.flush();
	}

	@Transactional
	public Disciplina merge() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}

		Disciplina merged = this.entityManager.merge(this);
		this.entityManager.flush();
		return merged;
	}

	@Transactional
	public Disciplina mergeWithoutFlush() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}

		Disciplina merged = this.entityManager.merge(this);
		return merged;
	}

	public static final EntityManager entityManager() {
		EntityManager em = new Disciplina().entityManager;

		if (em == null) {
			throw new IllegalStateException(
					"Entity manager has not been injected (is the Spring "
							+ "Aspects JAR configured as an AJC/AJDT aspects library?)");
		}

		return em;
	}

	public Incompatibilidade getIncompatibilidadeWith(
			InstituicaoEnsino instituicaoEnsino, Disciplina disciplina) {
		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Incompatibilidade o "
								+ " WHERE o.disciplina1.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.disciplina2.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.disciplina1 = :disciplina1 "
								+ " AND o.disciplina2 = :disciplina2 ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("disciplina1", this);
		q.setParameter("disciplina2", disciplina);

		Incompatibilidade incomp = null;
		try {
			incomp = (Incompatibilidade) q.getSingleResult();
		} catch (EmptyResultDataAccessException e) {
			incomp = null;
		}

		return incomp;
	}

	public static int count(InstituicaoEnsino instituicaoEnsino, Cenario cenario) {
		Query q = entityManager()
				.createQuery(
						" SELECT COUNT ( o ) FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);

		return ((Number) q.getSingleResult()).intValue();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findAll(InstituicaoEnsino instituicaoEnsino) {
		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findAllByCodigo(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo) {
		codigo = ((codigo == null) ? "" : codigo);
		codigo = ("%" + codigo.replace('*', '%') + "%");

		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario "
								+ " AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);
		q.setParameter("codigo", codigo);

		return q.getResultList();
	}

	public static Map<String, Disciplina> buildDisciplinaCodigoToDisciplinaMap(
			List<Disciplina> disciplinas) {
		Map<String, Disciplina> disciplinasMap = new HashMap<String, Disciplina>();

		for (Disciplina disciplina : disciplinas) {
			disciplinasMap.put(disciplina.getCodigo(), disciplina);
		}

		return disciplinasMap;
	}

	public static Map<Long, Disciplina> buildDisciplinaIdToDisciplinaMap(
			List<Disciplina> disciplinas) {
		Map<Long, Disciplina> disciplinasMap = new HashMap<Long, Disciplina>();

		for (Disciplina disciplina : disciplinas) {
			disciplinasMap.put(disciplina.getId(), disciplina);
		}

		return disciplinasMap;
	}

	public static Disciplina find(Long id, InstituicaoEnsino instituicaoEnsino) {
		if (id == null || instituicaoEnsino == null) {
			return null;
		}

		Disciplina disciplina = entityManager().find(Disciplina.class, id);

		if (disciplina != null
				&& disciplina.getTipoDisciplina() != null
				&& disciplina.getTipoDisciplina().getInstituicaoEnsino() != null
				&& disciplina.getTipoDisciplina().getInstituicaoEnsino() == instituicaoEnsino) {
			return disciplina;
		}

		return null;
	}

	public static List<Disciplina> find(InstituicaoEnsino instituicaoEnsino,
			int firstResult, int maxResults) {
		return find(instituicaoEnsino, firstResult, maxResults, null);
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> find(InstituicaoEnsino instituicaoEnsino,
			int firstResult, int maxResults, String orderBy) {
		orderBy = ((orderBy != null) ? " ORDER BY o." + orderBy : "");

		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ orderBy);

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setFirstResult(firstResult);
		q.setMaxResults(maxResults);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCenario(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario) {
		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findBySalas(
			InstituicaoEnsino instituicaoEnsino, List<Sala> salas) {
		Query q = entityManager()
				.createQuery(
						" SELECT DISTINCT ( o ) "
								+ " FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.sala IN ( :salas ) ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("salas", salas);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCursos(
			InstituicaoEnsino instituicaoEnsino, List<Curso> cursos) {
		Query q = entityManager()
				.createQuery(
						" SELECT DISTINCT ( o.disciplina ) "
								+ " FROM CurriculoDisciplina o "
								+ " WHERE o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.curriculo IN "
								+ " ( SELECT c FROM Curriculo c WHERE c.curso IN ( :cursos ) ) ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cursos", cursos);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCampusAndTurnoAndPeriodo(
			InstituicaoEnsino instituicaoEnsino, Campus campus, Turno turno,
			Integer periodo) {
		Query q = entityManager()
				.createQuery(
						" SELECT DISTINCT ( o.disciplina ) "
								+ " FROM CurriculoDisciplina o "
								+ " WHERE o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.periodo = :periodo "
								+ " AND o.curriculo.turno = :turno "
								+ " AND o.curriculo.campus = :campus ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("campus", campus);
		q.setParameter("turno", turno);
		q.setParameter("periodo", periodo);

		return q.getResultList();
	}

	public static Disciplina findByCodigo(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, String codigo) {
		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario "
								+ " AND o.codigo = :codigo ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);
		q.setParameter("codigo", codigo);

		return (Disciplina) q.getSingleResult();
	}

	public static Disciplina findByCodigo(InstituicaoEnsino instituicaoEnsino,
			String codigo) {
		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.codigo = :codigo ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("codigo", codigo);

		return (Disciplina) q.getSingleResult();
	}

	public static int count(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, String codigo, String nome,
			TipoDisciplina tipoDisciplina, String operadorCreditosTeorico,
			Integer creditosTeorico, String operadorCreditosPratico,
			Integer creditosPratico, Boolean exigeLaboratorio,
			String operadorMaxAlunosTeorico, Integer maxAlunosTeorico,
			String operadorMaxAlunosPratico, Integer maxAlunosPratico,
			Boolean aulasContinuas, Boolean professorUnico, Boolean usaSabado,
			Boolean usaDomingo, String dificuldade) {
		nome = ((nome == null) ? "" : nome);
		nome = ("%" + nome.replace('*', '%') + "%");
		codigo = ((codigo == null) ? "" : codigo);
		codigo = ("%" + codigo.replace('*', '%') + "%");

		String queryCampus = "";
		if (tipoDisciplina != null) {
			queryCampus = (" o.tipoDisciplina = :tipoDisciplina AND ");
		}

		if (creditosTeorico != null) {
			if (operadorCreditosTeorico != null)
				queryCampus += " creditosTeorico " + operadorCreditosTeorico
						+ " :creditosTeorico and ";
			else
				queryCampus += " creditosTeorico = :creditosTeorico and ";
		}

		if (creditosPratico != null) {
			if (operadorCreditosPratico != null)
				queryCampus += " creditosPratico " + operadorCreditosPratico
						+ " :creditosPratico and ";
			else
				queryCampus += " creditosPratico = :creditosPratico and ";
		}

		if (aulasContinuas != null) {
			queryCampus += (" o.aulasContinuas = :aulasContinuas AND ");
		}

		if (professorUnico != null) {
			queryCampus += (" o.professorUnico = :professorUnico AND ");
		}

		if (exigeLaboratorio != null) {
			queryCampus += (" o.laboratorio = :exigeLaboratorio AND ");
		}

		if (usaSabado != null) {
			queryCampus += (" o.usaSabado = :usaSabado AND ");
		}

		if (usaDomingo != null) {
			queryCampus += (" o.usaDomingo = :usaDomingo AND ");
		}

		if (dificuldade != null) {
			queryCampus += (" o.dificuldade = :dificuldade AND ");
		}

		if (maxAlunosTeorico != null) {
			if (operadorMaxAlunosTeorico != null)
				queryCampus += " maxAlunosTeorico " + operadorMaxAlunosTeorico
						+ " :maxAlunosTeorico and ";
			else
				queryCampus += " maxAlunosTeorico = :maxAlunosTeorico and ";
		}

		if (maxAlunosPratico != null) {
			if (operadorMaxAlunosPratico != null)
				queryCampus += " maxAlunosPratico " + operadorMaxAlunosPratico
						+ " :maxAlunosPratico and ";
			else
				queryCampus += " maxAlunosPratico = :maxAlunosPratico and ";
		}

		Query q = entityManager()
				.createQuery(
						" SELECT COUNT ( o ) FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario "
								+ " AND "
								+ queryCampus
								+ " LOWER ( o.nome ) LIKE LOWER ( :nome ) "
								+ " AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) ");

		if (tipoDisciplina != null) {
			q.setParameter("tipoDisciplina", tipoDisciplina);
		}

		if (creditosTeorico != null) {
			q.setParameter("creditosTeorico", creditosTeorico);
		}
		if (creditosPratico != null) {
			q.setParameter("creditosPratico", creditosPratico);
		}

		if (aulasContinuas != null) {
			q.setParameter("aulasContinuas", aulasContinuas);
		}

		if (professorUnico != null) {
			q.setParameter("professorUnico", professorUnico);
		}

		if (exigeLaboratorio != null) {
			q.setParameter("exigeLaboratorio", exigeLaboratorio);
		}

		if (usaSabado != null) {
			q.setParameter("usaSabado", usaSabado);
		}

		if (usaDomingo != null) {
			q.setParameter("usaDomingo", usaDomingo);
		}

		if (maxAlunosTeorico != null) {
			q.setParameter("maxAlunosTeorico", maxAlunosTeorico);
		}
		if (maxAlunosPratico != null) {
			q.setParameter("maxAlunosPratico", maxAlunosPratico);
		}

		if (dificuldade != null) {
			q.setParameter("dificuldade", Dificuldades.get(dificuldade));
		}

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);
		q.setParameter("nome", nome);
		q.setParameter("codigo", codigo);

		return ((Number) q.getSingleResult()).intValue();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findBy(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, String codigo, String nome,
			TipoDisciplina tipoDisciplina, String operadorCreditosTeorico,
			Integer creditosTeorico, String operadorCreditosPratico,
			Integer creditosPratico, Boolean exigeLaboratorio,
			String operadorMaxAlunosTeorico, Integer maxAlunosTeorico,
			String operadorMaxAlunosPratico, Integer maxAlunosPratico,
			Boolean aulasContinuas, Boolean professorUnico, Boolean usaSabado,
			Boolean usaDomingo, String dificuldade, int firstResult,
			int maxResults, String orderBy) {
		nome = ((nome == null) ? "" : nome);
		nome = ("%" + nome.replace('*', '%') + "%");
		codigo = ((codigo == null) ? "" : codigo);
		codigo = ("%" + codigo.replace('*', '%') + "%");

		orderBy = ((orderBy != null) ? " ORDER BY o."
				+ orderBy.replace("String", "") : "");

		String queryCampus = "";
		if (tipoDisciplina != null) {
			queryCampus = (" o.tipoDisciplina = :tipoDisciplina AND ");
		}

		if (creditosTeorico != null) {
			if (operadorCreditosTeorico != null)
				queryCampus += " creditosTeorico " + operadorCreditosTeorico
						+ " :creditosTeorico and ";
			else
				queryCampus += " creditosTeorico = :creditosTeorico and ";
		}

		if (creditosPratico != null) {
			if (operadorCreditosPratico != null)
				queryCampus += " creditosPratico " + operadorCreditosPratico
						+ " :creditosPratico and ";
			else
				queryCampus += " creditosPratico = :creditosPratico and ";
		}

		if (aulasContinuas != null) {
			queryCampus += (" o.aulasContinuas = :aulasContinuas AND ");
		}

		if (professorUnico != null) {
			queryCampus += (" o.professorUnico = :professorUnico AND ");
		}

		if (exigeLaboratorio != null) {
			queryCampus += (" o.laboratorio = :exigeLaboratorio AND ");
		}

		if (usaSabado != null) {
			queryCampus += (" o.usaSabado = :usaSabado AND ");
		}

		if (usaDomingo != null) {
			queryCampus += (" o.usaDomingo = :usaDomingo AND ");
		}

		if (dificuldade != null) {
			queryCampus += (" o.dificuldade = :dificuldade AND ");
		}

		if (maxAlunosTeorico != null) {
			if (operadorMaxAlunosTeorico != null)
				queryCampus += " maxAlunosTeorico " + operadorMaxAlunosTeorico
						+ " :maxAlunosTeorico and ";
			else
				queryCampus += " maxAlunosTeorico = :maxAlunosTeorico and ";
		}

		if (maxAlunosPratico != null) {
			if (operadorMaxAlunosPratico != null)
				queryCampus += " maxAlunosPratico " + operadorMaxAlunosPratico
						+ " :maxAlunosPratico and ";
			else
				queryCampus += " maxAlunosPratico = :maxAlunosPratico and ";
		}

		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario "
								+ " AND "
								+ queryCampus
								+ " LOWER ( o.nome ) LIKE LOWER ( :nome ) "
								+ " AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) "
								+ orderBy);

		if (tipoDisciplina != null) {
			q.setParameter("tipoDisciplina", tipoDisciplina);
		}

		if (creditosTeorico != null) {
			q.setParameter("creditosTeorico", creditosTeorico);
		}
		if (creditosPratico != null) {
			q.setParameter("creditosPratico", creditosPratico);
		}

		if (aulasContinuas != null) {
			q.setParameter("aulasContinuas", aulasContinuas);
		}

		if (professorUnico != null) {
			q.setParameter("professorUnico", professorUnico);
		}

		if (exigeLaboratorio != null) {
			q.setParameter("exigeLaboratorio", exigeLaboratorio);
		}

		if (usaSabado != null) {
			q.setParameter("usaSabado", usaSabado);
		}

		if (usaDomingo != null) {
			q.setParameter("usaDomingo", usaDomingo);
		}

		if (maxAlunosTeorico != null) {
			q.setParameter("maxAlunosTeorico", maxAlunosTeorico);
		}
		if (maxAlunosPratico != null) {
			q.setParameter("maxAlunosPratico", maxAlunosPratico);
		}

		if (dificuldade != null) {
			q.setParameter("dificuldade", Dificuldades.get(dificuldade));
		}

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);
		q.setParameter("nome", nome);
		q.setParameter("codigo", codigo);
		q.setFirstResult(firstResult);
		q.setMaxResults(maxResults);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findBy(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, String codigo, String nome, int firstResult,
			int maxResults, String orderBy) {
		nome = ((nome == null) ? "" : nome);
		nome = ("%" + nome.replace('*', '%') + "%");
		codigo = ((codigo == null) ? "" : codigo);
		codigo = ("%" + codigo.replace('*', '%') + "%");

		orderBy = ((orderBy != null) ? " ORDER BY o."
				+ orderBy.replace("String", "") : "");

		Query q = entityManager()
				.createQuery(
						" SELECT DISTINCT o FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario "
								+ " AND ( LOWER ( o.nome ) LIKE LOWER ( :nome ) "
								+ " OR LOWER ( o.codigo ) LIKE LOWER ( :codigo ) ) "
								+ orderBy);

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);
		q.setParameter("nome", nome);
		q.setParameter("codigo", codigo);
		q.setFirstResult(firstResult);
		q.setMaxResults(maxResults);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCenarioOtimizado(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario) {

		Query q = entityManager()
				.createQuery(
						"SELECT DISTINCT o FROM Disciplina o " +
						" JOIN o.atendimentosOperacionais a " +
						" WHERE o.cenario = :cenario " +
						" AND o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);
		

		return q.getResultList();
	}

	private static Query findByEntityAndName(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			String codigo, String specificQuery) {
		codigo = ((codigo == null) ? "" : codigo);
		codigo = ("%" + codigo.replace('*', '%') + "%");

		Query q = entityManager()
				.createQuery(
						" SELECT DISTINCT ( o.disciplina ) "
								+ " FROM CurriculoDisciplina o "
								+ " WHERE o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.disciplina.cenario = :cenario "
								+ " AND LOWER ( o.disciplina.codigo ) LIKE LOWER ( :codigo ) "
								+ specificQuery
								+ " ORDER BY o.disciplina.nome ASC");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("cenario", cenario);
		q.setParameter("codigo", codigo);

		return q;
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCursoAndName(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			String codigo, List<Curso> cursos) {
		String specificQuery = " AND o.curriculo IN "
				+ " ( SELECT c FROM Curriculo c WHERE c.curso IN ( :cursos ) ) ";

		Query q = findByEntityAndName(instituicaoEnsino, cenario, codigo,
				specificQuery);
		q.setParameter("cursos", cursos);

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCurriculoIdAndName(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			String codigo, Long curriculoId) {
		String specificQuery = " AND o.curriculo =  " + curriculoId;

		Query q = findByEntityAndName(instituicaoEnsino, cenario, codigo,
				specificQuery);

		return q.getResultList();
	}

	public Boolean isIncompativelCom(InstituicaoEnsino instituicaoEnsino,
			Disciplina disciplina) {
		Query q = entityManager()
				.createQuery(
						" SELECT COUNT ( o ) FROM Incompatibilidade o "
								+ " WHERE o.disciplina1.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.disciplina2.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.disciplina1 = :disciplina1 "
								+ " AND o.disciplina2 = :disciplina2 ");

		q.setParameter("disciplina1", this);
		q.setParameter("disciplina2", disciplina);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);

		return (((Number) q.getSingleResult()).intValue() > 0);
	}

	@SuppressWarnings("unchecked")
	public List<HorarioDisponivelCenario> getHorarios(
			InstituicaoEnsino instituicaoEnsino) {
		Query q = entityManager()
				.createQuery(
						" SELECT o FROM HorarioDisponivelCenario o, IN ( o.atendimentosOperacionais ) c "
								+ " WHERE c.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND c.disciplina = :disciplina "
								+ " AND o.horarioAula.semanaLetiva.instituicaoEnsino = :instituicaoEnsino ");

		q.setParameter("disciplina", this);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);

		return q.getResultList();
	}

	public static boolean checkCodigoUnique(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo) {
		Query q = entityManager()
				.createQuery(
						" SELECT COUNT ( o ) FROM Disciplina o "
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.cenario = :cenario "
								+ " AND o.codigo = :codigo ");

		q.setParameter("cenario", cenario);
		q.setParameter("codigo", codigo);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);

		Number size = (Number) q.setMaxResults(1).getSingleResult();
		return (size.intValue() > 0);
	}

	public Integer getCreditosTotal() {
		return (getCreditosPratico() + getCreditosTeorico());
	}

	public Cenario getCenario() {
		return this.cenario;
	}

	public Integer getTotalCreditos() {
		return getCreditosPratico() + getCreditosTeorico();
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public TipoDisciplina getTipoDisciplina() {
		return this.tipoDisciplina;
	}

	public void setTipoDisciplina(TipoDisciplina tipoDisciplina) {
		this.tipoDisciplina = tipoDisciplina;
	}

	public DivisaoCredito getDivisaoCreditos() {
		return this.divisaoCreditos;
	}

	public void setDivisaoCreditos(DivisaoCredito divisaoCreditos) {
		this.divisaoCreditos = divisaoCreditos;
	}

	public String getCodigo() {
		return this.codigo;
	}

	public void setCodigo(String codigo) {
		this.codigo = codigo;
	}

	public String getNome() {
		return this.nome;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}

	public Integer getCreditosTeorico() {
		return this.creditosTeorico;
	}

	public void setCreditosTeorico(Integer creditosTeorico) {
		this.creditosTeorico = creditosTeorico;
	}

	public Integer getCreditosPratico() {
		return this.creditosPratico;
	}

	public void setCreditosPratico(Integer creditosPratico) {
		this.creditosPratico = creditosPratico;
	}

	public Boolean getLaboratorio() {
		return this.laboratorio;
	}

	public void setLaboratorio(Boolean laboratorio) {
		this.laboratorio = laboratorio;
	}

	public Dificuldades getDificuldade() {
		return this.dificuldade;
	}

	public void setDificuldade(Dificuldades dificuldade) {
		this.dificuldade = dificuldade;
	}

	public Integer getMaxAlunosTeorico() {
		return this.maxAlunosTeorico;
	}

	public void setMaxAlunosTeorico(Integer maxAlunosTeorico) {
		this.maxAlunosTeorico = maxAlunosTeorico;
	}

	public Integer getMaxAlunosPratico() {
		return this.maxAlunosPratico;
	}

	public void setMaxAlunosPratico(Integer maxAlunosPratico) {
		this.maxAlunosPratico = maxAlunosPratico;
	}

	public Boolean getAulasContinuas() {
		return this.aulasContinuas;
	}

	public void setAulasContinuas(Boolean aulasContinuas) {
		this.aulasContinuas = aulasContinuas;
	}

	public Boolean getProfessorUnico() {
		return this.professorUnico;
	}

	public void setProfessorUnico(Boolean professorUnico) {
		this.professorUnico = professorUnico;
	}

	public Boolean getUsaSabado() {
		return this.usaSabado;
	}

	public void setUsaSabado(Boolean usaSabado) {
		this.usaSabado = usaSabado;
	}

	public Boolean getUsaDomingo() {
		return this.usaDomingo;
	}

	public void setUsaDomingo(Boolean usaDomingo) {
		this.usaDomingo = usaDomingo;
	}

	public Integer getCargaHoraria() {
		return this.cargaHoraria;
	}

	public void setCargaHoraria(Integer cargaHoraria) {
		this.cargaHoraria = cargaHoraria;
	}

	/*
	 * private Set< HorarioDisponivelCenario > getHorarios() { return
	 * this.horarios; }
	 * 
	 * public void setHorarios( Set< HorarioDisponivelCenario > horarios ) {
	 * this.horarios = horarios; }
	 */

	public Set<ProfessorDisciplina> getProfessores() {
		return this.professores;
	}

	public void setProfessores(Set<ProfessorDisciplina> professores) {
		this.professores = professores;
	}

	public Set<Incompatibilidade> getIncompatibilidades() {
		return this.incompatibilidades;
	}

	public void setIncompatibilidades(Set<Incompatibilidade> incompatibilidades) {
		this.incompatibilidades = incompatibilidades;
	}

	public Set<Equivalencia> getEliminam() {
		return this.eliminam;
	}

	public void setEliminam(Set<Equivalencia> eliminam) {
		this.eliminam = eliminam;
	}

	public Set<Equivalencia> getEliminadasPor() {
		return this.eliminadasPor;
	}

	public void setEliminadasPor(Set<Equivalencia> eliminadasPor) {
		this.eliminadasPor = eliminadasPor;
	}

	public Set<CurriculoDisciplina> getCurriculos() {
		return this.curriculos;
	}

	public void setCurriculos(Set<CurriculoDisciplina> curriculos) {
		this.curriculos = curriculos;
	}

	public Set<Demanda> getDemandas() {
		return this.demandas;
	}

	public void setDemandas(Set<Demanda> demandas) {
		this.demandas = demandas;
	}

	public Set<Fixacao> getFixacoes() {
		return this.fixacoes;
	}

	public void setFixacoes(Set<Fixacao> fixacoes) {
		this.fixacoes = fixacoes;
	}

	public Set<AtendimentoOperacional> getAtendimentosOperacionais() {
		return this.atendimentosOperacionais;
	}

	public void setAtendimentosOperacionais(
			Set<AtendimentoOperacional> atendimentosOperacionais) {
		this.atendimentosOperacionais = atendimentosOperacionais;
	}

	public Set<AtendimentoTatico> getAtendimentosTaticos() {
		return this.atendimentosTaticos;
	}

	public void setAtendimentosTaticos(
			Set<AtendimentoTatico> atendimentosTaticos) {
		this.atendimentosTaticos = atendimentosTaticos;
	}

	// @Override
	public int compareTo(Disciplina o) {
		return getCodigo().compareTo(o.getCodigo());
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null || !(obj instanceof Disciplina)) {
			return false;
		}

		Disciplina other = (Disciplina) obj;

		if (this.id == null) {
			if (other.id != null) {
				return false;
			}
		} else if (!this.id.equals(other.id)) {
			return false;
		}

		return true;
	}

	public int hashCode() {
		final int prime = 31;
		int result = 1;

		result = (prime * result + ((this.id == null) ? 0 : this.id.hashCode()));
		result = (prime * result + ((this.version == null) ? 0 : this.version
				.hashCode()));

		return result;
	}

	/**
	 * Este m�todo foi implementado por conta da issue TRIEDA-1154 (Os
	 * "horarios disponiveis" de uma disciplina ja associada a alguma matriz
	 * curricular devem pertencer somente 'a semana letiva da matriz curricular
	 * correspondente).
	 * 
	 * @return as semanas letivas associadas com a disciplina em quest�o, isto
	 *         �, as semanas letivas associadas com as matrizes curriculares
	 *         que cont�m a disciplina em quest�o.
	 */
	public Set<SemanaLetiva> getSemanasLetivas() {
		Set<SemanaLetiva> semanasLetivas = new HashSet<SemanaLetiva>();
		for (CurriculoDisciplina curriculoDisciplina : getCurriculos()) {
			semanasLetivas.add(curriculoDisciplina.getCurriculo()
					.getSemanaLetiva());
		}
		return semanasLetivas;
	}

	/**
	 * Informa se a disciplina ocupa grade de hor�rios ou n�o. Uma
	 * disciplina ocupa grade de hor�rios nos seguintes casos: - a disciplina
	 * � do tipo "Presencial" ou "Telepresencial"; - a disciplina possui
	 * cr�ditos te�ricos ou pr�ticos;
	 * 
	 * @return true caso a disciplina ocupa grade de hor�rios e false caso
	 *         contr�rio
	 */
	public boolean ocupaGrade() {
		int totalCreditos = this.getCreditosTeorico()
				+ this.getCreditosPratico();
		return this.getTipoDisciplina().ocupaGrade() && totalCreditos > 0;
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByProfessor(
			InstituicaoEnsino instituicaoEnsino, Professor professor) {

		Query q = entityManager()
				.createQuery(
						" SELECT o FROM Disciplina o "
								+ " INNER JOIN o.professores p"
								+ " WHERE o.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino "
								+ " AND p.professor = :professor ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("professor", professor);

		return q.getResultList();
	}

	public Set<Turma> getTurmas() {
		return turmas;
	}

	public void setTurmas(Set<Turma> turmas) {
		this.turmas = turmas;
	}

	public Disciplina clone(CenarioClone novoCenario) {
		Disciplina clone = new Disciplina();
		clone.setAulasContinuas(this.getAulasContinuas());
		clone.setCargaHoraria(this.getCargaHoraria());
		clone.setCenario(novoCenario.getCenario());
		clone.setCodigo(this.getCodigo());
		clone.setCreditosPratico(this.getCreditosPratico());
		clone.setCreditosTeorico(this.getCreditosTeorico());
		clone.setDificuldade(this.getDificuldade());
		clone.setLaboratorio(this.getLaboratorio());
		clone.setMaxAlunosPratico(this.getMaxAlunosPratico());
		clone.setMaxAlunosTeorico(this.getMaxAlunosTeorico());
		clone.setNome(this.getNome());
		clone.setProfessorUnico(this.getProfessorUnico());
		clone.setDivisaoCreditos(this.getDivisaoCreditos());
		clone.setUsaDomingo(this.getUsaDomingo());
		clone.setUsaSabado(this.getUsaSabado());

		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, Disciplina entidadeClone) {
		entidadeClone.setTipoDisciplina(novoCenario.getEntidadeClonada(this
				.getTipoDisciplina()));
		if (this.getDivisaoCreditos() != null) {
			entidadeClone.setDivisaoCreditos(novoCenario.clone(this
					.getDivisaoCreditos()));
		}

		for (GrupoSala grupoSala : this.getGruposSala()) {
			entidadeClone.getGruposSala().add(
					novoCenario.getEntidadeClonada(grupoSala));
			// novoCenario.getEntidadeClonada(grupoSala).getDisciplinas().add(entidadeClone);
		}

		for (Sala sala : this.getSalas()) {
			entidadeClone.getSalas().add(novoCenario.getEntidadeClonada(sala));
			// novoCenario.getEntidadeClonada(sala).getDisciplinas().add(entidadeClone);
		}
	}

	public Set<DisponibilidadeDisciplina> getDisponibilidades() {
		return disponibilidades;
	}

	public void setDisponibilidades(
			Set<DisponibilidadeDisciplina> disponibilidades) {
		this.disponibilidades = disponibilidades;
	}

	private void removeTodasDisponibilidades() {
		for (DisponibilidadeDisciplina disp : this.disponibilidades) {
			disp.remove();
		}
		this.disponibilidades.clear();
		this.mergeWithoutFlush();
	}

	private void populaDisponibilidades(
			Map<Semanas, Map<Date, Integer>> diaSemanaMapHorarioInicioMapHorarioFim) {
		// Cria a segunda estrutura de dados com os horarios concatenados para
		// cada dia da semana
		Map<Semanas, Map<Date, Date>> diaSemanaMapHorarioInicioMapHorarioFimConcatenado = new HashMap<Semanas, Map<Date, Date>>();
		Set<Date> todosHorarios = new HashSet<Date>();
		for (Semanas diaSemana : diaSemanaMapHorarioInicioMapHorarioFim
				.keySet()) {
			Map<Date, Date> horarioInicioHorarioFimMap = new HashMap<Date, Date>();
			List<Date> horariosOrdenados = new ArrayList<Date>();
			horariosOrdenados.addAll(diaSemanaMapHorarioInicioMapHorarioFim
					.get(diaSemana).keySet());
			Collections.sort(horariosOrdenados);

			Date primeiroHorario = horariosOrdenados.get(0);
			Calendar horaFim = Calendar.getInstance();
			horaFim.setTime(horariosOrdenados.get(0));
			horaFim.set(1979, Calendar.NOVEMBER, 6);
			horaFim.add(Calendar.MINUTE, diaSemanaMapHorarioInicioMapHorarioFim
					.get(diaSemana).get(horariosOrdenados.get(0)));
			for (int i = 1; i < horariosOrdenados.size(); i++) {
				Calendar horaInicio = Calendar.getInstance();
				horaInicio.setTime(horariosOrdenados.get(i));
				horaInicio.set(1979, Calendar.NOVEMBER, 6);

				if (horaFim.compareTo(horaInicio) >= 0) {
					horaFim.setTime(horariosOrdenados.get(i));
					horaFim.set(1979, Calendar.NOVEMBER, 6);
					horaFim.add(
							Calendar.MINUTE,
							diaSemanaMapHorarioInicioMapHorarioFim.get(
									diaSemana).get(horariosOrdenados.get(i)));
				} else {
					horarioInicioHorarioFimMap.put(primeiroHorario,
							horaFim.getTime());
					primeiroHorario = horariosOrdenados.get(i);
					horaFim = Calendar.getInstance();
					horaFim.setTime(horariosOrdenados.get(i));
					horaFim.set(1979, Calendar.NOVEMBER, 6);
					horaFim.add(
							Calendar.MINUTE,
							diaSemanaMapHorarioInicioMapHorarioFim.get(
									diaSemana).get(horariosOrdenados.get(i)));
				}
			}
			horarioInicioHorarioFimMap.put(primeiroHorario, horaFim.getTime());
			todosHorarios.addAll(horarioInicioHorarioFimMap.values());
			todosHorarios.addAll(horarioInicioHorarioFimMap.keySet());
			diaSemanaMapHorarioInicioMapHorarioFimConcatenado.put(diaSemana,
					horarioInicioHorarioFimMap);
		}

		// Remove disponibilidades da disciplina
		removeTodasDisponibilidades();

		// Cria a ultima estrutura de dados juntando os dias da semana. Para
		// juntar os dias da semana os horarios concatenados
		// deverao ser quebrados caso exista diferenca entre os dias da semana
		List<Date> todosHorariosOrdenados = new ArrayList<Date>();
		todosHorariosOrdenados.addAll(todosHorarios);
		Collections.sort(todosHorariosOrdenados, new Comparator<Date>() {
			public int compare(Date o1, Date o2) {
				Calendar horaInicio = Calendar.getInstance();
				horaInicio.setTime(o1);
				horaInicio.set(1979, Calendar.NOVEMBER, 6);

				Calendar horaFim = Calendar.getInstance();
				horaFim.setTime(o2);
				horaFim.set(1979, Calendar.NOVEMBER, 6);

				return horaInicio.compareTo(horaFim);
			}
		});
		for (int i = 1; i < todosHorariosOrdenados.size(); i++) {
			DisponibilidadeDisciplina disponibilidade = new DisponibilidadeDisciplina();
			disponibilidade.setHorarioInicio(todosHorariosOrdenados.get(i - 1));
			disponibilidade.setHorarioFim(todosHorariosOrdenados.get(i));
			disponibilidade.setDisciplina(this);
			disponibilidade.setSegunda(false);
			disponibilidade.setTerca(false);
			disponibilidade.setQuarta(false);
			disponibilidade.setQuinta(false);
			disponibilidade.setSexta(false);
			disponibilidade.setSabado(false);
			disponibilidade.setDomingo(false);
			boolean nenhumaDisponibilidade = true;
			for (Semanas diaSemana : Semanas.values()) {
				if (estaContidoEm(todosHorariosOrdenados.get(i - 1),
						todosHorariosOrdenados.get(i),
						diaSemanaMapHorarioInicioMapHorarioFimConcatenado
								.get(diaSemana))) {
					switch (diaSemana) {
					case SEG:
						disponibilidade.setSegunda(true);
						nenhumaDisponibilidade = false;
						break;
					case TER:
						disponibilidade.setTerca(true);
						nenhumaDisponibilidade = false;
						break;
					case QUA:
						disponibilidade.setQuarta(true);
						nenhumaDisponibilidade = false;
						break;
					case QUI:
						disponibilidade.setQuinta(true);
						nenhumaDisponibilidade = false;
						break;
					case SEX:
						disponibilidade.setSexta(true);
						nenhumaDisponibilidade = false;
						break;
					case SAB:
						disponibilidade.setSabado(this.getUsaSabado());
						if (this.getUsaSabado())
							nenhumaDisponibilidade = false;
						break;
					case DOM:
						disponibilidade.setDomingo(this.getUsaDomingo());
						if (this.getUsaDomingo())
							nenhumaDisponibilidade = false;
						break;
					default:
						break;

					}
				}
			}
			if (!nenhumaDisponibilidade) {
				this.disponibilidades.add(disponibilidade);// disponibilidade.persist();
			}
		}

		this.mergeWithoutFlush();
	}

	public static void atualizaDisponibilidadesDisciplinas(
			Map<Disciplina, Set<SemanaLetiva>> disciplinaToSemanasLetivasMap,
			Cenario cenario) {
		for (Entry<Disciplina, Set<SemanaLetiva>> entry : disciplinaToSemanasLetivasMap
				.entrySet()) {
			Disciplina disciplina = entry.getKey();
			Set<SemanaLetiva> semanasLetivasDaDisciplina = entry.getValue();
			// recalcula disponibilidades da disciplina com base nas semanas
			// letivas associadas com a disciplina (via cadastro de currículos)
			disciplina.preencheHorarios(semanasLetivasDaDisciplina);
		}
	}

	@Transactional
	static public void atualizaDisponibilidadesDisciplinas(
			Map<Disciplina, List<TriedaTrio<Semanas, Calendar, Calendar>>> disciplinaToDisponibilidadesMap,
			List<SemanaLetiva> semanasLetivas) {
		// coleta as disciplinas disponíveis por dia da semana e tempo de aula
		int count = 0;
		Map<Disciplina, Map<Semanas, Map<Date, Integer>>> discToDiaSemToHiToDuracaoMap = new HashMap<Disciplina, Map<Semanas, Map<Date, Integer>>>();
		for (Entry<Disciplina, List<TriedaTrio<Semanas, Calendar, Calendar>>> entry : disciplinaToDisponibilidadesMap
				.entrySet()) {
			Disciplina disciplina = entry.getKey();
			List<TriedaTrio<Semanas, Calendar, Calendar>> disponibilidades = entry
					.getValue();

			// obtém mapa de disponibilidades da disciplina
			Map<Semanas, Map<Date, Integer>> diaSemToHiToDuracaoMapDaDisciplina = null;
			if (discToDiaSemToHiToDuracaoMap.containsKey(disciplina)) {
				diaSemToHiToDuracaoMapDaDisciplina = discToDiaSemToHiToDuracaoMap
						.get(disciplina);
			} else {
				diaSemToHiToDuracaoMapDaDisciplina = new HashMap<Semanas, Map<Date, Integer>>();
				discToDiaSemToHiToDuracaoMap.put(disciplina,
						diaSemToHiToDuracaoMapDaDisciplina);
			}

			// para cada semana letiva
			for (SemanaLetiva semanaLetiva : semanasLetivas) {
				// para cada tempo de aula
				for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
					// para cada disponibilidade do ambiente lida do excel
					for (TriedaTrio<Semanas, Calendar, Calendar> trio : disponibilidades) {
						// verifica se o intervalo de horas é compatível
						boolean horarioAulaEstahContidoEmDisponibilidade = horarioAula
								.estahContidoEm(trio.getSegundo(),
										trio.getTerceiro());
						// para cada dia da semana
						for (HorarioDisponivelCenario hdc : horarioAula
								.getHorariosDisponiveisCenario()) {
							Semanas diaSem = hdc.getDiaSemana();
							// verifica se o dia da semana é compatível com as
							// disponibilidades da disciplina
							if (horarioAulaEstahContidoEmDisponibilidade
									&& diaSem.equals(trio.getPrimeiro())) {
								// armazena informação da semana letiva
								// compatível com disponibilidade da disciplina
								Map<Date, Integer> hiToDuracaoMapDoDiaSemDaDisciplina = null;
								if (diaSemToHiToDuracaoMapDaDisciplina
										.containsKey(diaSem)) {
									hiToDuracaoMapDoDiaSemDaDisciplina = diaSemToHiToDuracaoMapDaDisciplina
											.get(diaSem);
								} else {
									hiToDuracaoMapDoDiaSemDaDisciplina = new HashMap<Date, Integer>();
									diaSemToHiToDuracaoMapDaDisciplina.put(
											diaSem,
											hiToDuracaoMapDoDiaSemDaDisciplina);
								}
								hiToDuracaoMapDoDiaSemDaDisciplina.put(
										horarioAula.getHorario(),
										semanaLetiva.getTempo());
							}
						}
					}
					count++;
					if (count == 100) {
						System.out
								.println("   100 horários das disciplinas processados");
						count = 0;
					}
				}
			}
		}

		for (Entry<Disciplina, Map<Semanas, Map<Date, Integer>>> entry : discToDiaSemToHiToDuracaoMap
				.entrySet()) {
			Disciplina disciplina = entry.getKey();
			disciplina.populaDisponibilidades(entry.getValue());
		}
	}
}