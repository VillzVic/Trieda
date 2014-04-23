package com.gapso.trieda.domain;

import java.io.Serializable;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.TimeUnit;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "AUL_ID" )
@Table( name = "AULAS" )
public class Aula
	implements Serializable
{
	private static final long serialVersionUID = -8810399104404941276L;

	@PersistenceContext
	transient EntityManager entityManager;
	
	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "AUL_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;
	
	@Column( name = "AUL_QTD_CRED_TEORICOS" )
	private Integer creditosTeoricos;
	
	@Column( name = "AUL_QTD_CRED_PRATICOS" )
	private Integer creditosPraticos;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = HorarioDisponivelCenario.class )
	@JoinColumn( name = "HDC_ID" )
	private HorarioDisponivelCenario horarioDisponivelCenario;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Sala.class )
	@JoinColumn( name = "SAL_ID" )
	private Sala sala;
	
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Professor.class )
	@JoinColumn( name = "PRF_ID" )
	private Professor professor;
	
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = ProfessorVirtual.class )
	@JoinColumn( name = "PRV_ID" )
	private ProfessorVirtual professorVirtual;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;
	
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
	@JoinTable(name="AULAS_TURMAS",
	joinColumns=@JoinColumn(name="AUL_ID"),
	inverseJoinColumns=@JoinColumn(name="TUR_ID"))
    private Set<Turma> turmas = new HashSet<Turma>();
    
	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public Integer getVersion() {
		return version;
	}

	public void setVersion(Integer version) {
		this.version = version;
	}

	public Integer getCreditosTeoricos() {
		return creditosTeoricos;
	}

	public void setCreditosTeoricos(Integer creditosTeoricos) {
		this.creditosTeoricos = creditosTeoricos;
	}

	public Integer getCreditosPraticos() {
		return creditosPraticos;
	}

	public void setCreditosPraticos(Integer creditosPraticos) {
		this.creditosPraticos = creditosPraticos;
	}

	public HorarioDisponivelCenario getHorarioDisponivelCenario() {
		return horarioDisponivelCenario;
	}

	public void setHorarioDisponivelCenario(HorarioDisponivelCenario horarioDisponivelCenario) {
		this.horarioDisponivelCenario = horarioDisponivelCenario;
	}

	public Sala getSala() {
		return sala;
	}

	public void setSala(Sala sala) {
		this.sala = sala;
	}

	public Professor getProfessor() {
		return professor;
	}

	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	public ProfessorVirtual getProfessorVirtual() {
		return professorVirtual;
	}

	public void setProfessorVirtual(ProfessorVirtual professorVirtual) {
		this.professorVirtual = professorVirtual;
	}

	public Cenario getCenario() {
		return cenario;
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public Set<Turma> getTurmas() {
		return turmas;
	}

	public void setTurmas(Set<Turma> turmas) {
		this.turmas = turmas;
	}
	
	@Transactional
	public void detach()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		this.entityManager.detach( this );
	}

	@Transactional
	public void persist()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		this.entityManager.persist( this );
	}

	@Transactional
	public void remove()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		if ( this.entityManager.contains( this ) )
		{
			this.entityManager.remove( this );
		}
		else
		{
			Aula attached = this.entityManager.find(
				this.getClass(), this.id );

			if ( attached != null )
			{
				this.entityManager.remove( attached );
			}
		}
	}

	@Transactional
	public void flush()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		this.entityManager.flush();
	}

	@Transactional
	public Aula merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Aula merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Aula().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}
	
	public static Aula find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Aula aula = entityManager().find( Aula.class, id );
		
		if (aula != null && aula.getCenario().getInstituicaoEnsino().equals(instituicaoEnsino))
		{
			return aula;
		}

		return null;
	}
	
	public boolean temIntersecaoDeDiaSemanaCom(Aula o)
	{
		return this.getHorarioDisponivelCenario().getDiaSemana().equals(o.getHorarioDisponivelCenario().getDiaSemana());
	}
	
	@SuppressWarnings("unchecked")
	public static List<Aula> findBy(InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Sala sala)
	{		
		Query q = entityManager().createQuery(
				" SELECT o FROM Aula o " +
				" WHERE o.cenario.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				" AND o.sala = :sala ");

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "sala", sala );

		return q.getResultList();
	}
	
	public boolean temIntersecaoDeHorarioCom(Aula o)
	{
		Calendar horaInicio = Calendar.getInstance();
		horaInicio.setTime(this.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		horaInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar horaFim = Calendar.getInstance();
		horaFim.setTime(this.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		horaFim.set(1979,Calendar.NOVEMBER,6);
		horaFim.add(Calendar.MINUTE,this.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo()*(this.getCreditosPraticos()+this.getCreditosTeoricos()));
		
		Calendar oHoraInicio = Calendar.getInstance();
		oHoraInicio.setTime(o.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		oHoraInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar oHoraFim = Calendar.getInstance();
		oHoraFim.setTime(o.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		oHoraFim.set(1979,Calendar.NOVEMBER,6);
		oHoraFim.add(Calendar.MINUTE,o.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo()*(o.getCreditosPraticos()+o.getCreditosTeoricos()));
		
		return (horaInicio.compareTo(oHoraInicio) <= 0 && oHoraInicio.compareTo(horaFim) <= 0 ) || // hi1 < hi2 < hf1
			   (horaInicio.compareTo(oHoraFim) <= 0  && oHoraFim.compareTo(horaFim) <= 0 ) || // hi1 < hf2 < hf1
			   (oHoraInicio.compareTo(horaInicio) <= 0 && horaInicio.compareTo(oHoraFim) <= 0 ) || // hi2 < hi1 < hf2
			   (oHoraInicio.compareTo(horaFim) <= 0 && horaFim.compareTo(oHoraFim) <= 0 ); // hi2 < hf1 < hf2	
	}
	
	private boolean comparaPorHorarioSeVemAntesDe(Aula o)
    {
        // dado que o que interessa aqui é comparar somente os horários, então, é necessário
        // garantir que todas as datas na comparação estejam dentro de um mesmo dia
		Calendar horaFim = Calendar.getInstance();
		horaFim.setTime(this.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		horaFim.set(1979,Calendar.NOVEMBER,6);
		horaFim.add(Calendar.MINUTE,this.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo()*(o.getCreditosPraticos()+o.getCreditosTeoricos()));
		
		Calendar oHoraInicio = Calendar.getInstance();
		oHoraInicio.setTime(o.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		oHoraInicio.set(1979,Calendar.NOVEMBER,6);

        return horaFim.before(oHoraInicio);
    }
	
	private boolean comparaPorHorarioSeEhDepoisDe(Aula o)
    {
        // dado que o que interessa aqui é comparar somente os horários, então, é necessário
        // garantir que todas as datas na comparação estejam dentro de um mesmo dia
		Calendar horaInicio = Calendar.getInstance();
		horaInicio.setTime(this.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		horaInicio.set(1979,Calendar.NOVEMBER,6);

		Calendar oHoraFim = Calendar.getInstance();
		oHoraFim.setTime(o.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		oHoraFim.set(1979,Calendar.NOVEMBER,6);
		oHoraFim.add(Calendar.MINUTE,o.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo()*(o.getCreditosPraticos()+o.getCreditosTeoricos()));

        return horaInicio.after(oHoraFim);
    }
	
	private boolean ehViavelDeslocarParaAtender(Aula o, Map<String, Long> tempoDeslocamentoUnidades)
    {
        if (this.getSala().getUnidade().getId().equals(o.getSala().getUnidade().getId()))
        {
            return true;
        }

        // dado que o que interessa aqui é comparar somente os horários, então, é necessário
        // garantir que todas as datas na comparação estejam dentro de um mesmo dia
		Calendar oHoraInicio = Calendar.getInstance();
		oHoraInicio.setTime(o.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		oHoraInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar horaFim = Calendar.getInstance();
		horaFim.setTime(this.getHorarioDisponivelCenario().getHorarioAula().getHorario());
		horaFim.set(1979,Calendar.NOVEMBER,6);
		horaFim.add(Calendar.MINUTE,this.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo()*(this.getCreditosPraticos()+this.getCreditosTeoricos()));

        Long intervaloEntreDemandas = oHoraInicio.getTimeInMillis() - horaFim.getTimeInMillis();

        String key = this.getSala().getUnidade().getId() + "-" + o.getSala().getUnidade().getId();
        Long tempoDeslocamento = tempoDeslocamentoUnidades.get(key);

        return tempoDeslocamento <= intervaloEntreDemandas;
    }
	
	private boolean temDiaSemanaConsecutivoCom(Aula o)
    {
            switch (this.getHorarioDisponivelCenario().getDiaSemana())
            {
                case DOM:
                if (o.getHorarioDisponivelCenario().getDiaSemana().equals(Semanas.SEG)) return true;
                break;

                case SEG:
                if (o.getHorarioDisponivelCenario().getDiaSemana().equals(Semanas.TER)) return true;
                break;

                case TER:
                if (o.getHorarioDisponivelCenario().getDiaSemana().equals(Semanas.QUA)) return true;
                break;

                case QUA:
                if (o.getHorarioDisponivelCenario().getDiaSemana().equals(Semanas.QUI)) return true;
                break;

                case QUI:
                if (o.getHorarioDisponivelCenario().getDiaSemana().equals(Semanas.SEX)) return true;
                break;

                case SEX:
                if (o.getHorarioDisponivelCenario().getDiaSemana().equals(Semanas.SAB)) return true;
                break;

                case SAB:
                if (o.getHorarioDisponivelCenario().getDiaSemana().equals(Semanas.DOM)) return true;
                break;
            }

        return false;
    }
	
	public TriedaTrio<Boolean, String, Boolean> ehCompativelCom(Aula outra, Map<String, Long> tempoDeslocamentoUnidades, Parametro parametro)
	{
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		String motivoIncompatibilidade = "";
		boolean incompatibilidadeForte = false;
		// verifica se há interseção de dia da semana
		if (this.temIntersecaoDeDiaSemanaCom(outra))
		{
			// verifica se há interseção de horário
			if (this.temIntersecaoDeHorarioCom(outra))
			{
				motivoIncompatibilidade = "Intersecao de horario com a aula de " + outra.getHorarioDisponivelCenario().getDiaSemana()
						+ " com horario de inicio: " +  df.format(outra.getHorarioDisponivelCenario().getHorarioAula().getHorario()) 
						+ " na sala: " + outra.getSala().getCodigo();
				incompatibilidadeForte = true;
				return TriedaTrio.create(false, motivoIncompatibilidade, incompatibilidadeForte);
			}

			// verifica se o deslocamento é viável
			if (this.comparaPorHorarioSeVemAntesDe(outra))
			{
				if (!this.ehViavelDeslocarParaAtender(outra, tempoDeslocamentoUnidades))
				{
					String key = this.getSala().getUnidade().getId() + "-" + outra.getSala().getUnidade().getId();
					Long tempoDeslocamento = tempoDeslocamentoUnidades.get(key);
					motivoIncompatibilidade = "Tempo de deslocamento entre unidades e inviavel com a aula de " + outra.getHorarioDisponivelCenario().getDiaSemana() + 
							" com horario de inicio: " +  df.format(outra.getHorarioDisponivelCenario().getHorarioAula().getHorario()) +
							" / Unidade:" + outra.getSala().getUnidade().getNome() +
							" / Deslocamento(h):" + TimeUnit.MILLISECONDS.toHours(tempoDeslocamento);
					incompatibilidadeForte = false;
					return TriedaTrio.create(false, motivoIncompatibilidade, incompatibilidadeForte);
				}
			}
			else
			{
				if (!outra.ehViavelDeslocarParaAtender(this, tempoDeslocamentoUnidades))
				{
					String key = outra.getSala().getUnidade().getId() + "-" + this.getSala().getUnidade().getId();
					Long tempoDeslocamento = tempoDeslocamentoUnidades.get(key);
					motivoIncompatibilidade = "Tempo de deslocamento entre unidades é inviavel com a aula de " + outra.getHorarioDisponivelCenario().getDiaSemana() + 
							" com horario de inicio: " +  df.format(outra.getHorarioDisponivelCenario().getHorarioAula().getHorario()) +
							" / Unidade:" + outra.getSala().getUnidade().getNome() +
							" / Deslocamento(h):" + TimeUnit.MILLISECONDS.toHours(tempoDeslocamento);
					incompatibilidadeForte = false;
					return TriedaTrio.create(false, motivoIncompatibilidade, incompatibilidadeForte);
				}
			}
		}
		else
		{
			// verifica se há incompatibilidade por conta de interjornada
			if (this.temDiaSemanaConsecutivoCom(outra))
			{
				Calendar horaFim = Calendar.getInstance();
				horaFim.setTime(this.getHorarioDisponivelCenario().getHorarioAula().getHorario());
				horaFim.set(1979,Calendar.NOVEMBER,6);
				horaFim.add(Calendar.MINUTE,this.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo()*(this.getCreditosPraticos()+this.getCreditosTeoricos()));

				Calendar oHoraInicio = Calendar.getInstance();
				oHoraInicio.setTime(outra.getHorarioDisponivelCenario().getHorarioAula().getHorario());
				oHoraInicio.set(1979,Calendar.NOVEMBER,6);
				
				Long dif = horaFim.getTimeInMillis() - oHoraInicio.getTimeInMillis();
				if (TimeUnit.MILLISECONDS.toHours(dif) < parametro.getEvitarUltimoEPrimeiroHorarioProfessorValue())
				{
					motivoIncompatibilidade = "Interjornada maxima violada com a aula de " + outra.getHorarioDisponivelCenario().getDiaSemana() + 
							" com horario de inicio: " +  df.format(outra.getHorarioDisponivelCenario().getHorarioAula().getHorario()) +
							" / Interjornada(h):" + TimeUnit.MILLISECONDS.toHours(dif);
					incompatibilidadeForte = false;
					return TriedaTrio.create(false, motivoIncompatibilidade, incompatibilidadeForte);
				}
			}
			if (outra.temDiaSemanaConsecutivoCom(this))
			{
				System.out.println("temDiaSemanaConsecutivoCom");
				Calendar horaInicio = Calendar.getInstance();
				horaInicio.setTime(this.getHorarioDisponivelCenario().getHorarioAula().getHorario());
				horaInicio.set(1979,Calendar.NOVEMBER,6);

				Calendar oHoraFim = Calendar.getInstance();
				oHoraFim.setTime(outra.getHorarioDisponivelCenario().getHorarioAula().getHorario());
				oHoraFim.set(1979,Calendar.NOVEMBER,6);
				oHoraFim.add(Calendar.MINUTE,outra.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo()*(outra.getCreditosPraticos()+outra.getCreditosTeoricos()));
				
				Long dif = horaInicio.getTimeInMillis() - oHoraFim.getTimeInMillis();
				if (TimeUnit.MILLISECONDS.toHours(dif) < parametro.getEvitarUltimoEPrimeiroHorarioProfessorValue())
				{
					motivoIncompatibilidade = "Interjornada maxima violada com a aula de: " + outra.getHorarioDisponivelCenario().getDiaSemana() + 
							" com horario de inicio: " +  df.format(outra.getHorarioDisponivelCenario().getHorarioAula().getHorario()) +
							" / Interjornada(h):" + TimeUnit.MILLISECONDS.toHours(dif);
					incompatibilidadeForte = false;
					return TriedaTrio.create(false, motivoIncompatibilidade, incompatibilidadeForte);
				}
			}
		}

		return TriedaTrio.create(true, motivoIncompatibilidade, incompatibilidadeForte);
	}
	
	public TriedaTrio<Boolean,List<String>,List<String>> ehViavel(Map<String, Long> tempoDeslocamentoUnidades, Turma turma, Map<String, Aula> possiveisAtendimentosConflitantes, Parametro parametros)
	{
		List<String> conflitosFortes = new ArrayList<String>();
		List<String> conflitosFracos = new ArrayList<String>();
		
		// Verificar se o ambiente suporta a quantidade de alunos alocados à aula
		if (getSala().getCapacidadeInstalada() < turma.getAlunos().size()) {
			conflitosFortes.add("Capacidade maxima alcançada");
			return TriedaTrio.create(false,conflitosFortes, conflitosFracos);
		}
		
		Integer numCreditosTeoricoTurma = 0;
		Integer numCreditosPraticoTurma = 0;
		for (Aula aula : turma.getAulas())
		{
			numCreditosTeoricoTurma += aula.getCreditosTeoricos();
			numCreditosPraticoTurma += aula.getCreditosPraticos();
		}
		// Verificar se a turma suporta os créditos da aula em questão
		if (!turma.getAulas().contains(this))
		{
			if (getCreditosTeoricos() > 0 ? numCreditosTeoricoTurma + getCreditosTeoricos() > turma.getDisciplina().getCreditosTeorico() : numCreditosPraticoTurma + getCreditosPraticos() > turma.getDisciplina().getCreditosPratico()) {
				conflitosFortes.add("Numero de creditos nao suportados");
				return TriedaTrio.create(false,conflitosFortes, conflitosFracos);
			}
		}
		
	    // Colher aulas que podem ser conflitantes com a aula em questão
		Set<Aula> aulasQuePodemSerConflitantes = new HashSet<Aula>();
		aulasQuePodemSerConflitantes.addAll(possiveisAtendimentosConflitantes.values());
		// Colher aulas dos alunos (tem que buscar tanto aulas parciais quanto não-parciais, com exceção da aula em questã)
		for (AlunoDemanda aluno : turma.getAlunos())
		{
			for (Turma turmaAluno : aluno.getTurmas())
			{
				aulasQuePodemSerConflitantes.addAll(turmaAluno.getAulas(getHorarioDisponivelCenario().getDiaSemana()));
			}
		}
		if (getProfessor() != null) {
			aulasQuePodemSerConflitantes.addAll(getProfessor().getAulas(getHorarioDisponivelCenario().getDiaSemana()));
	        aulasQuePodemSerConflitantes.addAll(getProfessor().getAulas(Semanas.get(getHorarioDisponivelCenario().getDiaSemana().ordinal() -1))); // interjornada
	        aulasQuePodemSerConflitantes.addAll(getProfessor().getAulas(Semanas.get(getHorarioDisponivelCenario().getDiaSemana().ordinal() +1))); // interjornada
		}
	    // Colher aulas do ambiente (tem que buscar tanto aulas parciais quanto não-parciais, com exceção da aula em questã)
		aulasQuePodemSerConflitantes.addAll(getSala().getAulas(getHorarioDisponivelCenario().getDiaSemana()));
		
		Aula aulaEmQuestao = null;
		for (Aula aula : aulasQuePodemSerConflitantes)
		{
			String aulaKey = this.getSala().getId() + "-" + this.getHorarioDisponivelCenario().getId();
			String aulaConflitanteKey = aula.getSala().getId() + "-" + aula.getHorarioDisponivelCenario().getId();
			if (aulaKey.equals(aulaConflitanteKey))
			{
				System.out.println("Aula key " + aulaKey);
				aulaEmQuestao = aula;
			}
		}
		if (aulaEmQuestao != null)
			aulasQuePodemSerConflitantes.remove(aulaEmQuestao);
	        // Verificar conflitos entre a aula em questão e as demais aulas candidatas a conflitantes
		List<String> conflitosFortesDetectados = new ArrayList<String>();
	    List<String> conflitosFracosDetectados = new ArrayList<String>();
		for (Aula aul : aulasQuePodemSerConflitantes) {
	        TriedaTrio<Boolean, String, Boolean> compativel = ehCompativelCom(aul, tempoDeslocamentoUnidades, parametros);

			if (!compativel.getPrimeiro()) {
				if (compativel.getTerceiro()) {
					conflitosFortesDetectados.add(compativel.getSegundo());
				} else {
					conflitosFracosDetectados.add(compativel.getSegundo());
				}
			}
		}
		
	        // Retorna o resultado
	    boolean aulaEhViavel = conflitosFortesDetectados.isEmpty();	
		return TriedaTrio.create(aulaEhViavel,conflitosFortesDetectados,conflitosFracosDetectados);
	}
	
	int getTotalCreditos()
	{
		return this.getCreditosPraticos() + this.getCreditosTeoricos();
	}
	
	public boolean isTeorica()
	{
		return this.getCreditosTeoricos() > 0;
	}
	
	public List<HorarioDisponivelCenario> getHDCs()
	{
		SemanaLetiva sle = horarioDisponivelCenario.getHorarioAula().getSemanaLetiva();
		List<HorarioDisponivelCenario> hdcs = new ArrayList<HorarioDisponivelCenario>();	
		HorarioDisponivelCenario hdc = this.horarioDisponivelCenario;
		hdcs.add(hdc);
		for (int i = 1; i < this.getTotalCreditos(); i++) {
			hdc = sle.getNextHorario(hdc);
			hdcs.add(hdc);
		}
		return hdcs;
	}
}
