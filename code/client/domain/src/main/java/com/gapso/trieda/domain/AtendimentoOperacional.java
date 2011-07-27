package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
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
@RooEntity(identifierColumn = "ATP_ID")
@Table(name = "ATENDIMENTO_OPERACIONAL")
public class AtendimentoOperacional implements Serializable
{
	private static final long serialVersionUID = -1061352455612316076L;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Cenario.class)
	@JoinColumn(name = "CEN_ID")
	private Cenario cenario;

	@NotNull
	private String turma;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Sala.class)
	@JoinColumn(name = "SAL_ID")
	private Sala sala;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = HorarioDisponivelCenario.class)
	@JoinColumn(name = "HDC_ID")
	private HorarioDisponivelCenario HorarioDisponivelCenario;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Oferta.class)
	@JoinColumn(name = "OFE_ID")
	private Oferta oferta;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Disciplina.class)
	@JoinColumn(name = "DIS_ID")
	private Disciplina disciplina;

	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Professor.class )
	@JoinColumn(name = "PRF_ID")
	private Professor professor;

	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = ProfessorVirtual.class )
	@JoinColumn(name = "PRV_ID")
	private ProfessorVirtual professorVirtual;

	@Column(name = "ATP_CREDITOTEOTICO")
	private Boolean creditoTeorico;

	@NotNull
	@Column(name = "ATP_QUANTIDADE")
	@Min(0L)
	@Max(999L)
	private Integer quantidadeAlunos;

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("Id: ").append(getId()).append(", ");
		sb.append("Version: ").append(getVersion()).append(", ");
		sb.append("Cenario: ")
				.append((getCenario() != null) ? getCenario().getNome()
						: "null").append(", ");
		sb.append("Turma: ").append(getTurma()).append(", ");
		sb.append("Sala: ")
				.append((getSala() != null) ? getSala().getCodigo() : "null")
				.append(", ");
		sb.append("HorarioDisponivelCenario: ")
				.append((getHorarioDisponivelCenario() != null) ? getHorarioDisponivelCenario()
						.getHorarioAula().getHorario() : "null").append(", ");
		sb.append("Professor: ")
				.append((getProfessor() != null) ? getProfessor().getNome()
						: "null").append(", ");
		sb.append("ProfessorVirtual: ")
				.append((getProfessorVirtual() != null) ? getProfessorVirtual()
						.getId() : "null").append(", ");
		sb.append("CreditoTeorico: ").append(getCreditoTeorico()).append(", ");
		sb.append("Oferta: ").append(getOferta()).append(", ");
		sb.append("Disciplina: ")
				.append((getDisciplina() != null) ? getDisciplina().getCodigo()
						: "null").append(", ");
		sb.append("QuantidadeAlunos: ").append(getQuantidadeAlunos())
				.append(", ");
		return sb.toString();
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	@Column(name = "ATP_ID")
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

	@Transactional
	public void detach() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.detach(this);
	}

	@Transactional
	public void persist() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.persist(this);
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
	public void flush() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.flush();
	}

	@Transactional
	public AtendimentoOperacional merge() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		AtendimentoOperacional merged = this.entityManager.merge(this);
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new AtendimentoOperacional().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
					"Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
		}

		return em;
	}

	@SuppressWarnings("unchecked")
	public static List<Turno> findAllTurnosByCursos(List<Campus> campi) {
		Query q = entityManager()
				.createQuery(
						"SELECT DISTINCT o.oferta.turno FROM AtendimentoOperacional o WHERE o.oferta.campus IN (:campi)");
		q.setParameter("campi", campi);
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> findByCenario( Cenario cenario )
	{
		Query q = entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o WHERE cenario = :cenario" );

		q.setParameter( "cenario", cenario );
		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoOperacional > findByCenario( Cenario cenario,
			Campus campus , Unidade unidade, Sala sala, Turno turno )
	{
		Query q = entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o" +
			" WHERE cenario = :cenario" +
			" AND o.oferta.turno = :turno" +
			" AND o.oferta.campus = :campus" +
			" AND o.sala = :sala" +
			" AND o.sala.unidade = :unidade" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "campus", campus );
		q.setParameter( "unidade", unidade );
		q.setParameter( "sala", sala );
		q.setParameter( "turno", turno );

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findAll()
	{
		return entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o" ).getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findAllBy(
		Professor professor, Turno turno )
	{
		Query q = entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o " +
			"WHERE o.oferta.turno = :turno AND o.professor = :professor" );

		q.setParameter( "turno", turno );
		q.setParameter( "professor", professor );

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findAllPublicadoBy(
		Professor professor, Turno turno, boolean isAdmin )
	{
		String publicado = "";
		if ( !isAdmin )
		{
			publicado = " AND o.oferta.campus.publicado = :publicado ";
		}

		String queryString = "SELECT o FROM AtendimentoOperacional o"
			+ " WHERE o.oferta.turno = :turno "
			+ " AND o.professor = :professor " + publicado;

		Query q = entityManager().createQuery( queryString );

		q.setParameter( "turno", turno );
		q.setParameter( "professor", professor );

		if ( !isAdmin )
		{
			q.setParameter( "publicado", true );
		}

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> findAllPublicadoBy(
			ProfessorVirtual professorVirtual, Turno turno, boolean isAdmin) {
		String publicado = "";
		if (!isAdmin)
			publicado = " AND o.oferta.campus.publicado = :publicado ";
		Query q = entityManager()
				.createQuery(
						"SELECT o FROM AtendimentoOperacional o WHERE o.oferta.turno = :turno AND o.professorVirtual = :professorVirtual "
								+ publicado);
		q.setParameter("turno", turno);
		q.setParameter("professorVirtual", professorVirtual);
		if (!isAdmin)
			q.setParameter("publicado", true);
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> findAllBy(Curso curso) {
		Query q = entityManager()
				.createQuery(
						"SELECT o FROM AtendimentoOperacional o WHERE o.oferta.curriculo.curso = :curso");
		q.setParameter("curso", curso);
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<AtendimentoOperacional> findAllBy(Oferta oferta) {
		Query q = entityManager()
				.createQuery(
						"SELECT o FROM AtendimentoOperacional o WHERE o.oferta = :oferta");
		q.setParameter("oferta", oferta);
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findAll( Campus campus )
	{
		Query q = entityManager().createQuery(
				"SELECT o FROM AtendimentoOperacional o WHERE o.oferta.campus = :campus" );

		q.setParameter( "campus", campus );
		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findAllBy(
		Campus campus, Turno turno )
	{
		Query q = entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o " +
			"WHERE o.oferta.campus = :campus AND o.oferta.turno = :turno" );

		q.setParameter( "campus", campus );
		q.setParameter( "turno", turno );

		return q.getResultList();
	}

	public static AtendimentoOperacional find( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		return entityManager().find( AtendimentoOperacional.class, id );
	}

	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findBySalaAndTurno(
		Sala sala, Turno turno )
	{
		Query q = entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o " +
			"WHERE o.sala = :sala AND o.oferta.turno = :turno" );

		q.setParameter( "sala", sala );
		q.setParameter( "turno", turno );
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findBy(
		Campus campus, Curriculo curriculo, Integer periodo, Turno turno )
	{
		Query q = entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o WHERE o.oferta.curriculo = :curriculo "
			+ "AND o.oferta.campus = :campus AND o.oferta.turno = :turno AND o.disciplina "
			+ "IN (SELECT d.disciplina FROM CurriculoDisciplina d "
			+ "WHERE d.curriculo = :curriculo AND d.periodo = :periodo)" );

		q.setParameter( "campus", campus );
		q.setParameter( "curriculo", curriculo );
		q.setParameter( "periodo", periodo );
		q.setParameter( "turno", turno );

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< AtendimentoOperacional > findBy(
		Campus campus, Curriculo curriculo, Integer periodo, Turno turno, Curso curso )
	{
		Query q = entityManager().createQuery(
			"SELECT o FROM AtendimentoOperacional o WHERE o.oferta.curriculo = :curriculo "
			+ "AND o.oferta.campus = :campus "
			+ "AND o.oferta.curso = :curso "
			+ "AND o.oferta.turno = :turno "
			+ "AND o.disciplina IN (SELECT d.disciplina FROM CurriculoDisciplina d " +
									"WHERE d.curriculo = :curriculo AND d.periodo = :periodo)" );

		q.setParameter( "campus", campus );
		q.setParameter( "curriculo", curriculo );
		q.setParameter( "periodo", periodo );
		q.setParameter( "turno", turno );
		q.setParameter( "curso", curso );

		return q.getResultList();
	}

	public Cenario getCenario() {
		return cenario;
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public String getTurma() {
		return turma;
	}

	public void setTurma(String turma) {
		this.turma = turma;
	}

	public Sala getSala() {
		return sala;
	}

	public void setSala(Sala sala) {
		this.sala = sala;
	}

	public HorarioDisponivelCenario getHorarioDisponivelCenario() {
		return HorarioDisponivelCenario;
	}

	public void setHorarioDisponivelCenario(
			HorarioDisponivelCenario horarioDisponivelCenario) {
		HorarioDisponivelCenario = horarioDisponivelCenario;
	}

	public Professor getProfessor()
	{
		if ( professor == null )
		{
			return new Professor();
		}

		return professor;
	}

	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	public Boolean getCreditoTeorico() {
		return this.creditoTeorico;
	}

	public void setCreditoTeorico(Boolean creditoTeorico) {
		this.creditoTeorico = creditoTeorico;
	}

	public Oferta getOferta() {
		return oferta;
	}

	public void setOferta(Oferta oferta) {
		this.oferta = oferta;
	}

	public Disciplina getDisciplina() {
		return disciplina;
	}

	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
	}

	public Integer getQuantidadeAlunos() {
		return quantidadeAlunos;
	}

	public void setQuantidadeAlunos(Integer quantidadeAlunos) {
		this.quantidadeAlunos = quantidadeAlunos;
	}

	public ProfessorVirtual getProfessorVirtual() {
		return professorVirtual;
	}

	public void setProfessorVirtual(ProfessorVirtual professorVirtual) {
		this.professorVirtual = professorVirtual;
	}

	public String getNaturalKey() {
		Oferta oferta = getOferta();
		Curriculo curriculo = oferta.getCurriculo();
		return oferta.getCampus().getId() + "-" + oferta.getTurno().getId()
				+ "-" + curriculo.getCurso().getId() + "-" + curriculo.getId()
				+ "-" + curriculo.getPeriodo(getDisciplina()) + "-"
				+ getDisciplina().getId() + "-" + getTurma() + "-"
				+ getCreditoTeorico();
	}

	static public List< AtendimentoOperacional > getAtendimentosOperacional(
		boolean isAdmin, Professor professor, ProfessorVirtual professorVirtual, Turno turno )
	{
		List< AtendimentoOperacional > atendimentosOperacional = null;
		if ( professor != null )
		{
			atendimentosOperacional = AtendimentoOperacional
				.findAllPublicadoBy( professor, turno, isAdmin );
		}
		else if ( professorVirtual != null )
		{
			atendimentosOperacional = AtendimentoOperacional
				.findAllPublicadoBy( professorVirtual, turno, isAdmin );
		}

		return atendimentosOperacional;
	}
}
