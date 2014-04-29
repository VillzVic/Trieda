package com.gapso.trieda.domain;

import java.io.Serializable;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Collection;
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
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "ATP_ID" )
@Table( name = "ATENDIMENTO_OPERACIONAL" )
public class AtendimentoOperacional
	implements Serializable
{
	private static final long serialVersionUID = -1061352455612316076L;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	@NotNull
	private String turma;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = Sala.class )
	@JoinColumn( name = "SAL_ID" )
	private Sala sala;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = HorarioDisponivelCenario.class )
	@JoinColumn( name = "HDC_ID" )
	private HorarioDisponivelCenario HorarioDisponivelCenario;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = Oferta.class)
	@JoinColumn( name = "OFE_ID" )
	private Oferta oferta;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = Disciplina.class )
	@JoinColumn( name = "DIS_ID" )
	private Disciplina disciplina;
	
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = Disciplina.class )
	@JoinColumn( name = "DIS_SUBSTITUTA_ID" )
	private Disciplina disciplinaSubstituta;

	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = Professor.class )
	@JoinColumn( name = "PRF_ID" )
	private Professor professor;

	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
		CascadeType.REFRESH }, targetEntity = ProfessorVirtual.class )
	@JoinColumn( name = "PRV_ID" )
	private ProfessorVirtual professorVirtual;
	
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE},
		mappedBy = "atendimentosOperacional")
	private Set<AlunoDemanda> alunosDemanda = new HashSet<AlunoDemanda>();
	
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REMOVE})
	@JoinTable(name="ATENDIMENTO_OPERACIONAL_MOTIVOS_USO",
	joinColumns=@JoinColumn(name="ATP_ID"),
	inverseJoinColumns=@JoinColumn(name="MOT_PRV_ID"))
	private Set<MotivoUsoProfessorVirtual> motivosUsoProfessorVirtual = new HashSet<MotivoUsoProfessorVirtual>();
	
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REMOVE})
	@JoinTable(name="ATENDIMENTO_OPERACIONAL_DICAS_ELIMINACAO",
	joinColumns=@JoinColumn(name="ATP_ID"),
	inverseJoinColumns=@JoinColumn(name="DIC_PRV_ID"))
	private Set<DicaEliminacaoProfessorVirtual> dicasEliminacaoProfessorVirtual = new HashSet<DicaEliminacaoProfessorVirtual>();

	@Column( name = "ATP_CREDITOTEOTICO" )
	private Boolean creditoTeorico;

	@NotNull
	@Column( name = "ATP_QUANTIDADE" )
	@Min( 0L )
	@Max( 999L )
	private Integer quantidadeAlunos;
	
	@Column( name = "ATP_CONFIRMADA" )
	private Boolean confirmada;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return this.instituicaoEnsino;
	}

	public void setInstituicaoEnsino( InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
		sb.append( "Cenario: " ).append( ( getCenario() != null ) ?
			getCenario().getNome() : "null" ).append( ", " );
		sb.append( "Turma: " ).append( getTurma() ).append( ", " );
		sb.append( "Sala: " ).append( ( getSala() != null ) ? getSala().getCodigo() : "null" ).append( ", " );
		sb.append( "HorarioDisponivelCenario: " ).append( ( getHorarioDisponivelCenario() != null ) ?
			getHorarioDisponivelCenario().getHorarioAula().getHorario() : "null" ).append( ", " );
		sb.append( "Professor: " ).append( ( getProfessor() != null ) ? getProfessor().getNome() : "null" ).append( ", " );
		sb.append( "ProfessorVirtual: " ).append( ( getProfessorVirtual() != null ) ?
			getProfessorVirtual().getId() : "null" ).append( ", " );
		sb.append( "CreditoTeorico: " ).append( getCreditoTeorico() ).append( ", " );
		sb.append( "Oferta: " ).append( getOferta() ).append( ", " );
		sb.append( "Disciplina: " ).append( ( getDisciplina() != null ) ?
			getDisciplina().getCodigo()	: "null" ).append( ", " );
		sb.append( "DisciplinaSubstituta: " ).append( ( getDisciplinaSubstituta() != null ) ?
				getDisciplinaSubstituta().getCodigo()	: "null" ).append( ", " );
		sb.append( "QuantidadeAlunos: " ).append( getQuantidadeAlunos() );

		return sb.toString();
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "ATP_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;

	public Long getId()
	{
		return this.id;
	}

	public void setId( Long id )
	{
		this.id = id;
	}

	public Integer getVersion()
	{
		return this.version;
	}

	public void setVersion( Integer version )
	{
		this.version = version;
	}
	
	public Boolean getConfirmada()
	{
		return this.confirmada;
	}

	public void setConfirmada( Boolean confirmada )
	{
		this.confirmada = confirmada;
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
			AtendimentoOperacional attached = this.entityManager.find(
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
	public AtendimentoOperacional merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		AtendimentoOperacional merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new AtendimentoOperacional().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?) " );
		}

		return em;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Turno > findAllTurnosByCursos(
		InstituicaoEnsino instituicaoEnsino, List< Campus > campi )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT o.oferta.turno " +
			" FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus IN ( :campi ) " );

		q.setParameter( "campi", campi );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		List< Turno > list = q.getResultList();
		return list;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o )" +
			" FROM AtendimentoOperacional o " +
			" WHERE cenario = :cenario " +
			" AND o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Sala sala, SemanaLetiva semanaLetiva )
	{
		String semanaLetivaQuery = "";

		if ( semanaLetiva != null )
		{
			semanaLetivaQuery = " AND o.oferta.curriculo.semanaLetiva = :semanaLetiva ";
		}
		
		Query q = entityManager().createQuery(
			"SELECT DISTINCT ( o ) FROM AtendimentoOperacional o "
			+ " WHERE cenario = :cenario "
			+ " AND o.sala = :sala "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino "
			+ semanaLetivaQuery );

		q.setParameter( "cenario", cenario );
		q.setParameter( "sala", sala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( semanaLetiva != null )
		{
			q.setParameter( "semanaLetiva", semanaLetiva );
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			"SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			"WHERE o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		List< AtendimentoOperacional > list = q.getResultList();
		return list;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		InstituicaoEnsino instituicaoEnsino, Professor professor, Turno turno )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.turno = :turno " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professor = :professor " );

		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "professor", professor );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllPublicadoBy(
		Professor professor, Turno turno, boolean isAdmin, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino)
	{
		final boolean apenasCampusPublicado = ( !isAdmin && isVisaoProfessor ); 

		String publicado = "";

		if ( apenasCampusPublicado )
		{
			publicado = ( " AND o.oferta.campus.publicado = :publicado " );
		}
		
		String turnoQuery = "";
		
		if ( turno != null)
		{
			turnoQuery = " AND o.oferta.turno = :turno";
		}

		String queryString = "SELECT DISTINCT ( o ) FROM AtendimentoOperacional o"
			+ " WHERE o.instituicaoEnsino = :instituicaoEnsino"
			+ turnoQuery
			+ " AND o.professor = :professor " + publicado;

		Query q = entityManager().createQuery( queryString );

		q.setParameter( "professor", professor );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( apenasCampusPublicado )
		{
			q.setParameter( "publicado", true );
		}
		
		if ( turno != null )
		{
			q.setParameter( "turno", turno );
		}
		
		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllPublicadoBy(
		ProfessorVirtual professorVirtual, Turno turno,
		boolean isAdmin, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino)
	{
		final boolean apenasCampusPublicado = ( !isAdmin && isVisaoProfessor );

		String publicado = "";

		if ( apenasCampusPublicado )
		{
			publicado = " AND o.oferta.campus.publicado = :publicado ";
		}
		
		String turnoQuery = "";
		
		if ( turno != null)
		{
			turnoQuery = " AND o.oferta.turno = :turno";
		}

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino" +
			turnoQuery +
			" AND o.professorVirtual = :professorVirtual " + publicado);

		q.setParameter( "professorVirtual", professorVirtual );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( apenasCampusPublicado )
		{
			q.setParameter( "publicado", true );
		}
		
		if ( turno != null )
		{
			q.setParameter( "turno", turno );
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		Curso curso, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.curriculo.curso = :curso " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "curso", curso );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		AlunoDemanda alunoDemanda, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o, IN (o.alunosDemanda) ald " +
			" WHERE ald = :alunoDemanda " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "alunoDemanda", alunoDemanda );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		Oferta oferta, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta = :oferta " +
			" AND o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "oferta", oferta );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAll(
		Campus campus, Cenario cenario, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario ");

		q.setParameter( "campus", campus );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		Campus campus, Cenario cenario, ProfessorVirtual professorVirtual, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.professorVirtual = :professorVirtual " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario ");

		q.setParameter( "professorVirtual", professorVirtual );
		q.setParameter( "campus", campus );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllByDiasHorariosAula(
			InstituicaoEnsino instituicaoEnsino, HorarioDisponivelCenario horarioDisponivelCenario )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.HorarioDisponivelCenario = :horarioDisponivelCenario " +
			" AND o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "horarioDisponivelCenario", horarioDisponivelCenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		Campus campus, Turno turno, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino" +
			" AND o.oferta.turno = :turno" );

		q.setParameter( "campus", campus );
		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> findAllBy(Collection<Campus> campi, Collection<Turno> turnos, InstituicaoEnsino instituicaoEnsino) {
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o "
			+ " WHERE o.oferta.campus IN ( :campi ) "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino"
			+ " AND o.oferta.turno IN ( :turnos )"
		);

		q.setParameter("campi",campi);
		q.setParameter("turnos",turnos);
		q.setParameter("instituicaoEnsino",instituicaoEnsino);

		return q.getResultList();
	}

	public static AtendimentoOperacional find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		AtendimentoOperacional at = entityManager().find(
			AtendimentoOperacional.class, id );

		if ( at != null
			&& at.getInstituicaoEnsino() != null 
			&& at.getInstituicaoEnsino() == instituicaoEnsino )
		{
			return at;
		}

		return null;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findBySalaAndTurno(
		Sala sala, Turno turno, SemanaLetiva semanaLetiva,
		InstituicaoEnsino instituicaoEnsino )
	{
		String semanaLetivaQuery = "";

		if ( semanaLetiva != null )
		{
			semanaLetivaQuery = " AND o.oferta.curriculo.semanaLetiva = :semanaLetiva ";
		}
		
		String turnoQuery = "";
		
		if ( turno != null )
		{
			turnoQuery = " AND o.oferta.turno = :turno";
		}

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o "
			+ " WHERE o.sala = :sala "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino"
			+ turnoQuery
			+ semanaLetivaQuery );

		q.setParameter( "sala", sala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( semanaLetiva != null )
		{
			q.setParameter( "semanaLetiva", semanaLetiva );
		}
		
		if ( turno != null )
		{
			q.setParameter( "turno", turno );
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findBySalaAndTurno(
		Sala sala, Turno turno, SemanaLetiva semanaLetiva,
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		String semanaLetivaQuery = "";

		if ( semanaLetiva != null )
		{
			semanaLetivaQuery = " AND o.oferta.curriculo.semanaLetiva = :semanaLetiva ";
		}
		
		String turnoQuery = "";
		
		if ( turno != null )
		{
			turnoQuery = " AND o.oferta.turno = :turno";
		}

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o "
			+ " WHERE o.sala = :sala "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino"
			+ " AND o.cenario = :cenario "
			+ turnoQuery
			+ semanaLetivaQuery );

		q.setParameter( "sala", sala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		if ( semanaLetiva != null )
		{
			q.setParameter( "semanaLetiva", semanaLetiva );
		}
		
		if ( turno != null )
		{
			q.setParameter( "turno", turno );
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List<AtendimentoOperacional> findBy(
		Aluno aluno, Turno turno, Campus campus, InstituicaoEnsino instituicaoEnsino)
	{
		String turnoQuery = "";
		
		if ( turno != null )
		{
			turnoQuery = " AND o.oferta.turno = :turno";
		}
		
		String campusQuery = "";
		
		if ( campus != null )
		{
			campusQuery = " AND o.oferta.campus = :campus ";
		}
		

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o, "
			+ " IN (o.alunosDemanda) ald"
			+ " WHERE o.instituicaoEnsino = :instituicaoEnsino"
			+ turnoQuery
			+ campusQuery
			+ " AND ald.aluno = :aluno "
		);

		q.setParameter("aluno", aluno);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		
		if ( turno != null )
		{
			q.setParameter("turno", turno);
		}
		
		if ( campus != null )
		{
			q.setParameter("campus", campus);
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findBy(
		InstituicaoEnsino instituicaoEnsino, Campus campus,
		Curriculo curriculo, Integer periodo, Turno turno, Curso curso )
	{
		String cursoQuery = "";

		if ( curso != null )
		{
			cursoQuery = " AND o.oferta.curso = :curso ";
		}

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.curriculo = :curriculo " +
			" AND o.oferta.campus = :campus " + cursoQuery +
			" AND o.oferta.turno = :turno " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina IN ( SELECT d.disciplina FROM CurriculoDisciplina d " +
								  " WHERE d.curriculo = :curriculo AND d.periodo = :periodo ) " );

		q.setParameter( "campus", campus );
		q.setParameter( "curriculo", curriculo );
		q.setParameter( "periodo", periodo );
		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( curso != null )
		{
			q.setParameter( "curso", curso );
		}

		return q.getResultList();
	}

	public Cenario getCenario()
	{
		return this.cenario;
	}

	public void setCenario( Cenario cenario )
	{
		this.cenario = cenario;
	}

	public String getTurma()
	{
		return this.turma;
	}

	public void setTurma( String turma )
	{
		this.turma = turma;
	}

	public Sala getSala()
	{
		return this.sala;
	}

	public void setSala( Sala sala )
	{
		this.sala = sala;
	}

	public HorarioDisponivelCenario getHorarioDisponivelCenario()
	{
		return this.HorarioDisponivelCenario;
	}

	public void setHorarioDisponivelCenario(
		HorarioDisponivelCenario horarioDisponivelCenario )
	{
		this.HorarioDisponivelCenario = horarioDisponivelCenario;
	}

	public Professor getProfessor()
	{
		if ( this.professor == null )
		{
			return new Professor();
		}

		return this.professor;
	}

	public void setProfessor( Professor professor )
	{
		this.professor = professor;
	}

	public Boolean getCreditoTeorico()
	{
		return this.creditoTeorico;
	}

	public void setCreditoTeorico( Boolean creditoTeorico )
	{
		this.creditoTeorico = creditoTeorico;
	}

	public Oferta getOferta()
	{
		return this.oferta;
	}

	public void setOferta( Oferta oferta )
	{
		this.oferta = oferta;
	}

	public Disciplina getDisciplina()
	{
		return this.disciplina;
	}

	public void setDisciplina( Disciplina disciplina )
	{
		this.disciplina = disciplina;
	}
	
	public Disciplina getDisciplinaSubstituta()
	{
		return this.disciplinaSubstituta;
	}

	public void setDisciplinaSubstituta( Disciplina disciplinaSubstituta )
	{
		this.disciplinaSubstituta = disciplinaSubstituta;
	}

	public Integer getQuantidadeAlunos()
	{
		return this.quantidadeAlunos;
	}

	public void setQuantidadeAlunos( Integer quantidadeAlunos )
	{
		this.quantidadeAlunos = quantidadeAlunos;
	}

	public ProfessorVirtual getProfessorVirtual()
	{
		return this.professorVirtual;
	}

	public void setProfessorVirtual(
		ProfessorVirtual professorVirtual )
	{
		this.professorVirtual = professorVirtual;
	}
	
	public void setAlunosDemanda(Set<AlunoDemanda> alunosDemanda){
		this.alunosDemanda = alunosDemanda;
	}
	
	public Set<AlunoDemanda> getAlunosDemanda(){
		return this.alunosDemanda;
	}

	public String getNaturalKey()
	{
		Oferta oferta = getOferta();
		Curriculo curriculo = oferta.getCurriculo();

		Integer periodo = curriculo.getPeriodo(this.getDisciplina());

		return oferta.getCampus().getId()
			+ "-" + oferta.getTurno().getId()
			+ "-" + curriculo.getCurso().getId()
			+ "-" + curriculo.getId()
			+ "-" + periodo
			+ "-" + getDisciplina().getId()
			+ "-" + getTurma()
			+ "-" + getCreditoTeorico();
	}

	static public List< AtendimentoOperacional > getAtendimentosOperacional(
		InstituicaoEnsino instituicaoEnsino, boolean isAdmin,
		Professor professor, ProfessorVirtual professorVirtual,
		Turno turno, boolean isVisaoProfessor)
	{
		List< AtendimentoOperacional > atendimentosOperacional = null;

		if ( professor != null )
		{
			atendimentosOperacional = AtendimentoOperacional.findAllPublicadoBy(
				professor, turno, isAdmin, isVisaoProfessor, instituicaoEnsino);
		}
		else if ( professorVirtual != null )
		{
			atendimentosOperacional = AtendimentoOperacional.findAllPublicadoBy(
				professorVirtual, turno, isAdmin, isVisaoProfessor, instituicaoEnsino);
		}

		return atendimentosOperacional;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllByDemanda(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta = :oferta " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina = :disciplina " );

		q.setParameter( "oferta", demanda.getOferta() );
		q.setParameter( "disciplina", demanda.getDisciplina() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	public static int countTurma(
			InstituicaoEnsino instituicaoEnsino, Campus campus )
		{
			Query q = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino AND o.dis_substituta_id IS NULL" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.turma " +
				" UNION " +
				" SELECT o1.dis_substituta_id, o1.turma FROM atendimento_operacional o1 " +
				" WHERE o1.ins_id = :instituicaoEnsino  AND o1.dis_substituta_id IS NOT NULL " +
				" AND o1.ofe_id IN (select f1.ofe_id from ofertas f1 where f1.cam_id = :campus)" +
				" GROUP BY o1.dis_substituta_id, o1.turma ");

				q.setParameter( "campus", campus.getId() );
				q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );

				return q.getResultList().size();
		}

	public static int countTurmaProfessoresVirtuais(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createNativeQuery(
			" SELECT o.dis_id, o.turma FROM atendimento_operacional o " +
			" WHERE o.ins_id = :instituicaoEnsino AND o.dis_substituta_id IS NULL" +
			" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
			" AND o.prv_id is not null " +
			" GROUP BY o.dis_id, o.turma " +
			" UNION " +
			" SELECT o1.dis_substituta_id, o1.turma FROM atendimento_operacional o1 " +
			" WHERE o1.ins_id = :instituicaoEnsino  AND o1.dis_substituta_id IS NOT NULL " +
			" AND o1.ofe_id IN (select f1.ofe_id from ofertas f1 where f1.cam_id = :campus)" +
			" AND o1.prv_id is not null " +
			" GROUP BY o1.dis_substituta_id, o1.turma ");

			q.setParameter( "campus", campus.getId() );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );

			return q.getResultList().size();
	}
	
	public static int countTurmaProfessoresInstituicao(
			InstituicaoEnsino instituicaoEnsino, Campus campus )
		{
			Query q = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino AND o.dis_substituta_id IS NULL" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" AND o.prv_id is null " +
				" GROUP BY o.dis_id, o.turma " +
				" UNION " +
				" SELECT o1.dis_substituta_id, o1.turma FROM atendimento_operacional o1 " +
				" WHERE o1.ins_id = :instituicaoEnsino  AND o1.dis_substituta_id IS NOT NULL " +
				" AND o1.ofe_id IN (select f1.ofe_id from ofertas f1 where f1.cam_id = :campus)" +
				" AND o1.prv_id is null " +
				" GROUP BY o1.dis_substituta_id, o1.turma ");

				q.setParameter( "campus", campus.getId() );
				q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );

				return q.getResultList().size();
		}
	
	public static double countTurmaByDisciplinas(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		double countTurmas = 0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(atp_quantidade), o.hdc_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.dis_substituta_id, o.hdc_id, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> turmaIdToAlnCredAtendMap = new HashMap<String,Integer>();
		Map<String,Map<Long,Integer>> turmaIdToDisAlnCredAtendMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int totalAlunos = ((BigDecimal)((Object[])registro)[2]).intValue();
			String turma = ((String)((Object[])registro)[4]);
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String turmaId = disIdASerUsada + "-" + turma;
			
			Map<Long,Integer> disToAlnCredAtendMap = turmaIdToDisAlnCredAtendMap.get(turmaId);
			if (disToAlnCredAtendMap == null) {
				disToAlnCredAtendMap = new HashMap<Long, Integer>();
				turmaIdToDisAlnCredAtendMap.put(turmaId,disToAlnCredAtendMap);
			}
			Integer totalAlnCredAtend = disToAlnCredAtendMap.get(disId);
			totalAlnCredAtend = (totalAlnCredAtend == null) ? 0 : totalAlnCredAtend;
			disToAlnCredAtendMap.put(disId, totalAlnCredAtend + totalAlunos);
			
			totalAlnCredAtend = turmaIdToAlnCredAtendMap.get(turmaId);
			totalAlnCredAtend = (totalAlnCredAtend == null) ? 0 : totalAlnCredAtend;
			turmaIdToAlnCredAtendMap.put(turmaId, totalAlnCredAtend + totalAlunos);
		}
		
		Map<Long,Double> disIdToTotalTurmas = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : turmaIdToDisAlnCredAtendMap.entrySet()) {
			String turmaId = entry.getKey();
			int totalAlnCredAtendDaTurma = turmaIdToAlnCredAtendMap.get(turmaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int alnCredAtendDaDiscNaTurma = entry2.getValue();
				double parcelaTotalTurmasDaDisc = (double)alnCredAtendDaDiscNaTurma / (double)totalAlnCredAtendDaTurma;
				
				Double totalTurmasDaDisc = disIdToTotalTurmas.get(disId);
				totalTurmasDaDisc = (totalTurmasDaDisc == null) ? 0.0 : totalTurmasDaDisc;
				disIdToTotalTurmas.put(disId, totalTurmasDaDisc + parcelaTotalTurmasDaDisc);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalTurmas.entrySet()) {
			Long disId = entry.getKey();
			if (disciplinasIDs.contains(disId)) {
				countTurmas += entry.getValue();
			}
		}

		return countTurmas;
	}
	
	public static double calcReceita(InstituicaoEnsino instituicaoEnsino, Campus campus) {
		Query q = entityManager().createNativeQuery(
				" SELECT SUM(o.atp_quantidade*of.ofe_receita) FROM atendimento_operacional o, ofertas of " +
				" WHERE o.ins_id = :instituicaoEnsino AND o.ofe_id=of.ofe_id" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)");

		q.setParameter( "campus", campus.getId() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		double receita = 0.0;
		for (Object registro : q.getResultList()) {
			double receitaLocal = (Double)registro;
			receita += receitaLocal;
		}

		return receita * 4.5 * 6.0;
	}
	
	public static double calcReceita(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, SUM(atp_quantidade*of.ofe_receita) FROM atendimento_operacional o, ofertas of " +
				" WHERE o.ins_id = :instituicaoEnsino AND o.ofe_id=of.ofe_id" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		double receitaSemanalTotal = 0.0;
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			if (disciplinasIDs.contains(disId)) {
				double receitaSemanal = ((Double)((Object[])registro)[1]).doubleValue();
				receitaSemanalTotal += receitaSemanal;
			}
		}

		return receitaSemanalTotal * 4.5 * 6.0;
	}
	
	public static double countCreditos( InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		double countCreditos = 0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(atp_quantidade), o.hdc_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.dis_substituta_id, o.hdc_id, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> tempoAulaIdToTotalAlnMap = new HashMap<String,Integer>();
		Map<String,Map<Long,Integer>> tempoAulaIdToDisAlnMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int totalAlunos = ((BigDecimal)((Object[])registro)[2]).intValue();
			Long hdcId = ((BigInteger)((Object[])registro)[3]).longValue();
			String turma = ((String)((Object[])registro)[4]);
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String tempoAulaId = disIdASerUsada + "-" + hdcId + "-" + turma;
			
			Map<Long,Integer> disToAlnMap = tempoAulaIdToDisAlnMap.get(tempoAulaId);
			if (disToAlnMap == null) {
				disToAlnMap = new HashMap<Long, Integer>();
				tempoAulaIdToDisAlnMap.put(tempoAulaId,disToAlnMap);
			}
			disToAlnMap.put(disId,totalAlunos);
			
			Integer total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			total = (total == null) ? 0 : total;
			tempoAulaIdToTotalAlnMap.put(tempoAulaId, total + totalAlunos);
		}
		
		Map<Long,Double> disIdToTotalCred = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : tempoAulaIdToDisAlnMap.entrySet()) {
			String tempoAulaId = entry.getKey();
			int total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int parcialAlunos = entry2.getValue();
				double contribTotalAlunos = (double)parcialAlunos / (double)total;
				
				Double value = disIdToTotalCred.get(disId);
				value = (value == null) ? 0.0 : value;
				disIdToTotalCred.put(disId, value + contribTotalAlunos);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalCred.entrySet()) {
			countCreditos += entry.getValue();
		}

		return countCreditos;
	}
	
	public static double countCreditosProfessoresInstituicao( InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		double countCreditos = 0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(atp_quantidade), o.hdc_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" AND o.prf_id IS NOT NULL" + 
				" GROUP BY o.dis_id, o.dis_substituta_id, o.hdc_id, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> tempoAulaIdToTotalAlnMap = new HashMap<String,Integer>();
		Map<String,Map<Long,Integer>> tempoAulaIdToDisAlnMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int totalAlunos = ((BigDecimal)((Object[])registro)[2]).intValue();
			Long hdcId = ((BigInteger)((Object[])registro)[3]).longValue();
			String turma = ((String)((Object[])registro)[4]);
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String tempoAulaId = disIdASerUsada + "-" + hdcId + "-" + turma;
			
			Map<Long,Integer> disToAlnMap = tempoAulaIdToDisAlnMap.get(tempoAulaId);
			if (disToAlnMap == null) {
				disToAlnMap = new HashMap<Long, Integer>();
				tempoAulaIdToDisAlnMap.put(tempoAulaId,disToAlnMap);
			}
			disToAlnMap.put(disId,totalAlunos);
			
			Integer total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			total = (total == null) ? 0 : total;
			tempoAulaIdToTotalAlnMap.put(tempoAulaId, total + totalAlunos);
		}
		
		Map<Long,Double> disIdToTotalCred = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : tempoAulaIdToDisAlnMap.entrySet()) {
			String tempoAulaId = entry.getKey();
			int total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int parcialAlunos = entry2.getValue();
				double contribTotalAlunos = (double)parcialAlunos / (double)total;
				
				Double value = disIdToTotalCred.get(disId);
				value = (value == null) ? 0.0 : value;
				disIdToTotalCred.put(disId, value + contribTotalAlunos);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalCred.entrySet()) {
			countCreditos += entry.getValue();
		}

		return countCreditos;
	}
	
	public static double countCreditosProfessoresVirtuais( InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		double countCreditos = 0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(atp_quantidade), o.hdc_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" AND o.prf_id IS NULL" + 
				" GROUP BY o.dis_id, o.dis_substituta_id, o.hdc_id, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> tempoAulaIdToTotalAlnMap = new HashMap<String,Integer>();
		Map<String,Map<Long,Integer>> tempoAulaIdToDisAlnMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int totalAlunos = ((BigDecimal)((Object[])registro)[2]).intValue();
			Long hdcId = ((BigInteger)((Object[])registro)[3]).longValue();
			String turma = ((String)((Object[])registro)[4]);
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String tempoAulaId = disIdASerUsada + "-" + hdcId + "-" + turma;
			
			Map<Long,Integer> disToAlnMap = tempoAulaIdToDisAlnMap.get(tempoAulaId);
			if (disToAlnMap == null) {
				disToAlnMap = new HashMap<Long, Integer>();
				tempoAulaIdToDisAlnMap.put(tempoAulaId,disToAlnMap);
			}
			disToAlnMap.put(disId,totalAlunos);
			
			Integer total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			total = (total == null) ? 0 : total;
			tempoAulaIdToTotalAlnMap.put(tempoAulaId, total + totalAlunos);
		}
		
		Map<Long,Double> disIdToTotalCred = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : tempoAulaIdToDisAlnMap.entrySet()) {
			String tempoAulaId = entry.getKey();
			int total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int parcialAlunos = entry2.getValue();
				double contribTotalAlunos = (double)parcialAlunos / (double)total;
				
				Double value = disIdToTotalCred.get(disId);
				value = (value == null) ? 0.0 : value;
				disIdToTotalCred.put(disId, value + contribTotalAlunos);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalCred.entrySet()) {
			countCreditos += entry.getValue();
		}

		return countCreditos;
	}
	
	public static double countCreditosByTurmas(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		double countCreditos = 0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(atp_quantidade), o.hdc_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.dis_substituta_id, o.hdc_id, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> tempoAulaIdToTotalAlnMap = new HashMap<String,Integer>();
		Map<String,Map<Long,Integer>> tempoAulaIdToDisAlnMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int totalAlunos = ((BigDecimal)((Object[])registro)[2]).intValue();
			Long hdcId = ((BigInteger)((Object[])registro)[3]).longValue();
			String turma = ((String)((Object[])registro)[4]);
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String tempoAulaId = disIdASerUsada + "-" + hdcId + "-" + turma;
			
			Map<Long,Integer> disToAlnMap = tempoAulaIdToDisAlnMap.get(tempoAulaId);
			if (disToAlnMap == null) {
				disToAlnMap = new HashMap<Long, Integer>();
				tempoAulaIdToDisAlnMap.put(tempoAulaId,disToAlnMap);
			}
			disToAlnMap.put(disId,totalAlunos);
			
			Integer total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			total = (total == null) ? 0 : total;
			tempoAulaIdToTotalAlnMap.put(tempoAulaId, total + totalAlunos);
		}
		
		Map<Long,Double> disIdToTotalCred = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : tempoAulaIdToDisAlnMap.entrySet()) {
			String tempoAulaId = entry.getKey();
			int total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int parcialAlunos = entry2.getValue();
				double contribTotalAlunos = (double)parcialAlunos / (double)total;
				
				Double value = disIdToTotalCred.get(disId);
				value = (value == null) ? 0.0 : value;
				disIdToTotalCred.put(disId, value + contribTotalAlunos);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalCred.entrySet()) {
			Long disId = entry.getKey();
			if (disciplinasIDs.contains(disId)) {
				countCreditos += entry.getValue();
			}
		}

		return countCreditos;
	}
	
	public static double calcCustoDocente(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		Map<Long,Double> profIdToCustoCredito = new HashMap<Long,Double>();
		Query q1 = entityManager().createNativeQuery(
				" SELECT p.prf_id, p.prf_valor_credito FROM professores p, titulacoes t " +
				" WHERE p.tit_id = t.tit_id AND t.ins_id = :instituicaoEnsino");
		q1.setParameter( "instituicaoEnsino", instituicaoEnsino.getId());
		for (Object registro : q1.getResultList()) {
			Long prfId = ((BigInteger)((Object[])registro)[0]).longValue();
			double custoCredito = ((Double)((Object[])registro)[1]);
			profIdToCustoCredito.put(prfId,custoCredito);
		}
		
		double custoDocente = 0.0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(atp_quantidade), o.hdc_id, o.turma, prf_id FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.dis_substituta_id, o.hdc_id, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> tempoAulaIdToTotalAlnCredAtendMap = new HashMap<String,Integer>();
		Map<String,Double> tempoAulaIdToCustoCreditoMap = new HashMap<String,Double>();
		Map<String,Map<Long,Integer>> tempoAulaIdToDisAlnCredAtendMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int alnCredAtend = ((BigDecimal)((Object[])registro)[2]).intValue();
			Long hdcId = ((BigInteger)((Object[])registro)[3]).longValue();
			String turma = ((String)((Object[])registro)[4]);
			Long prfId = (((Object[])registro)[5]) == null ? null : ((BigInteger)((Object[])registro)[5]).longValue();
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String tempoAulaId = disIdASerUsada + "-" + hdcId + "-" + turma;
			
			Map<Long,Integer> disToAlnCredAtendMap = tempoAulaIdToDisAlnCredAtendMap.get(tempoAulaId);
			if (disToAlnCredAtendMap == null) {
				disToAlnCredAtendMap = new HashMap<Long, Integer>();
				tempoAulaIdToDisAlnCredAtendMap.put(tempoAulaId,disToAlnCredAtendMap);
			}
			disToAlnCredAtendMap.put(disId,alnCredAtend);
			
			Integer totalAlnCredAtendDoTempoAula = tempoAulaIdToTotalAlnCredAtendMap.get(tempoAulaId);
			totalAlnCredAtendDoTempoAula = (totalAlnCredAtendDoTempoAula == null) ? 0 : totalAlnCredAtendDoTempoAula;
			tempoAulaIdToTotalAlnCredAtendMap.put(tempoAulaId, totalAlnCredAtendDoTempoAula + alnCredAtend);
			
			double custoCredito = 0.0;
			if (prfId != null) {
				custoCredito = profIdToCustoCredito.get(prfId);
			} else {
				custoCredito = campus.getValorCredito();
			}
			tempoAulaIdToCustoCreditoMap.put(tempoAulaId,custoCredito);
		}
		
		Map<Long,Double> disIdToTotalCustoDocente = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : tempoAulaIdToDisAlnCredAtendMap.entrySet()) {
			String tempoAulaId = entry.getKey();
			double custoCredito = tempoAulaIdToCustoCreditoMap.get(tempoAulaId);
			int totalAlnCredAtendDoTempoAula = tempoAulaIdToTotalAlnCredAtendMap.get(tempoAulaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int alnCredAtendDaDiscNoTempoAula = entry2.getValue();
				double contribCustoDocente = ((double)alnCredAtendDaDiscNoTempoAula / (double)totalAlnCredAtendDoTempoAula) * custoCredito;
				
				Double custoDocenteTotalDaDisc = disIdToTotalCustoDocente.get(disId);
				custoDocenteTotalDaDisc = (custoDocenteTotalDaDisc == null) ? 0.0 : custoDocenteTotalDaDisc;
				disIdToTotalCustoDocente.put(disId, custoDocenteTotalDaDisc + contribCustoDocente);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalCustoDocente.entrySet()) {
			Long disId = entry.getKey();
			if (disciplinasIDs.contains(disId)) {
				custoDocente += entry.getValue();
			}
		}

		return custoDocente * 4.5 * 6.0;
	}
	
	public static double calcCustoDocente(
			InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Map<Long,Double> profIdToCustoCredito = new HashMap<Long,Double>();
		Query q1 = entityManager().createNativeQuery(
				" SELECT p.prf_id, p.prf_valor_credito FROM professores p, titulacoes t " +
				" WHERE p.tit_id = t.tit_id AND t.ins_id = :instituicaoEnsino");
		q1.setParameter( "instituicaoEnsino", instituicaoEnsino.getId());
		for (Object registro : q1.getResultList()) {
			Long prfId = ((BigInteger)((Object[])registro)[0]).longValue();
			double custoCredito = ((Double)((Object[])registro)[1]);
			profIdToCustoCredito.put(prfId,custoCredito);
		}
		
		double custoDocente = 0.0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(atp_quantidade), o.hdc_id, o.turma, prf_id FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.dis_substituta_id, o.hdc_id, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> tempoAulaIdToTotalAlnCredAtendMap = new HashMap<String,Integer>();
		Map<String,Double> tempoAulaIdToCustoCreditoMap = new HashMap<String,Double>();
		Map<String,Map<Long,Integer>> tempoAulaIdToDisAlnCredAtendMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int alnCredAtend = ((BigDecimal)((Object[])registro)[2]).intValue();
			Long hdcId = ((BigInteger)((Object[])registro)[3]).longValue();
			String turma = ((String)((Object[])registro)[4]);
			Long prfId = (((Object[])registro)[5]) == null ? null : ((BigInteger)((Object[])registro)[5]).longValue();
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String tempoAulaId = disIdASerUsada + "-" + hdcId + "-" + turma;
			
			Map<Long,Integer> disToAlnCredAtendMap = tempoAulaIdToDisAlnCredAtendMap.get(tempoAulaId);
			if (disToAlnCredAtendMap == null) {
				disToAlnCredAtendMap = new HashMap<Long, Integer>();
				tempoAulaIdToDisAlnCredAtendMap.put(tempoAulaId,disToAlnCredAtendMap);
			}
			disToAlnCredAtendMap.put(disId,alnCredAtend);
			
			Integer totalAlnCredAtendDoTempoAula = tempoAulaIdToTotalAlnCredAtendMap.get(tempoAulaId);
			totalAlnCredAtendDoTempoAula = (totalAlnCredAtendDoTempoAula == null) ? 0 : totalAlnCredAtendDoTempoAula;
			tempoAulaIdToTotalAlnCredAtendMap.put(tempoAulaId, totalAlnCredAtendDoTempoAula + alnCredAtend);
			
			double custoCredito = 0.0;
			if (prfId != null) {
				custoCredito = profIdToCustoCredito.get(prfId);
			} else {
				custoCredito = campus.getValorCredito();
			}
			tempoAulaIdToCustoCreditoMap.put(tempoAulaId,custoCredito);
		}
		
		Map<Long,Double> disIdToTotalCustoDocente = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : tempoAulaIdToDisAlnCredAtendMap.entrySet()) {
			String tempoAulaId = entry.getKey();
			double custoCredito = tempoAulaIdToCustoCreditoMap.get(tempoAulaId);
			int totalAlnCredAtendDoTempoAula = tempoAulaIdToTotalAlnCredAtendMap.get(tempoAulaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int alnCredAtendDaDiscNoTempoAula = entry2.getValue();
				double contribCustoDocente = ((double)alnCredAtendDaDiscNoTempoAula / (double)totalAlnCredAtendDoTempoAula) * custoCredito;
				
				Double custoDocenteTotalDaDisc = disIdToTotalCustoDocente.get(disId);
				custoDocenteTotalDaDisc = (custoDocenteTotalDaDisc == null) ? 0.0 : custoDocenteTotalDaDisc;
				disIdToTotalCustoDocente.put(disId, custoDocenteTotalDaDisc + contribCustoDocente);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalCustoDocente.entrySet()) {
			custoDocente += entry.getValue();
		}

		return custoDocente * 4.5 * 6.0;
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllByCampi(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, List< Campus > campi )
	{
		if (campi.isEmpty()) {
			return new ArrayList< AtendimentoOperacional >();
		}
		
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus IN (:campi) " +
			" AND o.cenario = :cenario " +
			" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "campi", campi );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof AtendimentoOperacional ) )
		{
			return false;
		}

		AtendimentoOperacional other = ( AtendimentoOperacional ) obj;

		boolean validaTurma = ( this.getTurma().equals( other.getTurma() ) );
		boolean validaSala = ( this.getSala().equals( other.getSala() ) ); 

		boolean validaHdc = ( this.getHorarioDisponivelCenario().getHorarioAula().equals(
			other.getHorarioDisponivelCenario().getHorarioAula() ) );
		validaHdc = ( validaHdc && this.getHorarioDisponivelCenario().getDiaSemana().equals(
			other.getHorarioDisponivelCenario().getDiaSemana() ) ); 

		boolean validaOferta = ( this.getOferta().equals( other.getOferta() ) );
		boolean validaDisciplina = ( this.getDisciplina().equals( other.getDisciplina() ) );

		boolean validaProfessor = true;
		if ( this.getProfessor() != null )
		{
			validaProfessor = ( this.getProfessor().equals( other.getProfessor() ) );
		}

		boolean validaProfessorVirtual = true;
		if ( this.getProfessorVirtual() != null )
		{
			validaProfessorVirtual = ( this.getProfessorVirtual().equals( other.getProfessorVirtual() ) );
		}

		boolean validaCreditoTeorico = ( this.getCreditoTeorico().equals( other.getCreditoTeorico() ) );

		boolean validaInstituicaoEnsino = ( this.getInstituicaoEnsino().equals( other.getInstituicaoEnsino() ) );

		return ( validaTurma && validaSala && validaHdc && validaOferta
			&& validaDisciplina && validaProfessor &&  validaProfessorVirtual
			&&  validaCreditoTeorico && validaInstituicaoEnsino );
	}

	public static HorarioAula retornaAtendimentoMenorHorarioAula(
		List< AtendimentoOperacional > listAtendimentos )
	{
		if ( listAtendimentos == null || listAtendimentos.size() == 0 )
		{
			return null;
		}

		if ( listAtendimentos.size() == 1 )
		{
			return listAtendimentos.get( 0 ).getHorarioDisponivelCenario().getHorarioAula();
		}

		// Procura pelo horário correspondente ao início da aula
		HorarioAula menorHorario = listAtendimentos.get( 0 ).getHorarioDisponivelCenario().getHorarioAula();

		for ( int i = 1; i < listAtendimentos.size(); i++ )
		{
			HorarioAula h = listAtendimentos.get( i ).getHorarioDisponivelCenario().getHorarioAula();

			if ( h.getHorario().compareTo( menorHorario.getHorario() ) < 0 )
			{
				menorHorario = h;
			}
		}

		return menorHorario;
	}
	
	public static int countProfessores(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o.professor ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus " +
			" AND o.professor IS NOT NULL " );

			q.setParameter( "campus", campus );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			return q.getResultList().size();
	}
	
	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> getAtendimentosByOferta(
			InstituicaoEnsino instituicaoEnsino, Oferta oferta )
		{
			Query q = entityManager().createQuery(
				" SELECT o FROM AtendimentoOperacional o " +
				" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.oferta = :oferta " );

				q.setParameter( "oferta", oferta );
				q.setParameter( "instituicaoEnsino", instituicaoEnsino );

				return q.getResultList();
		}
	
	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> getAtendimentosOperacionaisByCampus(
			InstituicaoEnsino instituicaoEnsino, Campus campus)
		{
			Query q = entityManager().createQuery(
				" SELECT o FROM AtendimentoOperacional o " +
				" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.oferta.campus = :campus " );

				q.setParameter( "campus", campus );
				q.setParameter( "instituicaoEnsino", instituicaoEnsino );

				return q.getResultList();
	}
	
	public static int countProfessores(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario )
		{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o.professor ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND o.professor IS NOT NULL " );

			q.setParameter( "cenario", cenario );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			return q.getResultList().size();
		}
	
	public static int countProfessoresVirtuais(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o.professorVirtual ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus " +
			" AND o.professorVirtual IS NOT NULL " );

			q.setParameter( "campus", campus );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			return q.getResultList().size();
	}

	public static int countProfessoresVirtuais(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario )
		{
			Query q = entityManager().createQuery(
				" SELECT DISTINCT ( o.professorVirtual ) FROM AtendimentoOperacional o " +
				" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				" AND o.professorVirtual IS NOT NULL " );

				q.setParameter( "cenario", cenario );
				q.setParameter( "instituicaoEnsino", instituicaoEnsino );

				return q.getResultList().size();
		}
	
	@SuppressWarnings( "unchecked" )
	public static List< Object > findAllAlunosCargaHorariaOperacional(
		InstituicaoEnsino instituicaoEnsino )
	{
		
		Query q = entityManager().createQuery(
				" SELECT a.aluno.matricula, ao.HorarioDisponivelCenario.diaSemana, " +
				" COUNT(*)*ao.HorarioDisponivelCenario.horarioAula.semanaLetiva.tempo " +
				" FROM AlunoDemanda a JOIN a.atendimentosOperacional ao " +
				" WHERE ao.instituicaoEnsino = :instituicaoEnsino" +
				" GROUP BY a.aluno, ao.HorarioDisponivelCenario.diaSemana" );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}


	@SuppressWarnings("unchecked")
	public static List<Professor> findProfessoresUtilizados(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Campus campus, Curso curso, Turno turno, Titulacao titulacao,
			AreaTitulacao areaTitulacao, TipoContrato tipoContrato, String cpf,
			Integer firstResult, Integer maxResults, String orderBy) {
		
		String cursoQuery = curso == null ? "" : " AND o.oferta.curso = :curso ";
		String turnoQuery = turno == null ? "" : " AND o.oferta.turno = :turno ";
		String titulacaoQuery = titulacao == null ? "" : " AND o.professor.titulacao = :titulacao ";
		String areaTitulacaoQuery = areaTitulacao == null ? "" : " AND o.professor.areaTitulacao = :areaTitulacao ";
		String tipoContratoQuery = tipoContrato == null ? "" : " AND o.professor.tipoContrato = :tipoContrato ";
		String cpfQuery = cpf == null ? "" : " AND o.professor.cpf LIKE LOWER (:cpf)";
		if ( orderBy != null )
		{
			if( orderBy.contains("notaDesempenho") )
				orderBy = orderBy.replace("notaDesempenho", "ORDER BY AVG(d.nota)");
			else
				orderBy = " ORDER BY o.professor." + orderBy.replace("String", "");
		}
		else
		{
			orderBy = "";
		}

		Query q = entityManager().createQuery(
			" SELECT o.professor FROM AtendimentoOperacional o " +
			" LEFT JOIN o.professor.disciplinas d LEFT JOIN FETCH o.professor.atendimentosOperacionais " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			turnoQuery + cursoQuery + titulacaoQuery +
			areaTitulacaoQuery + tipoContratoQuery +
			cpfQuery +
			" AND o.professor IS NOT NULL " +
			" GROUP BY o.professor " +
			orderBy);

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		if (firstResult != null)
			q.setFirstResult( firstResult );
		if (maxResults != null)
			q.setMaxResults( maxResults );
		if (curso != null)
		{
			q.setParameter( "curso", curso );
		}
		if (turno != null)
		{
			q.setParameter( "turno", turno );
		}
		if (titulacao != null)
		{
			q.setParameter( "titulacao", titulacao );
		}
		if (areaTitulacao != null)
		{
			q.setParameter( "areaTitulacao", areaTitulacao );
		}
		if (tipoContrato != null)
		{
			q.setParameter( "tipoContrato", tipoContrato );
		}
		if (cpf != null)
		{
			q.setParameter( "cpf", cpf );
		}

		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List<Professor> findProfessoresUtilizados(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Campus campus, Curso curso, Turno turno, Titulacao titulacao,
			AreaTitulacao areaTitulacao, TipoContrato tipoContrato, String cpf) {
		
		String cursoQuery = curso == null ? "" : " AND o.oferta.curso = :curso ";
		String turnoQuery = turno == null ? "" : " AND o.oferta.turno = :turno ";
		String titulacaoQuery = titulacao == null ? "" : " AND o.professor.titulacao = :titulacao ";
		String areaTitulacaoQuery = areaTitulacao == null ? "" : " AND o.professor.areaTitulacao = :areaTitulacao ";
		String tipoContratoQuery = tipoContrato == null ? "" : " AND o.professor.tipoContrato = :tipoContrato ";
		String cpfQuery = cpf == null ? "" : " AND o.professor.cpf LIKE LOWER (:cpf)";

		Query q = entityManager().createQuery(
			" SELECT DISTINCT (o.professor) FROM AtendimentoOperacional o LEFT JOIN FETCH o.professor.atendimentosOperacionais " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			turnoQuery + cursoQuery + titulacaoQuery +
			areaTitulacaoQuery + tipoContratoQuery +
			cpfQuery +
			" AND o.professor IS NOT NULL " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		if (curso != null)
		{
			q.setParameter( "curso", curso );
		}
		if (turno != null)
		{
			q.setParameter( "turno", turno );
		}
		if (titulacao != null)
		{
			q.setParameter( "titulacao", titulacao );
		}
		if (areaTitulacao != null)
		{
			q.setParameter( "areaTitulacao", areaTitulacao );
		}
		if (tipoContrato != null)
		{
			q.setParameter( "tipoContrato", tipoContrato );
		}
		if (cpf != null)
		{
			q.setParameter( "cpf", cpf );
		}

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<ProfessorVirtual> findProfessoresVirtuaisUtilizados(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Campus campus, Curso curso, Turno turno, Titulacao titulacao,
			AreaTitulacao areaTitulacao) {

		String cursoQuery = curso == null ? "" : " AND o.oferta.curso = :curso ";
		String turnoQuery = turno == null ? "" : " AND o.oferta.turno = :turno ";
		String titulacaoQuery = titulacao == null ? "" : " AND o.professorVirtual.titulacao = :titulacao ";
		String areaTitulacaoQuery = areaTitulacao == null ? "" : " AND o.professorVirtual.areaTitulacao = :areaTitulacao ";
		
		Query q = entityManager().createQuery(
			" SELECT DISTINCT (o.professorVirtual) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			turnoQuery + cursoQuery + titulacaoQuery + areaTitulacaoQuery +
			" AND o.professor IS NULL ");

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		if (curso != null)
		{
			q.setParameter( "curso", curso );
		}
		if (turno != null)
		{
			q.setParameter( "turno", turno );
		}
		if (titulacao != null)
		{
			q.setParameter( "titulacao", titulacao );
		}
		if (areaTitulacao != null)
		{
			q.setParameter( "areaTitulacao", areaTitulacao );
		}

		return q.getResultList();
	}
	
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		Campus campus, Cenario cenario, InstituicaoEnsino instituicaoEnsino,
		Curso curso, Turno turno, Titulacao titulacao,
		AreaTitulacao areaTitulacao, TipoContrato tipoContrato)
	{
		String campusQuery = campus == null ? "" : " AND o.oferta.campus = :campus ";
		String cursoQuery = curso == null ? "" : " AND o.oferta.curso = :curso ";
		String turnoQuery = turno == null ? "" : " AND o.oferta.turno = :turno ";
		
		String titulacaoProfessorQuery = titulacao == null ? "" : " AND o.professor.titulacao = :titulacao ";
		String areaTitulacaoProfessorQuery = areaTitulacao == null ? "" : " AND o.professor.areaTitulacao = :areaTitulacao ";
		String tipoContratoProfessorQuery = tipoContrato == null ? "" : " AND o.professor.tipoContrato = :tipoContrato ";
		
		String titulacaoVirtualQuery = titulacao == null ? "" : " AND o.professorVirtual.titulacao = :titulacao ";
		String areaTitulacaoVirtualQuery = areaTitulacao == null ? "" : " AND o.professorVirtual.areaTitulacao = :areaTitulacao ";
		
		Query q1 = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " + campusQuery +
			turnoQuery + cursoQuery + titulacaoProfessorQuery +
			areaTitulacaoProfessorQuery + tipoContratoProfessorQuery +
			" AND o.professor IS NOT NULL ");

		Query q2 = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " + campusQuery +
			turnoQuery + cursoQuery + titulacaoVirtualQuery +
			areaTitulacaoVirtualQuery +
			" AND o.professor IS NULL ");
		
		q1.setParameter( "cenario", cenario );
		q1.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q2.setParameter( "cenario", cenario );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino );
		if( campus != null )
		{
			q1.setParameter( "campus", campus );
			q2.setParameter( "campus", campus );
		}
		if (curso != null)
		{
			q1.setParameter( "curso", curso );
			q2.setParameter( "curso", curso );
		}
		if (turno != null)
		{
			q1.setParameter( "turno", turno );
			q2.setParameter( "turno", turno );
		}
		if (titulacao != null)
		{
			q1.setParameter( "titulacao", titulacao );
			q2.setParameter( "titulacao", titulacao );
		}
		if (areaTitulacao != null)
		{
			q1.setParameter( "areaTitulacao", areaTitulacao );
			q2.setParameter( "areaTitulacao", areaTitulacao );
		}
		if (tipoContrato != null)
		{
			q1.setParameter( "tipoContrato", tipoContrato );
		}

		List<AtendimentoOperacional> result = new ArrayList<AtendimentoOperacional>();
		result.addAll(q1.getResultList());
		result.addAll(q2.getResultList());
		
		return result;
	}
	
	public static Integer sumCredAlunosAtendidos(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario, Campus campus )
		{
			
			List<TipoDisciplina> tiposDisciplinas = TipoDisciplina.findByCenario(instituicaoEnsino, cenario);
			List<TipoDisciplina> tiposDisciplinasPresenciais = new ArrayList<TipoDisciplina>(tiposDisciplinas.size());
			for (TipoDisciplina td : tiposDisciplinas) {
				if (td.ocupaGrade()) {
					tiposDisciplinasPresenciais.add(td);
				}
			}

			Query q = entityManager().createQuery(
					" SELECT COUNT(o) " +
					" FROM AtendimentoOperacional o INNER JOIN o.alunosDemanda a " +
					" WHERE o.oferta.campus = :campus " +
					" AND o.instituicaoEnsino = :instituicaoEnsino " +
					" AND o.disciplina.tipoDisciplina IN (:tiposDisciplinasPresenciais) " );

			q.setParameter("instituicaoEnsino",instituicaoEnsino);
			q.setParameter("campus",campus);
			q.setParameter("tiposDisciplinasPresenciais",tiposDisciplinasPresenciais);

			Object rs = q.getSingleResult();
			return ( rs == null ? 0 : ( (Number) rs ).intValue() );
		}

	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> findBy(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, Disciplina disciplina, String turma, Boolean creditoTeorico) 
	{
		String creditoTeoricoString = creditoTeorico == null ? "" : " AND o.creditoTeorico = :creditoTeorico";
		
		Query q1 = entityManager().createQuery(
				" SELECT o FROM AtendimentoOperacional o " +
				" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				" AND o.turma = :turma" +
				" AND o.disciplina = :disciplina " +
				" AND o.disciplinaSubstituta IS NULL " +
				creditoTeoricoString);

		Query q2 = entityManager().createQuery(
				" SELECT o FROM AtendimentoOperacional o " +
				" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				" AND o.turma = :turma" +
				" AND o.disciplinaSubstituta = :disciplina " +
				" AND o.disciplinaSubstituta IS NOT NULL " +
				creditoTeoricoString);

		q1.setParameter( "cenario", cenario );
		q1.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q1.setParameter( "disciplina", disciplina );
		q1.setParameter( "turma", turma );
		q2.setParameter( "cenario", cenario );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q2.setParameter( "disciplina", disciplina );
		q2.setParameter( "turma", turma );
		if (creditoTeorico != null)
		{
			q1.setParameter( "creditoTeorico", creditoTeorico );
			q2.setParameter( "creditoTeorico", creditoTeorico );
		}
		
		List<AtendimentoOperacional> result = new ArrayList<AtendimentoOperacional>();
		result.addAll(q1.getResultList());
		result.addAll(q2.getResultList());
		
		return result;
	}
	
	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> findBy(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, Disciplina disciplina, Campus campus, Oferta oferta, String turma) 
	{
		String ofertaString = "";
		if (oferta != null)
		{
			ofertaString = " AND o.oferta = :oferta ";
		}
		
		Query q1 = entityManager().createQuery(
				" SELECT o FROM AtendimentoOperacional o " +
				" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				" AND o.oferta.campus = :campus " +
				" AND o.turma = :turma" +
				ofertaString +
				" AND o.disciplina = :disciplina " +
				" AND o.disciplinaSubstituta IS NULL ");

		Query q2 = entityManager().createQuery(
				" SELECT o FROM AtendimentoOperacional o " +
				" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				" AND o.oferta.campus = :campus " +
				" AND o.turma = :turma" +
				ofertaString +
				" AND o.disciplinaSubstituta = :disciplina " +
				" AND o.disciplinaSubstituta IS NOT NULL ");

		q1.setParameter( "cenario", cenario );
		q1.setParameter( "campus", campus );
		q1.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q1.setParameter( "disciplina", disciplina );
		q1.setParameter( "turma", turma );
		q2.setParameter( "cenario", cenario );
		q2.setParameter( "campus", campus );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q2.setParameter( "disciplina", disciplina );
		q2.setParameter( "turma", turma );
		if (oferta != null)
		{
			q1.setParameter( "oferta", oferta );
			q2.setParameter( "oferta", oferta );
		}
		
		List<AtendimentoOperacional> result = new ArrayList<AtendimentoOperacional>();
		result.addAll(q1.getResultList());
		result.addAll(q2.getResultList());
		
		return result;
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, Professor professor )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.cenario = :cenario " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professor = :professor " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "professor", professor );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, ProfessorVirtual professorVirtual )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.cenario = :cenario " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professorVirtual = :professorVirtual " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "professorVirtual", professorVirtual );

		return q.getResultList();
	}

	public Set<MotivoUsoProfessorVirtual> getMotivoUsoProfessorVirtual() {
		return motivosUsoProfessorVirtual;
	}

	public void setMotivoUsoProfessorVirtual(Set<MotivoUsoProfessorVirtual> motivosUsoProfessorVirtual) {
		this.motivosUsoProfessorVirtual = motivosUsoProfessorVirtual;
	}

	public Set<DicaEliminacaoProfessorVirtual> getDicasEliminacaoProfessorVirtual() {
		return dicasEliminacaoProfessorVirtual;
	}

	public void setDicasEliminacaoProfessorVirtual(Set<DicaEliminacaoProfessorVirtual> dicasEliminacaoProfessorVirtual) {
		this.dicasEliminacaoProfessorVirtual = dicasEliminacaoProfessorVirtual;
	}

	public static List<MotivoUsoProfessorVirtual> findMotivosUsoProfessorVirtual(
		InstituicaoEnsino instituicaoEnsinoUser, Cenario cenario2,
		Long disciplinaId, Integer turma2, Boolean credTeorico) 
	{
		return null;
	}
}
