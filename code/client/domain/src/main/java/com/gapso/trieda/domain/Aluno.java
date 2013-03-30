package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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
import javax.persistence.UniqueConstraint;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "ALN_ID" )
@Table( name = "ALUNOS", uniqueConstraints =
@UniqueConstraint( columnNames = { "ALN_MATRICULA", "CEN_ID" } ) )
public class Aluno
	implements Serializable, Comparable< Aluno >
{
	private static final long serialVersionUID = 265242535107921721L;

	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	@NotNull
	@ManyToOne( targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	@NotNull
	@Column( name = "ALN_MATRICULA")
	@Size( min = 1, max = 50 )
	private String matricula;

	@NotNull
	@Column( name = "ALN_NOME" )
	@Size( min = 3, max = 500 )
	private String nome;
	
	@NotNull
	@Column( name = "ALN_FORMANDO" )
	private Boolean formando;

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
		sb.append( "Nome: " ).append( getNome() ).append( ", " );
		sb.append( "Matricula: " ).append( getMatricula() ).append( ", " );
		sb.append( "Formando: " ).append( getFormando() );

		return sb.toString();
	}

	public String getMatricula()
	{
		return this.matricula;
	}

	public void setMatricula( String matricula )
	{
		this.matricula = matricula;
	}

	public String getNome()
	{
		return this.nome;
	}

	public void setNome( String nome )
	{
		this.nome = nome;
	}
	
	public Boolean getFormando()
	{
		return this.formando;
	}

	public void setFormando( Boolean formando )
	{
		this.formando = formando;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "ALN_ID" )
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

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return this.instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	public Cenario getCenario()
	{
		return this.cenario;
	}

	public void setCenario( Cenario cenario )
	{
		this.cenario = cenario;
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
			Aluno attached = this.entityManager.find(
				this.getClass(), this.id );

			this.entityManager.remove( attached );
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
	public Aluno merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Aluno merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Aluno().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Aluno > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		return entityManager().createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " )
			.setParameter( "instituicaoEnsino", instituicaoEnsino ).getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Aluno > findMaxResults(
		InstituicaoEnsino instituicaoEnsino, int maxResults )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	public static List< Aluno > findByMatricula(
		InstituicaoEnsino instituicaoEnsino, String matricula )
	{
		return Aluno.findByNomeMatricula(
			instituicaoEnsino, null, matricula );
	}

	public static List< Aluno > findByNome(
		InstituicaoEnsino instituicaoEnsino, String nome )
	{
		return Aluno.findByNomeMatricula(
			instituicaoEnsino, nome, null );
	}

	@SuppressWarnings( "unchecked" )
	public static List< Aluno > findByNomeMatricula(
		InstituicaoEnsino instituicaoEnsino,
		String nome, String matricula )
	{
		String nomeQuery = "";
		if ( nome != null )
		{
			nomeQuery = " AND LOWER ( o.nome ) LIKE LOWER ( :nome ) ";
		}

		String matriculaQuery = "";
		if ( matricula != null )
		{
			matriculaQuery = " AND LOWER ( o.matricula ) LIKE LOWER ( :matricula ) ";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			nomeQuery + matriculaQuery );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( nome != null )
		{
			q.setParameter( "nome", nome );
		}

		if ( matricula != null )
		{
			q.setParameter( "matricula", matricula );	
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List<Aluno> findByCampus(InstituicaoEnsino instituicaoEnsino, Campus campus, boolean tatico) {
		
		String atendimentoEntity = (tatico) ? "AtendimentoTatico" : "AtendimentoOperacional";

		Query q = entityManager().createQuery(
				" SELECT DISTINCT ( ald.aluno ) FROM " + atendimentoEntity + " o, "
				+ " IN (o.alunosDemanda) ald"
				+ " WHERE o.instituicaoEnsino = :instituicaoEnsino"
				+ " AND o.oferta.campus = :campus "
				+ " ORDER BY ald.aluno.nome "
			);

		q.setParameter("campus", campus);
		q.setParameter("instituicaoEnsino",instituicaoEnsino);

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List<Aluno> findByCenario(InstituicaoEnsino instituicaoEnsino, Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}
	
	
	@SuppressWarnings( "unchecked" )
	public static List<Aluno> findBy(InstituicaoEnsino instituicaoEnsino, Cenario cenario, String nome, String matricula, int firstResult, int maxResults, String orderBy) {
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		matricula = ( ( matricula == null ) ? "" : matricula );
		matricula = ( "%" + matricula.replace( '*', '%' ) + "%" );

		EntityManager em = Campus.entityManager();
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );
		Query q = em.createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.nome ) LIKE LOWER ( :nome ) " +
			" AND LOWER ( o.matricula ) LIKE LOWER ( :matricula ) " +
			" AND o.cenario = :cenario " +
			" AND 1=1 " + orderBy
		);

		q.setParameter("nome",nome);
		q.setParameter("matricula",matricula);
		q.setParameter("cenario",cenario);
		q.setParameter("instituicaoEnsino",instituicaoEnsino);

		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}
	
	public static int count(InstituicaoEnsino instituicaoEnsino, Cenario cenario, String nome, String matricula) {
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		matricula = ( ( matricula == null ) ? "" : matricula );
		matricula = ( "%" + matricula.replace( '*', '%' ) + "%" );

		EntityManager em = Campus.entityManager();
		Query q = em.createQuery(
			" SELECT COUNT ( o ) FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.nome ) LIKE LOWER ( :nome ) " +
			" AND LOWER ( o.matricula ) LIKE LOWER ( :matricula ) " +
			" AND o.cenario = :cenario " +
			" AND 1=1 "
		);

		q.setParameter( "nome", nome );
		q.setParameter( "matricula", matricula );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static Aluno find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Aluno aluno = entityManager().find( Aluno.class, id );

		if ( aluno != null
			&& aluno.getInstituicaoEnsino() != null )
		{
			return aluno;
		}

		return null;
	}

	@SuppressWarnings( "unchecked" )
	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String matricula )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.matricula = :matricula " +
			" AND o.cenario = :cenario " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "matricula", matricula );
		q.setParameter( "cenario", cenario );

		List< Aluno > listAlunos = q.getResultList();
		return ( listAlunos.size() > 0 );
	}

	//@Override
	public int compareTo( Aluno o )
	{
		int result = this.getInstituicaoEnsino().compareTo( o.getInstituicaoEnsino() );

		if ( result == 0 )
		{
			result = this.getNome().compareTo( o.getNome() );
		}

		return result;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Aluno ) )
		{
			return false;
		}

		Aluno other = (Aluno) obj;

		// Comparando os id's
		if ( this.id == null )
		{
			if ( other.id != null )
			{
				return false;
			}
		}
		else if ( !this.id.equals( other.id ) )
		{
			return false;
		}

		return true;
	}
	
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( this.id == null ) ? 0 : this.id.hashCode() ) );
		result = ( prime * result + ( ( this.version == null ) ? 0 : this.version.hashCode() ) );

		return result;
	}

	public static Map< String, Aluno > buildMatriculaAlunoToAlunoMap( 
		List< Aluno > alunos )
	{
		Map< String, Aluno > alunosMap
			= new HashMap< String, Aluno >();

		for ( Aluno aluno : alunos )
		{
			alunosMap.put( aluno.getMatricula(), aluno );
		}

		return alunosMap;
	}
}
