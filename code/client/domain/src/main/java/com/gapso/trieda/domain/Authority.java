package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
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
@RooEntity( identifierColumn = "USERNAME" )
@Table( name = "AUTHORITIES" )
public class Authority
	implements Serializable
{
	private static final long serialVersionUID = 8739246006672184100L;

    @NotNull
    @Id
    @Column(name = "USERNAME")
    @Size(min = 1, max = 50)
    private String username;
    
    @Column(name = "AUTHORITY")
    private String authority;

	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return instituicaoEnsino;
	}

	public void setInstituicaoEnsino( InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	@PersistenceContext
    transient EntityManager entityManager;

	@Transactional
    public void persist() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.persist(this);
    }

	@Transactional
    public void remove() {
        if (this.entityManager == null) this.entityManager = entityManager();
        if (this.entityManager.contains(this)) {
            this.entityManager.remove(this);
        } else {
            Authority attached = this.entityManager.find(this.getClass(), this.username);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Authority merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Authority merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Authority().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?) " );
        }

        return em;
    }

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Username: " ).append( getUsername() ).append( ", " );
        sb.append( "Authority: " ).append( getAuthority() ).append( ", " );
        sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() );

        return sb.toString();
    }

	public String getUsername()
	{
        return this.username;
    }

	public void setUsername( String username )
	{
        this.username = username;
    }
	
	public String getAuthority()
	{
		return this.authority;
	}

	public void setAuthority( String authority )
	{
		this.authority = authority;
	}
	
	@SuppressWarnings( "unchecked" )
    public static List< Authority > findBy( InstituicaoEnsino instituicaoEnsino )
    {
		
		Query q = entityManager().createQuery(
			" SELECT o FROM Authority o WHERE" +
			" o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }
}
