package com.gapso.trieda.domain;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

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
@RooEntity( identifierColumn = "DIP_ID" )
@Table( name = "PROFESSORES_DISCIPLINAS" )
public class ProfessorDisciplina
	implements java.io.Serializable
{
	private static final long serialVersionUID = -6254398976446496178L;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Professor.class)
	@JoinColumn(name = "PRF_ID")
	private Professor professor;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Disciplina.class)
	@JoinColumn(name = "DIS_ID")
	private Disciplina disciplina;

	@NotNull
	@Column(name = "PRF_NOTA")
	@Min(0L)
	@Max(100L)
	private Integer nota;

	@NotNull
	@Column(name = "PRF_PREFERENCIA")
	@Min(0L)
	@Max(10L)
	private Integer preferencia;

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	@Column(name = "DIP_ID")
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
	public void remove() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		if (this.entityManager.contains(this)) {
			this.entityManager.remove(this);
		} else {
			ProfessorDisciplina attached = this.entityManager.find(
					this.getClass(), this.id);
			this.entityManager.remove(attached);
		}
	}

	@Transactional
	public void flush() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.flush();
	}

	@Transactional
	public ProfessorDisciplina merge() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		ProfessorDisciplina merged = this.entityManager.merge(this);
		this.entityManager.flush();
		return merged;
	}
	
	@Transactional
	public ProfessorDisciplina mergeWithoutFlush() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		ProfessorDisciplina merged = this.entityManager.merge(this);
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new ProfessorDisciplina().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		return entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " +
			" WHERE o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " )
			.setParameter( "instituicaoEnsino", instituicaoEnsino ).getResultList();
	}

	public static ProfessorDisciplina find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		ProfessorDisciplina pd = entityManager().find( ProfessorDisciplina.class, id );

		if ( pd != null && pd.getProfessor() != null && pd.getDisciplina() != null
			&& pd.getProfessor().getTipoContrato() != null
			&& pd.getDisciplina().getTipoDisciplina() != null
			&& pd.getProfessor().getTipoContrato().getInstituicaoEnsino() != null
			&& pd.getDisciplina().getTipoDisciplina().getInstituicaoEnsino() != null
			&& pd.getProfessor().getTipoContrato().getInstituicaoEnsino() == instituicaoEnsino
			&& pd.getDisciplina().getTipoDisciplina().getInstituicaoEnsino() == instituicaoEnsino )
		{
			return pd;
		}

		return null;
	}

	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " +
			" WHERE o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " )
			.setParameter( "instituicaoEnsino", instituicaoEnsino )
			.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino,
		Professor professor, Disciplina disciplina )
	{
		String where = " o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino AND ";

		if ( professor != null )
		{
			where += ( " o.professor = :professor AND " );
		}

		if ( disciplina != null )
		{
			where += ( " o.disciplina = :disciplina AND " );
		}
		if ( where.length() > 1 )
		{
			where = " WHERE " + where.substring( 0, where.length() - 4 );
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM ProfessorDisciplina o " + where );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( professor != null )
		{
			q.setParameter( "professor", professor );
		}

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > findBy(
		InstituicaoEnsino instituicaoEnsino, Professor professor,
		Disciplina disciplina, int firstResult, int maxResults,	String orderBy )
	{
		String where = " o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino AND ";

		if ( professor != null )
		{
			where += ( " o.professor = :professor AND " );
		}

		if ( disciplina != null )
		{
			where += ( " o.disciplina = :disciplina AND " );
		}

		if ( where.length() > 1 )
		{
			where = ( " WHERE " + where.substring( 0, where.length() - 4 ) );
		}

		where += ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " + where );

		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( professor != null )
		{
			q.setParameter( "professor", professor );
		}

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}

		return q.getResultList();
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Professor: " ).append( getProfessor() ).append( ", " );
		sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
		sb.append( "Nota: ").append( getNota() ).append( ", " );
		sb.append( "Preferencia: " ).append( getPreferencia() );

		return sb.toString();
	}

	public static Map< String, ProfessorDisciplina > buildCursoNaturalKeyToProfessorDisciplinaMap(
		List< ProfessorDisciplina > pds )
	{
		Map< String, ProfessorDisciplina > map
			= new HashMap< String, ProfessorDisciplina >();

		for ( ProfessorDisciplina pd : pds )
		{
			map.put( pd.getNaturalKeyString(), pd );
		}

		return map;
	}

	public Professor getProfessor()
	{
		return this.professor;
	}

	public void setProfessor( Professor professor )
	{
		this.professor = professor;
	}

	public Disciplina getDisciplina()
	{
		return this.disciplina;
	}

	public void setDisciplina( Disciplina disciplina )
	{
		this.disciplina = disciplina;
	}

	public Integer getNota()
	{
		return this.nota;
	}

	public void setNota( Integer nota )
	{
		this.nota = nota;
	}

	public Integer getPreferencia()
	{
		return this.preferencia;
	}

	public void setPreferencia( Integer preferencia )
	{
		this.preferencia = preferencia;
	}

	public String getNaturalKeyString()
	{
		Professor professor = getProfessor();

		return professor.getCpf()
			+ "-" + professor.getNome()
			+ "-" + getDisciplina().getCodigo();
	}
}
