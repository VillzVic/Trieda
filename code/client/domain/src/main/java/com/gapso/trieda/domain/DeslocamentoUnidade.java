package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Digits;
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
@RooEntity( identifierColumn = "DEU_ID" )
@Table( name = "DESLOCAMENTOS_UNIDADES" )
public class DeslocamentoUnidade
	implements Serializable
{
	private static final long serialVersionUID = -8847212098556601964L;

    @NotNull
    @ManyToOne( targetEntity = Unidade.class, fetch = FetchType.LAZY )
    @JoinColumn( name = "UNI_ORIG_ID" )
    private Unidade origem;

    @NotNull
    @ManyToOne( targetEntity = Unidade.class, fetch = FetchType.LAZY )
    @JoinColumn( name = "UNI_DEST_ID" )
    private Unidade destino;

    @Column( name = "DEC_TEMPO" )
    @Min( 0L )
    @Max( 999L )
    private Integer tempo;

    @Column( name = "DEC_CUSTO" )
    @Digits( integer = 4, fraction = 2 )
    private Double custo;

	public Unidade getOrigem()
	{
        return this.origem;
    }

	public void setOrigem( Unidade origem )
	{
        this.origem = origem;
    }

	public Unidade getDestino()
	{
        return this.destino;
    }

	public void setDestino( Unidade destino )
	{
        this.destino = destino;
    }

	public Integer getTempo()
	{
        return this.tempo;
    }

	public void setTempo( Integer tempo )
	{
        this.tempo = tempo;
    }

	public Double getCusto()
	{
		return this.custo;
	}

	public void setCusto( Double custo )
	{
		this.custo = custo;
	}

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "DEU_ID" )
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
            this.entityManager.remove( this );
        }
        else
        {
            DeslocamentoUnidade attached = this.entityManager.find(
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
    public DeslocamentoUnidade merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        DeslocamentoUnidade merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new DeslocamentoUnidade().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?) " );
        }

        return em;
    }

	public static long count(
		InstituicaoEnsino instituicaoEnsino )
	{
        return DeslocamentoUnidade.findAll( instituicaoEnsino ).size();
    }

	@SuppressWarnings( "unchecked" )
    public static List< DeslocamentoUnidade > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
	        " SELECT o FROM DeslocamentoUnidade o " +
	        " WHERE o.origem.campus.instituicaoEnsino = :instituicaoEnsino " +
	        " AND o.destino.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

	public static DeslocamentoUnidade find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        DeslocamentoUnidade du = entityManager().find( DeslocamentoUnidade.class, id );
        
        if ( du != null && du.getOrigem() != null && du.getDestino() != null
       		&& du.getOrigem().getCampus() != null
       		&& du.getDestino().getCampus() != null
       		&& du.getOrigem().getCampus().getInstituicaoEnsino() != null
       		&& du.getDestino().getCampus().getInstituicaoEnsino() != null
       		&& du.getOrigem().getCampus().getInstituicaoEnsino() == instituicaoEnsino
       		&& du.getDestino().getCampus().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return du;
        }

        return null;
    }

	@SuppressWarnings( "unchecked" )
    public static List< DeslocamentoUnidade > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
		Query q = entityManager().createQuery(
	        " SELECT o FROM DeslocamentoUnidade o " +
	        " WHERE o.origem.campus.instituicaoEnsino = :instituicaoEnsino " +
	        " AND o.destino.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
    public static List< DeslocamentoUnidade > findAllByCampus(
    	InstituicaoEnsino instituicaoEnsino, Campus campus )
    {
		Query q = entityManager().createQuery(
			" SELECT o FROM DeslocamentoUnidade o " +
			" WHERE o.origem.campus = :campus " +
			" AND o.origem.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.destino.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campus", campus );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
    public static List< DeslocamentoUnidade > findAllByUnidade(
    	InstituicaoEnsino instituicaoEnsino, Unidade unidade )
    {
		Query q = entityManager().createQuery(
			" SELECT o FROM DeslocamentoUnidade o " +
			" WHERE ( o.origem = :unidade OR o.destino = :unidade ) " +
			" AND o.origem.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.destino.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "unidade", unidade );

        return q.getResultList();
    }

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Origem: " ).append( getOrigem() ).append( ", " );
        sb.append( "Destino: " ).append( getDestino() ).append( ", " );
        sb.append( "Custo: " ).append( getCusto() ).append( ", " );
        sb.append( "Tempo: " ).append( getTempo() );

        return sb.toString();
    }
}
