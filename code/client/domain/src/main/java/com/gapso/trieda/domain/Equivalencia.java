package com.gapso.trieda.domain;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
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
@RooEntity( identifierColumn = "EQV_ID" )
@Table( name = "EQUIVALENCIAS" )
public class Equivalencia
	implements java.io.Serializable
{
	private static final long serialVersionUID = -8632323368932009356L;

    @NotNull
    @ManyToOne( targetEntity = Disciplina.class )
    @JoinColumn( name = "DIS_CURSOU_ID" )
    private Disciplina cursou;

    @NotNull
    @ManyToOne( targetEntity = Disciplina.class )
    @JoinColumn( name = "DIS_ELIMINA_ID" )
    private Disciplina elimina;
    
	@Column( name = "EQV_GERAL" )
	private Boolean equivalenciaGeral;
	
	@ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE })
	@JoinTable(name="cursos_equivalencias",
	joinColumns={ @JoinColumn(name="eqv_id") },
	inverseJoinColumns={ @JoinColumn(name="cur_id") })
	private Set< Curso > cursos = new HashSet< Curso >();


	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Cursou: " ).append( getCursou() ).append( ", " );
        sb.append( "Elimina: " ).append( getElimina()).append( ", " );

        return sb.toString();
    }

	public Disciplina getCursou()
	{
        return this.cursou;
    }

	public void setCursou( Disciplina cursou )
	{
        this.cursou = cursou;
    }

	public Disciplina getElimina()
	{
        return this.elimina;
    }

	public void setElimina( Disciplina elimina )
	{
        this.elimina = elimina;
    }
	
	public Boolean getEquivalenciaGeral()
	{
        return this.equivalenciaGeral;
    }

	public void setEquivalenciaGeral( Boolean equivalenciaGeral )
	{
        this.equivalenciaGeral = equivalenciaGeral;
    }
	
	public Set< Curso > getCursos()
	{
        return this.cursos;
    }

	public void setCursos( Set< Curso > cursos )
	{
        this.cursos = cursos;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "EQV_ID" )
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
            Equivalencia attached = this.entityManager.find(
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
    public Equivalencia merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Equivalencia merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Equivalencia().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	@SuppressWarnings( "unchecked" )
    public static List< Equivalencia > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
	        " SELECT o FROM Equivalencia o " +
    		" WHERE o.cursou.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

	public static Equivalencia find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Equivalencia equivalencia
        	= entityManager().find( Equivalencia.class, id );

        if ( equivalencia != null && equivalencia.getCursou() != null
        	&& equivalencia.getCursou().getTipoDisciplina() != null
        	&& equivalencia.getCursou().getTipoDisciplina().getInstituicaoEnsino() != null
        	&& equivalencia.getCursou().getTipoDisciplina().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return equivalencia;
        }

        return null;
    }
	
	public static int count(
		InstituicaoEnsino instituicaoEnsino, Disciplina disciplina, Curso curso )
	{
		String where = " WHERE o.cursou.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino ";
		String from = " FROM Equivalencia o ";

		if ( disciplina != null )
		{
			where += ( " AND o.cursou = :disciplina " );
		}
		
		Set<Curso> cursoBusca = new HashSet<Curso>();
		if ( curso != null )
		{
			cursoBusca.add(curso);
			from += ", IN ( o.cursos ) cur";
			where += ( " AND cur IN ( :curso )" );
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o )" + from + where );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}
		if ( curso != null )
		{
			q.setParameter( "curso", cursoBusca);
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Equivalencia > findBy(
		InstituicaoEnsino instituicaoEnsino, Disciplina disciplina,
		Curso curso, int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		String where = " WHERE o.cursou.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino ";
		String from = "FROM Equivalencia o";

		if ( disciplina != null )
		{
			where += ( " AND o.cursou = :disciplina " );
		}
		
		Set<Curso> cursoBusca = new HashSet<Curso>();
		if ( curso != null )
		{
			cursoBusca.add(curso);
			from += ", IN ( o.cursos ) cur";
			where += ( " AND cur IN ( :curso )" );
		}

		Query q = entityManager().createQuery(
			" SELECT o "+ from + where + orderBy );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}
		if ( curso != null )
		{
			q.setParameter( "curso", cursoBusca);
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
    public static List< Equivalencia > find(InstituicaoEnsino instituicaoEnsino) {
		Query q = entityManager().createQuery(
        	" SELECT o FROM Equivalencia o " +
    		" WHERE o.cursou.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
    public static List< Equivalencia > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
		Query q = entityManager().createQuery(
        	" SELECT o FROM Equivalencia o " +
    		" WHERE o.cursou.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
    	q.setFirstResult( firstResult );
    	q.setMaxResults( maxResults );

        return q.getResultList();
    }

	public static Map< String, Equivalencia > buildEquivalenciaCursouCodigoEliminaCodigoToEquivalenciaMap(
		List< Equivalencia > equivalencias )
	{
		Map< String, Equivalencia > equivalenciasMap
			= new HashMap< String, Equivalencia >();

		for ( Equivalencia equivalencia : equivalencias )
		{
			equivalenciasMap.put( equivalencia.getCursou().getCodigo() +
					equivalencia.getElimina().getCodigo(), equivalencia );
		}

		return equivalenciasMap;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Equivalencia > findBy(InstituicaoEnsino instituicaoEnsino,
		Disciplina cursou, Disciplina elimina)
	{
		Query q = entityManager().createQuery(
	       	" SELECT o FROM Equivalencia o " +
	   		" WHERE o.cursou.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
	       	" AND o.cursou = :cursou AND o.elimina = :elimina");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cursou", cursou);
		q.setParameter( "elimina", elimina);
		
		return q.getResultList();
	}

}
