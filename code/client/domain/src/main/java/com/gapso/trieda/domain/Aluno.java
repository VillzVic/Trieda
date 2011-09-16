package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

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
@UniqueConstraint( columnNames = { "ALN_CPF" } ) )
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
	@Column( name = "ALN_CPF")
	@Size( min = 14, max = 14 )
	private String cpf;

	@NotNull
	@Column( name = "ALN_NOME" )
	@Size( min = 3, max = 50 )
	private String nome;

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
		sb.append( "Cpf: " ).append( getCpf() ).append( ", " );
		sb.append( "Nome: " ).append( getNome() );

		return sb.toString();
	}

	public String getCpf()
	{
		return this.cpf;
	}

	public void setCpf( String cpf )
	{
		this.cpf = cpf;
	}

	public String getNome()
	{
		return this.nome;
	}

	public void setNome( String nome )
	{
		this.nome = nome;
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
		return instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	public Cenario getCenario()
	{
		return cenario;
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
	public static List< Aluno > findByNomeCpf(
		InstituicaoEnsino instituicaoEnsino, String nome, String cpf )
	{
		String nomeQuery = "";
		if ( nome != null )
		{
			nomeQuery = " AND LOWER ( :nome ) LIKE LOWER ( o.nome ) ";
		}

		String cpfQuery = "";
		if ( cpf != null )
		{
			cpfQuery = " AND LOWER ( :cpf ) LIKE LOWER ( o.cpf ) ";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			nomeQuery + cpfQuery );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( nome != null )
		{
			q.setParameter( "nome", nome );
		}

		if ( cpf != null )
		{
			q.setParameter( "cpf", cpf );	
		}

		return q.getResultList();
	}

	public static Aluno find( Long id, InstituicaoEnsino instituicaoEnsino )
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
		Cenario cenario, String cpf )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Aluno o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cpf = :cpf " +
			" AND o.cenario = :cenario " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cpf", cpf );
		q.setParameter( "cenario", cenario );

		List< Aluno > listAlunos = q.getResultList();
		return ( listAlunos.size() > 0 );
	}

	@Override
	public int compareTo( Aluno o )
	{
		return this.getNome().compareTo( o.getNome() );
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Aluno ) )
		{
			return false;
		}

		Aluno other = (Aluno) obj;

		if ( id == null )
		{
			if ( other.id != null )
			{
				return false;
			}
		}
		else if ( !id.equals( other.id ) )
		{
			return false;
		}

		return true;
	}
}
