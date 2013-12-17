package com.gapso.trieda.domain;

import java.io.Serializable;
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
@RooEntity( identifierColumn = "TIT_ID" )
@Table( name = "TITULACOES" )
public class Titulacao
	implements Serializable
{
	private static final long serialVersionUID = 8929281662490204744L;

    @NotNull
    @Column( name = "TIT_NOME" )
    @Size( min = 1, max = 50 )
    private String nome;
    
	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
        sb.append( "Nome: " ).append( getNome() );

        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TIT_ID")
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

	@NotNull
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
	
	public Cenario getCenario() {
		return this.cenario;
	}
	
	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

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
            Titulacao attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Titulacao merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Titulacao merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Titulacao().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	public static int count( InstituicaoEnsino instituicaoEnsino )
	{
        return Titulacao.findAll( instituicaoEnsino ).size();
    }

	@SuppressWarnings("unchecked")
    public static List< Titulacao > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
        return entityManager().createQuery(
        	" SELECT o FROM Titulacao o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.getResultList();
    }
	
	@SuppressWarnings("unchecked")
    public static List< Titulacao > findByCenario(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
        return entityManager().createQuery(
        	" SELECT o FROM Titulacao o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino" +
        	" AND o.cenario = :cenario " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setParameter( "cenario", cenario )
        	.getResultList();
    }

	public static Titulacao find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Titulacao titulacao = entityManager().find( Titulacao.class, id );

        if ( titulacao != null
        	&& titulacao.getInstituicaoEnsino()!= null
        	&& titulacao.getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return titulacao;
        }

        return null;
    }

	@SuppressWarnings("unchecked")
    public static List< Titulacao > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
        return entityManager().createQuery(
        	" SELECT o FROM Titulacao o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
    }

	public static Map< String, Titulacao > buildTitulacaoNomeToTitulacaoMap(
		List< Titulacao > titulacoes )
	{
		Map< String, Titulacao > titulacoesMap
			= new HashMap< String, Titulacao >();

		for ( Titulacao titulacao : titulacoes )
		{
			titulacoesMap.put( titulacao.getNome(), titulacao );
		}

		return titulacoesMap;
	}

	public String getNome()
	{
        return this.nome;
    }

	public void setNome( String nome )
	{
        this.nome = nome;
    }
}
