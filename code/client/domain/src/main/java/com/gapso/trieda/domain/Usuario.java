package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.FetchType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
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
@RooEntity( identifierColumn = "USERNAME" )
@Table( name = "users" )
public class Usuario
	implements Serializable
{
	private static final long serialVersionUID = 2505879126546359228L;

    @NotNull
    @Column( name = "USU_NOME" )
    @Size( min = 1, max = 500 )
    private String nome;

    @Column( name = "USU_EMAIL" )
    @Size( min = 5, max = 100 )
    private String email;

    @Id
    @NotNull
    @Column( name = "USERNAME", unique = true )
    @Size( min = 5, max = 50 )
    private String username;

    @NotNull
    @Column( name = "PASSWORD" )
    @Size( min = 5, max = 255 )
    private String password;
    
    @NotNull
    @Column( name = "ENABLED" )
    private Boolean enabled;
    
    @ManyToOne( cascade = {
    	CascadeType.PERSIST, CascadeType.DETACH,
    	CascadeType.MERGE, CascadeType.REFRESH }, fetch=FetchType.LAZY )
    @JoinColumn( name = "PRF_ID" )
    private Professor professor;

    @OneToOne( cascade = { CascadeType.MERGE, CascadeType.REFRESH }, targetEntity = Authority.class, fetch=FetchType.LAZY )
    @JoinColumn( name = "USERNAME" )
    private Authority authority;

	@ManyToOne( cascade = { CascadeType.PERSIST,
		CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class, fetch=FetchType.EAGER )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;
	
	@OneToMany(mappedBy = "cenario")
	private Set<RequisicaoOtimizacao> requisicoesDeOtimizacao = new HashSet<RequisicaoOtimizacao>();
	
/*    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
	@JoinTable(name="PERFIS_ACESSO_USUARIOS",
	joinColumns=@JoinColumn(name="USERNAME"),
	inverseJoinColumns=@JoinColumn(name="PER_ACE_ID"))
    private Set<PerfilAcesso> perfisAcesso = new HashSet<PerfilAcesso>();*/

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return this.instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	public String getNome()
	{
        return this.nome;
    }

	public void setNome( String nome )
	{
        this.nome = nome;
    }

	public String getEmail()
	{
        return this.email;
    }

	public void setEmail( String email )
	{
        this.email = email;
    }

	public String getUsername()
	{
        return this.username;
    }

	public void setUsername( String username )
	{
        this.username = username;
    }

	public String getPassword()
	{
        return this.password;
    }

	public void setPassword( String password )
	{
        this.password = password;
    }

	public Boolean getEnabled()
	{
        return this.enabled;
    }

	public void setEnabled( Boolean enabled )
	{
        this.enabled = enabled;
    }

	public Professor getProfessor()
	{
		return this.professor;
	}

	public void setProfessor( Professor professor )
	{
		this.professor = professor;
	}

	public Authority getAuthority()
	{
		return this.authority;
	}

	public void setAuthority( Authority authority )
	{
		this.authority = authority;
	}
	
	public Set<RequisicaoOtimizacao> getRequisicoesDeOtimizacao() {
		return this.requisicoesDeOtimizacao;
	}

	public void setRequisicoesDeOtimizacao(Set<RequisicaoOtimizacao> requisicoesDeOtimizacao) {
		this.requisicoesDeOtimizacao = requisicoesDeOtimizacao;
	}
	
/*	public Set<PerfilAcesso> getPerfisAcesso() {
		return this.perfisAcesso;
	}
	
	public void setPerfisAcesso(Set<PerfilAcesso> perfisAcesso) {
		this.perfisAcesso = perfisAcesso;
	}*/

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Instituicoes de Ensino: " ).append( getInstituicaoEnsino() ).append( ", "  );
        sb.append( "Nome: " ).append( getNome() ).append( ", " );
        sb.append( "Email: " ).append( getEmail() ).append( ", " );
        sb.append( "Username: " ).append( getUsername() ).append( ", " );
        sb.append( "Password: " ).append( getPassword() ).append( ", " );
        sb.append( "Enabled: " ).append( getEnabled() ).append( ", " );
        sb.append( "Professor: " ).append( getProfessor() ).append(", " );
        sb.append( "Authority: " ).append( getAuthority() );
        sb.append("Requisi��es de Otimiza��o: ").append(getRequisicoesDeOtimizacao() == null ? "null" : getRequisicoesDeOtimizacao().size() ).append( ", " );

        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Version
    @Column( name = "version" )
    private Integer version;

	public Integer getVersion()
	{
        return this.version;
    }

	public void setVersion( Integer version )
	{
        this.version = version;
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
        	if ( this.getAuthority() != null )
        	{
        		this.getAuthority().remove();
        	}

            this.entityManager.remove( this );
        }
        else
        {
            Usuario attached = this.entityManager.find(
            	this.getClass(), this.username );

            if ( attached.getAuthority() != null )
            {
            	attached.getAuthority().remove();
            }

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
    public Usuario merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Usuario merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Usuario().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	public static int count( String nome, String username,
		String email, InstituicaoEnsino instituicaoEnsino )
	{
		String insQuery = "";
		if (instituicaoEnsino != null) {
			insQuery = " o.instituicaoEnsino = :instituicaoEnsino AND";
		}
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Usuario o " +
			" WHERE " + insQuery +
			" LOWER ( o.nome ) LIKE LOWER ( :nome ) " +
			" AND LOWER ( o.username ) LIKE LOWER ( :username ) " +
			" AND LOWER ( o.email ) LIKE LOWER ( :email ) " +
			" AND (o.username != 'trieda.root')");

		nome = "%" + nome.replace('*', '%') + "%";
		username = "%" + username.replace('*', '%') + "%";
		email = "%" + email.replace('*', '%') + "%";

		q.setParameter( "nome", nome );
		q.setParameter( "username", username );
		q.setParameter( "email", email );
		if (instituicaoEnsino != null) {
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		}

        return ( (Number) q.getSingleResult() ).intValue();
    }

	@SuppressWarnings( "unchecked" )
    public static List< Usuario > findAllBy( String nome, String username,
    	String email, int firstResult, int maxResults,
    	String orderBy, InstituicaoEnsino instituicaoEnsino )
    {
		orderBy = ( ( orderBy != null ) ? "ORDER BY o." + orderBy : "" );
		
		String instituicaoEnsinoQuery = "";
		
		if (instituicaoEnsino != null)
		{
			instituicaoEnsinoQuery = " o.instituicaoEnsino = :instituicaoEnsino AND ";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM Usuario o WHERE" +
			" LOWER( o.nome ) LIKE LOWER ( :nome ) AND " +
			" LOWER( o.username ) LIKE LOWER( :username ) AND " +
			instituicaoEnsinoQuery + " o.instituicaoEnsino IS NOT NULL AND " +
			" LOWER( o.email ) LIKE LOWER ( :email ) " + orderBy );

		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		username = ( "%" + username.replace( '*', '%' ) + "%" );
		email = ( "%" + email.replace( '*', '%' ) + "%" );

		q.setParameter( "nome", nome );
		q.setParameter( "username", username );
		q.setParameter( "email", email );
		if (instituicaoEnsino != null)
		{
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		}
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

        return q.getResultList();
    }

	public static Usuario find( String username )
	{
        if ( username == null )
        {
        	return null;
        }

        Usuario usuario = entityManager().< Usuario >find( Usuario.class, username );
       	return usuario;
    }

	public static boolean checkUsernameUnique(
		Cenario cenario, String username,
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Usuario o " +
			" WHERE o.username = :username " +
			" AND o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "username", username );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}
	
	@SuppressWarnings( "unchecked" )
    public static List< Usuario > findBy( InstituicaoEnsino instituicaoEnsino )
    {
		
		Query q = entityManager().createQuery(
			" SELECT o FROM Usuario o WHERE" +
			" o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }
}
