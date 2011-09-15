package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.PersistenceContext;
import javax.persistence.Table;
import javax.persistence.Version;
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
@RooEntity( identifierColumn = "INS_ID" )
@Table( name = "INSTITUICAO_ENSINO" )
public class InstituicaoEnsino
	implements Serializable, Comparable< InstituicaoEnsino >
{
	private static final long serialVersionUID = 5574796519360717359L;

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "INS_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;

	@NotNull
	@Column( name = "INS_NOME" )
	private String nomeInstituicao;

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

	public String getNomeInstituicao()
	{
		return this.nomeInstituicao;
	}

	public void setNomeInstituicao( String nomeInstituicao )
	{
		this.nomeInstituicao = nomeInstituicao;
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
			InstituicaoEnsino attached = this.entityManager.find(
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
	public InstituicaoEnsino merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		InstituicaoEnsino merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new InstituicaoEnsino().entityManager;
		if ( em == null )
		{
			throw new IllegalStateException(
				"Entity manager has not been injected (is the Spring " +
				"Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings("unchecked")
	public static List< InstituicaoEnsino > findAll()
	{
		return entityManager().createQuery(
			"SELECT o FROM InstituicaoEnsino o" ).getResultList();
	}

	public static InstituicaoEnsino find( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		return entityManager().find( InstituicaoEnsino.class, id );
	}

	public String getNaturalKey()
	{
		return this.getId() + "-" + this.getNomeInstituicao();
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Nome da instituicao: " ).append( getNomeInstituicao() );

		return sb.toString();
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( this.getId() == null ) ? 0 : this.getId().hashCode() ) );
		result = ( prime * result + ( ( this.getVersion() == null ) ? 0 : this.getVersion().hashCode() ) );

		return result;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof InstituicaoEnsino ) )
		{
			return false;
		}

		InstituicaoEnsino other = (InstituicaoEnsino) obj;

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

	@Override
	public int compareTo( InstituicaoEnsino o )
	{
		return this.getId().compareTo( o.getId() );
	}
}
