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
@RooEntity( identifierColumn = "CDE_ID" )
@Table( name = "CURSO_DESCOMPARTILHA" )
public class CursoDescompartilha
	implements Serializable
{
	private static final long serialVersionUID = -6796413707742091455L;

    @NotNull
    @ManyToOne(targetEntity = Parametro.class)
    @JoinColumn(name = "PAR_ID")
    private Parametro parametro;

    @NotNull
    @ManyToOne(targetEntity = Curso.class)
    @JoinColumn(name = "CUR1_ID")
    private Curso curso1;

    @NotNull
    @ManyToOne(targetEntity = Curso.class)
    @JoinColumn(name = "CUR2_ID")
    private Curso curso2;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CDE_ID")
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
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
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
        	CursoDescompartilha attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public CursoDescompartilha merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        CursoDescompartilha merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new CursoDescompartilha().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?) " );
        }

        return em;
    }

	public static int count(
			Parametro parametro )
	{
        return CursoDescompartilha.findAll( parametro ).size();
    }

	@SuppressWarnings("unchecked")
    public static List< CursoDescompartilha > findAll(
    	Parametro parametro )
    {
        return entityManager().createQuery(
        	" SELECT o FROM CursoDescompartilha o " +
        	" WHERE o.parametro = :parametro " )
        	.setParameter( "parametro", parametro ).getResultList();
    }

	public static CursoDescompartilha find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        CursoDescompartilha cd = entityManager().find(
        	CursoDescompartilha.class, id );

        if ( cd != null && cd.getParametro() != null
        	&& !cd.getParametro().getTurnos().isEmpty()
        	&& cd.getParametro().getInstituicaoEnsino() != null
        	&& cd.getParametro().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return cd;
        }

        return null;
    }

	@SuppressWarnings("unchecked")
    public static List< CursoDescompartilha > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
        return entityManager().createQuery(
        	" SELECT o FROM CursoDescompartilha o " +
        	" WHERE o.parametro.turno.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
    }

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Parametro: " ).append( getParametro() ).append( ", " );
        sb.append( "Curso1: " ).append( getCurso1() ).append( ", " );
        sb.append( "Curso2: " ).append( getCurso2() );

        return sb.toString();
    }

	public Parametro getParametro()
	{
		return parametro;
	}

	public void setParametro( Parametro parametro )
	{
		this.parametro = parametro;
	}

	public Curso getCurso1()
	{
        return this.curso1;
    }

	public void setCurso1( Curso curso1 )
	{
        this.curso1 = curso1;
    }

	public Curso getCurso2()
	{
        return this.curso2;
    }

	public void setCurso2( Curso curso2 )
	{
        this.curso2 = curso2;
    }
}
