package com.gapso.trieda.domain;

import java.io.Serializable;
import java.math.BigInteger;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
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

	@Column( name = "ATP_CREDITOTEOTICO" )
	private Boolean creditoTeorico;

	@NotNull
	@Column( name = "ATP_QUANTIDADE" )
	@Min( 0L )
	@Max( 999L )
	private Integer quantidadeAlunos;

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
			if ( this.professorVirtual != null )
			{
				this.professorVirtual.remove();
			}

			this.entityManager.remove( this );
		}
		else
		{
			AtendimentoOperacional attached = this.entityManager.find(
					this.getClass(), this.id );

			if ( attached != null )
			{
				if ( attached.professorVirtual != null )
				{
					attached.professorVirtual.remove();
				}

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
		Campus campus, Unidade unidade, Sala sala,
		Turno turno, SemanaLetiva semanaLetiva )
	{
		String semanaLetivaQuery = "";

		if ( semanaLetiva != null )
		{
			semanaLetivaQuery = " AND o.oferta.curriculo.semanaLetiva = :semanaLetiva ";
		}
		
		Query q = entityManager().createQuery(
			"SELECT DISTINCT ( o ) FROM AtendimentoOperacional o "
			+ " WHERE cenario = :cenario "
			+ " AND o.oferta.turno = :turno "
			+ " AND o.oferta.campus = :campus "
			+ " AND o.sala = :sala "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino "
			+ " AND o.sala.unidade = :unidade "
			+ semanaLetivaQuery );

		q.setParameter( "cenario", cenario );
		q.setParameter( "campus", campus );
		q.setParameter( "unidade", unidade );
		q.setParameter( "sala", sala );
		q.setParameter( "turno", turno );
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

		String queryString = "SELECT DISTINCT ( o ) FROM AtendimentoOperacional o"
			+ " WHERE o.oferta.turno = :turno "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino "
			+ " AND o.professor = :professor " + publicado;

		Query q = entityManager().createQuery( queryString );

		q.setParameter( "turno", turno );
		q.setParameter( "professor", professor );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( apenasCampusPublicado )
		{
			q.setParameter( "publicado", true );
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

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.turno = :turno " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professorVirtual = :professorVirtual " + publicado);

		q.setParameter( "turno", turno );
		q.setParameter( "professorVirtual", professorVirtual );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( apenasCampusPublicado )
		{
			q.setParameter( "publicado", true );
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
		Campus campus, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "campus", campus );
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
	public static List<AtendimentoOperacional> findAllBy(Collection<Campus> campi, Turno turno, InstituicaoEnsino instituicaoEnsino) {
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o "
			+ " WHERE o.oferta.campus IN ( :campi ) "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino"
			+ " AND o.oferta.turno = :turno"
		);

		q.setParameter("campi",campi);
		q.setParameter("turno",turno);
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

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o "
			+ " WHERE o.sala = :sala "
			+ " AND o.instituicaoEnsino = :instituicaoEnsino"
			+ " AND o.oferta.turno = :turno"
			+ semanaLetivaQuery );

		q.setParameter( "sala", sala );
		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( semanaLetiva != null )
		{
			q.setParameter( "semanaLetiva", semanaLetiva );
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List<AtendimentoOperacional> findBy(
		Aluno aluno, Turno turno, Campus campus, InstituicaoEnsino instituicaoEnsino)
	{

		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o, "
			+ " IN (o.alunosDemanda) ald"
			+ " WHERE o.instituicaoEnsino = :instituicaoEnsino"
			+ " AND o.oferta.turno = :turno"
			+ " AND o.oferta.campus = :campus "
			+ " AND ald.aluno = :aluno "
		);

		q.setParameter("aluno", aluno);
		q.setParameter("turno", turno);
		q.setParameter("campus", campus);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);

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
	
	public static int countTurmaByDisciplinas(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas)
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
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
		
		int countTurmas = 0;
		for (Object registro : q.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			if (disciplinasIDs.contains(disId)) {
				countTurmas++;
			}
		}

		return countTurmas;
	}
	
	public static double calcReceita(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		Query q = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.hdc_id, o.turma, SUM(o.atp_quantidade*of.ofe_receita) FROM atendimento_operacional o, ofertas of " +
				" WHERE o.ins_id = :instituicaoEnsino AND o.dis_substituta_id IS NULL AND o.ofe_id=of.ofe_id" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.hdc_id, o.turma" +
				" UNION " +
				" SELECT o1.dis_substituta_id, o1.hdc_id, o1.turma, SUM(o1.atp_quantidade*of1.ofe_receita) FROM atendimento_operacional o1, ofertas of1 " +
				" WHERE o1.ins_id = :instituicaoEnsino  AND o1.dis_substituta_id IS NOT NULL AND o1.ofe_id=of1.ofe_id" +
				" AND o1.ofe_id IN (select f1.ofe_id from ofertas f1 where f1.cam_id = :campus)" +
				" GROUP BY o1.dis_substituta_id, o1.hdc_id, o1.turma ");

		q.setParameter( "campus", campus.getId() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		double receita = 0.0;
		for (Object registro : q.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			if (disciplinasIDs.contains(disId)) {
				double qtdAlunos_x_ReceitaCredito = ((Double)((Object[])registro)[3]).doubleValue();
				receita += qtdAlunos_x_ReceitaCredito;
			}
		}

		return receita * 4.5 * 6.0;
	}
	
	public static int countCreditosByTurmas(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		Query q = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.hdc_id, o.turma FROM atendimento_operacional o " +
				" WHERE o.ins_id = :instituicaoEnsino AND o.dis_substituta_id IS NULL" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.hdc_id, o.turma" +
				" UNION " +
				" SELECT o1.dis_substituta_id, o1.hdc_id, o1.turma FROM atendimento_operacional o1 " +
				" WHERE o1.ins_id = :instituicaoEnsino  AND o1.dis_substituta_id IS NOT NULL " +
				" AND o1.ofe_id IN (select f1.ofe_id from ofertas f1 where f1.cam_id = :campus)" +
				" GROUP BY o1.dis_substituta_id, o1.hdc_id, o1.turma ");

		q.setParameter( "campus", campus.getId() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		int countTurmas = 0;
		for (Object registro : q.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			if (disciplinasIDs.contains(disId)) {
				countTurmas++;
			}
		}

		return countTurmas;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findAllByCampus(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus " +
			" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "campus", campus );
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
}
